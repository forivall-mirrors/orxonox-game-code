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

#ifndef _GUIManager_H__
#define _GUIManager_H__

#include "CorePrereqs.h"

#include <map>
#include <set>
#include <string>
#include <CEGUIForwardRefs.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "util/OgreForwardRefs.h"
#include "util/Singleton.h"
#include "input/InputHandler.h"

namespace orxonox // tolua_export
{ // tolua_export
    class PlayerInfo; // Forward declaration

    /**
    @class GUIManager
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands. It also acts as a key and mouse handler.

        The GUIManager is a singleton and can be called anywhere when access on the GUI is needed.
        Creation of the GUIManager is therefore not possible and the cunstructor is private.

        Since the GUI needs user input, the GUIManager implements the functions needed to act as a key and/or mouse handler.
        Those input events are then injected into CEGUI in Lua.
    */
    class _CoreExport GUIManager // tolua_export
        : public Singleton<GUIManager>, public InputHandler
    { // tolua_export
        friend class Singleton<GUIManager>;
    public:
        GUIManager(Ogre::RenderWindow* renderWindow, const std::pair<int, int>& mousePosition, bool bFullScreen);
        ~GUIManager();

        void preUpdate(const Clock& time);

        static void showGUI(const std::string& name, bool hidePrevious=false, bool showCursor=true);
        void showGUIExtra(const std::string& name, const std::string& ptr, bool hidePrevious=false, bool showCursor=true);
        static void hideGUI(const std::string& name);
        void keyESC();
        void setBackground(const std::string& name);

        void setCamera(Ogre::Camera* camera);
        Ogre::Camera* getCamera() { return this->camera_; }

        static GUIManager* getInstancePtr() { return singletonPtr_s; }

        inline void setPlayer(const std::string& guiname, PlayerInfo* player)
            { this->players_[guiname] = player; }
        inline PlayerInfo* getPlayer(const std::string& guiname) const
            { std::map<std::string, PlayerInfo*>::const_iterator it = this->players_.find(guiname); return (it != this->players_.end()) ? it->second : 0; }

        // TODO: Temporary hack because the tolua exported CEGUI method does not seem to work
        static void subscribeEventHelper(CEGUI::Window* window, const std::string& event, const std::string& function); //tolua_export

    private:
        GUIManager(const GUIManager& instance); //!< private and undefined copy c'tor (this is a singleton class)

        void executeCode(const std::string& str);

        // keyHandler functions
        void keyPressed (const KeyEvent& evt);
        void keyReleased(const KeyEvent& evt);

        // mouseHandler functions
        void buttonPressed (MouseButtonCode::ByEnum id);
        void buttonReleased(MouseButtonCode::ByEnum id);
        void mouseMoved    (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled (int abs, int rel);

        scoped_ptr<CEGUI::OgreCEGUIRenderer> guiRenderer_;      //!< CEGUI's interface to the Ogre Engine
        scoped_ptr<LuaState>                 luaState_;         //!< LuaState, access point to the Lua engine
        scoped_ptr<CEGUI::LuaScriptModule>   scriptModule_;     //!< CEGUI's script module to use Lua
        scoped_ptr<CEGUI::System>            guiSystem_;        //!< CEGUI's main system
        shared_ptr<ResourceInfo>             rootFileInfo_;     //!< Resource information about the root script
        Ogre::RenderWindow*                  renderWindow_;     //!< Ogre's render window to give CEGUI access to it
        CEGUI::ResourceProvider*             resourceProvider_; //!< CEGUI's resource provider
        CEGUI::Logger*                       ceguiLogger_;      //!< CEGUI's logger to be able to log CEGUI errors in our log
        std::map<std::string, PlayerInfo*>   players_;          //!< Stores the player (owner) for each gui
        Ogre::Camera*                        camera_;           //!< Camera used to render the scene with the GUI

        static GUIManager*                   singletonPtr_s;    //!< Singleton reference to GUIManager
        bool                                 bShowIngameGUI_;

    }; // tolua_export
} // tolua_export

#endif /* _GUIManager_H__ */
