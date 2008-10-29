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
#include <OgrePrerequisites.h>
#include "core/GameState.h"
#include "GSGraphics.h"

namespace orxonox
{
    class _OrxonoxExport GSLevel : public OrxonoxClass //,public GameState<GSGraphics>
    {
        friend class ClassIdentifier<GSLevel>;
    public:
        GSLevel();
        ~GSLevel();

        // this has to be public because proteced triggers a bug in msvc
        // when taking the function address.
        void setTimeFactor(float factor);
        float getTimeFactor() { return this->timeFactor_; }

    protected:
        void enter(Ogre::Viewport* viewport);
        void leave();
        void ticked(const Clock& time);

        void loadLevel();
        void unloadLevel();

        float timeFactor_;       //!< A factor that sets the gamespeed. 1 is normal.

        // console commands
        void keybind(const std::string& command);
        void tkeybind(const std::string& command);
        void keybindInternal(const std::string& command, bool bTemporary);

        KeyBinder*            keyBinder_;        //!< tool that loads and manages the input bindings
        SimpleInputState*     inputState_;
        Radar*                radar_;            //!< represents the Radar (not the HUD part)
        XMLFile*              startFile_;        //!< current hard coded default level
        CameraHandler*        cameraHandler_;
        LevelManager*         levelManager_;

        // config values
        std::string           keyDetectorCallbackCode_;

    private:
        void setConfigValues();

    };
}

#endif /* _GSLevel_H__ */
