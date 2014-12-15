//
//  TowerDefenseEnemy.h
//  Orxonox
//
//  Created by Jonas Erb on 22.10.14.

/**
@brief
See TowerDefenseReadme.txt for Information.

@ingroup TowerDefense
*/



#ifndef Orxonox_Tower_h
#define Orxonox_Tower_h

#include "TDCoordinate.h"
#include "gametypes/Deathmatch.h"
#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/pawns/SpaceShip.h"
#include "util/Output.h"
#include "controllers/ArtificialController.h"
#include "TowerDefense.h"

namespace orxonox
{
/* Class to give the TowerDefenseEnemy spaceships waypoints and
 *
 */
	class _TowerDefenseExport TowerDefenseEnemy : public SpaceShip 
	{
	public:
		TowerDefenseEnemy(Context* context);
        virtual ~TowerDefenseEnemy();

        //health gibt es unter: health_

        virtual void tick(float dt);
        void addWaypoint(TDCoordinate* coord);
        void popWaypoint();
        TDCoordinate peekWaypoint();

        virtual void damage(float damage, float healthdamage, float shielddamage, Pawn* originator);


	protected:
        
        WeakPtr<TowerDefense> getGame();
        WeakPtr<TowerDefense> game;
        TowerDefense* td;
        bool once_;
        std::vector<TDCoordinate*> Waypointsvector_;

	};



}

#endif /* _TowerDefense_H__ */
