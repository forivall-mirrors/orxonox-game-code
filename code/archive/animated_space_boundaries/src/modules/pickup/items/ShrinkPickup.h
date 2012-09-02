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
    @file ShrinkPickup.h
    @brief Declaration of the ShrinkPickup class.
    @ingroup PickupItems
*/


#ifndef _ShrinkPickup_H__
#define _ShrinkPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>
#include <vector>

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    /**
    @author
        Sandro Sgier

    @ingroup PickupItems
    */

    /**
    @brief
        The ShrinkPickup is a Pickupable that causes the pawn to shrink to a certain size for a certain time with a certain speed, all of them specified in the following variables:
        - The @b shrinkFactor It determines how much the ship is going to shrink (e.g. the factor 2 would make the ship shrinking to half its size).
        - The @b duration Specifies how long the ship will keep small.
        - The @b shrinkSpeed Defines how fast the ship shrinks and grows.


        An example of a XML implementation of a HealthPickup would be:
        @code
        <HealthPickup
            shrinkFactor = "5.0"
            duration = "5.0"
            shrinkSpeed = "5.0"
        />
        @endcode

    @author
        Sandro Sgier

    @ingroup PickupItems
    */

    class _PickupExport ShrinkPickup : public Pickup, public Tickable
    {
        public:
            ShrinkPickup(BaseObject* creator); //!< Constructor.
            virtual ~ShrinkPickup(); //!< Destructor.
            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.
            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode);
            inline float getShrinkFactor(void) const
                { return this->shrinkFactor_; }
            inline float getDuration(void) const
                { return this->duration_; }
            inline float getShrinkSpeed(void) const
                { return this->shrinkSpeed_; }
            void setShrinkFactor(float factor);
            void setDuration(float duration);
            void setShrinkSpeed(float speed);
            void tick(float dt);

        protected:
            void initializeIdentifier(void);

        private:
            void initialize(void);
            float duration_;            //!< determines how long the pickup will be active
            float shrinkFactor_;        //shrink factor of the space ship
            float shrinkSpeed_;         //speed of shrinking
            bool isActive_;             //true if ship is shrinking or small
            bool isTerminating_;        //true if ship is growing
            int size_;                  //number of camera positions
            std::list<SmartPtr<CameraPosition> > cameraPositions_;
            float defaultCameraPos_;    //all default, actual and small values...
            Ogre::Vector3 defaultScale_;
            Ogre::Vector3 actualScale_;
            Ogre::Vector3 smallScale_;
            float defaultMass_;
            float actualMass_;
            float smallMass_;            
            Pawn* carrierToPawnHelper(void);
            Pawn* pawn_;
            Timer durationTimer_;
            void terminate(void);
    };
}

#endif
