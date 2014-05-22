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
    @file JumpBoots.cc
    @brief Implementation of the JumpBoots class.
*/

#include "JumpBoots.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpBoots);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpBoots::JumpBoots(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpBoots);

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
    JumpBoots::~JumpBoots()
    {

    }

    //xml port for loading sounds
    void JumpBoots::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpBoots, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpBoots::tick(float dt)
    {
        SUPER(JumpBoots, tick, dt);

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
        		figure_->StopBoots(this);
        	}
        }
    }

    void JumpBoots::touchFigure()
    {
    	JumpItem::touchFigure();

    	attachedToFigure_ = figure_->StartBoots(this);
    	if (attachedToFigure_)
    	{
    		//Starte Feuer-Animation
    	}
    }
}
