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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

/**
    @file CountDown.cc
    @brief Countdown HUD element, counting down from CountDown::counter_ to zero.
    
    In use it would like this:
        @code
        <OverlayGroup name="spacefightHUD" scale = "1, 1">
          <CountDown
              position  = "0.49, 0.05"
              pickpoint = "0.0, 0.0"
              font      = "ShareTechMono"
              textsize  = 0.06
              colour    = "1.0, 1.0, 1.0, 1.0"
              align     = "right"                  
              counter   = "10.0"
              active    = "false"
           >
            <events>
              <activity>
                <EventListener event="startcounting" />
              </activity>
            </events>
          </CountDown>
        </OverlayGroup>
        @endcode
        The counter is triggered by an event called "startcounting" and counts from 10 to 0.
*/

#include "CountDown.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "infos/PlayerInfo.h"
#include "util/Convert.h"

namespace orxonox
{
    RegisterClass(CountDown);

    CountDown::CountDown(Context* context) : OverlayText(context)
    {
        RegisterObject(CountDown);

        this->owner_ = 0;
        this->hasStopped_ = false;
    }
    
    CountDown::~CountDown()
    {
    }
    
    void CountDown::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CountDown, XMLPort, xmlelement, mode);
        XMLPortParam(CountDown, "counter", setCounter, getCounter, xmlelement, mode).defaultValues(10);
    }
    
    void CountDown::tick(float dt)
    {
        SUPER(CountDown, tick, dt);
        if (this->isActive() && !this->hasStopped_)
        {
            if (this->counter_ <= 0)
            {
                this->counter_ = 0;
                this->hasStopped_ = true;
                this->setCaption("");
            }
            else
            {
                this->counter_ -= dt;
                this->setCaption(multi_cast<std::string>((int)this->counter_)); //TODO: evtl. initialize with +0.5f
            }
        }
    }

    void CountDown::changedOwner()
    {
        SUPER(CountDown, changedOwner);

        this->owner_ = orxonox_cast<PlayerInfo*>(this->getOwner());
    }
}
