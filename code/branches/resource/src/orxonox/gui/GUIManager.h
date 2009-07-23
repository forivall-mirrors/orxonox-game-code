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
    Declaration of the GUIManager class.
*/

#ifndef _GUIManager_H__
#define _GUIManager_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <string>
#include <CEGUIForwardRefs.h>
#include <boost/scoped_ptr.hpp>

#include "util/OgreForwardRefs.h"
#include "core/input/InputHandler.h"

// tolua_begin
namespace orxonox
{
    /**
    @class GUIManager
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands. It also acts as a key and mouse handler.

        The GUIManager is a singleton and can be called anywhere when access on the GUI is needed.
        Creation of the GUIManager is therefore not possible and the cunstructor is private.

        Since the GUI needs user input, the GUIManager implements the functions needed to act as a key and/or mouse handler.
        Those input events are then injected into CEGUI in Lua.
    */
    class _OrxonoxExport GUIManager
// tolua_end
        : public InputHandler
// tolua_begin
    {
// tolua_end
    public:
        GUIManager(Ogre::RenderWindow* renderWindow);
        ~GUIManager();

        void update(const Clock& time);

        void showGUI(const std::string& name);
        void executeCode(const std::string& str);

        void setCamera(Ogre::Camera* camera);

        static GUIManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; } // tolua_export
        static GUIManager* getInstancePtr() { return singletonRef_s; }

        void getLevelList(); //tolua_export

    private:
        GUIManager(const GUIManager& instance);                 //!< private constructor (this is a singleton class)

        void loadLuaCode();

        // keyHandler functions
        void keyPressed (const KeyEvent& evt);
        void keyReleased(const KeyEvent& evt);

        // mouseHandler functions
        void buttonPressed (MouseButtonCode::ByEnum id);
        void buttonReleased(MouseButtonCode::ByEnum id);
        void mouseMoved    (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled (int abs, int rel);

        boost::scoped_ptr<CEGUI::OgreCEGUIRenderer> guiRenderer_;  //!< CEGUI's interface to the Ogre Engine
        boost::scoped_ptr<CEGUI::LuaScriptModule>   scriptModule_; //!< CEGUI's script module to use Lua
        boost::scoped_ptr<CEGUI::System>            guiSystem_;    //!< CEGUI's main system
        Ogre::RenderWindow*      renderWindow_;     //!< Ogre's render window to give CEGUI access to it
        CEGUI::ResourceProvider* resourceProvider_; //!< CEGUI's resource provider
        CEGUI::Logger*           ceguiLogger_;      //!< CEGUI's logger to be able to log CEGUI errors in our log
        lua_State*               luaState_;         //!< Lua state, access point to the Lua engine

        static GUIManager*       singletonRef_s;    //!< Singleton reference to GUIManager

    }; // tolua_export
} // tolua_export

#endif /* _GUIManager_H__ */
