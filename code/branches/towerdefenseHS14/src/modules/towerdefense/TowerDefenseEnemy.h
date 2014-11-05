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

namespace orxonox
{
/* Class to give the TowerDefenseEnemy spaceships waypoints and
 *
 */
	class _TowerDefenseExport TowerDefenseEnemy : public Pawn
	{

	public:

		TowerDefenseEnemy(Context* context);
        virtual ~TowerDefenseEnemy() {};

        //health gibt es unter: health_

        void addWaypoint(TDCoordinate coord);
        void popWaypoint();
        TDCoordinate peekWaypoint();
	private:

        std::vector<TDCoordinate> Waypointsvector_;

	};



}

#endif /* _TowerDefense_H__ */
