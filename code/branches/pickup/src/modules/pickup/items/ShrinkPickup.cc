#include "ShrinkPickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "pickup/PickupIdentifier.h"
#include "worldentities/pawns/Pawn.h"

#include "weaponsystem/WeaponSlot.h"
#include "weaponsystem/Weapon.h"

namespace orxonox
{
    CreateFactory(ShrinkPickup);

    ShrinkPickup::ShrinkPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(ShrinkPickup);

        this->initialize();
    }

    ShrinkPickup::~ShrinkPickup()
    {

    }

	void ShrinkPickup::initialize(void)
	{
		this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
	}

    void ShrinkPickup::changedUsed(void)
	{
		SUPER(ShrinkPickup, changedUsed);

        if(this->isUsed())
        {
            this->pawn = this->carrierToPawnHelper();
            if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->Pickupable::destroy();
	
			COUT(0) << "shrinking..." << endl;
			//this->pawn->setScale3D(this->pawn->getScale3D() / 2.0);
			std::set<WorldEntity*> set = this->pawn->getAttachedObjects();
			for(std::set<WorldEntity*>::iterator it = set.begin(); it != set.end(); it++)
			{
				(*it)->setScale((*it)->getScale() / 5.0);
				(*it)->setPosition((*it)->getPosition() / 5.0);
			}
			
			durationTimer.setTimer(10, false, createExecutor(createFunctor(&ShrinkPickup::terminate, this)));
        }
		else
        {
			this->Pickupable::destroy();
        }
	}

	void ShrinkPickup::terminate(void)
	{
		//this->pawn->setScale3D(this->pawn->getScale3D() * 5.0);

		std::set<WorldEntity*> set = this->pawn->getAttachedObjects();
		for(std::set<WorldEntity*>::iterator it = set.begin(); it != set.end(); it++)
		{
			(*it)->setScale((*it)->getScale() * 5.0);
		}		
		setUsed(false);
	}

    Pawn* ShrinkPickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = dynamic_cast<Pawn*>(carrier);

        if(pawn == NULL)
        {
            COUT(1) << "Invalid PickupCarrier in ShrinkPickup." << std::endl;
        }

        return pawn;
    }

	/**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void ShrinkPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new ShrinkPickup(this);

        SUPER(ShrinkPickup, clone, item);
    }
}
