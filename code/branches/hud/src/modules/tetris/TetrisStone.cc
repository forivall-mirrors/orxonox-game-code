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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TetrisStone.cc
    @brief Implementation of the TetrisStone class.
*/

#include "TetrisStone.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Tetris.h"

namespace orxonox
{
    CreateFactory(TetrisStone);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TetrisStone::TetrisStone(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(TetrisStone);
        
        this->size_ = 10.0f;
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisStone::enableMovement, this)));
    }

    /**
    @brief
        Overloaded the function to rotate the stone.
    @param value
        A vector whose first component is the angle by which to rotate.
    */
    void TetrisStone::moveFrontBack(const Vector2& value)
    {
        if(value.x < 0)
        {
            this->setVelocity(this->getVelocity()*1.1);
        }
    }

    /**
    @brief
        Overloaded the function to steer the stone right and left
    @param value
        A vector whose first component is the direction in which we want to steer the stone.
    */
    void TetrisStone::moveRightLeft(const Vector2& value)
    {
        if(!this->delay_)
        {
            const Vector3& position = this->getPosition();
            Vector3 newPos = Vector3(position.x+value.x/abs(value.x)*this->size_, position.y, position.z);
            if(!this->tetris_->isValidMove(this, newPos))
                return;

            this->setPosition(newPos);
            this->delay_ = true;
            this->delayTimer_.startTimer();
        }
    }

    /**
    @brief
        Is called when the player changed.
    */
    void TetrisStone::changedPlayer()
    {
        this->setVelocity(0, 0, 0);
    }
}
