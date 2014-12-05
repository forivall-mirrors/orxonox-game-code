//
//  TowerDefenseTower.h
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/**
@brief
See TowerDefenseTowerDefenseReadme.txt for Information.

@ingroup TowerDefenseTowerDefense
*/


#ifndef Orxonox_TowerDefenseTower_h
#define Orxonox_TowerDefenseTower_h

#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/pawns/SpaceShip.h"


namespace orxonox
{
    class _TowerDefenseExport TowerDefenseTower : public Pawn
    {
    public:
        TowerDefenseTower(Context* context);
        virtual ~TowerDefenseTower() {};

        // Maybe later override these to move TowerDefenseTowers with cursor keys
        /*
        virtual void moveFrontBack(const Vector2& value);
        virtual void moveRightLeft(const Vector2& value);
        */

        // Overriding these to stop TowerDefenseTowers from spasing out
        void setOrientation(const Quaternion& orientation);
        virtual void rotateYaw(const Vector2& value);
        virtual void rotatePitch(const Vector2& value);
        virtual void rotateRoll(const Vector2& value);
        virtual bool upgradeTower();


        void setGame(TowerDefense* Towerdefense)
        { assert(Towerdefense); game_ = Towerdefense; }
    private:
        TowerDefense* game_;
        int upgrade;
    };
}


#endif
