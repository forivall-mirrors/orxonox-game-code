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
    @file TetrisBrick.cc
    @brief Implementation of the TetrisBrick class.
*/

#include "TetrisBrick.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "TetrisCenterpoint.h"
#include "TetrisStone.h"
#include "Tetris.h"

namespace orxonox
{
    CreateFactory(TetrisBrick);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TetrisBrick::TetrisBrick(BaseObject* creator): ControllableEntity(creator)
    {
        RegisterObject(TetrisBrick);

        this->shapeIndex_ = 1; //<! TODO: random number between 0 and 7
        this->stonesPerBrick_ = 4; //<! most tetris bricks is formed by 4 stones
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisBrick::enableMovement, this)));
        this->lockRotation_ = false;
        this->tetris_ = this->getTetris();
        this->size_ = 10.0f; //TODO: fix this via this->tetris_->center_->getStoneSize();
        this->rotationCount_ = 0;
        this->createBrick(); //<! create a whole new Brick;
    }

    /**
    @brief
        This function partly initializes a TetrisBrick as an array of TetrisStones
    */
    void TetrisBrick::createBrick(void)
    { //Index 0 : single stone, 1 : 4 in a row; 2: 4-Block right shifted; 3: 'T' 4: 4-Block left shifted;
      //Index 5 : 4-Block; 6 : 'L'; 7 : mirrored 'L';
        orxout()<< "TetrisBrick::createBrick" << endl;
        if(this->shapeIndex_ == 0)
            this->stonesPerBrick_ = 1;
        for (unsigned int i = 0; i < this->stonesPerBrick_; i++)
        {
            // Create a new stone and add it to the brick.
            TetrisStone* stone = new TetrisStone(this);
            stone->setHealth(1.0f);
            this->brickStones_.push_back(stone);
            this->attach(stone);
            this->formBrick(stone, i);
            if(this->tetris_ != NULL)
            {
                stone->setGame(this->tetris_);
                if(this->tetris_->getCenterpoint() != NULL)
                    stone->addTemplate(this->tetris_->getCenterpoint()->getStoneTemplate());
                else
                	orxout()<< "tetris_->getCenterpoint == NULL in TetrisBrick.cc"<< endl;
            }
            else
                orxout()<< "tetris_ == NULL in TetrisBrick.cc"<< endl;
        }
    }

    /**
    @brief
        This function creates the shape of a TetrisBrick. ! Spaghetti-Code !
    @param i
        The stone's number.
    @param stone
        The TetrisStone that is placed relative to the brick's position.
    */
    void TetrisBrick::formBrick(TetrisStone* stone, unsigned int i)
    {
    	if(i != 0 && this->shapeIndex_ == 0)
    	    orxout() << "So it has come to this in TetrisBrick.cc"<< endl;
        if(i == 0) //setting the first stone as
        {
            stone->setPosition(0.0f, 0.0f, 0.0f);
        }
        else if(i == 1)
        {
            stone->setPosition(0.0f, size_, 0.0f);
        }
        else if(i == 2)
        {
            if(this->shapeIndex_ == 1 || this->shapeIndex_ == 6 || this->shapeIndex_ == 7)
            {
            	stone->setPosition(0.0f, 2*size_, 0.0f);
            }
            else if(this->shapeIndex_ == 3 || this->shapeIndex_ == 4|| this->shapeIndex_ == 5)
            {
            	stone->setPosition(size_, 0, 0.0f);
            }
            else if(this->shapeIndex_ == 2)
            {
            	stone->setPosition(-size_, 0, 0.0f);
            }
        }
        else if(i == 3)
        {
            if(this->shapeIndex_ == 2 || this->shapeIndex_ == 5)
            {
            	stone->setPosition(size_, size_, 0.0f);
            }
            else if(this->shapeIndex_ == 1)
            {
            	stone->setPosition(0, 3*size_, 0.0f);
            }
            else if(this->shapeIndex_ == 3 || this->shapeIndex_ == 7)
            {
            	stone->setPosition(-size_, 0, 0.0f);
            }
            else if(this->shapeIndex_ == 4)
            {
            	stone->setPosition(-size_, size_, 0.0f);
            }
            else if(this->shapeIndex_ == 6)
            {
            	stone->setPosition(size_, 0, 0.0f);
            }
        }
    }

    bool TetrisBrick::isValidMove(const Vector3& position, bool isRotation = false)
    {
    	return this->tetris_->isValidMove(this,position, isRotation);
    }

    TetrisStone* TetrisBrick::getStone(unsigned int i)
    {
    	if(i < this->brickStones_.size())
            return this->brickStones_[i];
    	else return NULL;
    }


    Tetris* TetrisBrick::getTetris()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Tetris)))
        {
            Tetris* tetrisGametype = orxonox_cast<Tetris*>(this->getGametype().get());
            return tetrisGametype;
        }
        return NULL;
    }

    bool TetrisBrick::contains(TetrisStone* stone)
    {
        for(unsigned int i = 0; i < brickStones_.size(); i++)
        {
            if(stone == brickStones_[i])
                return true;
        }
        return false;
    }

    /**
    @brief
        Overloaded the function to rotate the Brick.
    @param value
        A vector whose first component is the angle by which to rotate.
    */
    void TetrisBrick::moveFrontBack(const Vector2& value)
    {
        if(value.x < 0) //speedup on key down
        {
            this->setVelocity(this->getVelocity()*1.1);
        }
        else if(!this->lockRotation_) //rotate when key up is pressed
        {
        	if(!isValidMove(this->getPosition(), true)) //catch illegal rotations
        	    return;
            this->lockRotation_ = true; // multiple calls of this function have to be filtered out.
            this->rotationTimer_.setTimer(0.1f, false, createExecutor(createFunctor(&TetrisBrick::unlockRotation, this)));
            Quaternion q(Degree(90), Vector3::UNIT_Z);
            this->setOrientation(this->getOrientation()*q); //rotation: roll 90°
            this->rotationCount_ = (this->rotationCount_ + 1) % 4;
        }
    }

    /**
    @brief
        Overloaded the function to steer the Brick right and left
    @param value
        A vector whose first component is the direction in which we want to steer the Brick.
    */
    void TetrisBrick::moveRightLeft(const Vector2& value)
    {
        if(!this->delay_)
        {
            const Vector3& position = this->getPosition();
            Vector3 newPos = Vector3(position.x+value.x/abs(value.x)*this->size_, position.y, position.z);
            if(!this->isValidMove(newPos))
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
    void TetrisBrick::changedPlayer()
    {
        this->setVelocity(0.0f, 0.0f, 0.0f);
    }

}
