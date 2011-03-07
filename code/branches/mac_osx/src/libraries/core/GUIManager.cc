/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "GUIManager.h"

#include <boost/bind.hpp>
#include <memory>
extern "C" {
#include <lua.h>
}
#include <CEGUIDefaultLogger.h>
#include <CEGUIExceptions.h>
#include <CEGUIInputEvent.h>
#include <CEGUIMouseCursor.h>
#include <CEGUIResourceProvider.h>
#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <ogreceguirenderer/OgreCEGUIRenderer.h>

#include "SpecialConfig.h" // Configures the macro below
#ifdef CEGUILUA_USE_INTERNAL_LIBRARY
#   include <ceguilua/CEGUILua.h>
#else
#   include <CEGUILua.h>
#endif

#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/OrxAssert.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "LuaState.h"
#include "PathConfig.h"
#include "Resource.h"
#include "command/ConsoleCommand.h"
#include "input/InputManager.h"
#include "input/InputState.h"
#include "input/KeyBinderManager.h"

namespace orxonox
{
    static void key_esc()
        { GUIManager::getInstance().keyESC(); }
    SetConsoleCommand("keyESC", &key_esc);

    class CEGUILogger : public CEGUI::DefaultLogger
    {
    public:
        void logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level = CEGUI::Standard)
        {
            int orxonoxLevel = CEGUI::Standard;
            switch (level)
            {
                case CEGUI::Errors:      orxonoxLevel = 1; break;
                case CEGUI::Warnings:    orxonoxLevel = 2; break;
                case CEGUI::Standard:    orxonoxLevel = 4; break;
                case CEGUI::Informative: orxonoxLevel = 5; break;
                case CEGUI::Insane:      orxonoxLevel = 6; break;
                default: OrxAssert(false, "CEGUI log level out of range, inpect immediately!");
            }
            OutputHandler::getOutStream(orxonoxLevel)
                << "CEGUI: " << message << std::endl;

            CEGUI::DefaultLogger::logEvent(message, level);
        }
    };

    static CEGUI::MouseButton convertButton(MouseButtonCode::ByEnum button);

    GUIManager* GUIManager::singletonPtr_s = 0;

    SetConsoleCommand("showGUI", &GUIManager::showGUI).defaultValue(1, false).defaultValue(2, false);
    SetConsoleCommand("hideGUI", &GUIManager::hideGUI);

    /**
    @brief
        Constructs the GUIManager by starting up CEGUI

        Creates the interface to Ogre, sets up the CEGUI renderer and the Lua script module together with the Lua engine.
        The log is set up and connected to the CEGUILogger.
        After Lua setup tolua++-elements are linked to Lua-state to give Lua access to C++-code.
        Finally initial Lua code is executed (maybe we can do this with the CEGUI startup script automatically).
    @return true if success, otherwise false
    */
    GUIManager::GUIManager(const std::pair<int, int>& mousePosition)
        : resourceProvider_(NULL)
        , camera_(NULL)
    {
        using namespace CEGUI;

        COUT(3) << "Initialising CEGUI." << std::endl;

        // Note: No SceneManager specified yet
        guiRenderer_.reset(new OgreCEGUIRenderer(GraphicsManager::getInstance().getRenderWindow(), Ogre::RENDER_QUEUE_OVERLAY, false, 3000));
        resourceProvider_ = guiRenderer_->createResourceProvider();
        resourceProvider_->setDefaultResourceGroup("General");

        // Setup scripting
        luaState_.reset(new LuaState());
        rootFileInfo_ = Resource::getInfo("InitialiseGUI.lua");
        // This is necessary to ensure that input events also use the right resource info when triggering lua functions
        luaState_->setDefaultResourceInfo(this->rootFileInfo_);
        scriptModule_.reset(new LuaScriptModule(luaState_->getInternalLuaState()));
        scriptModule_->setDefaultPCallErrorHandler(LuaState::ERROR_HANDLER_NAME);

        // Create our own logger to specify the filepath
        std::auto_ptr<CEGUILogger> ceguiLogger(new CEGUILogger());
        ceguiLogger->setLogFilename(PathConfig::getLogPathString() + "cegui.log");
        // set the log level according to ours (translate by subtracting 1)
        ceguiLogger->setLoggingLevel(
            static_cast<LoggingLevel>(OutputHandler::getInstance().getSoftDebugLevel("logFile") - 1));
        this->ceguiLogger_ = ceguiLogger.release();

        // Create the CEGUI system singleton
        guiSystem_.reset(new System(guiRenderer_.get(), resourceProvider_, 0, scriptModule_.get()));

        // Align CEGUI mouse with OIS mouse
        guiSystem_->injectMousePosition((float)mousePosition.first, (float)mousePosition.second);

        // Initialise the Lua framework and load the schemes
        this->luaState_->doFile("InitialiseGUI.lua");

        // Create the root nodes
        this->rootWindow_ = CEGUI::WindowManager::getSingleton().createWindow("MenuWidgets/StaticImage", "AbsoluteRootWindow");
        this->rootWindow_->setProperty("FrameEnabled", "False");
        this->hudRootWindow_ = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "HUDRootWindow");
        this->menuRootWindow_ = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "MenuRootWindow");
        // And connect them
        CEGUI::System::getSingleton().setGUISheet(this->rootWindow_);
        this->rootWindow_->addChildWindow(this->hudRootWindow_);
        this->rootWindow_->addChildWindow(this->menuRootWindow_);

        // No background to start with (sets the alpha value to 0)
        this->setBackgroundImage("");

        // Set up the sheet manager in the Lua framework
        this->luaState_->doFile("SheetManager.lua");
    }

    /**
    @brief
        Basically shuts down CEGUI (member smart pointers) but first unloads our Tolua modules.
    */
    GUIManager::~GUIManager()
    {
    }

    /**
    @brief
        used to tick the GUI
    @param time
        clock which provides time value for the GUI System

        Ticking the GUI means updating it with a certain regularity.
        The elapsed time since the last call is given in the time value provided by the clock.
        This time value is then used to provide a fluent animation of the GUI.
    */
    void GUIManager::preUpdate(const Clock& time)
    {
        assert(guiSystem_);
        this->protectedCall(boost::bind(&CEGUI::System::injectTimePulse, _1, time.getDeltaTime()));
    }

    /**
    @brief
        Tells the GUIManager which SceneManager to use
    @param camera
        The current camera on which the GUI should be displayed on.

        In fact the GUIManager needs the SceneManager and not the Camera to display the GUI.
        This means the GUI is not bound to a camera but rather to the SceneManager.
        Hiding the GUI when needed can therefore not be resolved by just NOT setting the current camera.
    */
    void GUIManager::setCamera(Ogre::Camera* camera)
    {
        this->camera_ = camera;
        if (camera == NULL)
            this->guiRenderer_->setTargetSceneManager(0);
        else
            this->guiRenderer_->setTargetSceneManager(camera->getSceneManager());
    }

    /**
    @brief
        Executes Lua code
    @param str
        reference to string object holding the Lua code which is to be executed
    */
    void GUIManager::executeCode(const std::string& str)
    {
        this->luaState_->doString(str, rootFileInfo_);
    }

    /** Loads a GUI sheet by Lua script
    @param name
        The name of the GUI (like the script name, but without the extension)
    */
    void GUIManager::loadGUI(const std::string& name)
    {
        this->executeCode("loadSheet(\"" + name + "\")");
    }

    /**
    @brief
        Displays specified GUI on screen
    @param name
        The name of the GUI
    @param bHidePrevious
        If true all displayed GUIs on the stack, that are below this GUI are hidden.
    @param bNoInput
        If true the GUI is transparent to input.

        The function executes the Lua function with the same name in case the GUIManager is ready.
    */
    /*static*/ void GUIManager::showGUI(const std::string& name, bool bHidePrevious, bool bNoInput)
    {
        GUIManager::getInstance().executeCode("showMenuSheet(\"" + name + "\", " + multi_cast<std::string>(bHidePrevious) + ", " + multi_cast<std::string>(bNoInput) + ")");
    }

    /**
    @brief
        Hack-ish. Needed for GUIOverlay.
    */
    void GUIManager::showGUIExtra(const std::string& name, const std::string& ptr, bool bHidePrevious, bool bNoInput)
    {
        this->executeCode("showMenuSheet(\"" + name + "\", " + multi_cast<std::string>(bHidePrevious) + ", " + multi_cast<std::string>(bNoInput) + ", " + ptr + ")");
    }

    /**
    @brief
        Hides specified GUI.
    @param name
        The name of the GUI.
    */
    /*static*/ void GUIManager::hideGUI(const std::string& name)
    {
        GUIManager::getInstance().executeCode("hideMenuSheet(\"" + name + "\")");
    }

    const std::string& GUIManager::createInputState(const std::string& name, TriBool::Value showCursor, TriBool::Value useKeyboard, bool bBlockJoyStick)
    {
        InputState* state = InputManager::getInstance().createInputState(name);

        /* Table that maps isFullScreen() and showCursor to mouseExclusive
        isFullscreen / showCursor | True  | False | Dontcare
        ----------------------------------------------------
        true                      | True  | True  | Dontcare
        ----------------------------------------------------
        false                     | False | True  | Dontcare
        */

#ifdef ORXONOX_PLATFORM_APPLE
        // There is no non exclusive mode on OS X yet
        state->setMouseExclusive(TriBool::True);
#else
        if (showCursor == TriBool::Dontcare)
            state->setMouseExclusive(TriBool::Dontcare);
        else if (GraphicsManager::getInstance().isFullScreen() || showCursor == TriBool::False)
            state->setMouseExclusive(TriBool::True);
        else
            state->setMouseExclusive(TriBool::False);
#endif

        if (showCursor == TriBool::True)
            state->setMouseHandler(this);
        else if (showCursor == TriBool::False)
            state->setMouseHandler(&InputHandler::EMPTY);

        if (useKeyboard == TriBool::True)
            state->setKeyHandler(this);
        else if (useKeyboard == TriBool::False)
            state->setKeyHandler(&InputHandler::EMPTY);

        if (bBlockJoyStick)
            state->setJoyStickHandler(&InputHandler::EMPTY);

        return state->getName();
    }

    void GUIManager::keyESC()
    {
        this->executeCode("keyESC()");
    }

    void GUIManager::setBackgroundImage(const std::string& imageSet, const std::string imageName)
    {
        if (imageSet.empty() || imageName.empty())
            this->setBackgroundImage("");
        else
            this->setBackgroundImage("set: " + imageSet + " image: " + imageName);
    }

    void GUIManager::setBackgroundImage(const std::string& image)
    {
        if (image.empty())
            this->rootWindow_->setProperty("Alpha", "0.0");
        else
            this->rootWindow_->setProperty("Alpha", "1.0");
        this->rootWindow_->setProperty("Image", image);
    }

    void GUIManager::buttonPressed(const KeyEvent& evt)
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectKeyDown, _1, evt.getKeyCode()));
        this->protectedCall(boost::bind(&CEGUI::System::injectChar, _1, evt.getText()));
    }

    void GUIManager::buttonReleased(const KeyEvent& evt)
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectKeyUp, _1, evt.getKeyCode()));
    }

    /**
    @brief
        Function receiving a mouse button pressed event.
    @param id
        ID of the mouse button which got pressed

        This function is inherited by MouseHandler and injects the event into CEGUI.
        It is for CEGUI to process the event.
    */
    void GUIManager::buttonPressed(MouseButtonCode::ByEnum id)
    {
        //guiSystem_->injectMouseButtonDown(convertButton(id));
        this->protectedCall(boost::bind(&CEGUI::System::injectMouseButtonDown, _1, convertButton(id)));
    }

    /**
    @brief
        Function receiving a mouse button released event.
    @param id
        ID of the mouse button which got released

        This function is inherited by MouseHandler and injects the event into CEGUI.
        It is for CEGUI to process the event.
    */
    void GUIManager::buttonReleased(MouseButtonCode::ByEnum id)
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectMouseButtonUp, _1, convertButton(id)));
    }

    void GUIManager::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectMousePosition, _1, (float)abs.x, (float)abs.y));
    }

    void GUIManager::mouseScrolled(int abs, int rel)
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectMouseWheelChange, _1, (float)rel));
    }

    /**
    @brief
        converts mouse event code to CEGUI event code
    @param button
        code of the mouse button as we use it in Orxonox
    @return
        code of the mouse button as it is used by CEGUI

        Simple conversion from mouse event code in Orxonox to the one used in CEGUI.
     */
    static inline CEGUI::MouseButton convertButton(MouseButtonCode::ByEnum button)
    {
        switch (button)
        {
        case MouseButtonCode::Left:
            return CEGUI::LeftButton;

        case MouseButtonCode::Right:
            return CEGUI::RightButton;

        case MouseButtonCode::Middle:
            return CEGUI::MiddleButton;

        case MouseButtonCode::Button3:
            return CEGUI::X1Button;

        case MouseButtonCode::Button4:
            return CEGUI::X2Button;

        default:
            return CEGUI::NoButton;
        }
    }

    /** Executes a CEGUI function normally, but catches CEGUI::ScriptException.
        When a ScriptException occurs, the error message will be displayed and
        the program carries on.
    @remarks
        The exception behaviour may pose problems if the code is not written
        exception-safe (and you can forget about that in Lua). The program might
        be left in an undefined state. But otherwise one script error would
        terminate the whole program...
    @note
        Your life gets easier if you use boost::bind to create the object/function.
    @param function
        Any callable object/function that takes this->guiSystem_ as its only parameter.
    @return
        True if input was handled, false otherwise. A caught exception yields true.
    */
    template <typename FunctionType>
    bool GUIManager::protectedCall(FunctionType function)
    {
        try
        {
            return function(this->guiSystem_);
        }
        catch (CEGUI::ScriptException& ex)
        {
            // Display the error and proceed. See @remarks why this can be dangerous.
            COUT(1) << ex.getMessage() << std::endl;
            return true;
        }
    }

    void GUIManager::subscribeEventHelper(CEGUI::Window* window, const std::string& event, const std::string& function)
    {
        window->subscribeScriptedEvent(event, function);
    }
}
