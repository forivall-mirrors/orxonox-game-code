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

#ifndef _GSGraphics_H__
#define _GSGraphics_H__

#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include "core/GameState.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _OrxonoxExport GSGraphics : public GameState, public OrxonoxClass
    {
    public:
        GSGraphics();
        ~GSGraphics();

        void setConfigValues();

    private:
        void enter();
        void leave();
        void ticked(float dt, uint64_t time);

        // config values
        float                 debugRefreshTime_;

        Ogre::Root*           ogreRoot_;
        GraphicsEngine*       graphicsEngine_;   //!< our dearest graphics engine <3
        InputManager*         inputManager_;
        InGameConsole*        console_;
        GUIManager*           guiManager_;


        // variables for time statistics
        unsigned long         frameCount_;

        //const unsigned long refreshTime = (unsigned long)(debugRefreshTime_ * 1000000.0f);
        //unsigned long refreshStartTime;
        //unsigned long tickTime;
        //unsigned long oldFrameCount;

        //unsigned long timeBeforeTick;
        //unsigned long timeBeforeTickOld;
        //unsigned long timeAfterTick;
    };
}

#endif /* _GSGraphics_H__ */
