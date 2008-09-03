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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Reto Grieder
 *
 */

/**
    @file Core.h
    @brief Declaration of the Core class.

    The Core class is a singleton, only used to configure some variables
    in the core through the config-file.
*/

#ifndef _Clock_H__
#define _Clock_H__

#include "CorePrereqs.h"
#include <OgrePrerequisites.h>
#include "util/Integers.h"

namespace orxonox
{
    class _CoreExport Clock
    {
        friend class RootGameState;

    public:
        Clock();
        ~Clock();

        uint64_t getMicroseconds()   const { return tickTime_; }
        uint64_t getMilliseconds()   const { return tickTime_ / 1000; }
        int      getSeconds()        const { return tickTime_ / 1000000; }
        float    getSecondsPrecise() const { return (float)tickTime_ / 1000000.0f; }

        float    getDeltaTime()      const { return tickDtFloat_; }
        int      getDeltaTimeMicroseconds() const { return tickDt_; }

        uint64_t getRealMicroseconds() const;

    private:
        Clock(const Clock& instance);
        void capture();

        Ogre::Timer*  timer_;
        uint64_t      storedTime_;
        uint64_t      tickTime_;
        int           tickDt_;
        float         tickDtFloat_;
        unsigned long lastTimersTime_;
    };
}

#endif /* _Clock_H__ */
