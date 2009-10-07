/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      ...
 *
 */

#include "DroppedItem.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "BaseItem.h"
#include "graphics/Billboard.h"
#include "graphics/Model.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(DroppedItem);

    /**
    @brief
        Constructor. Registers object and sets default values.
    */
    DroppedItem::DroppedItem(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(DroppedItem);

        this->triggerDistance_ = 20.0f;
        this->timeToLive_ = 0;
        this->item_ = NULL;
    }

    /**
    @brief
        Default destructor.
    */
    //TODO: Destroy something?
    DroppedItem::~DroppedItem()
    {
        
    }

    /**
    @brief
        Checks whether any pawn is in triggerDistance of the Item and calls this->trigger if so.
    @param dt
        The  duration of the last time interval.    
    */
    //TODO: Replace this with a DistanceTrigger!
    void DroppedItem::tick(float dt)
    {
        if (this->item_)
        {
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); it++) //!< Iterate through all Pawns.
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                if (distance.length() < this->triggerDistance_)
                    this->trigger(*it);
            }
        }
    }

    /**
    @brief
        Called when the DroppedItem is triggered. Adds the item to the triggering pawn.
    */
    void DroppedItem::trigger(Pawn* pawn)
    {
        if (this->item_->pickedUp(pawn)) //If pickup was successful.
        {
            COUT(3) << "DroppedItem '" << this->item_->getPickupIdentifier() << "' picked up." << std::endl;
            delete this;
        }
    }

    /**
    @brief
        Creates a timer to call this->timerCallback() at expiration of timeToLive.
    */
    //TODO: Better Comments.
    void DroppedItem::createTimer()
    {
        if (this->timeToLive_ > 0)
        {
            ExecutorMember<DroppedItem>* exec = createExecutor(createFunctor(&DroppedItem::timerCallback));
            this->timer_.setTimer(this->timeToLive_, false, this, exec, false);
        }
    }
    
    /**
    @brief
        Destroys the item. Called by the set timer upon its expiration.
    */
    //TODO: Choose better function name if this doesn't create dependency inconsistencies. e.g. this->destroy() or this->timeOut()
    //Make certain that only one pawn has the same item, because if not, deliting the item would lead to a possible segfault.
    //If the item is destroyed here, shouldn't it be destroyed in the destructor as well?
    void DroppedItem::timerCallback()
    {
        if (this->item_)
        {
            COUT(3) << "Delete DroppedItem with '" << this->item_->getPickupIdentifier() << "'" << std::endl;
            delete this->item_;
        }

        delete this;
    }

    /**
    @brief
        
    */
    //TODO: Comment.
    //This is for pawns dropping items they have...
    //Probably better to create a spawner with only 1 item in it.
    //Various different thigs are done here, which in my opinion should eighter be done in XML or some where else, preferably in XML.
    //Each pickup should have a XML template where the Model and Billboard, and so on, is specified.
    //The position, item and timetoLive should be specified by this Classes XMLPort function.
    //These adjustments above, will very likely create inkonsistencies in the level files, possibly templates.
    /*static*/ DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, const Vector3& position, const ColourValue& flareColour, float timeToLive)
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

    /**
    @brief

    */
    //TODO: See one function above.
    DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour, float timeToLive)
    {
        Vector3 after = pawn->getPosition() + pawn->getOrientation() * Vector3(0.0f, 0.0f, 50.0f);
        return DroppedItem::createDefaultDrop(item, after, flareColour, timeToLive);
    }
}
