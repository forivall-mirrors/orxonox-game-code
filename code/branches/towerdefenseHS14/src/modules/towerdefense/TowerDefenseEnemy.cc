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
        //needed to keep track of the PlayerStats coded in TowerDefense.h
        this->td = orxonox_cast<TowerDefense*>(this->getGametype().get());

    }
    //add credit if enemy is destroyed
    TowerDefenseEnemy::~TowerDefenseEnemy(){
    		this->td->addCredit(1);
    }

    void TowerDefenseEnemy::addWaypoint(TDCoordinate* coord)
    {
    	this->Waypointsvector_.push_back(coord);
    }


    void TowerDefenseEnemy::tick(float dt)
    {
        SUPER(TowerDefenseEnemy, tick, dt);
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
