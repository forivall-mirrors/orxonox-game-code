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
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of the GUIManager class.
*/

#include "OrxonoxStableHeaders.h"
#include "GUIManager.h"

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include <OgreRoot.h>
#include <CEGUI.h>
#include <CEGUILua.h>
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/tolua/tolua_bind.h"
#include "core/ConsoleCommand.h"
#include "core/Exception.h"
#include "core/Core.h"
#include "tolua/tolua_bind.h"
#include "GraphicsEngine.h"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

namespace orxonox
{
    SetConsoleCommandShortcut(GUIManager, showGUI_s).setKeybindMode(KeybindMode::OnPress);

    GUIManager::GUIManager()
        //: emptySceneManager_(0)
        : backgroundSceneManager_(0)
        //, emptyCamera_(0)
        , backgroundCamera_(0)
        //, viewport_(0)
        , renderWindow_(0)
        , guiRenderer_(0)
        , resourceProvider_(0)
        , scriptModule_(0)
        , guiSystem_(0)
        , state_(Uninitialised)
    {
    }

    GUIManager::~GUIManager()
    {
        // TODO: destruct at least something
    }

    bool GUIManager::initialise()
    {
        using namespace CEGUI;
        if (state_ == Uninitialised)
        {
            COUT(3) << "Intialising CEGUI." << std::endl;

            try
            {
                // get the render window
                renderWindow_ = GraphicsEngine::getSingleton().getRenderWindow();

                // Full screen viewport with Z order = 0 (top most). Don't yet feed a camera (so nothing gets rendered)
                //this->viewport_ = renderWindow_->addViewport(0, 3);
                //this->viewport_->setOverlaysEnabled(false);
                //this->viewport_->setShadowsEnabled(false);
                //this->viewport_->setSkiesEnabled(false);
                //this->viewport_->setClearEveryFrame(false);

                // Note: No SceneManager specified yet
                this->guiRenderer_ = new OgreCEGUIRenderer(renderWindow_, Ogre::RENDER_QUEUE_MAIN, true, 3000);
                this->resourceProvider_ = guiRenderer_->createResourceProvider();
                this->resourceProvider_->setDefaultResourceGroup("GUI");
                
                // setup scripting
                this->scriptModule_ = new LuaScriptModule();

                // create the CEGUI system singleton
                this->guiSystem_ = new System(this->guiRenderer_, this->resourceProvider_, 0, this->scriptModule_);
                
                // set the log level according to ours (translate by subtracting 1)
                Logger::getSingleton().setLoggingLevel(
                    (LoggingLevel)(Core::getSoftDebugLevel(OutputHandler::LD_Logfile) - 1));
                
                // do this after 'new CEGUI::Sytem' because that creates the lua state in the first place
                tolua_Core_open(this->scriptModule_->getLuaState());
                tolua_Orxonox_open(this->scriptModule_->getLuaState());

                // register us as input handler
                SimpleInputState* state = InputManager::getInstance().createSimpleInputState("gui", 30);
                state->setHandler(this);
                state->setJoyStickHandler(new EmptyHandler());

                // load the background scene
                loadScenes();
            }
            catch (CEGUI::Exception& ex)
            {
                throw GeneralException(ex.getMessage().c_str(), ex.getLine(),
                    ex.getFileName().c_str(), ex.getName().c_str());
            }

            state_ = Ready;
        }
        
        return true;
    }

    void GUIManager::loadScenes()
    {
        // first of all, we need to have our own SceneManager for the GUI. The reason
        // is that we might have multiple viewports when in play mode (e.g. the view of
        // a camera fixed at the back of the ship). That forces us to create our own
        // full screen viewport that is on top of all the others, but doesn't clear the
        // port before rendering, so everything from the GUI gets on top eventually.
        // But in order to realise that, we also need a SceneManager with an empty scene,
        // because the SceneManager is responsible for the render queue.
        //this->emptySceneManager_ = Ogre::Root::getSingleton()
        //    .createSceneManager(Ogre::ST_GENERIC, "GUI/EmptySceneManager");

        // we also need a camera or we won't see anything at all.
        // The camera settings don't matter at all for an empty scene since the GUI
        // gets rendered on top of the screen rather than into the scene.
        //this->emptyCamera_ = this->emptySceneManager_->createCamera("GUI/EmptyCamera");

        // Create another SceneManager that enables to display some 3D
        // scene in the background of the main menu.
        this->backgroundSceneManager_ = Ogre::Root::getSingleton()
            .createSceneManager(Ogre::ST_GENERIC, "GUI/BackgroundSceneManager");
        this->backgroundCamera_ = backgroundSceneManager_->createCamera("GUI/BackgroundCamera");

        // TODO: create something 3D
        try
        {
            this->scriptModule_->executeScriptFile("loadGUI.lua", "GUI");
        }
        catch (CEGUI::Exception& ex)
        {
            throw GeneralException(ex.getMessage().c_str(), ex.getLine(),
                ex.getFileName().c_str(), ex.getName().c_str());
        }
    }

    void GUIManager::showGUI(const std::string& name, Ogre::SceneManager* sceneManager)// bool showBackground)
    {
        if (state_ != Uninitialised)
        {
            if (state_ == OnDisplay)
                _hideGUI();

            COUT(3) << "Loading GUI " << name << std::endl;
            try
            {
                if (!sceneManager)
                {
                    // currently, only an image is loaded. We could do 3D, see loadBackground.
                    //this->viewport_->setClearEveryFrame(true);
                    this->guiRenderer_->setTargetSceneManager(this->backgroundSceneManager_);
                    //this->viewport_->setCamera(this->backgroundCamera_);

                    lua_pushboolean(this->scriptModule_->getLuaState(), true);
                    lua_setglobal(this->scriptModule_->getLuaState(), "showBackground");
                }
                else
                {
                    //this->viewport_->setClearEveryFrame(false);
                    this->guiRenderer_->setTargetSceneManager(sceneManager);
                    //this->viewport_->setCamera(this->emptyCamera_);

                    lua_pushboolean(this->scriptModule_->getLuaState(), false);
                    lua_setglobal(this->scriptModule_->getLuaState(), "showBackground");
                }

                this->scriptModule_->executeScriptGlobal("showMainMenu");

                InputManager::getInstance().requestEnterState("gui");

                this->state_ = OnDisplay;
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

    void GUIManager::_hideGUI()
    {
        if (this->state_ != OnDisplay)
            return;
        //this->viewport_->setCamera(0);
        this->guiRenderer_->setTargetSceneManager(0);
        this->state_ = Ready;
        InputManager::getInstance().requestLeaveState("gui");
    }

    void GUIManager::mouseButtonPressed(MouseButton::Enum id)
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

    void GUIManager::mouseButtonReleased(MouseButton::Enum id)
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


    /**
    @brief
        Returns a unique instance of GUIManager.
    @return
        The instance
    */
    GUIManager& GUIManager::getInstance()
    {
        static GUIManager instance;
        return instance;
    }

    inline CEGUI::MouseButton GUIManager::convertButton(MouseButton::Enum button)
    {
        switch (button)
        {
        case MouseButton::Left:
            return CEGUI::LeftButton;

        case MouseButton::Right:
            return CEGUI::RightButton;

        case MouseButton::Middle:
            return CEGUI::MiddleButton;

        case MouseButton::Button3:
            return CEGUI::X1Button;

        case MouseButton::Button4:
            return CEGUI::X2Button;

        default:
            return CEGUI::NoButton;
        }
    }
}
