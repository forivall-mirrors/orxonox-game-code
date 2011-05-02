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

        private:
            void initialize(void);
			const float duration;			//determines how long the pickup will be active
    		const float shrinkFactor;		//shrink factor of the space ship
			const float shrinkDelay;		//how long it takes to shrink to the final size
			double factorPerCall;
			bool isTerminating;
			vector<float> defaultScales;
			vector<float> smallScales;
			vector<float> actualScales;
			vector<float> defaultPositions;
			vector<float> smallPositions;
			vector<float> actualPositions;
			Pawn* carrierToPawnHelper(void);
			Pawn* pawn;
			Timer durationTimer;
			void terminate(void);

			
    };
}

#endif