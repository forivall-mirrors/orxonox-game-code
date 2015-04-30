//
//  TowerDefenseTower.cc
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/* Not implemented fully */

#include "TowerDefenseTower.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(TowerDefenseTower);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TowerDefenseTower::TowerDefenseTower(Context* context) : Turret(context)
    {
        RegisterObject(TowerDefenseTower);
        game_ =NULL;
        this->setCollisionType(WorldEntity::None);
        upgrade = 0;
        this->addTemplate("towerdefensetower");



        //this->removeAllEngines();

        /*
        this->size_ = 10.0f;
        this->delay_ = false;
        this->delayTimer_.setTimer(0.2f, false, createExecutor(createFunctor(&TetrisStone::enableMovement, this)));
        */
    }

    /*
    void TowerDefenseTower::setOrientation(const Quaternion& orientation)
    {
    }

    void TowerDefenseTower::rotateYaw(const Vector2& value)
    {
    }

    void TowerDefenseTower::rotatePitch(const Vector2& value)
    {
    }

    void TowerDefenseTower::rotateRoll(const Vector2& value)
    {
    }
    */

    bool TowerDefenseTower::upgradeTower()
    {
        if(upgrade < 3)
        {
            upgrade++;
            float reloadrate = getReloadRate();
            float reloadwaittime = getReloadWaitTime();
            this->setDamageMultiplier((upgrade+1)*1.5);
            this->setRotationThrust(2*this->getRotationThrust());
            reloadrate = 0.7f*reloadrate;
            reloadwaittime = 0.7f*reloadwaittime;
            setReloadRate(reloadrate);
            setReloadWaitTime(reloadwaittime);
            //this->addTemplate("towerturret1");
        }
        else
        {
            orxout() << "Tower fully upgraded" << endl;
            return false;
        }
        return true;
    }

    // This function is called whenever a player presses the up or the down key.
    // You have to implement what happens when the up or the down key is pressed.
    // value.x < 0 means: down key is pressed.
    // I suggest to create a new class which is a controllable entity I will refer to as "TowerDefenseTowerMover". This is the controllable entity that the
    // player controls in order to move the TowerDefenseTower along the centerpoint and in order to place the TowerDefenseTower at the appropriate position.
    //

    // The TowerDefenseTower itsself is controlled by a WayPointPatroController at the instance you place it on the centerpoint.
    //(don't forget to set the team_ parameter such that all TowerDefenseTower are in the same team)

    //How to move a TowerDefenseTower: simply attach the TowerDefenseTower to the TowerDefenseTowerMover
    //How to place a TowerDefenseTower: detach the TowerDefenseTower from the TowerDefenseTowerMover

    /**
    @brief
        Overloaded the function to rotate the stone.
    @param value
        A vector whose first component is the angle by which to rotate.
    */
    /*
    void TowerDefenseTower::moveFrontBack(const Vector2& value)
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
    void TowerDefenseTower::moveRightLeft(const Vector2& value)
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
