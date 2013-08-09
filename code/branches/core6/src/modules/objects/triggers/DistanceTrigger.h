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
 *      Benjamin Knecht
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file DistanceTrigger.h
    @brief Definition of the DistanceTrigger class.
    @ingroup NormalTrigger
*/

#ifndef _DistanceTrigger_H__
#define _DistanceTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include <set>

#include "core/ClassTreeMask.h"

#include "interfaces/PlayerTrigger.h"

#include "Trigger.h"

namespace orxonox
{
    
  /**
  @brief
      Enum for the beacon mode of the DistanceTrigger.
      
  @ingroup NormalTrigger
  */
  namespace distanceTriggerBeaconMode
  {
      enum Value {
          off,
          identify,
          exclude
      };
  }

    /**
    @brief
        The DistanceTrigger is a Trigger that triggers whenever an object (that is of the specified target type) is in a specified range of the DistanceTrigger. The object can be specified further by setting the <em>beaconMode</em> and attaching a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" to the object.
        Parameters are (additional to the ones of Trigger):
        - @b distance Which specifies the maximum distance at which the DistanceTrigger still triggers, i.e. its range. Default is <code>100</code>.
        - @b target Which specifies the class of objects that can trigger the DistanceTrigger. Default is <code>"Pawn"</code>.
        - @b beaconMode Which specifies, whether the DistanceTrigger operates on @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacons" or not. If <em>off</em> the DistanceMultiTrigger works as usual. If set to <em>identify</em> the DistanceTrigger is only triggered by objects that have a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon", with the same name as specified in <em>targetname</em>, attached to them. If set to <em>exclude</em> the DistanceTrigger is only triggered by objects that don't have a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon", with the same name as specified in <em>targetname</em>, attached to them. Default is <em>off</em>.
        - @b targetname Which specifies the name @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacons" need to have to make the DistanceTrigger react to them if it is in <em>beacon-mode</em> (the beaconMode is not <em>off</em>).

        A simple DistanceTrigger could look like this:
        @code
        <DistanceTrigger position="0,0,0" switch="true" target="Pawn" distance="20" />
        @endcode

        An implementation that only reacts to objects with a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" attached would look like this:
        @code
        <DistanceTrigger position="0,0,0" target="Pawn" beaconMode="identify" targetname="beacon1" distance="30" />
        @endcode
        This particular DistanceTrigger would only react if an object was in range, that had a @ref orxonox::DistanceTriggerBeacon "DistanceTriggerBeacon" with the name <em>beacon1</em> attached.

    @see Trigger
        For more information on @ref orxonox::Trigger "Triggers".

    @author
        Benjamin Knecht
    @author
        Damian 'Mozork' Frick

    @ingroup NormalTrigger
    */
    class _ObjectsExport DistanceTrigger : public Trigger, public PlayerTrigger
    {
        public:
            DistanceTrigger(Context* context); // Constructor. Registers and initializes the object.
            virtual ~DistanceTrigger();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); // Method for creating a DistanceTrigger object through XML.

            void addTarget(const std::string& targets); // Add some target to the DistanceTrigger.
            void removeTarget(const std::string& targets); // Remove some target from the DistanceTrigger.

            /**
            @brief Set the range of the DistanceTrigger.
            @param distance The range to be set.
            */
            inline void setDistance(float distance)
                { this->distance_ = distance; }
            /**
            @brief Get the range of the DistanceTrigger.
            @return Returns the range of the distance trigger.
            */
            inline float getDistance() const
                { return this->distance_; }

            void setBeaconModeDirect(distanceTriggerBeaconMode::Value mode); // Set the beacon mode.
            /**
            @brief Get the beacon mode.
            @return Returns the mode as an enum.
            */
            inline distanceTriggerBeaconMode::Value getBeaconModeDirect(void) const
            { return this->beaconMode_; }
            void setBeaconMode(const std::string& mode); // Set the beacon mode.
            const std::string& getBeaconMode(void) const; // Get the beacon mode.

            /**
            @brief Set the name a DistanceTriggerBeacon needs to have to make the DistanceTrigger react to it if in beacon-mode.
            @param targetname The name as a string.
            */
            inline void setTargetName(const std::string& targetname)
                { this->targetName_ = targetname; }
            /**
            @brief Get the target name.
            @return Returns the target name as a string.
            */
            inline const std::string& getTargetName(void)
                { return this->targetName_; }

            bool checkDistance(); // Check, whether there are entities that are targets of this DistanceTrigger in its range.

        protected:
            virtual bool isTriggered(TriggerMode::Value mode); // Check whether the DistanceTrigger is triggered.
            /**
            @brief Notifies interested parties about a change of the DistanceTrigger's target mask.
            */
            virtual void notifyMaskUpdate() {}

            ClassTreeMask targetMask_; //!< The target mask, specifies by which types of objects the DistanceTrigger can be triggered. 

        private:
            //! Strings for the beacon modes.
            static const std::string beaconModeOff_s;
            static const std::string beaconModeIdentify_s;
            static const std::string beaconModeExlcude_s;

            float distance_; //!< The range of the DistanceTrigger.
            
            distanceTriggerBeaconMode::Value beaconMode_; //!< The beacon mode.
            std::string targetName_; //!< The name a DistanceTriggerBeacon needs to have to make the DistanceTrigger react to it if in beacon-mode.
            ClassTreeMask beaconMask_; //!< A mask, that only accepts DistanceTriggerBeacons.
            
            WeakPtr<WorldEntity> cache_; //!< Caches the entity that triggered the DistanceTrigger last.
    };
}

#endif /* _DistanceTrigger_H__ */
