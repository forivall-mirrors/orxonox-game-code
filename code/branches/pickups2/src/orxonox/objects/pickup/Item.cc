/* has to be added to player
   multimap<std::string, Item*> Equipment;*/


#include "Item.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/String.h"
#include "objects/worldentities/pawns/Pawn.h"


namespace orxonox
{
Item::Item(BaseObject* creator) : BaseObject(creator)
{
	RegisterObject(Item);

	this->playerBaseClass_ = 0;
}

Item::~Item()
{
}
    void Item::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Item, XMLPort, xmlelement, mode);

        XMLPortParam(Item, "playerclass", setPlayerBaseClassName, getPlayerBaseClassName, xmlelement, mode);
    }

bool Item::addTo (Pawn* player)
{

	return player->getPickUp().insert(this);
	/*if(checkSlot(player)==true)
	player->pickUp.Equipment.insert ( std::pair<std::string, Item*>(this->getName(),this) );
	else
	COUT(3) << "swap?" << std::endl;*/
}
bool Item::remove(Pawn* player)
{
 	/*if(player->pickUp.Equipment.find(this->getName())!= player->pickUp.Equipment.end())
 	{
 	std::multimap<std::string,Item*>::iterator it;
 	it=player->pickUp.Equipment.find(this->getName());
  	player->pickUp.Equipment.erase (it);
  	return true;
  	}
  	else
  	return false;*/
  	return player->getPickUp().erase(this);
}
bool Item::checkSlot(Pawn* player)
{
	/*std::multimap<std::string,Item*>::iterator it;
	for ( it=player->getPickUp().getEquipment().begin() ; it != player->getPickUp().getEquipment().end(); it++ )
	{
	if((*it).second->playerBaseClass_==this->playerBaseClass_)
		//das isch schmarre...machs mit isExactlyA(...)
	return false;
	}
	return true;*/
	return player->getPickUp().checkSlot(this);
}
void Item::setPlayerBaseClassName(const std::string& name)
{
	this->playerBaseClass_ = ClassByString(name);
}

const std::string& Item::getPlayerBaseClassName() const
{
	if (this->playerBaseClass_)
		return this->playerBaseClass_->getName();
	else
		return BLANKSTRING;
}
}
