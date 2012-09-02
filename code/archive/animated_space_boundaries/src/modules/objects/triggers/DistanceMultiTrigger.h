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
    @ingroup MultiTrigger
*/

#ifndef _DistanceMultiTrigger_H__
#define _DistanceMultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include <map>

#include "core/WeakPtr.h"

#include "worldentities/WorldEntity.h"

#include "MultiTrigger.h"

namespace orxonox
{

    /**
    @brief
        Enum for the beacon mode of the DistanceMultiTrigger.
        
    @ingroup MultiTrigger
    */
    namespace distanceMultiTriggerBeaconMode
    {
        enum Value {
            off, //!< The DistanceMultiTrigger is not in <em>beacon-mode</em>.
            identify, //!< The DistanceTrigger is in <em>identify-mode</em>.
            exclude //!< The DistanceTrigger is in <em>exclude-mode</em>.
        };
    }

    /**
    @brief
        The DistanceMultiTrigger is a MultiTrigger that triggers whenever an object (that is of the specified target type) is in a specified range of the DistanceMultiTrigger. The object can be specified further by setting the <em>beaconMode</em> and attaching a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" to the object.
        Parameters are (additional to the ones of MultiTrigger):
        - @b distance Which specifies the maximum distance at which the DistanceMultiTrigger still triggers. Default is <code>100</code>.
        - @b beaconMode Which specifies, whether the DistanceMultiTrigger operates on @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacons" or not. If <em>off</em> the DistanceMultiTrigger works as usual. If set to <em>identify</em> the DistanceMultiTrigger is only triggered by objects that have a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon", with the same name as specified in <em>targetname</em>, attached to them. If set to <em>exclude</em> the DistanceMultiTrigger is only triggered by objects that don't have a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon", with the same name as specified in <em>targetname</em>, attached to them. Default is <em>off</em>.
        - @b targetname Which specifies the name @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacons" need to have to make the DistanceMultiTrigger react to them if it is in <em>beacon-mode</em> (the beaconMode is not <em>off</em>).

        A simple DistanceMultiTrigger could look like this:
        @code
        <DistanceMultiTrigger position="0,0,0" switch="true" target="Pawn" distance="20" />
        @endcode

        An implementation that only reacts to objects with a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" attached would look like this:
        @code
        <DistanceMultiTrigger position="0,0,0" target="Pawn" beaconMode="identify" targetname="beacon1" distance="30" />
        @endcode
        This particular DistanceMultiTrigger would only react if an object was in range, that had a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" with the name <em>beacon1</em> attached.

    @see MultiTrigger
        For more information on @ref orxonox::MultiTrigger "MultiTriggers".

    @author
        Damian 'Mozork' Frick

    @ingroup MultiTrigger
    */
    class _ObjectsExport DistanceMultiTrigger : public MultiTrigger
    {

        public:
            DistanceMultiTrigger(BaseObject* creator); // Default Constructor. Registers the object and initializes default values.
            virtual ~DistanceMultiTrigger(); // Destructor.

            void XMLPort(Element& xmlelement, XMLPort::Mode mode); // Method for creating a DistanceMultiTrigger object through XML.
            
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
            
            void setBeaconModeDirect(distanceMultiTriggerBeaconMode::Value mode); // Set the beacon mode.
            /**
            @brief Get the beacon mode.
            @return Returns the mode as an enum.
            */
            inline distanceMultiTriggerBeaconMode::Value getBeaconModeDirect(void) const
                { return this->beaconMode_; }
            void setBeaconMode(const std::string& mode); // Set the beacon mode.
            const std::string& getBeaconMode(void) const; // Get the beacon mode.

            /**
            @brief Set the target name of DistanceTriggerBeacons that triggers this DistanceMultiTrigger.
            @param targetname The name of the DistanceTriggerBeacon as a string.
            */
            inline void setTargetName(const std::string& targetname)
                { this->targetName_ = targetname; }
            /**
            @brief Get the target name of the DistanceTriggerbeacon, that triggers this DistanceMultiTrigger.
            @return Returns the target name as a string.
            */
            inline const std::string& getTargetName(void) const
                { return this->targetName_; }

        protected:
            virtual std::queue<MultiTriggerState*>* letTrigger(void); // This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

            bool addToRange(WorldEntity* entity); // Add a given entity to the entities, that currently are in range of the DistanceMultiTrigger.
            bool removeFromRange(WorldEntity* entity); // Remove a given entity from the set of entities, that currently are in range of the DistanceMultiTrigger.

        private:
            //! Strings for the beacon modes.
            static const std::string beaconModeOff_s;
            static const std::string beaconModeIdentify_s;
            static const std::string beaconModeExlcude_s;
            
            float distance_; //!< The distance at which the DistanceMultiTrigger triggers.

            distanceMultiTriggerBeaconMode::Value beaconMode_; //!< The beacon mode, the DistanceMultiTrigger is in.
            std::string targetName_; //!< The target name, used in <em>single-target</em> mode.
            ClassTreeMask beaconMask_; //!< A mask, that only accepts DistanceTriggerBeacons.

            std::map<WorldEntity*, WeakPtr<WorldEntity>* > range_; //!< The set of entities that currently are in range of the DistanceMultiTrigger.

    };

}

#endif // _DistanceMultiTrigger_H__
