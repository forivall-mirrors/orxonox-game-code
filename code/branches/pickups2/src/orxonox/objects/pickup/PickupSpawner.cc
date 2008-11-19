#include "PickupSpawner.h"
#include "Item.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "objects/worldentities/triggers/DistanceTrigger.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Template.h"

namespace orxonox
{
CreateFactory(PickupSpawner);

PickupSpawner::PickupSpawner(BaseObject* creator) : PositionableEntity(creator)
{
	RegisterObject(PickupSpawner);

	this->template_ = 0;
	this->distance_ = 50;
}

PickupSpawner::~PickupSpawner()
{
}

void PickupSpawner::XMLPort(Element& xmlelement, XMLPort::Mode mode)
{
	SUPER(PickupSpawner, XMLPort, xmlelement, mode);

	XMLPortParam(PickupSpawner, "item", setItemTemplate, getItemTemplate, xmlelement, mode);
	XMLPortParam(PickupSpawner, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(50.0f);
}

void PickupSpawner::tick(float dt)
{
  if (this->isActive())
  {
    for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
    {
      Vector3 distanceVec = it->getWorldPosition() - this->getWorldPosition();
      if (distanceVec.length() < this->distance_)
        this->triggering(*it);
    }
  }
}

void PickupSpawner::setItemTemplate(const std::string& itemtemplate)
{
	this->itemtemplate_ = itemtemplate;
	this->template_ = Template::getTemplate(itemtemplate);
}

void PickupSpawner::triggering(Pawn* player)
{
	if (this->isActive() && this->template_ && this->template_->getBaseclassIdentifier())
	{
		COUT(0) << "activated" << std::endl;
		//if(player->isA(itemtemplate_->getPlayerBaseClass())) 
		{
		BaseObject* newobject = this->template_->getBaseclassIdentifier()->fabricate(this);
		Item* newitem = dynamic_cast<Item*>(newobject);
		if (newitem)
		{
			newitem->addTemplate(this->itemtemplate_);
			if (newitem->pickedUp(player)== true)
				this->setActive(false);
			else
				delete newobject;
		}
		}
		//else
		//	delete newobject;
	}
}
}
