/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"

#include "core/CoreIncludes.h"
#include "Timer.h"

namespace orxonox
{
    /**
        @brief Constructor: Sets the default-values.
    */
    TimerBase::TimerBase()
    {
        RegisterRootObject(TimerBase);

        this->interval_ = 0;
        this->bLoop_ = false;
        this->bActive_ = false;

        this->time_ = 0;
    }

    /**
        @brief Updates the timer before the frames are rendered.
    */
    void TimerBase::tick(float dt)
    {
        if (this->bActive_)
        {
            // If active: Decrease the timer by the duration of the last frame
            this->time_ -= dt;

            if (this->time_ <= 0)
            {
                // It's time to call the function
                if (this->bLoop_)
                    // Q: Why '+=' and not '='? A: Think about it. It's more accurate like that. Seriously.
                    this->time_ += this->interval_;
                else
                    this->stopTimer(); // Stop the timer if we don't want to loop

                this->run();
            }
        }
    }

}
