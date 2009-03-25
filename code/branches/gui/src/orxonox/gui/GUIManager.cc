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
    @brief
        Implementation of the GUIManager class.
*/

#include "OrxonoxStableHeaders.h"
#include "GUIManager.h"

#include <boost/filesystem/path.hpp>
#include <OgreRenderWindow.h>
#include <CEGUI.h>
#include <CEGUIDefaultLogger.h>
#include <ogreceguirenderer/OgreCEGUIRenderer.h>
#include "SpecialConfig.h" // Configures the macro below
#ifdef CEGUILUA_USE_INTERNAL_LIBRARY
#   include <ceguilua/CEGUILua.h>
#else
#   include <CEGUILua.h>
#endif

#include "util/Exception.h"
#include "core/ConsoleCommand.h"
#include "core/Core.h"
#include "core/Clock.h"
#include "ToluaBindCore.h"
#include "ToluaBindOrxonox.h"

extern "C" {
#include <lua.h>
}

namespace orxonox
{
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
                boost::filesystem::path ceguiLogFilepath(Core::getLogPath() / "cegui.log");
                this->ceguiLogger_ = new DefaultLogger();
                this->ceguiLogger_->setLogFilename(ceguiLogFilepath.string());
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

    void GUIManager::loadLuaCode()
    {
        try
        {
            this->scriptModule_->executeScriptFile("loadGUI_2.lua", "GUI");
            lua_pushfstring(this->scriptModule_->getLuaState(), Core::getMediaPathString().c_str());
            lua_setglobal(this->scriptModule_->getLuaState(), "datapath");
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

    void GUIManager::update(const Clock& time)
    {
        assert(guiSystem_);
        guiSystem_->injectTimePulse(time.getDeltaTime());
    }

    void GUIManager::executeCode(const std::string& str)
    {
        this->scriptModule_->executeString(str);
    }

    void GUIManager::setCamera(Ogre::Camera* camera)
    {
        this->guiRenderer_->setTargetSceneManager(camera->getSceneManager());
    }

    void GUIManager::showGUI(const std::string& name)
    {
        if (state_ != Uninitialised)
        {
            COUT(3) << "Loading GUI " << name << std::endl;
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


    inline CEGUI::MouseButton GUIManager::convertButton(MouseButtonCode::ByEnum button)
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
