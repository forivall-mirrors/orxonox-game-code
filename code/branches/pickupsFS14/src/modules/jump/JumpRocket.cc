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
    @file JumpRocket.cc
    @brief If this rocket is created, attachedToFigure_ is set to false. When the figure picks it up, the variable is set to true and the figure starts flying fast until the fuel is reduced to zero.
*/

#include "JumpRocket.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"
#include "JumpFigure.h"
#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpRocket);

    JumpRocket::JumpRocket(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpRocket);

        initialFuel_ = 3.0;
        fuel_ = initialFuel_;
        attachedToFigure_ = false;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    JumpRocket::~JumpRocket()
    {

    }

    void JumpRocket::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpRocket, XMLPort, xmlelement, mode);
    }

    void JumpRocket::tick(float dt)
    {
        SUPER(JumpRocket, tick, dt);

        Vector3 rocketPosition = getWorldPosition();

        if (attachedToFigure_ == false && figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getWorldPosition();

            if(figurePosition.x > rocketPosition.x-width_ && figurePosition.x < rocketPosition.x+width_ && figurePosition.z > rocketPosition.z-height_ && figurePosition.z < rocketPosition.z+height_)
            {
            	touchFigure();
            }
        }
        else if (attachedToFigure_ == true)
        {
        	fuel_ -= dt;
        	if (fuel_ < 0.0)
        	{
        		figure_->StopRocket(this);
        	}
        }
    }

    void JumpRocket::touchFigure()
    {
    	JumpItem::touchFigure();

    	attachedToFigure_ = figure_->StartRocket(this);
    }

    float JumpRocket::getFuelState()
    {
        return fuel_/initialFuel_;
    }
}
