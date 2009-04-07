#include "OrxonoxStableHeaders.h"
#include "PickupSpawner.h"
#include "BaseItem.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "objects/worldentities/triggers/DistanceTrigger.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Template.h"

namespace orxonox
{
CreateFactory(PickupSpawner);

PickupSpawner::PickupSpawner(BaseObject* creator) : StaticEntity(creator)
{
	RegisterObject(PickupSpawner);

	this->template_ = 0;
	this->distance_ = 20;
	this->respawntimer_= 0;
}

PickupSpawner::~PickupSpawner()
{
}

void PickupSpawner::XMLPort(Element& xmlelement, XMLPort::Mode mode)
{
	SUPER(PickupSpawner, XMLPort, xmlelement, mode);

	XMLPortParam(PickupSpawner, "item", setItemTemplate, getItemTemplate, xmlelement, mode);
	XMLPortParam(PickupSpawner, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(20.0f);
	XMLPortParam(PickupSpawner, "respawntimer", setRespawnTimer, getRespawnTimer, xmlelement, mode);

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
		COUT(0) << "ITEM PICKED UP" << std::endl;
		//if(player->isA(itemtemplate_->getPlayerBaseClass()))
		{
		BaseObject* newobject = this->template_->getBaseclassIdentifier()->fabricate(this);
		BaseItem* newitem = dynamic_cast<BaseItem*>(newobject);
		if (newitem)
		{
			newitem->addTemplate(this->itemtemplate_);
			if (newitem->pickedUp(player)== true)
			{
				if(respawntimer_!=0)
					this->triggerRespawnTimer();
				this->setActive(false);
				this->fireEvent();
			}
			else
				delete newobject;
		}
		}
		//else
		//	delete newobject;
	}
}

void PickupSpawner::triggerRespawnTimer()
{

	if(respawntimer_!=0)
	{
		ExecutorMember<BaseObject>* executor = createExecutor(createFunctor(&BaseObject::setActive));
		executor->setDefaultValues(true);
		RespawnTimer_.setTimer(this->respawntimer_, false, this, executor);
		COUT(0) << "TIMER SET" << std::endl;
	}
}
void PickupSpawner::changedActivity()
{
/*
	COUT(0) << "Visble?" << std::endl;
	if(isActive())
	{
		setVisible(true);
		COUT(0) << "Visble!" << std::endl;
	}
	if(isActive()==false)
	{
		setVisible(false);
		COUT(0) << "INvisble!" << std::endl;
	}

*/
	SUPER(PickupSpawner, changedActivity);

	for (std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin(); it != this->getAttachedObjects().end(); ++it)
		(*it)->setVisible(this->isActive());
}


}



