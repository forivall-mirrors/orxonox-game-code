//
//  TowerDefenseEnemy.cc
//  Orxonox
//
//  Created by Jonas Erb on 22.10.14.

/**
@brief
See TowerDefenseReadme.txt for Information.

@ingroup TowerDefense
*/
#include "TowerDefenseEnemy.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(TowerDefenseEnemy);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TowerDefenseEnemy::TowerDefenseEnemy(Context* context) : SpaceShip(context)
    {
        RegisterObject(TowerDefenseEnemy);

        this->setCollisionType(WorldEntity::Dynamic);

    }

    void TowerDefenseEnemy::addWaypoint(TDCoordinate* coord)
    {
    	orxout() << "test4" << endl;
    	this->Waypointsvector_.push_back(coord);
    	orxout() << "test5" << endl;
    }
/*
    void TowerDefenseEnemy::popWaypoint()
    {
    	if(Waypointsvector_.size()>0)
    		Waypointsvector_.pop_back();
    }

    TDCoordinate TowerDefenseEnemy::peekWaypoint()
    {
    	if(Waypointsvector_.size()<=0){
    		TDCoordinate* coord = TDCoordinate(-1,-1);
    		return coord;
    	}else{
        	return Waypointsvector_.at(Waypointsvector_.size()-1);

    	}


    }
*/

}
