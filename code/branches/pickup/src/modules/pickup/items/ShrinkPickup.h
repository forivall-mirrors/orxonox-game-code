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
    class _PickupExport ShrinkPickup : public Pickup, public Tickable
    {
        public:
            ShrinkPickup(BaseObject* creator); //!< Constructor.
            virtual ~ShrinkPickup(); //!< Destructor.
            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
			virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.
			void tick(float dt);

        private:
            void initialize(void);
			
			float duration_;			//!< determines how long the pickup will be active
    		float shrinkFactor_;		//shrink factor of the space ship
			float shrinkSpeed_;
			bool isActive_;
			bool isTerminating_;
			int size_;
			std::list<SmartPtr<CameraPosition> > cameraPositions_;
			float defaultCameraPos_;
			Ogre::Vector3 defaultScale_;
			Ogre::Vector3 actualScale_;
			Ogre::Vector3 smallScale_;
			float defaultMass_;
			float actualMass_;
			float smallMass_;			
			Pawn* carrierToPawnHelper(void);
			Pawn* pawn;
			Timer durationTimer;
			void terminate(void);

    };
}

#endif