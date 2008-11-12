#include "PickupSpawner.h"
#include "core/CoreIncludes.h"
#include "core/Template.h"

namespace orxonox
{
CreateFactory(PickupSpawner);

PickupSpawner::PickupSpawner(BaseObject* creator) : PositionableEntity(creator)
{
	RegisterObject(PickupSpawner);

	this->template_ = 0;
}

void PickupSpawner::setItemTemplate(const std::string& itemtemplate)
{
	this->itemtemplate_ = itemtemplate;
	this->template_ = Template::getTemplate(itemtemplate);
}

    void PickupSpawner::processEvent(Event& event)
    {
        SUPER(PickupSpawner, processEvent, event);

        SetSubclassEvent(PickupSpawner, "pickup", triggering, event, DistanceTrigger);
    }

void activateSpawner(bool active)
	{
	if active=true
	this->isActive =true;
	else
	this->isActive = false;
	}

void triggering(bool active, DistanceTrigger* trigger)
{
	if (active= true && this->template_ && this->template_->getBaseclassIdentifier())
	{
		Pawn* player = trigger->getPlayer(); // getPlayer muss noch implementiert werden
		if(player->isA(itemtemplate_->getPlayerBaseClass())) 
		{
		BaseObject* newobject = this->template_->getBaseclassIdentifier()->fabricate();
		Item* newitem = dynamic_cast<Item*>(newobject);
		if (newitem)
		newitem->addTemplate(this->itemtemplate_);
		newitem->pickedUp(player);
		}
		else
			delete newobject;
		activateSpawner(false);
	}
}
}