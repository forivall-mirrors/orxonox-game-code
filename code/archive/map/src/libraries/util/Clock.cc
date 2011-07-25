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

#include "Clock.h"
#include <OgreTimer.h>

namespace orxonox
{
    Clock::Clock()
        : timer_(new Ogre::Timer())
        , tickTime_(0)
        , tickDt_(0)
        , tickDtFloat_(0.0f)
    {
    }

    Clock::~Clock()
    {
        delete timer_;
    }

    /**
    @remarks
        Mind the types! Ogre::Timer::getMicroseconds() will return an unsigned
        long, which will eventually overflow. But if you use the subtraction of
        the current time minus the last time the timer gave us and sum these up to
        a 64 bit integer, we get the desired result.
        Also mind that we don't have to store the last timer's time as unsigned long
        as well because (unsigned long)tickTime_ will do exactly that.
    */
    void Clock::capture()
    {
        tickDt_ = timer_->getMicroseconds() - (unsigned long)tickTime_;
        tickTime_ += tickDt_;
        tickDtFloat_ = static_cast<float>(tickDt_) / 1000000.0f;
    }

    unsigned long long Clock::getRealMicroseconds() const
    {
        return tickTime_ + (timer_->getMicroseconds() - (unsigned long)tickTime_);
    }
}
