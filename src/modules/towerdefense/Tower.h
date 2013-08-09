//
//  Tower.h
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/**
@brief
See TowerDefenseReadme.txt for Information.

@ingroup TowerDefense
*/


#ifndef Orxonox_Tower_h
#define Orxonox_Tower_h

#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/pawns/SpaceShip.h"


namespace orxonox
{
    class _TowerDefenseExport Tower : public Pawn
    {
    public:
        Tower(Context* context);
        virtual ~Tower() {};

        // Maybe later override these to move towers with cursor keys
        /*
        virtual void moveFrontBack(const Vector2& value);
        virtual void moveRightLeft(const Vector2& value);
        */

        // Overriding these to stop towers from spasing out
        void setOrientation(const Quaternion& orientation);
        virtual void rotateYaw(const Vector2& value);
        virtual void rotatePitch(const Vector2& value);
        virtual void rotateRoll(const Vector2& value);

        void setGame(TowerDefense* towerdefense)
        { assert(towerdefense); game_ = towerdefense; }
    private:
        TowerDefense* game_;
    };
}


#endif
