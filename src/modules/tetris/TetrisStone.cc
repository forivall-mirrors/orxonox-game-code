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
    @ingroup Tetris
*/

#include "TetrisStone.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Tetris.h"

namespace orxonox
{
    RegisterClass(TetrisStone);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TetrisStone::TetrisStone(Context* context) : MovableEntity(context)
    {
        RegisterObject(TetrisStone);

        this->size_ = 10.0f;
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisStone::enableMovement, this)));
        this->lockRotation_ = false;

    }

    /**
    @brief
        Overloaded the function to rotate the stone.
    @param value
        A vector whose first component is the angle by which to rotate.
    */
    void TetrisStone::moveFrontBack(const Vector2& value)
    {
        if(value.x < 0) //speedup on key down
        {
            this->setVelocity(this->getVelocity()*1.1);
        }
        else if(!this->lockRotation_) //rotate when key up is pressed
        {
            this->lockRotation_ = true; // multiple calls of this function have to be filtered out.
            this->rotationTimer_.setTimer(0.1f, false, createExecutor(createFunctor(&TetrisStone::unlockRotation, this)));
            Quaternion q(Degree(90), Vector3::UNIT_Z);
            this->setOrientation(this->getOrientation()*q); //rotation: roll 90°

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
