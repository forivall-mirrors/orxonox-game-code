//
//  Enemy.cc
//  Orxonox
//
//  Created by Jonas Erb on 22.10.14.

/**
@brief
See TowerDefenseReadme.txt for Information.

@ingroup TowerDefense
*/
#include "Enemy.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(Enemy);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Enemy::Enemy(Context* context) : Pawn(context)
    {
        RegisterObject(Enemy);

        this->setCollisionType(WorldEntity::Dynamic);

    }

    void Enemy::addWaypoint(TDCoordinate coord)
    {
    	Waypointsvector_.push_back(coord);
    }

    void Enemy::popWaypoint()
    {
    	if(Waypointsvector_.size()>0)
    		Waypointsvector_.pop_back();
    }

    TDCoordinate Enemy::peekWaypoint()
    {
    	if(Waypointsvector_.size()<=0){
    		TDCoordinate coord = TDCoordinate(-1,-1);
    		return coord;
    	}else{
        	return Waypointsvector_.at(Waypointsvector_.size()-1);

    	}


    }

}
