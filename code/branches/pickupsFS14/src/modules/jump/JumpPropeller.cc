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
    @file JumpPropeller.cc
    @brief If this propeller is created, attachedToFigure_ is set to false. When the figure picks it up, the variable is set to true and the figure starts flying until the fuel is reduced to zero.
*/

#include "JumpPropeller.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPropeller);

    JumpPropeller::JumpPropeller(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpPropeller);

        initialFuel_ = 3.0;
        fuel_ = initialFuel_;
        attachedToFigure_ = false;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    JumpPropeller::~JumpPropeller()
    {

    }

    void JumpPropeller::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPropeller, XMLPort, xmlelement, mode);
    }

    void JumpPropeller::tick(float dt)
    {
        SUPER(JumpPropeller, tick, dt);

        Vector3 PropellerPosition = getWorldPosition();

        if (attachedToFigure_ == false && figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getWorldPosition();

            if(figurePosition.x > PropellerPosition.x-width_ && figurePosition.x < PropellerPosition.x+width_ && figurePosition.z > PropellerPosition.z-height_ && figurePosition.z < PropellerPosition.z+height_)
            {
            	touchFigure();
            }
        }
        else if (attachedToFigure_ == true)
        {
        	fuel_ -= dt;
        	if (fuel_ < 0.0)
        	{
        		figure_->StopPropeller(this);
        	}
        }
    }

    void JumpPropeller::touchFigure()
    {
    	JumpItem::touchFigure();

    	attachedToFigure_ = figure_->StartPropeller(this);
    	if (attachedToFigure_)
    	{
    		setAngularVelocity(Vector3(0,0,10.0));
    	}
    }

    float JumpPropeller::getFuelState()
    {
        return fuel_/initialFuel_;
    }
}
