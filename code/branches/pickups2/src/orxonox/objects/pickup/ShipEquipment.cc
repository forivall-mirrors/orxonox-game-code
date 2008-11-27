
#include "ShipEquipment.h"

namespace orxonox
{
	bool insert(Item* item)
	{
	if(checkSlot(player)==true)
		Equipment.insert ( std::pair<std::string, Item*>(item->getName(),item) );
	else
		COUT(3) << "swap?" << std::endl;
	




	};
	bool erase (Item* item)
	{
	if(Equipment.find(item->getName())!= Equipment.end())
 	{
 		std::multimap<std::string,Item*>::iterator it;
 		it=Equipment.find(item->getName());
  		Equipment.erase (it);
  		return true;
  	}
  	else
  		return false;
	};
	bool checkSlot(Item* item) const
	{
	std::multimap<std::string,Item*>::iterator it;
	for ( it=Equipment.begin() ; it != Equipment.end(); it++ )
	{
		if((*it).second->playerBaseClass_==item->playerBaseClass_)
		return false;
	}
	return true;
	};
}
