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
    @file JumpSpring.cc
    @brief Implementation of the JumpSpring class.
*/

#include "JumpSpring.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpSpring);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpSpring::JumpSpring(Context* context) : JumpItem(context)
    {
        RegisterObject(JumpSpring);

        stretch_ = 1.0;

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));

        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    /**
    @brief
        Destructor.
    */
    JumpSpring::~JumpSpring()
    {

    }

    //xml port for loading sounds
    void JumpSpring::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpSpring, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpSpring::tick(float dt)
    {
        SUPER(JumpSpring, tick, dt);

        if (stretch_ > 1.0)
        {
        	stretch_ -= dt;
        	setScale3D(1.0, 1.0, stretch_);
        }

        Vector3 springPosition = getWorldPosition();

        if (figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getWorldPosition();
            Vector3 figureVelocity = figure_->getVelocity();

            if(figureVelocity.z < 0 && figurePosition.x > springPosition.x-width_ && figurePosition.x < springPosition.x+width_ && figurePosition.z > springPosition.z-height_ && figurePosition.z < springPosition.z+height_)
            {
            	touchFigure();
            }
        }
    }

    void JumpSpring::touchFigure()
    {
    	JumpItem::touchFigure();

    	stretch_ = 3.0;

    	accelerateFigure();
    }

    void JumpSpring::accelerateFigure()
    {
    	if (figure_ != 0)
    	{
			figure_->JumpFromSpring(this);
    	}
    }
}
