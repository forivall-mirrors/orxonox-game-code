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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

/**
    @file DistanceMultiTrigger.h
    @brief Definition of the DistanceMultiTrigger class.
*/

#ifndef _DistanceMultiTrigger_H__
#define _DistanceMultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "worldentities/WorldEntity.h"
#include "core/WeakPtr.h"
#include <map>

#include "MultiTrigger.h"

namespace orxonox
{

    /**
    @brief
        The DistanceMultiTrigger is a trigger that triggers whenever an object (that is of the specified target type) is in a specified range of the DistanceMultiTrigger. The object can be specified further by adding a DistanceTriggerBeacon (just attaching it) to the objects that can trigger this DistanceMultiTrigger and specify the name of the DistanceTriggerBeacon with the parameter targetname and only objects that hav a DistanceTriggerBeacon with that name attached will trigger the DistanceMultiTrigger.
    @see MultiTrigger.h
        For more information on MultiTriggers.
    @author
        Damian 'Mozork' Frick
    */
    class _ObjectsExport DistanceMultiTrigger : public MultiTrigger
    {

        public:
            DistanceMultiTrigger(BaseObject* creator); //!< Default Constructor. Registers the object and initializes default values.
            ~DistanceMultiTrigger(); //!< Destructor.

            void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a DistanceMultiTrigger object through XML.

            /**
            @brief Set the target name of DistanceTriggerBeacons that triggers this DistanceMultiTrigger.
            @param targetname The name of the DistanceTriggerBeacon as a string.
            */
            inline void setTargetName(const std::string& targetname)
                { if(targetname.compare(BLANKSTRING) != 0) this->singleTargetMode_ = true; else this->singleTargetMode_ = false; this->targetName_ = targetname; }
            /**
            @brief Get the target name of the DistanceTriggerbeacon, that triggers this DistanceMultiTrigger.
            @return Returns the target name as a string.
            */
            inline const std::string& getTargetName(void)
                { return this->targetName_; }

            /**
            @brief Set the distance at which the DistanceMultiTrigger triggers.
            @param distance The distance.
            */
            inline void setDistance(float distance)
                { if(distance >= 0) this->distance_ = distance; }
            /**
            @brief Get the distance at which the DistanceMultiTrigger triggers.
            @return Returns the distance.
            */
            inline float getDistance() const
                { return this->distance_; }

        protected:
            virtual std::queue<MultiTriggerState*>* letTrigger(void); //!< This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

            /**
            @brief Add a given entity to the entities, that currently are in range of the DistanceMultiTrigger.
            @param entity A pointer to the entity.
            @return Returns true if successful, false if not.
            */
            inline bool addToRange(WorldEntity* entity)
                { std::pair<std::map<WorldEntity*, WeakPtr<WorldEntity>* >::iterator, bool> pair = this->range_.insert(std::pair<WorldEntity*, WeakPtr<WorldEntity>* >(entity, new WeakPtr<WorldEntity>(entity))); return pair.second; }
            /**
            @brief Remove a given entity from the set of entities, that currently are in range of the DistanceMultiTrigger.
            @param entity A pointer ot the entity.
            @return Returns true if successful.
            */
            inline bool removeFromRange(WorldEntity* entity)
                { WeakPtr<WorldEntity>* weakptr = this->range_.find(entity)->second; bool erased = this->range_.erase(entity) > 0; if(erased) delete weakptr; return erased; }

        private:
            float distance_; //!< The distance at which the DistanceMultiTrigger triggers.
            std::string targetName_; //!< The target name, used in singleTargetMode.
            bool singleTargetMode_; //!< To indicate whe the MultiDistanceTrigger is in single-target-mode.

            std::map<WorldEntity*, WeakPtr<WorldEntity>* > range_; //!< The set of entities that currently are in range of the DistanceMultiTrigger.

    };

}

#endif // _DistanceMultiTrigger_H__
