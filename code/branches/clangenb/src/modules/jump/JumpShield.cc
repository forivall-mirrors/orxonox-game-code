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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpShield.cc
    @brief If this shield is created, attachedToFigure_ is set to false. When the figure picks it up, the variable is set to true and the figure is protected until the fuel is reduced to zero.
*/

#include "JumpShield.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpShield);

    JumpShield::JumpShield(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpShield);

        initialFuel_ = 7.0;
        fuel_ = initialFuel_;
        attachedToFigure_ = false;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    JumpShield::~JumpShield()
    {

    }

    void JumpShield::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpShield, XMLPort, xmlelement, mode);
    }

    void JumpShield::tick(float dt)
    {
        SUPER(JumpShield, tick, dt);

        Vector3 shieldPosition = getWorldPosition();

        if (attachedToFigure_ == false && figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getWorldPosition();

            if(figurePosition.x > shieldPosition.x-width_ && figurePosition.x < shieldPosition.x+width_ && figurePosition.z > shieldPosition.z-height_ && figurePosition.z < shieldPosition.z+height_)
            {
                touchFigure();
            }
        }
        else if (attachedToFigure_ == true)
        {
            fuel_ -= dt;
            if (fuel_ < 0.0)
            {
                figure_->StopShield(this);
            }
        }
    }

    void JumpShield::touchFigure()
    {
        JumpItem::touchFigure();

        attachedToFigure_ = figure_->StartShield(this);
    }

    float JumpShield::getFuelState()
    {
        return fuel_/initialFuel_;
    }
}
