//
//  Tower.cc
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/* Not implemented fully */

#include "Tower.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(Tower);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Tower::Tower(Context* context) : Pawn(context)
    {
        RegisterObject(Tower);

        this->setCollisionType(WorldEntity::Dynamic);

        //this->removeAllEngines();

        /*
        this->size_ = 10.0f;
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisStone::enableMovement, this)));
        */
    }

    void Tower::setOrientation(const Quaternion& orientation)
    {
        static int ori;
        orxout() << "orientation " << ++ori << endl;
    }

    void Tower::rotateYaw(const Vector2& value)
    {
        static int yaw;
        //orxout() << "rotateYaw " << ++yaw << endl;
    }

    void Tower::rotatePitch(const Vector2& value)
    {
        static int pitch;
        //orxout() << "rotatePitch " << ++pitch << endl;
    }

    void Tower::rotateRoll(const Vector2& value)
    {
        static int roll;
        orxout() << "rotateRoll" << ++roll << endl;
    }

    // This function is called whenever a player presses the up or the down key.
    // You have to implement what happens when the up or the down key is pressed.
    // value.x < 0 means: down key is pressed.
    // I suggest to create a new class which is a controllable entity I will refer to as "TowerMover". This is the controllable entity that the
    // player controls in order to move the tower along the centerpoint and in order to place the tower at the appropriate position.
    //

    // The tower itsself is controlled by a WayPointPatroController at the instance you place it on the centerpoint.
    //(don't forget to set the team_ parameter such that all tower are in the same team)

    //How to move a tower: simply attach the tower to the TowerMover
    //How to place a tower: detach the tower from the TowerMover

    /**
    @brief
        Overloaded the function to rotate the stone.
    @param value
        A vector whose first component is the angle by which to rotate.
    */
    /*
    void Tower::moveFrontBack(const Vector2& value)
    {
        //orxout() << "frontBack.x: " << value.x << endl;
    }
    */

    /**
    @brief
        Overloaded the function to steer the stone right and left
    @param value
        A vector whose first component is the direction in which we want to steer the stone.
    */
    /*
    void Tower::moveRightLeft(const Vector2& value)
    {
        //orxout() << "rightLeft.x: " << value.x << endl;

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
    */
}
