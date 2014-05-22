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
    @file JumpRocket.cc
    @brief Implementation of the JumpRocket class.
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

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpRocket::JumpRocket(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpRocket);

        fuel_ = 3.0;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    /**
    @brief
        Destructor.
    */
    JumpRocket::~JumpRocket()
    {

    }

    //xml port for loading sounds
    void JumpRocket::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpRocket, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
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
    	if (attachedToFigure_)
    	{
    		//Starte Feuer-Animation
    	}
    }
}
