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
    @brief Declaration of the GUIManager class.
*/

#ifndef _GUIManager_H__
#define _GUIManager_H__

#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <CEGUIForwardRefs.h>
#include <CEGUIInputEvent.h>
#include <CEGUISystem.h>
#include "core/input/InputInterfaces.h"

// tolua_begin
namespace orxonox
{
    /**
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands
    */
    class _OrxonoxExport GUIManager
// tolua_end
		: public KeyHandler, public MouseHandler
    { // tolua_export
    public:
        enum State
        {
            Uninitialised,
            Ready,
            OnDisplay
        };

        GUIManager();
        ~GUIManager();

        bool initialise(Ogre::RenderWindow* renderWindow);
        void tick(float dt)
        {
            assert(guiSystem_);
            guiSystem_->injectTimePulse(dt);
        }
        void showGUI(const std::string& name, Ogre::SceneManager* sceneManager);// bool showBackground); // tolua_export
        void hideGUI(); // tolua_export

        Ogre::Camera* getCamera() { return this->backgroundCamera_; }

        static void showGUI_s(const std::string& name, Ogre::SceneManager* sceneManager)//bool showBackground)
        {
            getInstance().showGUI(name, sceneManager);
        }

        static GUIManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; } // tolua_export
        static GUIManager* getInstancePtr() { return singletonRef_s; }

    private:
        GUIManager(const GUIManager& instance);

        void keyPressed (const KeyEvent& evt)
        { guiSystem_->injectKeyDown(evt.key); guiSystem_->injectChar(evt.text); }
        void keyReleased(const KeyEvent& evt)
        { guiSystem_->injectKeyUp(evt.key); }
        void keyHeld    (const KeyEvent& evt)
        { }

        void mouseButtonPressed (MouseButton::Enum id);
        void mouseButtonReleased(MouseButton::Enum id);
        void mouseButtonHeld    (MouseButton::Enum id)
        { }
        void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
        { guiSystem_->injectMouseMove(rel.x, rel.y); }
        void mouseScrolled      (int abs, int rel)
        { guiSystem_->injectMouseWheelChange(rel);}

        void tickInput(float dt) { }
        void tickKey(float dt) { }
        void tickMouse(float dt) { }

        void loadScenes();

        //Ogre::SceneManager*       emptySceneManager_;
        Ogre::SceneManager*       backgroundSceneManager_;
        //Ogre::Camera*             emptyCamera_;
        Ogre::Camera*             backgroundCamera_;
        //Ogre::Viewport*           viewport_;
        Ogre::RenderWindow*       renderWindow_;
        CEGUI::OgreCEGUIRenderer* guiRenderer_;
        CEGUI::ResourceProvider*  resourceProvider_;
        CEGUI::LuaScriptModule*   scriptModule_;
        CEGUI::System*            guiSystem_;
        CEGUI::Imageset*          backgroundImage_;
        lua_State*                luaState_;

        State state_;

        static CEGUI::MouseButton convertButton(MouseButton::Enum button);

        static GUIManager*        singletonRef_s;
    }; // tolua_export
} // tolua_export

#endif /* _GUIManager_H__ */
