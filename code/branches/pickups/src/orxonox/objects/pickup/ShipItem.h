#ifndef _ShipItems_H__
#define _ShipItems_H__
#include <string>
#include "SpaceShip.h"
#include <map>
#include "ShipEquipmentClasses.h"

enum itemType{ //Diffrent Types of Items
	
		Usable,
		Powerups,
		Permanent
		};

namespace orxonox{




Class _OrxonoxExport ShipItem : public SpaceShip{
	
	public:
		itemType CheckType();
	 
	private:
		itemType type; 
		
	












#endif
