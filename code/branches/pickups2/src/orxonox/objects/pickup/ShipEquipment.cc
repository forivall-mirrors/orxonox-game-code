#include "Item.h"
#include "ShipEquipment.h"
#include "objects/worldentities/pawns/Pawn.h"


namespace orxonox
{

	bool ShipEquipment::insert(Item* item)
	{
	if(checkSlot(item)==true)
	{
		Equipment.insert ( std::pair<std::string, Item*>(item->getName(),item) );
		return true;
	}
		COUT(3) << "swap?" << std::endl;
		return false;
	
	return false;
	};
	bool ShipEquipment::erase (Item* item)
	{
	std::multimap<std::string,Item*>::iterator it = Equipment.find(item->getName());
	if(it != Equipment.end())
 	{
		//it->second->dropped(this->getPlayer());
  		Equipment.erase (it);
  		return true;
  	}
  	else
  		return false;
	};

	void ShipEquipment::eraseAll()
	{
		
		for (std::multimap<std::string,Item*>::iterator it = Equipment.begin(); it != Equipment.end(); )
		{
		
			(it++)->second->dropped(this->getPlayer());
		}
	}

	bool ShipEquipment::checkSlot(Item* item)
	{
	std::multimap<std::string,Item*>::iterator it;
	for ( it= getPlayer()->getPickUp().getEquipment().begin() ; it != getPlayer()->getPickUp().getEquipment().end(); it++ )
	{
		if((*it).second->getPlayerBaseClass()==item->getPlayerBaseClass())
		return false;
	}
	return true;
	};
}
