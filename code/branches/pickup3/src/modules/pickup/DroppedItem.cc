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

#include "core/CoreIncludes.h"
#include "interfaces/Pickupable.h"
#include "graphics/Model.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Registers object and sets default values.
    */
    DroppedItem::DroppedItem(BaseObject* creator) : PickupSpawner(creator)
    {
        this->initialize();
    }

    DroppedItem::DroppedItem(BaseObject* creator, Pickupable* item, const Vector3& position, float triggerDistance) : PickupSpawner(creator, item, triggerDistance, 0, 1)
    {   
        this->initialize();
        
        this->createDrop(position);
    }
    
    void DroppedItem::initialize(void)
    {
        RegisterObject(DroppedItem);
        
        this->gotPickedUp_ = false;
    }

    /**
    @brief
        Default destructor.
    */
    DroppedItem::~DroppedItem()
    {
        if(this->gotPickedUp_ && this->pickup_ != NULL)
        {
            this->pickup_ = NULL;
        }
    }

    Pickupable* DroppedItem::getPickup(void)
    {
        return this->pickup_;
    }
    
    void DroppedItem::createDrop(const Vector3& position)
    {
        this->setPosition(position);
        
        //TODO: Make this work.
        //const Model& model = PickupManager::getModel(item->getPickupIdentifier());
        //this->attach(model);
    }

//TODO: Remove.
    //TODO: Comment.
    //Each pickup should have a XML template where the Model and Billboard, and so on, is specified.
//    /*static*/ DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, const Vector3& position, const ColourValue& flareColour, float timeToLive)
//     {
//         //TODO: triggerDistance?
//         float triggerDistance = 20.0;
//         DroppedItem* droppedItem = new DroppedItem(item, item, triggerDistance, 0, 1);
//         
//         //TODO: Do this somehwere else?
//         Model* model = new Model(item);
//         Billboard* billboard = new Billboard(item);
// 
//         model->setMeshSource("sphere.mesh");
//         model->setScale(3.0f);
// 
//         billboard->setMaterial("Examples/Flare");
//         billboard->setColour(flareColour);
//         billboard->setScale(0.5f);
// 
//         droppedItem->setPosition(position);
//         droppedItem->attach(model);
//         droppedItem->attach(billboard);
// 
//         COUT(3) << "Created DroppedItem for '" << item->getPickupIdentifier() << "' at (" << position.x << "," << position.y << "," << position.z << ")." << std::endl;
// 
//         return droppedItem;
//     }

    //TODO: See one function above.
//     DroppedItem* DroppedItem::createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour, float timeToLive)
//     {
//         Vector3 after = pawn->getPosition() + pawn->getOrientation() * Vector3(0.0f, 0.0f, 50.0f);
//         return DroppedItem::createDefaultDrop(item, after, flareColour, timeToLive);
//     }
}
