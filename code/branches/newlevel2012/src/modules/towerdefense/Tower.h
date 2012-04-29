//
//  Tower.h
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Orxonox_Tower_h
#define Orxonox_Tower_h

#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    class _OrxonoxExport Tower : public ControllableEntity
    {
	public:
		Tower(BaseObject* creator);
		virtual ~Tower() {};
		
		virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
		virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
		
		
		
    };
}


#endif
