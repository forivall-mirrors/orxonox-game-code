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
#include "ConsoleCommand.h"
#include "Core.h"
#include "LuaState.h"
#include "PathConfig.h"
#include "Resource.h"

namespace orxonox
{
    static void key_esc()
        { GUIManager::getInstance().keyESC(); }
    SetConsoleCommandShortcutExternAlias(key_esc, "keyESC");

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

    SetConsoleCommandShortcut(GUIManager, showGUI).accessLevel(AccessLevel::User).defaultValue(1, false).defaultValue(2, true);
    SetConsoleCommandShortcut(GUIManager, hideGUI).accessLevel(AccessLevel::User);

    /**
    @brief
        Constructs the GUIManager by starting up CEGUI

        Creates the interface to Ogre, sets up the CEGUI renderer and the Lua script module together with the Lua engine.
        The log is set up and connected to the CEGUILogger.
        After Lua setup tolua++-elements are linked to Lua-state to give Lua access to C++-code.
        Finally initial Lua code is executed (maybe we can do this with the CEGUI startup script automatically).
    @param renderWindow
        Ogre's render window. Without this, the GUI cannot be displayed.
    @return true if success, otherwise false
    */
    GUIManager::GUIManager(Ogre::RenderWindow* renderWindow, const std::pair<int, int>& mousePosition, bool bFullScreen)
        : renderWindow_(renderWindow)
        , resourceProvider_(0)
        , camera_(NULL)
        , bShowIngameGUI_(false)
    {
        using namespace CEGUI;

        COUT(3) << "Initialising CEGUI." << std::endl;

        // Note: No SceneManager specified yet
        guiRenderer_.reset(new OgreCEGUIRenderer(renderWindow_, Ogre::RENDER_QUEUE_OVERLAY, false, 3000));
        resourceProvider_ = guiRenderer_->createResourceProvider();
        resourceProvider_->setDefaultResourceGroup("General");

        // setup scripting
        luaState_.reset(new LuaState());
        rootFileInfo_ = Resource::getInfo("InitialiseGUI.lua");
        // This is necessary to ensure that input events also use the right resource info when triggering lua functions
        luaState_->setDefaultResourceInfo(this->rootFileInfo_);
        scriptModule_.reset(new LuaScriptModule(luaState_->getInternalLuaState()));

        // Create our own logger to specify the filepath
        std::auto_ptr<CEGUILogger> ceguiLogger(new CEGUILogger());
        ceguiLogger->setLogFilename(PathConfig::getLogPathString() + "cegui.log");
        // set the log level according to ours (translate by subtracting 1)
        ceguiLogger->setLoggingLevel(
            static_cast<LoggingLevel>(OutputHandler::getInstance().getSoftDebugLevel("logFile") - 1));
        this->ceguiLogger_ = ceguiLogger.release();

        // create the CEGUI system singleton
        guiSystem_.reset(new System(guiRenderer_.get(), resourceProvider_, 0, scriptModule_.get()));

        // Initialise the basic Lua code
        this->luaState_->doFile("InitialiseGUI.lua");

        // Align CEGUI mouse with OIS mouse
        guiSystem_->injectMousePosition((float)mousePosition.first, (float)mousePosition.second);

        // Hide the mouse cursor unless playing in full screen mode
        if (!bFullScreen)
            CEGUI::MouseCursor::getSingleton().hide();
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
        guiSystem_->injectTimePulse(time.getDeltaTime());
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

        This function gives total access to the GUI. You can execute ANY Lua code here.
    */
    void GUIManager::executeCode(const std::string& str)
    {
        this->luaState_->doString(str, rootFileInfo_);
    }

    /**
    @brief
        Displays specified GUI on screen
    @param name
        The name of the GUI

        The function executes the Lua function with the same name in case the GUIManager is ready.
        For more details check out loadGUI_2.lua where the function presides.
    */
    /*static*/ void GUIManager::showGUI(const std::string& name, bool hidePrevious, bool showCursor)
    {
        GUIManager::getInstance().executeCode("showGUI(\"" + name + "\", " + multi_cast<std::string>(hidePrevious) + ", " + multi_cast<std::string>(showCursor) + ")");
    }

    /**
    @brief
        Hack-ish. Needed for GUIOverlay.
    */
    void GUIManager::showGUIExtra(const std::string& name, const std::string& ptr, bool hidePrevious, bool showCursor)
    {
        this->executeCode("showGUI(\"" + name + "\", " + multi_cast<std::string>(hidePrevious) + ", " + multi_cast<std::string>(showCursor) + ", " + ptr + ")");
    }

    /**
    @brief
        Hides specified GUI.
    @param name
        The name of the GUI.
    */
    /*static*/ void GUIManager::hideGUI(const std::string& name)
    {
        GUIManager::getInstance().executeCode("hideGUI(\"" + name + "\")");
    }

    void GUIManager::keyESC()
    {
        this->executeCode("keyESC()");
    }

    void GUIManager::setBackground(const std::string& name)
    {
        this->executeCode("setBackground(\"" + name + "\")");
    }

    void GUIManager::keyPressed(const KeyEvent& evt)
    {
        guiSystem_->injectKeyDown(evt.getKeyCode());
        guiSystem_->injectChar(evt.getText());
    }
    void GUIManager::keyReleased(const KeyEvent& evt)
    {
        guiSystem_->injectKeyUp(evt.getKeyCode());
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
        try
        {
            guiSystem_->injectMouseButtonDown(convertButton(id));
        }
        catch (CEGUI::ScriptException& ex)
        {
            // We simply ignore the exception and proceed
            COUT(1) << ex.getMessage() << std::endl;
        }
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
        try
        {
            guiSystem_->injectMouseButtonUp(convertButton(id));
        }
        catch (CEGUI::ScriptException& ex)
        {
            // We simply ignore the exception and proceed
            COUT(1) << ex.getMessage() << std::endl;
        }
    }

    void GUIManager::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        guiSystem_->injectMousePosition(static_cast<float>(abs.x), static_cast<float>(abs.y));
    }
    void GUIManager::mouseScrolled(int abs, int rel)
    {
        guiSystem_->injectMouseWheelChange(static_cast<float>(rel));
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

    void GUIManager::subscribeEventHelper(CEGUI::Window* window, const std::string& event, const std::string& function)
    {
        window->subscribeScriptedEvent(event, function);
    }
}
