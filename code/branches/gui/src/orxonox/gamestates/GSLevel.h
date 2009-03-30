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

#ifndef _GSLevel_H__
#define _GSLevel_H__

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"
#include "core/GameState.h"

namespace orxonox
{
    class _OrxonoxExport GSLevel : public GameState, public OrxonoxClass
    {
    public:
        GSLevel(const std::string& name);
        ~GSLevel();
        void setConfigValues();

        void activate();
        void deactivate();
        void update(const Clock& time);

        static void showIngameGUI(bool show);

    protected:
        void loadLevel();
        void unloadLevel();

        // console commands
        void keybind(const std::string& command);
        void tkeybind(const std::string& command);
        void keybindInternal(const std::string& command, bool bTemporary);

        KeyBinder*            keyBinder_;               //!< tool that loads and manages the input bindings
        SimpleInputState*     gameInputState_;          //!< input state for normal ingame playing
        SimpleInputState*     guiMouseOnlyInputState_;  //!< input state if we only need the mouse to use the GUI
        SimpleInputState*     guiKeysOnlyInputState_;   //!< input state if we only need the keys to use the GUI
        Radar*                radar_;                   //!< represents the Radar (not the HUD part)
        XMLFile*              startFile_;               //!< current hard coded default level
        CameraManager*        cameraManager_;
        LevelManager*         levelManager_;
        PlayerManager*        playerManager_;

        //##### ConfigValues #####
        std::string           keyDetectorCallbackCode_;

        // console commands
        ConsoleCommand*       ccKeybind_;
        ConsoleCommand*       ccTkeybind_;
    };
}

#endif /* _GSLevel_H__ */
