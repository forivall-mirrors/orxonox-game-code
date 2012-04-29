//
//  Tower.cc
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Tower.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(Tower);
	
    /**
	 @brief
	 Constructor. Registers and initializes the object.
	 */
    Tower::Tower(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Tower);
        /*
        this->size_ = 10.0f;
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisStone::enableMovement, this)));
		*/
    }
	
    /**
	 @brief
	 Overloaded the function to rotate the stone.
	 @param value
	 A vector whose first component is the angle by which to rotate.
	 */
    void Tower::moveFrontBack(const Vector2& value)
    {
        orxout() << "frontBack.x: " << value.x << endl;
    }
	
    /**
	 @brief
	 Overloaded the function to steer the stone right and left
	 @param value
	 A vector whose first component is the direction in which we want to steer the stone.
	 */
    void Tower::moveRightLeft(const Vector2& value)
    {
		orxout() << "rightLeft.x: " << value.x << endl;
		/*
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
		 */
    }
}