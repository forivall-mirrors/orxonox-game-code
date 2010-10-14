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
            PickupSpawner(BaseObject* creator, Pickupable* pickup, float triggerDistance, float respawnTime, int maxSpawnedItems); //!< Constructor.
            virtual ~PickupSpawner(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< Method for creating a PickupSpawner through XML.
            virtual void changedActivity(); //!< Invoked when activity has changed (set visibilty).
            virtual void tick(float dt); //!< Tick, checks if any Pawn is close enough to trigger.

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

            /**
            @brief Get the maximum number of items that will be spawned by this PickupSpawner.
            @return Returns the maximum number of items spawned by this PickupSpawner.
            */
            inline int getMaxSpawnedItems(void)
                { return this->maxSpawnedItems_; }
            void setMaxSpawnedItems(int items); //!< Sets the maximum number of spawned items.

        protected:
            void decrementSpawnsRemaining(void); //!< Decrements the number of remaining spawns.

            void startRespawnTimer(void);

            virtual Pickupable* getPickup(void); //!< Creates a new Pickupable.

            void setPickupable(Pickupable* pickup); //!< Sets a Pickupable for the PickupSpawner to spawn.
            const Pickupable* getPickupable(void); //!< Get the Pickupable that is spawned by this PickupSpawner.

            Pickupable* pickup_; //!< The pickup to be spawned.

        private:
            void initialize(void);

            void trigger(Pawn* pawn); //!< Method called when a Pawn is close enough.
            void respawnTimerCallback(); //!< Method called when the timer runs out.

            int maxSpawnedItems_; //!< Maximum number of items spawned by this PickupSpawner.
            int spawnsRemaining_; //!< Number of items that can be spawned by this PickupSpawner until it selfdestructs.

            float triggerDistance_; //!< Distance in which this gets triggered.

            float respawnTime_; //!< Time after which this gets re-actived.
            Timer respawnTimer_; //!< Timer used for re-activating.

            bool selfDestruct_; //!< True if the PickupSpawner is selfdestructing.

            static const int INF = -1; //!< Constant for infinity.
    };
}

#endif /* _PickupSpawner_H__ */
