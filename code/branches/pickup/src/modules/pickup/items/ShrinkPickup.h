#ifndef _ShrinkPickup_H__
#define _ShrinkPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

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
    		Pawn* carrierToPawnHelper(void);
			Pawn* pawn;
			Timer durationTimer;
			void terminate(void);

			
    };
}

#endif
