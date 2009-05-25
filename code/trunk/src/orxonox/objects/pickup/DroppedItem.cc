#include "DroppedItem.h"

#include "BaseItem.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "objects/worldentities/Model.h"
#include "objects/worldentities/Billboard.h"

#include "core/CoreIncludes.h"
#include "core/Core.h"

namespace orxonox
{
    CreateFactory(DroppedItem);

    DroppedItem::DroppedItem(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(DroppedItem);

        this->triggerDistance_ = 20.0f;
        this->timeToLive_ = 0;
        this->item_ = 0;
    }
    DroppedItem::~DroppedItem()
    {
    }
    void DroppedItem::tick(float dt)
    {
        if (this->item_)
        {
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); it++)
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                if (distance.length() < this->triggerDistance_)
                    this->trigger(*it);
            }
        }
    }
    void DroppedItem::trigger(Pawn* pawn)
    {
        if (this->item_->pickedUp(pawn))
        {
            COUT(3) << "DroppedItem '" << this->item_->getPickupIdentifier() << "' picked up." << std::endl;
            delete this;
        }
    }
    void DroppedItem::createTimer()
    {
        if (this->timeToLive_ > 0)
        {
            ExecutorMember<DroppedItem>* exec = createExecutor(createFunctor(&DroppedItem::timerCallback));
            this->timer_.setTimer(this->timeToLive_, false, this, exec, false);
        }
    }
    void DroppedItem::timerCallback()
    {
        if (this->item_)
        {
            COUT(3) << "Delete DroppedItem with '" << this->item_->getPickupIdentifier() << "'" << std::endl;
            delete this->item_;
        }

        delete this;
    }

    DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, const Vector3& position, const ColourValue& flareColour, float timeToLive)
    {
        DroppedItem* drop = new DroppedItem(item);
        Model* model = new Model(item);
        Billboard* billboard = new Billboard(item);
        
        model->setMeshSource("sphere.mesh");
        model->setScale(3.0f);

        billboard->setMaterial("Examples/Flare");
        billboard->setColour(flareColour);
        billboard->setScale(0.5f);

        drop->setPosition(position);
        drop->attach(model);
        drop->attach(billboard);

        drop->setItem(item);

        drop->setTimeToLive(timeToLive);
        drop->createTimer();

        COUT(3) << "Created DroppedItem for '" << item->getPickupIdentifier() << "' at (" << position.x << "," << position.y << "," << position.z << ")." << std::endl;

        return drop;
    }
    DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour, float timeToLive)
    {
        Vector3 after = pawn->getPosition() + pawn->getOrientation() * Vector3(0.0f, 0.0f, 50.0f);
        return DroppedItem::createDefaultDrop(item, after, flareColour, timeToLive);
    }
}
