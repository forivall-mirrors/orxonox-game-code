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
    class _OrxonoxExport Tower : public SpaceShip
    {
	public:
		Tower(BaseObject* creator);
		virtual ~Tower() {};
		
		virtual void moveFrontBack(const Vector2& value);
		virtual void moveRightLeft(const Vector2& value);
		
		void setGame(TowerDefense* towerdefense)
		{ assert(towerdefense); game_ = towerdefense; }
	private:
		TowerDefense* game_;
    };
}


#endif
