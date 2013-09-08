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

#include <fstream>
#include <memory>
#include <boost/bind.hpp>
#include <OgreRenderQueue.h>
#include <OgreRenderWindow.h>

#if CEGUI_VERSION >= 0x000800
#   include <CEGUI/DefaultLogger.h>
#   include <CEGUI/Exceptions.h>
#   include <CEGUI/FontManager.h>
#   include <CEGUI/InputEvent.h>
#   include <CEGUI/MouseCursor.h>
#   include <CEGUI/ResourceProvider.h>
#   include <CEGUI/System.h>
#   include <CEGUI/Window.h>
#   include <CEGUI/WindowManager.h>
#   include <CEGUI/XMLAttributes.h>
#   include <CEGUI/widgets/Listbox.h>
#   include <CEGUI/widgets/ListboxItem.h>
#else
#   include <CEGUIDefaultLogger.h>
#   include <CEGUIExceptions.h>
#   include <CEGUIFontManager.h>
#   include <CEGUIInputEvent.h>
#   include <CEGUIMouseCursor.h>
#   include <CEGUIResourceProvider.h>
#   include <CEGUISystem.h>
#   include <CEGUIWindow.h>
#   include <CEGUIWindowManager.h>
#   include <CEGUIXMLAttributes.h>
#   include <elements/CEGUIListbox.h>
#   include <elements/CEGUIListboxItem.h>
#endif

#ifdef ORXONOX_OLD_CEGUI
#   include <CEGUILua.h>
#   include <ogreceguirenderer/OgreCEGUIRenderer.h>
extern "C" {
#   include <lauxlib.h>
}
#else
#   if CEGUI_VERSION >= 0x000800
#       include <CEGUI/ScriptModules/Lua/ScriptModule.h>
#       include <CEGUI/RendererModules/Ogre/ImageCodec.h>
#       include <CEGUI/RendererModules/Ogre/Renderer.h>
#       include <CEGUI/RendererModules/Ogre/ResourceProvider.h>
#   else
#       include <ScriptingModules/LuaScriptModule/CEGUILua.h>
#       include <RendererModules/Ogre/CEGUIOgreImageCodec.h>
#       include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#       include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#   endif
#   include <OgreCamera.h>
#   include <OgreRenderQueueListener.h>
#   include <OgreRenderSystem.h>
#   include <OgreRoot.h>
#   include <OgreSceneManager.h>
#endif

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(ORXONOX_COMPILER_MINGW)
#  include <windows.h>
#endif

#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Output.h"
#include "util/Exception.h"
#include "util/Math.h"
#include "util/OrxAssert.h"
#include "util/output/BaseWriter.h"
#include "config/ConfigValueIncludes.h"
#include "Core.h"
#include "CoreIncludes.h"
#include "Game.h"
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
            OutputLevel orxonoxLevel = level::debug_output;
            switch (level)
            {
                case CEGUI::Errors:      orxonoxLevel = level::internal_error; break;
                case CEGUI::Warnings:    orxonoxLevel = level::internal_warning; break;
                case CEGUI::Standard:    orxonoxLevel = level::verbose; break;
                case CEGUI::Informative: orxonoxLevel = level::verbose_more; break;
                case CEGUI::Insane:      orxonoxLevel = level::verbose_ultra; break;
                default: OrxAssert(false, "CEGUI log level out of range, inspect immediately!");
            }

            orxout(orxonoxLevel, context::cegui) << message << endl;

            CEGUI::DefaultLogger::logEvent(message, level);
        }

        /// Carbon copy from CEGUIDefaultLogger.cpp with a bugfix for Windows
        void setLogFilename(const CEGUI::String& filename, bool append = false)
        {
            // Close current log file (if any)
            if (d_ostream.is_open())
                d_ostream.close();

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(ORXONOX_COMPILER_MINGW)
            // filename.c_str() is UTF-8 encoded, but Windows expects characters
            // according to the current codepage or UTF-16 (wchar)
            d_ostream.open(utf8ToUtf16(filename.c_str()).c_str(), std::ios_base::out | (append ? std::ios_base::app : std::ios_base::trunc));
#else
            d_ostream.open(filename.c_str(), std::ios_base::out | (append ? std::ios_base::app : std::ios_base::trunc));
#endif
            if (!d_ostream)
                ThrowException(General, "Setting the CEGUI log filename failed");

            // Initialise width for date & time alignment.
            d_ostream.width(2);

            // Write out cached log strings.
            if (d_caching)
            {
                d_caching = false;

                std::vector<std::pair<CEGUI::String, CEGUI::LoggingLevel> >::iterator it = d_cache.begin();

                while (it != d_cache.end())
                {
                    if (d_level >= it->second)
                    {
                        d_ostream << it->first;
                        // Ensure new event is written to the file, rather than just being buffered.
                        d_ostream.flush();
                    }
                    ++it;
                }

                d_cache.clear();
            }
        }

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(ORXONOX_COMPILER_MINGW)
        /// Converts a UTF-8 character sequence to Windows UTF-16
        static std::wstring utf8ToUtf16(const std::string& utf8text)
        {
            const int textLen = MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(),
                utf8text.size() + 1, 0, 0);

            if (textLen == 0)
                ThrowException(General, "Utf8ToUtf16 - MultiByteToWideChar failed");

            std::wstring wideStr(textLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(), utf8text.size() + 1,
                &wideStr[0], wideStr.size());
            return wideStr;
        }
#endif
    };

#ifdef ORXONOX_OLD_CEGUI
    /** Class with the same memory layout as CEGUI::LuaScriptModule. <br>
        We need this to fix a problem with an uninitialised member variable
        in CEGUI < 0.7 <br>
        Notice the "public" modifier for the otherwise private variables.
    */
    class LuaScriptModuleWorkaround : public CEGUI::ScriptModule
    {
    public:
        LuaScriptModuleWorkaround();
        ~LuaScriptModuleWorkaround();

    public:
        bool d_ownsState;
        lua_State* d_state;
        CEGUI::String d_errFuncName;
        int d_errFuncIndex;
        CEGUI::String d_activeErrFuncName;
        int d_activeErrFuncIndex;
    };
#else
    /// RenderQueueListener based class used to hook into the ogre rendering system
    class RQListener : public Ogre::RenderQueueListener
    {
    public:
        /// Callback from Ogre invoked before other stuff in our target queue is rendered
        void renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue)
        {
            if (id == Ogre::RENDER_QUEUE_OVERLAY && invocation.empty())
            {
                CEGUI::System::getSingleton().renderGUI();

                // Important workaround! (at least required by CEGUI 0.7.5)
                // If we don't reset the scissor test, OGRE will only render overlays
                // in the area where CEGUI last drew, which is usually nothing
                // or a little box where the focused element is.
                Ogre::Root::getSingleton().getRenderSystem()->setScissorTest(false);
            }
        }
    };
#endif

    static CEGUI::MouseButton convertButton(MouseButtonCode::ByEnum button);

    GUIManager* GUIManager::singletonPtr_s = 0;
    /*static*/ const std::string GUIManager::defaultScheme_ = "TaharezGreen"; //Alternative: Orxonox (not fully complete yet, see the graphics menu)

    SetConsoleCommand("showGUI", &GUIManager::showGUI).defaultValue(1, false).defaultValue(2, false);
    SetConsoleCommand("hideGUI", &GUIManager::hideGUI);
    SetConsoleCommand("toggleGUI", &GUIManager::toggleGUI).defaultValue(1, false).defaultValue(2, false);

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
        : guiRenderer_(NULL)
        , resourceProvider_(NULL)
#ifndef ORXONOX_OLD_CEGUI
        , rqListener_(NULL)
        , imageCodec_(NULL)
#endif
        , luaState_(NULL)
        , scriptModule_(NULL)
        , guiSystem_(NULL)
        , ceguiLogger_(NULL)
        , rootWindow_(NULL)
        , hudRootWindow_(NULL)
        , menuRootWindow_(NULL)
        , camera_(NULL)
        , destructionHelper_(this)
    {
        RegisterObject(GUIManager);

        orxout(internal_status) << "initializing GUIManager..." << endl;

        this->setConfigValues();

        using namespace CEGUI;

        orxout(internal_info) << "Initialising CEGUI." << endl;

        this->oldCEGUI_ = false;

        // Note: No SceneManager specified yet
#ifdef ORXONOX_OLD_CEGUI
        guiRenderer_ = new OgreCEGUIRenderer(GraphicsManager::getInstance().getRenderWindow(), Ogre::RENDER_QUEUE_OVERLAY, false, 3000);
        resourceProvider_ = guiRenderer_->createResourceProvider();
        this->oldCEGUI_ = true;
#else
        guiRenderer_ = &OgreRenderer::create(*GraphicsManager::getInstance().getRenderWindow());
        // We use our own RenderQueueListener so we can draw UNDER overlays
        guiRenderer_->setFrameControlExecutionEnabled(false);
        rqListener_ = new RQListener();
        resourceProvider_ = &OgreRenderer::createOgreResourceProvider();
        imageCodec_ = &OgreRenderer::createOgreImageCodec();
#endif
        resourceProvider_->setDefaultResourceGroup("General");

        // Setup scripting
        luaState_ = new LuaState();
        rootFileInfo_ = Resource::getInfo("InitialiseGUI.lua");
        // This is necessary to ensure that input events also use the right resource info when triggering lua functions
        luaState_->setDefaultResourceInfo(this->rootFileInfo_);
#ifdef ORXONOX_OLD_CEGUI
        scriptModule_ = new LuaScriptModule(luaState_->getInternalLuaState());
        // Ugly workaround: older CEGUILua versions don't initialise the member
        // d_activeErrFuncIndex at all. That leads to "error in error handling"
        // problems when a Lua error occurs.
        // We fix this by setting the member manually.
        reinterpret_cast<LuaScriptModuleWorkaround*>(scriptModule_)->d_activeErrFuncIndex = LUA_NOREF;
        luaState_->doString("ORXONOX_OLD_CEGUI = true");
#else
        scriptModule_ = &LuaScriptModule::create(luaState_->getInternalLuaState());
#endif
        scriptModule_->setDefaultPCallErrorHandler(LuaState::ERROR_HANDLER_NAME);

        // Create our own logger to specify the filepath
        std::auto_ptr<CEGUILogger> ceguiLogger(new CEGUILogger());
        ceguiLogger->setLogFilename(PathConfig::getLogPathString() + "cegui.log");
        ceguiLogger->setLoggingLevel(static_cast<CEGUI::LoggingLevel>(this->outputLevelCeguiLog_));
        this->ceguiLogger_ = ceguiLogger.release();

        // Create the CEGUI system singleton
#ifdef ORXONOX_OLD_CEGUI
        guiSystem_ = new System(guiRenderer_, resourceProvider_, 0, scriptModule_);
        // Add functions that have been renamed in newer versions
        luaState_->doString("CEGUI.SchemeManager.create = CEGUI.SchemeManager.loadScheme");
        luaState_->doString("CEGUI.Window.getUnclippedOuterRect = CEGUI.Window.getUnclippedPixelRect");
        luaState_->doString("CEGUI.ImagesetManager.createFromImageFile= CEGUI.ImagesetManager.createImagesetFromImageFile");
#else
        guiSystem_ = &System::create(*guiRenderer_, resourceProvider_, 0, imageCodec_, scriptModule_);
#endif

        CEGUI::String defaultXMLParserName = CEGUI::System::getSingleton().getDefaultXMLParserName();
        try
        {
            // Force Xerces parser (CEGUI 0.7.5+)
            CEGUI::System::getSingleton().setXMLParser("XercesParser");
        }
        catch (const CEGUI::GenericException& e)
        {
            // Fall back to default parser
            orxout(internal_warning) << "Cannot use XercesParser for CEGUI - using " << defaultXMLParserName << " instead" << endl;
            CEGUI::System::getSingleton().setXMLParser(defaultXMLParserName);
        }

        // Align CEGUI mouse with OIS mouse
        guiSystem_->injectMousePosition((float)mousePosition.first, (float)mousePosition.second);

        // Initialise the Lua framework and load the schemes
        orxout(user_info) << "Loading user interface..." << endl;
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

        orxout(internal_status) << "finished initializing GUIManager" << endl;
    }

    void GUIManager::destroy()
    {
        orxout(internal_status) << "destroying GUIManager..." << endl;

        using namespace CEGUI;

#ifdef ORXONOX_OLD_CEGUI
        safeObjectDelete(&guiSystem_);
        safeObjectDelete(&guiRenderer_);
        safeObjectDelete(&scriptModule_);
#else
        System::destroy();
        OgreRenderer::destroyOgreResourceProvider(*resourceProvider_);
        OgreRenderer::destroyOgreImageCodec(*imageCodec_);
        OgreRenderer::destroy(*guiRenderer_);
        LuaScriptModule::destroy(*scriptModule_);
        safeObjectDelete(&ceguiLogger_);
        safeObjectDelete(&rqListener_);
#endif
        safeObjectDelete(&luaState_);

        orxout(internal_status) << "finished destroying GUIManager" << endl;
    }

    void GUIManager::setConfigValues(void)
    {
        SetConfigValue(guiScheme_, GUIManager::defaultScheme_).description("Changes the current GUI scheme.").callback(this, &GUIManager::changedGUIScheme);
        SetConfigValue(numScrollLines_, 1).description("How many lines to scroll in a list if the scroll wheel is used");
        SetConfigValue(bPreloadMenuSheets_, false).description("Pre-load menu sheets during startup");

        SetConfigValueExternal(outputLevelCeguiLog_, BaseWriter::getConfigurableSectionName(), "outputLevelCeguiLog", CEGUI::Standard).description("The log level of the CEGUI log file").callback(this, &GUIManager::changedCeguiOutputLevel);
    }

    void GUIManager::changedGUIScheme(void)
    {
    }

    void GUIManager::changedCeguiOutputLevel()
    {
        if (this->ceguiLogger_)
            this->ceguiLogger_->setLoggingLevel(static_cast<CEGUI::LoggingLevel>(this->outputLevelCeguiLog_));
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
#ifdef ORXONOX_OLD_CEGUI
        if (camera == NULL)
            this->guiRenderer_->setTargetSceneManager(0);
        else
            this->guiRenderer_->setTargetSceneManager(camera->getSceneManager());
#else
        if (camera_ != NULL && camera_->getSceneManager() != NULL)
            camera_->getSceneManager()->removeRenderQueueListener(rqListener_);
        if (camera != NULL && camera->getSceneManager() != NULL)
            camera->getSceneManager()->addRenderQueueListener(rqListener_);
#endif
        this->camera_ = camera;
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

    /**
    @brief
        Toggles specified GUI.
        If the GUI with the input name is already shown and on the top, it is hidden, else it is shown.
    */
    /*static*/ void GUIManager::toggleGUI(const std::string& name, bool bHidePrevious, bool bNoInput)
    {
        GUIManager::getInstance().executeCode("getGUIFirstActive(\"" + name + "\", " + multi_cast<std::string>(bHidePrevious) + ", " + multi_cast<std::string>(bNoInput) + ")");
    }

    /**
    @brief
        Helper method to toggle a specified GUI.
        Is called by lua.
    */
    void GUIManager::toggleGUIHelper(const std::string& name, bool bHidePrevious, bool bNoInput, bool show)
    {
        if(show)
            GUIManager::showGUI(name, bHidePrevious, bNoInput);
        else
            GUIManager::hideGUI(name);
    }

    const std::string& GUIManager::createInputState(const std::string& name, tribool showCursor, tribool useKeyboard, bool bBlockJoyStick)
    {
        InputState* state = InputManager::getInstance().createInputState(name);
        if (!state)
            return BLANKSTRING;

        /* Table that maps isFullScreen() and showCursor to mouseExclusive
        isFullscreen / showCursor | True  | False | Dontcare
        ----------------------------------------------------
        true                      | True  | True  | Dontcare
        ----------------------------------------------------
        false                     | False | True  | Dontcare
        */

#ifdef ORXONOX_PLATFORM_APPLE
        // There is no non exclusive mode on OS X yet
        state->setMouseExclusive(true);
#else
        if (showCursor == dontcare)
            state->setMouseExclusive(dontcare);
        else if (GraphicsManager::getInstance().isFullScreen() || showCursor == false)
            state->setMouseExclusive(true);
        else
            state->setMouseExclusive(false);
#endif

        if (showCursor == true)
            state->setMouseHandler(this);
        else if (showCursor == false)
            state->setMouseHandler(&InputHandler::EMPTY);

        if (useKeyboard == true)
            state->setKeyHandler(this);
        else if (useKeyboard == false)
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
        this->protectedCall(boost::bind(&CEGUI::System::injectMouseWheelChange, _1, (float)sgn(rel) * this->numScrollLines_));
    }

    /**
        @brief Indicates that the mouse left the application's window.
    */
    void GUIManager::mouseLeft()
    {
        this->protectedCall(boost::bind(&CEGUI::System::injectMouseLeaves, _1));
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
            orxout(internal_error) << ex.getMessage() << endl;
            return true;
        }
    }

    /**
    @brief
        Subscribe the input function to the input event for the input window.
        This is a helper to be used in lua, because subscribeScriptedEvent() doesn't work in lua.
    @param window
        The window for which the event is subscribed.
    @param event
        The type of event to which we subscribe.
    @param function
        The function that is called when the event occurs.
    */
    void GUIManager::subscribeEventHelper(CEGUI::Window* window, const std::string& event, const std::string& function)
    {
        window->subscribeScriptedEvent(event, function);
    }

    /**
    @brief
        Set the input tooltip text for the input ListboxItem.
    @param item
        The ListboxItem for which the tooltip should be set.
    @param tooltip
        The tooltip text that should be set.
    */
    void GUIManager::setTooltipTextHelper(CEGUI::ListboxItem* item, const std::string& tooltip)
    {
        item->setTooltipText(tooltip);
    }

    /**
    @brief
        Set whether the tooltips for the input Listbox are enabled.
    @param listbox
        The Listbox for which to enable (or disable) tooltips.
    @param enabled
        Whether to enable or disable the tooltips.
    */
    void GUIManager::setItemTooltipsEnabledHelper(CEGUI::Listbox* listbox, bool enabled)
    {
        listbox->setItemTooltipsEnabled(enabled);
    }

    /** Helper method to get the developer's mode without having to export Core.h.
    @see Core::inDevMode
    */
    /*static*/ bool GUIManager::inDevMode()
    {
         return Core::getInstance().inDevMode();
    }

    /**
        @brief Callback of window event listener, called if the window is resized. Sets the display size of CEGUI.
    */
    void GUIManager::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        this->guiRenderer_->setDisplaySize(CEGUI::Size((float)newWidth, (float)newHeight));
    }

    /**
        @brief Notify CEGUI if the windows loses the focus (stops highlighting of menu items, etc).
    */
    void GUIManager::windowFocusChanged(bool bFocus)
    {
        if (!bFocus)
            this->mouseLeft();
    }

    /**
    @brief
        Adds a new freetype font to the CEGUI system.
    @param name
        The name of the new font.
    @param size
        The font size of the new font in pixels.
        @param fontName
        The filename of the font.
    */
    /*static*/ void GUIManager::addFontHelper(const std::string& name, int size, const std::string& fontName)
    {
#ifdef ORXONOX_OLD_CEGUI
        if(CEGUI::FontManager::getSingleton().isFontPresent(name)) // If a font with that name already exists.
            return;

        CEGUI::Font* font = NULL;
        CEGUI::XMLAttributes xmlAttributes;

        // Attributes specified within CEGUIFont
        xmlAttributes.add("Name", name);
        xmlAttributes.add("Filename", fontName);
        xmlAttributes.add("ResourceGroup", "");
        xmlAttributes.add("AutoScaled", "true");
        xmlAttributes.add("NativeHorzRes", "800");
        xmlAttributes.add("NativeVertRes", "600");

        // Attributes specified within CEGUIXMLAttributes
        xmlAttributes.add("Size", multi_cast<std::string>(size));
        xmlAttributes.add("AntiAlias", "true");

        font = CEGUI::FontManager::getSingleton().createFont("FreeType", xmlAttributes);
        if(font != NULL)
            font->load();
#else
        if(CEGUI::FontManager::getSingleton().isDefined(name)) // If a font with that name already exists.
            return;

        CEGUI::FontManager::getSingleton().createFreeTypeFont(name, (float)size, true, fontName, "", true, 800.0f, 600.0f);
#endif
    }

}
