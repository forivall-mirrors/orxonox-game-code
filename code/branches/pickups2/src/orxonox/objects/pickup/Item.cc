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

void Item::addTo (Pawn* player)
{
	//player->Equipment.insert ( std::pair<std::string, Item*>(this->itemname,this) );
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
