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
 *      ...
 *
 */

/**
    @file JumpShield.cc
    @brief Implementation of the JumpShield class.
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

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpShield::JumpShield(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpShield);

        fuel_ = 7.0;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    /**
    @brief
        Destructor.
    */
    JumpShield::~JumpShield()
    {

    }

    //xml port for loading sounds
    void JumpShield::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpShield, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
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
    	if (attachedToFigure_)
    	{
    		//Starte Feuer-Animation
    	}
    }
}
