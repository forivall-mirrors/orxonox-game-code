#ifndef _ShipItems_H__
#define _ShipItems_H__
#include <string>
#include "SpaceShip.h"
#include <map>
#include "ShipEquipmentClasses.h"



namespace orxonox{
class _OrxonoxExport ShipItem{

	enum itemType{ //Diffrent Types of Items

			Useable,
			Powerups,
			Permanent
			};

	public:
		itemType CheckType();
		//virtual ChecksubType();

	private:
		itemType type;
		string itemname;
	};


/* Useables are Items the Player can choose when to activate and then show an instant action/effect
 * or for a certain duration */
class _OrxonoxExport Useable : public ShipItem{


	enum subItemTypeUseable{ //Diffrent Types of Items of the Type Useable
			Rockets,
			Repairkits,
			Triggers,		//trigger events usable by the player (Quests f.exp.)
			Boosters,
			Shields,
			Appearance,
			};

			public:
				//subItemTypeUseable CheckSubType();

			private:
				subItemTypeUseable subtype_usable;
};

/* Permanents are Items, that effect the Player or his Ship from the instance he picks them up
   until he drops them again */
class _OrxonoxExport Permanent : public ShipItem{

	enum subItemTypePermanent{ //Diffrent Types of Items of the Type Permanent
			Weapon,
			Thrusters,
			Shields,
			Armor
			};
			public:
				subItemTypePermanent CheckSubType();

			private:
				subItemTypePermanent subtype_permanent;
};

// Powerups effect the Player right after he picks them up for a certain Duration /or permanently.
class _OrxonoxExport Powerups : public ShipItem{

	enum subItem TypePowerups{ //Diffrent Types of Items of the Type Powerups
			Weapon,
			Boosters,
			Shields,
			Repairkits,
			Appearance,
			Trigger
			};
			public:
			//subItemTypePowerups CheckSubType();

			private:
			subItemTypePowerups subtype_powerups;
};

}










#endif
