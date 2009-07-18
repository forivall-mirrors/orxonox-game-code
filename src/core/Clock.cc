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

/**
    @file
    @brief
*/

#include "Clock.h"
#include <OgreTimer.h>

namespace orxonox
{
    Clock::Clock()
        : timer_(new Ogre::Timer())
        , storedTime_(0)
        , tickTime_(0)
        , tickDt_(0)
        , tickDtFloat_(0.0f)
        , lastTimersTime_(0)
    {
    }

    Clock::~Clock()
    {
        delete timer_;
    }
    
    void Clock::capture()
    {
        unsigned long timersTime = timer_->getMicroseconds();
        tickTime_ = storedTime_ + timersTime;
        tickDt_ = timersTime - lastTimersTime_;
        tickDtFloat_ = static_cast<float>(tickDt_) / 1000000.0f;

        if (timersTime > 0xFFFFFFFF/4)
        {
            // Ogre timer will overflow at 2^32 microseconds if unsigned long is 32 bit
            storedTime_ += timersTime;
            lastTimersTime_ = 0;
            timer_->reset();
        }
        else
        {
            lastTimersTime_ = timersTime;
        }
    }

    unsigned long long Clock::getRealMicroseconds() const
    {
        return this->timer_->getMicroseconds() + this->storedTime_;
    }
}
