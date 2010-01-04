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
    @brief Definition of PickupSpawner.
*/

#ifndef _PickupSpawner_H__
#define _PickupSpawner_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    /**
        @brief PickupSpawner.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport PickupSpawner : public StaticEntity, public Tickable
    {
    public:
        PickupSpawner(BaseObject* creator);
        virtual ~PickupSpawner();

        virtual void changedActivity();                                 //!< Invoked when activity has changed (set visibilty).
        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< Method for creating a PickupSpawner through XML.
        virtual void tick(float dt);

        void trigger(Pawn* pawn);                                       //!< Method called when a Pawn is close enough.
        void respawnTimerCallback();                                    //!< Method called when the timer runs out.

        /**
            @brief Get the template name for the item to spawn.
            @return Returns the name of the template of the item to spawn.
        */
        inline const std::string& getItemTemplateName() const
            { return this->itemTemplateName_; }
        void setItemTemplateName(const std::string& name);              //!< Set the template name of the item to spawn.

        /**
            @brief Get the template for the item to spawn.
            @return Returns the template of the item to spawn.
        */
        inline Template* getItemTemplate() const
            { return this->itemTemplate_; }

        /**
            @brief Get the distance in which to trigger.
            @return Returns the distance in which this gets triggered.
        */
        inline float getTriggerDistance() const
            { return this->triggerDistance_; }
        /**
            @brief Set the distance in which to trigger.
            @param value The new distance in which to trigger.
        */
        inline void setTriggerDistance(float value)
            { this->triggerDistance_ = value; }

        /**
            @brief Get the time to respawn.
            @returns Returns the time after which this gets re-actived.
        */
        inline float getRespawnTime() const
            { return this->respawnTime_; }
        /**
            @brief Set the time to respawn.
            @param time New time after which this gets re-actived.
        */
        inline void setRespawnTime(float time)
            { this->respawnTime_ = time; }
    private:
        std::string itemTemplateName_;          //!< Template name of the item to spawn.
        Template* itemTemplate_;                //!< Template of the item to spawn.

        float triggerDistance_;                 //!< Distance in which this gets triggered.

        /* Pickup animation */
        float tickSum_;                         //!< Adds up tick to use in sine movement
        static const float bounceSpeed_s;       //!< Speed of pickup to bounce up and down
        static const float bounceDistance_s;    //!< Distance the pickup bounces up and down
        static const float rotationSpeed_s;     //!< Rotation speed of pickup

        float respawnTime_;                     //!< Time after which this gets re-actived.
        Timer respawnTimer_;                    //!< Timer used for re-activating.
    };
}

#endif /* _PickupSpawner_H__ */
