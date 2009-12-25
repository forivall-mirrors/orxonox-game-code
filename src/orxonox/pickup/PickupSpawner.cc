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

/**
    @file
    @brief Implementation of PickupSpawner.
*/

#include "PickupSpawner.h"

#include "BaseItem.h"

#include "core/CoreIncludes.h"
#include "core/GUIManager.h"     // HACK; see below
#include "core/Template.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "PickupInventory.h"    // HACK; Only for hack, remove later


namespace orxonox
{
    const float PickupSpawner::bounceSpeed_s = 6.0f;
    const float PickupSpawner::rotationSpeed_s = 1.0f;
    const float PickupSpawner::bounceDistance_s = 4.0f;

    CreateFactory(PickupSpawner);

    /**
        @brief Constructor. Registers the PickupSpawner.
        @param creator Pointer to the object which created this item.
    */
    PickupSpawner::PickupSpawner(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(PickupSpawner);

        this->itemTemplate_ = 0;
        this->triggerDistance_ = 20;
        this->respawnTime_ = 0.0f;
        this->tickSum_ = 0.0f;
    }
    //! Deconstructor.
    PickupSpawner::~PickupSpawner()
    {
    }
    /**
        @brief Method for creating a PickupSpawner through XML.
        @param xmlelement XML element which contains the PickupSpawner.
        @param mode XMLPort mode.
    */
    void PickupSpawner::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupSpawner, XMLPort, xmlelement, mode);

        XMLPortParam(PickupSpawner, "item", setItemTemplateName, getItemTemplateName, xmlelement, mode);
        XMLPortParam(PickupSpawner, "triggerDistance", setTriggerDistance, getTriggerDistance, xmlelement, mode);
        XMLPortParam(PickupSpawner, "respawnTime", setRespawnTime, getRespawnTime, xmlelement, mode);

        // HACKs
        // Load the GUI image as soon as the PickupSpawner gets loaded
        //  = less delays while running
        BaseObject* newObject = this->itemTemplate_->getBaseclassIdentifier()->fabricate(this);
        BaseItem* asItem = orxonox_cast<BaseItem*>(newObject);
        if (asItem)
        {
            asItem->addTemplate(this->itemTemplate_);
            PickupInventory::getImageForItem(asItem);
            newObject->destroy();
        }

        //  & load the GUI itself too, along with some empty windows
        //   = even less delays
        GUIManager::showGUI("PickupInventory");
        GUIManager::hideGUI("PickupInventory");
        PickupInventory::getSingleton();
    }
    /**
        @brief Invoked when the activity has changed. Sets visibility of attached objects.
    */
    void PickupSpawner::changedActivity()
    {
        SUPER(PickupSpawner, changedActivity);

        for (std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin(); it != this->getAttachedObjects().end(); it++)
            (*it)->setVisible(this->isActive());
    }
    /**
        @brief Set the template name of the item to spawn, also loads the template.
        @param name Name of the new template.
    */
    void PickupSpawner::setItemTemplateName(const std::string& name)
    {
        this->itemTemplateName_ = name;
        this->itemTemplate_ = Template::getTemplate(name);
    }
    /**
        @brief Tick, checks if any Pawn is close enough to trigger.
        @param dt Time since last tick.
    */
    void PickupSpawner::tick(float dt)
    {
        if (this->isActive())
        {
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                if (distance.length() < this->triggerDistance_)
                    this->trigger(*it);
            }
            this->yaw(Radian(rotationSpeed_s*dt));
            this->tickSum_ += bounceSpeed_s*dt;
            this->translate(Vector3(0,bounceDistance_s*dt*sin(this->tickSum_),0));
            if (this->tickSum_ > 2*Ogre::Math::PI)
                this->tickSum_ -= 2*Ogre::Math::PI;
        }
    }
    /**
        @brief
            Trigger the PickupSpawner.

            Adds the pickup to the Pawn that triggered,
            sets the timer to re-activate and deactives the PickupSpawner.

        @param pawn Pawn which triggered the PickupSpawner.
    */
    void PickupSpawner::trigger(Pawn* pawn)
    {
        if (this->isActive() && this->itemTemplate_ && this->itemTemplate_->getBaseclassIdentifier())
        {
            BaseObject* newObject = this->itemTemplate_->getBaseclassIdentifier()->fabricate(this);
            BaseItem* asItem = orxonox_cast<BaseItem*>(newObject);
            if (asItem)
            {
                asItem->setPickupIdentifier(this->itemTemplateName_);
                asItem->addTemplate(this->itemTemplate_);

                if (asItem->pickedUp(pawn))
                {
                    COUT(3) << this->itemTemplateName_ << " got picked up." << std::endl;

                    if (this->respawnTime_ > 0.0f)
                    {
                        this->respawnTimer_.setTimer(this->respawnTime_, false, createExecutor(createFunctor(&PickupSpawner::respawnTimerCallback, this)));

                        this->setActive(false);
                        this->fireEvent();
                    }
                }
                else
                    newObject->destroy();
            }
        }
    }
    /**
        @brief Invoked by the timer, re-activates the PickupSpawner.
    */
    void PickupSpawner::respawnTimerCallback()
    {
        COUT(3) << "PickupSpawner reactivated." << std::endl;

        this->setActive(true);
    }
}
