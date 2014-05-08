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
    @file JumpProjectile.cc
    @brief Implementation of the JumpProjectile class.
*/

#include "JumpProjectile.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpProjectile);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpProjectile::JumpProjectile(Context* context) : MovableEntity(context)
    {
        RegisterObject(JumpProjectile);

        this->figure_ = 0;

        this->registerVariables();

        this->setPosition(Vector3(0,0,0));
        this->setVelocity(Vector3(0,0,0));
        this->setAcceleration(Vector3(0,0,0));
    }

    /**
    @brief
        Destructor.
    */
    JumpProjectile::~JumpProjectile()
    {
        /*if (this->isInitialized())
        {
            if (this->bDeleteBats_)
                delete this->figure_;

            delete[] this->batID_;
        }*/
    }

    //xml port for loading sounds
    void JumpProjectile::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpProjectile, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Register variables to synchronize over the network.
    */
    void JumpProjectile::registerVariables()
    {
        registerVariable( this->fieldWidth_ );
        registerVariable( this->fieldHeight_ );
        //registerVariable( this->batID_[1], VariableDirection::ToClient, new NetworkCallback<JumpProjectile>( this, &JumpProjectile::applyBats) );
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpProjectile::tick(float dt)
    {
        SUPER(JumpProjectile, tick, dt);

        Vector3 platformPosition = this->getPosition();

        if (figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getPosition();
            Vector3 figureVelocity = figure_->getVelocity();

            if(figureVelocity.z < 0 && figurePosition.x > platformPosition.x-10 && figurePosition.x < platformPosition.x+10 && figurePosition.z > platformPosition.z-4 && figurePosition.z < platformPosition.z+4)
            {
            	touchFigure();
            }
        }
    }

    /**
    @brief
        Set the bats for the ball.
    @param bats
        An array (of size 2) of weak pointers, to be set as the new bats.
    */
    void JumpProjectile::setFigure(WeakPtr<JumpFigure> newFigure)
    {
        figure_ = newFigure;
    }

    void JumpProjectile::accelerateFigure()
    {

    }

    void JumpProjectile::touchFigure()
    {

    }
}
