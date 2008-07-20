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

// forward declarations
namespace CEGUI
{
    class OgreCEGUIRenderer;
    class LuaScriptModule;
}

namespace orxonox // tolua_export
{ // tolua_export
    /**
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands
    */
    class _OrxonoxExport GUIManager : public KeyHandler, public MouseHandler
    /*
    class GUIManager { // tolua_export
    */
    {
    public:
        enum State
        {
            Uninitialised,
            Ready,
            OnDisplay
        };

        bool initialise();
        void tick(float dt);
        void showGUI(const std::string& name, bool showBackground); // tolua_export
        void _hideGUI(); // tolua_export

        static GUIManager& getInstance(); // tolua_export
        static void showGUI_s(const std::string& name, bool showBackground)
        {
            getInstance().showGUI(name, showBackground);
        }

    private:
        GUIManager();
        GUIManager(const GUIManager& instance);
        ~GUIManager();

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

        void loadScenes();

        Ogre::SceneManager*       emptySceneManager_;
        Ogre::SceneManager*       backgroundSceneManager_;
        Ogre::Camera*             emptyCamera_;
        Ogre::Camera*             backgroundCamera_;
        Ogre::Viewport*           viewport_;
        Ogre::RenderWindow*       renderWindow_;
        CEGUI::OgreCEGUIRenderer* guiRenderer_;
        CEGUI::ResourceProvider*  resourceProvider_;
        CEGUI::LuaScriptModule*   scriptModule_;
        CEGUI::System*            guiSystem_;
        CEGUI::Imageset*          backgroundImage_;

        State state_;


        static CEGUI::MouseButton convertButton(MouseButton::Enum button);
    }; // tolua_export

    inline void GUIManager::tick(float dt)
    {
        assert(guiSystem_);
        guiSystem_->injectTimePulse(dt);
    }
} // tolua_export

#endif /* _GUIManager_H__ */
