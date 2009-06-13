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

/**
@file
@brief
    Implementation of the GUIManager class.
*/

#include "GUIManager.h"

extern "C" {
#include <lua.h>
}
#include <CEGUIDefaultLogger.h>
#include <CEGUIExceptions.h>
#include <CEGUIInputEvent.h>
#include <CEGUIResourceProvider.h>
#include <CEGUISystem.h>
#include <ogreceguirenderer/OgreCEGUIRenderer.h>

#include "SpecialConfig.h" // Configures the macro below
#ifdef CEGUILUA_USE_INTERNAL_LIBRARY
#   include <ceguilua/CEGUILua.h>
#else
#   include <CEGUILua.h>
#endif

#include "util/Exception.h"
#include "core/Core.h"
#include "core/Clock.h"
#include "ToluaBindCore.h"
#include "ToluaBindOrxonox.h"
#include "core/Loader.h"

namespace orxonox
{
    static CEGUI::MouseButton convertButton(MouseButtonCode::ByEnum button);
    GUIManager* GUIManager::singletonRef_s = 0;

    GUIManager::GUIManager()
        : renderWindow_(0)
        , guiRenderer_(0)
        , resourceProvider_(0)
        , scriptModule_(0)
        , guiSystem_(0)
        , state_(Uninitialised)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    /**
    @brief
        Deconstructor of the GUIManager

        Basically shuts down CEGUI and destroys the Lua engine and afterwards the interface to the Ogre engine.
    */
    GUIManager::~GUIManager()
    {
        if (guiSystem_)
            delete guiSystem_;

        if (scriptModule_)
        {
            // destroy our own tolua interfaces
            lua_pushnil(luaState_);
            lua_setglobal(luaState_, "Orxonox");
            lua_pushnil(luaState_);
            lua_setglobal(luaState_, "Core");
            delete scriptModule_;
        }

        if (guiRenderer_)
            delete guiRenderer_;

        singletonRef_s = 0;
    }

    /**
    @brief
        Initialises the GUIManager by starting up CEGUI
    @param renderWindow
        Ogre's render window. Without this, the GUI cannot be displayed.
    @return true if success, otherwise false

        Before this call the GUIManager won't do anything, but can be accessed.

        Creates the interface to Ogre, sets up the CEGUI renderer and the Lua script module together with the Lua engine.
        The log is set up and connected to the CEGUILogger.
        After Lua setup tolua++-elements are linked to Lua-state to give Lua access to C++-code.
        Finally initial Lua code is executed (maybe we can do this with the CEGUI startup script automatically).
    */
    bool GUIManager::initialise(Ogre::RenderWindow* renderWindow)
    {
        using namespace CEGUI;
        if (state_ == Uninitialised)
        {
            COUT(3) << "Initialising CEGUI." << std::endl;

            try
            {
                // save the render window
                renderWindow_ = renderWindow;

                // Note: No SceneManager specified yet
                this->guiRenderer_ = new OgreCEGUIRenderer(renderWindow_, Ogre::RENDER_QUEUE_OVERLAY, true, 3000);
                this->resourceProvider_ = guiRenderer_->createResourceProvider();
                this->resourceProvider_->setDefaultResourceGroup("GUI");

                // setup scripting
                this->scriptModule_ = new LuaScriptModule();
                this->luaState_ = this->scriptModule_->getLuaState();

                // Create our own logger to specify the filepath
                this->ceguiLogger_ = new DefaultLogger();
                this->ceguiLogger_->setLogFilename(Core::getLogPathString() + "cegui.log");
                // set the log level according to ours (translate by subtracting 1)
                this->ceguiLogger_->setLoggingLevel(
                    (LoggingLevel)(Core::getSoftDebugLevel(OutputHandler::LD_Logfile) - 1));

                // create the CEGUI system singleton
                this->guiSystem_ = new System(this->guiRenderer_, this->resourceProvider_, 0, this->scriptModule_);

                // do this after 'new CEGUI::Sytem' because that creates the lua state in the first place
                tolua_Core_open(this->scriptModule_->getLuaState());
                tolua_Orxonox_open(this->scriptModule_->getLuaState());

                // initialise the basic lua code
                loadLuaCode();
            }
            catch (CEGUI::Exception& ex)
            {
#if CEGUI_VERSION_MAJOR == 0 && CEGUI_VERSION_MINOR < 6
                throw GeneralException(ex.getMessage().c_str());
#else
                throw GeneralException(ex.getMessage().c_str(), ex.getLine(),
                    ex.getFileName().c_str(), ex.getName().c_str());
#endif
            }

            state_ = Ready;
        }

        return true;
    }

    /**
    @brief
        Calls main Lua script
    @todo
        Replace loadGUI.lua with loadGUI_2.lua after merging this back to trunk.
        However CEGUI is able to execute a startup script. We could maybe put this call in this startup code.

        This function calls the main Lua script for our GUI.

        Additionally we set the datapath variable in Lua. This is needed so Lua can access the data used for the GUI.
    */
    void GUIManager::loadLuaCode()
    {
        try
        {
            // set datapath for GUI data
            lua_pushfstring(this->scriptModule_->getLuaState(), Core::getMediaPathString().c_str());
            lua_setglobal(this->scriptModule_->getLuaState(), "datapath");
            // call main Lua script
            this->scriptModule_->executeScriptFile("loadGUI_3.lua", "GUI");
        }
        catch (CEGUI::Exception& ex)
        {
#if CEGUI_VERSION_MINOR < 6
            throw GeneralException(ex.getMessage().c_str());
#else
            throw GeneralException(ex.getMessage().c_str(), ex.getLine(),
                ex.getFileName().c_str(), ex.getName().c_str());
#endif
        }
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
    void GUIManager::update(const Clock& time)
    {
        assert(guiSystem_);
        guiSystem_->injectTimePulse(time.getDeltaTime());
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
        try
        {
            this->scriptModule_->executeString(str);
        }
        catch (CEGUI::Exception& ex)
        {
            COUT(2) << "CEGUI Error: \"" << ex.getMessage() << "\" while executing code \"" << str << "\"" << std::endl;
        }
    }

    /**

    */
    void GUIManager::getLevelList()
    {
        lua_State* L = this->scriptModule_->getLuaState();
        lua_newtable(L);

        std::vector<std::string> list = Loader::getLevelList();

        int j = 1;
        for (std::vector<std::string>::iterator i = list.begin(); i != list.end(); i++)
        {
            lua_pushnumber(L,j);
            lua_pushstring(L,i->c_str());
            lua_settable(L,-3);
            j++;
        }
        lua_setglobal(L, "levellist");
    }

    /**
    @brief
        Registers a GUIOverlay with the GUIManager so that the GUIOverlay can be accessed by it's name through the GUIManager.
    @param name
        The name of the GUI.
    @param overlay
        A pointer to the GUIOverlay of the GUI.
    @return
        Returns false if the Overlay was already present.
    */
    bool GUIManager::registerOverlay(const std::string& name, GUIOverlay* overlay)
    {
        return (this->guiOverlays_.insert(std::pair<std::string, GUIOverlay*>(name, overlay))).second;
    }

    /**
    @brief
        Get the GUIOverlay of the GUI with the given name.
    @param name
        The name of the GUI.
    @return
        Returns a pointer to the GUIOverlay.
    */
    GUIOverlay* GUIManager::getOverlay(const std::string& name)
    {
        return (this->guiOverlays_.find(name))->second;
    }

    /**
    @brief
        Tells the GUIManager which SceneManager to use
    @param camera
        The current camera on which the GUI should be displayed on.

        In fact the GUIManager needs the SceneManager and not the Camera to display the GUI.
        This means the GUI is not bound to a camera but rather to the SceneManager.
        Hidding the GUI when needed can therefore not be solved by just NOT setting the current camera.
    */
    void GUIManager::setCamera(Ogre::Camera* camera)
    {
        if (camera == NULL)
            this->guiRenderer_->setTargetSceneManager(0);
        else
            this->guiRenderer_->setTargetSceneManager(camera->getSceneManager());
    }

    /**
    @brief
        Displays specified GUI on screen
    @param name
        The name of the GUI

        The function executes the Lua function with the same name in case the GUIManager is ready.
        For more details check out loadGUI_2.lua where the function presides.
    */
    void GUIManager::showGUI(const std::string& name)
    {
        if (state_ != Uninitialised)
        {
            //COUT(3) << "Loading GUI " << name << std::endl;
            try
            {
                this->scriptModule_->executeString(std::string("showGUI(\"") + name + "\")");
            }
            catch (CEGUI::Exception& ex)
            {
                COUT(2) << "Error while executing lua script. Message:\n" << ex.getMessage() << std::endl;
            }
            catch (...)
            {
                COUT(2) << "Could show a menu due to unknown reasons." << std::endl;
            }
        }
        else
        {
            COUT(2) << "Warning: GUI Manager not yet initialised, cannot load a GUI" << std::endl;
        }
    }

    void GUIManager::keyPressed(const KeyEvent& evt)
    {
        guiSystem_->injectKeyDown(evt.key); guiSystem_->injectChar(evt.text);
    }
    void GUIManager::keyReleased(const KeyEvent& evt)
    {
        guiSystem_->injectKeyUp(evt.key);
    }

    /**
    @brief
        Function receiving a mouse button pressed event.
    @param id
        ID of the mouse button which got pressed

        This function is inherited by MouseHandler and injects the event into CEGUI.
        It is for CEGUI to process the event.
    */
    void GUIManager::mouseButtonPressed(MouseButtonCode::ByEnum id)
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
    void GUIManager::mouseButtonReleased(MouseButtonCode::ByEnum id)
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
        guiSystem_->injectMouseMove(rel.x, rel.y);
    }
    void GUIManager::mouseScrolled(int abs, int rel)
    {
        guiSystem_->injectMouseWheelChange(rel);
    }

    /**
    @brief
        converts mouse event code to CEGUI event code
    @param button
        code of the mouse button as we use it in Orxonox
    @return
        code of the mouse button as it is used by CEGUI

        Simple convertion from mouse event code in Orxonox to the one used in CEGUI.
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
}
