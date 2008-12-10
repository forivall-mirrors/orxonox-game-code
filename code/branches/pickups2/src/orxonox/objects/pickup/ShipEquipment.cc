#include "Item.h"
#include "ShipEquipment.h"
#include "objects/worldentities/pawns/Pawn.h"


namespace orxonox
{
/**
@brief
    Insert a permanent Item to the Equipment. Is usually called by the addTo function in Items.
    
@param item
    pointer to the item which is to be inserted.

@return
    if new item has sucessfully been added it will return true, in any other case the return value is false.
*/
	bool ShipEquipment::insert(Item* item)
	{
	if(checkSlot(item)==NULL)
	{
		Equipment.insert ( std::pair<std::string, Item*>(item->getName(),item) );
		return true;
	}
	else
	{
		COUT(3) << "SWAP?" <<  endl;
		//Abfrage- irgendne ifschleife... 
		if((checkSlot(item)->dropped(player))==true);
		{
			Equipment.insert ( std::pair<std::string, Item*>(item->getName(),item) );
			COUT(3) << "SWAPPED!" <<  endl;
			return true;
		}
		return false;
	}

	return false;
	};

/**
@brief
    Erases a permanent Item in the Equipment. Is usually called by the remove/dropped function in Items.
    
@param item
    pointer to the item which is to be erased.

@return
    if new item has sucessfully been erased it will return true, in any other case the return value is false.
*/
	bool ShipEquipment::erase (Item* item)
	{
	std::multimap<std::string,Item*>::iterator it = Equipment.find(item->getName());
	if(it != Equipment.end())
 	{
  		Equipment.erase (it);
  		return true;
  	}
  	return false;
	};
	/*void print(std::multimap<std::string, Item*> eut)
	{
		std::multimap<std::string,Item*>::iterator it;
		COUT(3) << "Liste:" <<  endl;
		for ( it=eut.begin() ; it != eut.end(); ++it )
		    COUT(3) << (*it).first << endl;

	}*/
/**
@brief
    Erases all permanent Items in the Equipment. Its Triggered by hitting the L button.

*/
	void ShipEquipment::eraseAll()
	{
		//print(Equipment);
		for (std::multimap<std::string,Item*>::iterator it = Equipment.begin(); it != Equipment.end(); )
		{

			(it++)->second->dropped(this->getPlayer());
		}
		//print(Equipment);
	}

	Item* ShipEquipment::checkSlot(Item* item)
	{
	std::multimap<std::string,Item*>::iterator it;
	for ( it= getPlayer()->getPickUp().getEquipment().begin() ; it != getPlayer()->getPickUp().getEquipment().end(); it++ )
	{
		//if((*it).second->getPlayerBaseClass()==item->getPlayerBaseClass())
		if(item->isExactlyA((*it).second->getIdentifier()))
		return (*it).second;
	}
	return NULL;
	};
	
}
