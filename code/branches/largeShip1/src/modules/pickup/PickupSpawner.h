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
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file PickupSpawner.h
    @brief Definition of the PickupSpawner class.
    @ingroup Pickup
*/

#ifndef _PickupSpawner_H__
#define _PickupSpawner_H__

#include "PickupPrereqs.h"

#include <map>
#include <string>
#include "tools/Timer.h"

#include "interfaces/Pickupable.h"

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    /**
        @brief
            The PickupSpawner class is responsible for spawning @ref orxonox::Pickupable "Pickupables" of a specific type.
            Furthermore it can be specified how long the time interval between spawning two items is and how many @ref orxonox::Pickupable "Pickupables" are spawned at maximum, amongst other things. The parameters that can be used to further specify the behaviour of the PickupSpawner are:
            - The <b>triggerDistance</b> can be used to specify how far away an entity has to be to still trigger the PickupSPawner (and thus receive a @ref orxonox::Pickupable "Pickupable" form it). The default is 10.
            - The <b>respawnTime</b> is the time in seconds that passes until the PickupSpawner is enabled again, after having spawned a @ref orxonox::Pickupable "Pickupable". The default is 0.
            - The <b>maxSpawnedItems</b> is the number of @ref orxonox::Pickupable "Pickupables" that are spawned by this PickupSpawner at the most. The default is -1, which denotes infinity.

            A PickupSpawner is created in XML, which can be done in the following fashion:
            @code
            <PickupSpawner position="-100,0,-100" respawnTime="30" maxSpawnedItems="10">
                <pickup>
                    <SomePickup >
                </pickup>
            </PickupSpawner>
            @endcode
            As we can see, since the PickupSpawner is a StaticEntity, it also has spatial coordinates. We can also see, that the type of @ref orxonox::Pickupable "Pickupable" which is spawned hast to be specified as well.

        @author
            Daniel 'Huty' Haggenmueller
        @author
            Damian 'Mozork' Frick

        @ingroup Pickup
    */
    class _PickupExport PickupSpawner : public StaticEntity, public Tickable
    {
        public:
            PickupSpawner(BaseObject* creator); //!< Default Constructor.
            virtual ~PickupSpawner(); //!< Destructor.

            static PickupSpawner* createDroppedPickup(BaseObject* creator, Pickupable* pickup, PickupCarrier* carrier, float triggerDistance = 10.0);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< Method for creating a PickupSpawner through XML.
            virtual void tick(float dt); //!< Tick, checks if any Pawn is close enough to trigger.

            /**
            @brief Get the distance in which to trigger.
            @return Returns the distance in which this gets triggered.
            */
            inline float getTriggerDistance() const
                { return this->triggerDistance_; }
            /**
            @brief Get the time to respawn.
            @returns Returns the time after which this gets re-actived.
            */
            inline float getRespawnTime() const
                { return this->respawnTime_; }
            /**
            @brief Get the maximum number of items that will be spawned by this PickupSpawner.
            @return Returns the maximum number of items spawned by this PickupSpawner.
            */
            inline int getMaxSpawnedItems(void) const
                { return this->maxSpawnedItems_; }
            /**
            @brief Returns the name of the template which is used to create a pickup for this spawner.
            */
            inline const std::string& getPickupTemplateName() const
                { return this->pickupTemplateName_; }
            /**
            @brief Returns the template which is used to create a pickup for this spawner.
            */
            inline Template* getPickupTemplate() const
                {return this->pickupTemplate_; }

        private:
            void initialize(void);

            void trigger(PickupCarrier* carrier); //!< Method called when a carrier is close enough.
            void respawnTimerCallback(); //!< Method called when the timer runs out.

            void decrementSpawnsRemaining(void); //!< Decrements the number of remaining spawns.
            void startRespawnTimer(void); //!< Invoked by the timer, re-activates the PickupSpawner.

            /**
            @brief Helper method. Adds a PickupCarrier to the list of PickupCarrier that are blocked form getting a Pickupable from the PickupSpawner for a specified time.
            @param carrier A pointer to the PickupCarrier to be blocked.
            @param time The time for which the Pawn is blocked. Default is 5.
            */
            void block(PickupCarrier* carrier, unsigned int time = DEFAULT_BLOCKED_TIME)
                { this->blocked_.insert(std::pair<PickupCarrier*, std::time_t>(carrier, std::time(0)+time)); }

            /**
            @brief Set the distance in which to trigger.
            @param value The new distance in which to trigger.
            */
            inline void setTriggerDistance(float value)
                { this->triggerDistance_ = value; }
            /**
            @brief Set the time to respawn.
            @param time New time after which this gets re-actived.
            */
            inline void setRespawnTime(float time)
                { this->respawnTime_ = time; }

            void setMaxSpawnedItems(int items); //!< Sets the maximum number of spawned items.

            void setPickupTemplateName(const std::string& name);
            void setPickupTemplate(Template* temp);

            Pickupable* createPickup(void); //!< Creates a new Pickupable.
            void setPickupable(Pickupable* pickup); //!< Sets a Pickupable for the PickupSpawner to spawn.

            Pickupable* pickup_; //!< The pickup to be spawned.
            StaticEntity* representation_; //!< The active representation of the spawner.
            std::string pickupTemplateName_; //!< The name of the pickup template.
            Template* pickupTemplate_; //!< The template to be used to create a pickupable.

            int maxSpawnedItems_; //!< Maximum number of items spawned by this PickupSpawner.
            int spawnsRemaining_; //!< Number of items that can be spawned by this PickupSpawner until it selfdestructs.

            float triggerDistance_; //!< Distance in which this gets triggered.

            float respawnTime_; //!< Time after which this gets re-actived.
            Timer respawnTimer_; //!< Timer used for re-activating.
            std::map<PickupCarrier*, std::time_t> blocked_;

            bool selfDestruct_; //!< True if the PickupSpawner is selfdestructing.

            static const int INF = -1; //!< Constant for infinity.
            static const unsigned int DEFAULT_BLOCKED_TIME = 5; //!< The default time a PickupCarrier is blocked after picking up a Pickupable.
    };
}

#endif /* _PickupSpawner_H__ */
