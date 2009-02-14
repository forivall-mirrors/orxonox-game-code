/* has to be added to player
   multimap<std::string, BaseItem*> Equipment;*/


#include "OrxonoxStableHeaders.h"
#include "BaseItem.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/String.h"
#include "objects/worldentities/pawns/Pawn.h"


namespace orxonox
{
BaseItem::BaseItem(BaseObject* creator) : BaseObject(creator)
{
	RegisterObject(BaseItem);

	this->playerBaseClass_ = 0;
}

BaseItem::~BaseItem()
{
}
    void BaseItem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(BaseItem, XMLPort, xmlelement, mode);

        XMLPortParam(BaseItem, "playerclass", setPlayerBaseClassName, getPlayerBaseClassName, xmlelement, mode);
    }

bool BaseItem::addTo (Pawn* player)
{

	return player->getPickUp().insert(this);
	/*if(checkSlot(player)==true)
	player->pickUp.Equipment.insert ( std::pair<std::string, BaseItem*>(this->getName(),this) );
	else
	COUT(3) << "swap?" << std::endl;*/
}
bool BaseItem::remove(Pawn* player)
{
 	/*if(player->pickUp.Equipment.find(this->getName())!= player->pickUp.Equipment.end())
 	{
 	std::multimap<std::string,BaseItem*>::iterator it;
 	it=player->pickUp.Equipment.find(this->getName());
  	player->pickUp.Equipment.erase (it);
  	return true;
  	}
  	else
  	return false;*/
  	return player->getPickUp().erase(this);
}
bool BaseItem::checkSlot(Pawn* player)
{
	/*std::multimap<std::string,BaseItem*>::iterator it;
	for ( it=player->getPickUp().getEquipment().begin() ; it != player->getPickUp().getEquipment().end(); it++ )
	{
	if((*it).second->playerBaseClass_==this->playerBaseClass_)
		//das isch schmarre...machs mit isExactlyA(...)
	return false;
	}
	return true;*/
	return player->getPickUp().checkSlot(this);
}
void BaseItem::setPlayerBaseClassName(const std::string& name)
{
	this->playerBaseClass_ = ClassByString(name);
}

const std::string& BaseItem::getPlayerBaseClassName() const
{
	if (this->playerBaseClass_)
		return this->playerBaseClass_->getName();
	else
		return BLANKSTRING;
}
}
