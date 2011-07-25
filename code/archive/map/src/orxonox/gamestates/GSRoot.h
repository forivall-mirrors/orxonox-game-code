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

#ifndef _GSRoot_H__
#define _GSRoot_H__

#include "OrxonoxPrereqs.h"
#include "core/GameState.h"

namespace orxonox
{
    class _OrxonoxExport GSRoot : public GameState
    {
    public:
        GSRoot(const GameStateInfo& info);
        ~GSRoot();

        static void printObjects();

        void activate();
        void deactivate();
        void update(const Clock& time);

        // this has to be public because proteced triggers a bug in msvc
        // when taking the function address.
        void setTimeFactor(float factor);
        void pause();
        float getTimeFactor();

    private:
        bool                  bPaused_;
        float                 timeFactorPauseBackup_;
    };
}

#endif /* _GSRoot_H__ */
