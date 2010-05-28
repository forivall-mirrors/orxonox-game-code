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

#ifndef _Clock_H__
#define _Clock_H__

#include "UtilPrereqs.h"
#include "OgreForwardRefs.h"

namespace orxonox
{
    class _UtilExport Clock
    {
    public:
        Clock();
        ~Clock();

        void capture();

        unsigned long long getMicroseconds()   const { return tickTime_; }
        unsigned long long getMilliseconds()   const { return tickTime_ / 1000; }
        unsigned long      getSeconds()        const { return static_cast<long> (tickTime_ / 1000000); }
        float              getSecondsPrecise() const { return static_cast<float>(tickTime_ / 1000000.0f); }

        float              getDeltaTime()      const { return tickDtFloat_; }
        long               getDeltaTimeMicroseconds() const { return tickDt_; }

        unsigned long long getRealMicroseconds() const;

    private:
        Clock(const Clock& instance);

        Ogre::Timer*       timer_;
        unsigned long long tickTime_;
        long               tickDt_;
        float              tickDtFloat_;
    };
}

#endif /* _Clock_H__ */
