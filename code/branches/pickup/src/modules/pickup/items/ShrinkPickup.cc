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
		shrinkFactor_ = 5.0;
		duration_ = 5.0;
		shrinkDelay_ = 1.0;
		isActive_ = false;
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
		int i;	

		SUPER(ShrinkPickup, changedUsed);

        if(this->isUsed())
        {
            this->pawn = this->carrierToPawnHelper();
            if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->Pickupable::destroy();
	
			COUT(0) << "shrinking..." << endl;
			//this->pawn->setScale3D(this->pawn->getScale3D() / 2.0);
			std::set<WorldEntity*> set = this->pawn->getAttachedObjects();
			
			/*this->pawn->setScale3D(this->pawn->getScale3D() / 5.0);
            this->pawn->setMass(this->pawn->getMass() / 5.0);

            const std::list<SmartPtr<CameraPosition> > cameraPositions = this->pawn->getCameraPositions();
            unsigned int size = cameraPositions.size();
            for(unsigned int index = 0; index < size; index++)
            {
                CameraPosition* cameraPos = this->pawn->getCameraPosition(index);
                if(cameraPos == NULL)
                    continue;
                cameraPos->setPosition(cameraPos->getPosition()*5.0);
            }*/
			
			i = 0;
			for(std::set<WorldEntity*>::iterator it = set.begin(); it != set.end(); it++)
			{
				defaultScales_.push_back((*it)->getScale());
				actualScales_.push_back((*it)->getScale());
				defaultPositions_.push_back((*it)->getPosition());
				actualPositions_.push_back((*it)->getPosition());
				//(*it)->setScale((*it)->getScale() / 5.0);
				//(*it)->setPosition((*it)->getPosition() / 5.0);
			}
			size_ = defaultScales_.size();
			for(i = 0; i < size_; i++)
			{
				smallScales_.push_back(defaultScales_.at(i) / shrinkFactor_);
			}
			isActive_ = true;
			durationTimer.setTimer(10, false, createExecutor(createFunctor(&ShrinkPickup::terminate, this)));
        }
		else
        {
			this->Pickupable::destroy();
        }
	}

	void ShrinkPickup::tick(float dt)
	{
		double temp;
		int i;
		double k = dt / shrinkDelay_;
		if(isActive_)
		{
			for(i = 0; i < size_; i++)
			{
				temp = actualScales_.at(i);
				if(temp > smallScales_.at(i))
				{
					actualScales_.erase(i)
					actualScales_.insert(i, temp - (temp - smallScales_.at(i)) * k);
				}
				/*temp = actual;
				if(temp > smallScales[i])
				{
					actualScales[i] -= (actualScales[i] - smallScales[i]) * k;
				}*/

			}

			i = 0;
			
			std::set<WorldEntity*> set = this->pawn->getAttachedObjects();
			for(std::set<WorldEntity*>::iterator it = set.begin(); it != set.end(); it++)
			{
				//defaultScales.push_back((*it)->getScale());
				//actualScales.push_back((*it)->getScale());
				//defaultPositions.push_back((*it)->getPosition());
				//actualPositions.push_back((*it)->getPosition());
				//(*it)->setScale((*it)->getScale() *0.99);
				(*it)->setScale(actualScales_.at(i));
				//(*it)->setPosition((*it)->getPosition() / 5.0);
			}
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
