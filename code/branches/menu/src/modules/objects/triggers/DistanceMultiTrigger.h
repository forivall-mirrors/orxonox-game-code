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
        The DistanceMultiTrigger is a trigger that triggers whenever an object (that is of the specified target type) is in a specified range of the DistanceMultiTrigger. The object can be specified further by adding a DistanceTriggerBeacon (just attaching it) to the objects that can trigger this DistanceMultiTrigger and specify the name of the DistanceTriggerBeacon with the parameter targetname and only objects that have a DistanceTriggerBeacon with that name attached will trigger the DistanceMultiTrigger.
        Parameters are (additional to the ones of MultiTrigger):
            'distance', which specifies the maximum distance at which the DistanceMultiTrigger still triggers. Default is 100.
            'targetname', which, if not left blank, causes the DistancMultiTrigger to be in single-target mode, meaning, that it only reacts to objects that have a DistanceTriggerBeacon (therefore the target has to be set to DistanceTriggerBeacon for it to work), with the name specified by targetname, attached.

        A simple DistanceMultiTrigger would look like this:
        @code
        <DistanceMultiTrigger position="0,0,0" switch="true" target="Pawn" distance="20" />
        @endcode

        An implementation that only reacts to objects with a DistanceTriggerBeacon attached would look like this:
        @code
        <DistanceMultiTrigger position="0,0,0" target="DistanceMultiTrigger" targetname="beacon1" distance="30" />
        @endcode
        This particular DistanceMultiTrigger would only react if an object was in range, that had a DistanceTriggerBeacon with the name 'beacon1' attached.
    @see MultiTrigger.h
        For more information on MultiTriggers.
    @author
        Damian 'Mozork' Frick
    */
    class _ObjectsExport DistanceMultiTrigger : public MultiTrigger
    {

        public:
            DistanceMultiTrigger(BaseObject* creator); //!< Default Constructor. Registers the object and initializes default values.
            virtual ~DistanceMultiTrigger(); //!< Destructor.

            void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a DistanceMultiTrigger object through XML.

            void setTargetName(const std::string& targetname); //!< Set the target name of DistanceTriggerBeacons that triggers this DistanceMultiTrigger.
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

            bool addToRange(WorldEntity* entity); //!< Add a given entity to the entities, that currently are in range of the DistanceMultiTrigger.
            bool removeFromRange(WorldEntity* entity); //!< Remove a given entity from the set of entities, that currently are in range of the DistanceMultiTrigger.

        private:
            float distance_; //!< The distance at which the DistanceMultiTrigger triggers.
            std::string targetName_; //!< The target name, used in singleTargetMode.
            bool singleTargetMode_; //!< To indicate whe the MultiDistanceTrigger is in single-target-mode.

            std::map<WorldEntity*, WeakPtr<WorldEntity>* > range_; //!< The set of entities that currently are in range of the DistanceMultiTrigger.

    };

}

#endif // _DistanceMultiTrigger_H__
