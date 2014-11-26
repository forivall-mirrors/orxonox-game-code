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
    	this->Waypointsvector_.push_back(coord);
    }


    void TowerDefenseEnemy::tick(float dt)
    {
        SUPER(TowerDefenseEnemy, tick, dt);

        //ArtificialController* controller = (ArtificialController*)this->getController();
        Vector3 ship = this->getRVWorldPosition();
        Vector3* endpoint = new Vector3(500, 700, 150);

        float distance = ship.distance(*endpoint);

        //orxout() << "distance" << distance << endl;
        if(distance <50){
        //	orxout() << "ENEMY KILLED!!!!" << endl;
        	this->destroy(); }
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
