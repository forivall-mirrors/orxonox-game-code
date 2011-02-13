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
    @ingroup Graphics
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
#include "util/TriBool.h"
#include "util/Singleton.h"
#include "input/InputHandler.h"
#include "OrxonoxClass.h"
#include "WindowEventListener.h"

// Tolua includes (have to be relative to the current directory)
/*
$cfile "../util/TriBool.h" // tolua_export
*/

namespace orxonox // tolua_export
{ // tolua_export
    class PlayerInfo; // Forward declaration

    /**
    @class GUIManager
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands. It also acts as a key and mouse handler.

        The GUIManager is a singleton and can be called anywhere when access on the GUI is needed.

        Since the GUI needs user input, the GUIManager implements the functions needed to act as a key and/or mouse handler.
        Those input events are then injected into CEGUI in Lua.
    */
    class _CoreExport GUIManager // tolua_export
        : public Singleton<GUIManager>, public InputHandler, public WindowEventListener
    { // tolua_export
        friend class Singleton<GUIManager>;
    public:
        GUIManager(const std::pair<int, int>& mousePosition);
        ~GUIManager();

        void setConfigValues(void);
        void changedGUIScheme(void);

        void preUpdate(const Clock& time);

        void loadGUI(const std::string& name);
        static void showGUI(const std::string& name, bool bHidePrevious = false, bool bNoInput = false);
        void showGUIExtra(const std::string& name, const std::string& ptr, bool bHidePrevious = false, bool bNoInput = false);
        static void hideGUI(const std::string& name);
        void keyESC();
        void setBackgroundImage(const std::string& imageSet, const std::string imageName); // tolua_export
        void setBackgroundImage(const std::string& image);

        //! Creates a new InputState to be used with a GUI Sheet
        const std::string& createInputState(const std::string& name, TriBool::Value showCursor = TriBool::True, TriBool::Value useKeyboard = TriBool::True, bool bBlockJoyStick = false); // tolua_export
        LuaState* getLuaState(void)
            { return this->luaState_.get(); }

        //! Returns the root window for all menu sheets
        CEGUI::Window* getMenuRootWindow() { return this->menuRootWindow_; } // tolua_export
        //! Returns the root window for all HUD sheets
        CEGUI::Window* getHUDRootWindow() { return this->hudRootWindow_; } // tolua_export

        void setCamera(Ogre::Camera* camera);
        Ogre::Camera* getCamera() { return this->camera_; }

        inline void setPlayer(const std::string& guiname, PlayerInfo* player)
            { this->players_[guiname] = player; }
        inline orxonox::PlayerInfo* getPlayer(const std::string& guiname) const { std::map<std::string, PlayerInfo*>::const_iterator it = this->players_.find(guiname); return (it != this->players_.end()) ? it->second : 0; } // tolua_export

        // TODO: Temporary hack because the tolua exported CEGUI method does not seem to work
        static void subscribeEventHelper(CEGUI::Window* window, const std::string& event, const std::string& function); //tolua_export
        static void setTooltipTextHelper(CEGUI::ListboxItem* item, const std::string& toooltip); //tolua_export
        static void setItemTooltipsEnabledHelper(CEGUI::Listbox* listbox, bool enabled); //tolua_export

        static GUIManager& getInstance() { return Singleton<GUIManager>::getInstance(); } // tolua_export

    private:
        GUIManager(const GUIManager& instance); //!< private and undefined copy c'tor (this is a singleton class)
        void executeCode(const std::string& str);

        template <typename FunctionType>
        bool protectedCall(FunctionType function);

        // keyHandler functions
        void buttonPressed (const KeyEvent& evt);
        void buttonReleased(const KeyEvent& evt);

        // mouseHandler functions
        void buttonPressed (MouseButtonCode::ByEnum id);
        void buttonReleased(MouseButtonCode::ByEnum id);
        void mouseMoved    (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled (int abs, int rel);
        void mouseLeft     ();

        // window event handler
        virtual void windowResized(unsigned int newWidth, unsigned int newHeight);
        virtual void windowFocusChanged(bool bFocus);

        scoped_ptr<CEGUI::OgreCEGUIRenderer> guiRenderer_;      //!< CEGUI's interface to the Ogre Engine
        scoped_ptr<LuaState>                 luaState_;         //!< LuaState, access point to the Lua engine
        scoped_ptr<CEGUI::LuaScriptModule>   scriptModule_;     //!< CEGUI's script module to use Lua
        scoped_ptr<CEGUI::System>            guiSystem_;        //!< CEGUI's main system
        shared_ptr<ResourceInfo>             rootFileInfo_;     //!< Resource information about the root script
        CEGUI::ResourceProvider*             resourceProvider_; //!< CEGUI's resource provider
        CEGUI::Logger*                       ceguiLogger_;      //!< CEGUI's logger to be able to log CEGUI errors in our log
        CEGUI::Window*                       rootWindow_;       //!< Root node for all windows
        CEGUI::Window*                       hudRootWindow_;    //!< Root node for the HUD sheets
        CEGUI::Window*                       menuRootWindow_;   //!< Root node for the menu sheets (used by Lua)
        std::map<std::string, PlayerInfo*>   players_;          //!< Stores the player (owner) for each GUI
        Ogre::Camera*                        camera_;           //!< Camera used to render the scene with the GUI

        static GUIManager*                   singletonPtr_s;    //!< Singleton reference to GUIManager

        // The used CEGUI scheme.
        static const std::string defaultScheme_;
        std::string guiScheme_;

    }; // tolua_export
} // tolua_export

#endif /* _GUIManager_H__ */
