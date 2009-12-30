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
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
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
            this->destroy();
        }
    }
    void DroppedItem::createTimer()
    {
        if (this->timeToLive_ > 0)
        {
            this->timer_.setTimer(this->timeToLive_, false, createExecutor(createFunctor(&DroppedItem::timerCallback, this)), false);
        }
    }
    void DroppedItem::timerCallback()
    {
        if (this->item_)
        {
            COUT(3) << "Delete DroppedItem with '" << this->item_->getPickupIdentifier() << '\'' << std::endl;
            this->item_->destroy();
        }

        this->destroy();
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

        COUT(3) << "Created DroppedItem for '" << item->getPickupIdentifier() << "' at (" << position.x << ',' << position.y << ',' << position.z << ")." << std::endl;

        return drop;
    }
    DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour, float timeToLive)
    {
        Vector3 after = pawn->getPosition() + pawn->getOrientation() * Vector3(0.0f, 0.0f, 50.0f);
        return DroppedItem::createDefaultDrop(item, after, flareColour, timeToLive);
    }
}
