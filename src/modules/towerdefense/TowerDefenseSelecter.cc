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
    @file TowerDefenseSelecter.cc
    @brief This class represents your figure when you play the minigame. Here the movement of the figure, activating items, ... are handled.
*/

#include "TowerDefenseSelecter.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "graphics/Model.h"

namespace orxonox
{
    RegisterClass(TowerDefenseSelecter);

    TowerDefenseSelecter::TowerDefenseSelecter(Context* context) : ControllableEntity(context)
    {
        RegisterObject(TowerDefenseSelecter);

        // initialize variables
        moveUpPressed_ = false;
        moveDownPressed_ = false;
        moveLeftPressed_ = false;
        moveRightPressed_ = false;
        selectedPos_ = new TDCoordinate(0,0);

        Model* selecterModel = new Model(context);
        selecterModel->setMeshSource("cube.mesh");
        selecterModel->setScale(45);

    }

    TowerDefenseSelecter::~TowerDefenseSelecter()
    {

    }

    void TowerDefenseSelecter::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseSelecter, XMLPort, xmlelement, mode);
        // Beispielport // XMLPortParam(TowerDefenseSelecter, "mouseFactor", setMouseFactor, getMouseFactor, xmlelement, mode);

    }

    void TowerDefenseSelecter::tick(float dt)
    {
        SUPER(TowerDefenseSelecter, tick, dt);

        if (hasLocalController())
        {

            if (moveUpPressed_ == true)
            {
                if(this->selectedPos_->y >= 15)
                {
                }
                else
                {
                	this->selectedPos_->y +=1;
                	this->setPosition(selectedPos_->get3dcoordinate());
                }

            }
            if (moveDownPressed_ == true)
            {
            	if(this->selectedPos_->y <= 0)
            	{
				}
				else
				{
					this->selectedPos_->y -= 1;
					this->setPosition(selectedPos_->get3dcoordinate());
				}

            	moveDownPressed_ = false;
            }

            if (moveLeftPressed_ == true)
            {
                if(this->selectedPos_->x <= 0)
                {
                }
                else
                {
                	this->selectedPos_->x -=1;
                	this->setPosition(selectedPos_->get3dcoordinate());
                }

            }
            if (moveRightPressed_ == true)
            {
            	if(this->selectedPos_->x >= 15)
            	{
				}
				else
				{
					this->selectedPos_->x += 1;
					this->setPosition(selectedPos_->get3dcoordinate());
				}

            	moveDownPressed_ = false;
            }



            /*
            if (firePressed_ && timeSinceLastFire_ >= maxFireRate_)
            {
                firePressed_ = false;
                timeSinceLastFire_ = 0.0;
                fireSignal_ = true;
            }
            */
        }

        // Reset key variables
        moveUpPressed_ = false;
        moveDownPressed_ = false;
        moveLeftPressed_ = false;
        moveDownPressed_ = false;
        //firePressed_ = false;
    }


    void TowerDefenseSelecter::moveFrontBack(const Vector2& value)
    {
        if (value.x > 0)
        {
            moveUpPressed_ = true;
            moveDownPressed_ = false;
        }
        else
        {
            moveUpPressed_ = false;
            moveDownPressed_ = true;
        }
    }

    void TowerDefenseSelecter::moveRightLeft(const Vector2& value)
    {
        if (value.x > 0)
        {
            moveLeftPressed_ = false;
            moveRightPressed_ = true;
        }
        else
        {
            moveLeftPressed_ = true;
            moveRightPressed_ = false;
        }
    }

    void TowerDefenseSelecter::rotateYaw(const Vector2& value)
    {
    }

    void TowerDefenseSelecter::rotatePitch(const Vector2& value)
    {
    }

    void TowerDefenseSelecter::rotateRoll(const Vector2& value)
    {
    }

    void TowerDefenseSelecter::fire(unsigned int firemode)
    {
    }

    void TowerDefenseSelecter::fired(unsigned int firemode)
    {
        //firePressed_ = true;
    }
}
