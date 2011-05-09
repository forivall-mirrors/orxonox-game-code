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
			float shrinkDelay_;		//how long it takes to shrink to the final size
			bool isActive_;
			bool isTerminating_;
			int size_;
			std::vector<float> defaultScales_;
			std::vector<float> smallScales_;
			std::vector<float> actualScales_;
			std::vector<Vector3> defaultPositions_;
			std::vector<Vector3> smallPositions_;
			std::vector<Vector3> actualPositions_;
			Pawn* carrierToPawnHelper(void);
			Pawn* pawn;
			Timer durationTimer;
			void terminate(void);

    };
}

#endif