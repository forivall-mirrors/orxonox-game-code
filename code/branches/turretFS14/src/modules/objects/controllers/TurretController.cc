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
 *      Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

#include "TurretController.h"
#include "worldentities/pawns/Pawn.h"
#include "Turret.h"

 namespace orxonox
 {
 	RegisterClass(TurretController);

 	TurretController::TurretController(Context* context) : ArtificialController(context)
 	{
 		RegisterObject(TurretController);
 		this->startOrient_ = Quaternion::IDENTITY;
        this->startDir_ = Vector3::ZERO;
        this->localZ_ = Vector3::UNIT_Z;
        this->localY_ = Vector3::UNIT_Y;
        this->localX_ = Vector3::UNIT_X;
        this->attackRadius_ = 200;
        this->maxPitch_ = 90;
        this->maxYaw_ = 90;
        this->gotOrient_ = false;
 	}

 	TurretController::~TurretController()
 	{

 	}

 	void TurretController::searchTarget()
 	{
        Turret* turret = orxonox_cast<Turret*>(this->getControllableEntity());
        if(target_ && this->isInRange(target_->getWorldPosition()))
        {
        	return;
        }
        else
        {
        	this->forgetTarget();
        }


        ControllableEntity* parent = orxonox_cast<ControllableEntity*>(turret->getParent());
        if(parent)
        {
        	Pawn* parenttarget = orxonox_cast<Pawn*>(parent->getTarget());
        	if(parenttarget && this->isInRange(parenttarget->getWorldPosition()))
        	{
        		this->setTarget(parenttarget);
        		turret->setTarget(parenttarget);
        		return;
        	}
        }

  		for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
        {
        	Pawn* entity = orxonox_cast<Pawn*>(*it);
            if (ArtificialController::sameTeam(this->getControllableEntity(), entity, this->getGametype()))
            	continue;

            if(this->isInRange(entity->getWorldPosition()))
            {
            	this->setTarget(entity);
            	turret->setTarget(entity);
            	break;
            }
    	}		
 	}

 	bool TurretController::isInRange(const Vector3 &position)
    {
    	//Check distance
        Vector3 distance = position - this->getControllableEntity()->getWorldPosition();
        if(distance.squaredLength() > (this->attackRadius_ * this->attackRadius_))
        {
            return false;
        }

        //Check pitch
        Vector3 dir = getTransformedVector(distance, this->localX_, this->localY_, this->localZ_);
        Vector3 dirProjected = dir;
        dirProjected.x = 0;
        Vector3 startDirProjected = this->startDir_;
        startDirProjected.x = 0;
        Ogre::Real angle = startDirProjected.angleBetween(dirProjected).valueDegrees();
        if(angle > this->maxPitch_)
        {
            return false;
        }

        //Check yaw
        dirProjected = dir;
        dirProjected.y = 0;
        startDirProjected = this->startDir_;
        startDirProjected.y = 0;
        angle = startDirProjected.angleBetween(dirProjected).valueDegrees();
        if(angle > this->maxYaw_)
        {
            return false;
        }
        return true;
    }

    void TurretController::aimAtPositionRot(const Vector3 &position)
    {

        Vector3 currDir = this->getControllableEntity()->getWorldOrientation() * WorldEntity::FRONT;
        Vector3 targetDir = position - this->getControllableEntity()->getWorldPosition();

        Quaternion rot = currDir.getRotationTo(targetDir);

        //Don't make the rotation instantaneous
        rot = Quaternion::Slerp(0.1, Quaternion::IDENTITY, rot);

        this->getControllableEntity()->rotate(rot, WorldEntity::World);
    }
    

    void TurretController::aimAtTargetRot()
    {
    	this->aimAtPositionRot(this->target_->getWorldPosition());
    }

    bool TurretController::isLookingAtTargetNew(float angle) const
    {
        return (getAngle(this->getControllableEntity()->getWorldPosition(), this->getControllableEntity()->getWorldOrientation() * WorldEntity::FRONT, this->target_->getWorldPosition()) < angle);
    }

 	void TurretController::tick(float dt)
 	{
			if(!gotOrient_)
	        {
	            this->startOrient_ = this->getControllableEntity()->getOrientation();
	            this->localXStart_ = this->startOrient_ * this->localX_;
	            this->localXStart_.normalise();
	            this->localX_ = this->localXStart_;
	            this->localYStart_ = this->startOrient_ * this->localY_;
	            this->localYStart_.normalise();
	            this->localY_ = this->localYStart_;
	            this->localZStart_ = this->startOrient_ * this->localZ_;
	            this->localZStart_.normalise();
	            this->localZ_ = this->localZStart_;

	            //startDir should always be (0,0,-1)
	            this->startDir_ = getTransformedVector(this->startOrient_ * WorldEntity::FRONT, this->localX_, this->localY_, this->localZ_);

	            this->gotOrient_ = true;

	        }

	        WorldEntity* parent = this->getControllableEntity()->getParent();
	        if(parent)
	        {
	            Quaternion parentrot = parent->getOrientation();
	            this->localX_ = parentrot * this->localXStart_;
	            this->localY_ = parentrot * this->localYStart_;
	            this->localZ_ = parentrot * this->localZStart_;
	        }



	        if (!this->isActive() || !this->getControllableEntity())
	            return;

	 		this->searchTarget();
	 		if(target_)
	 		{
	 			this->aimAtTarget();
	 			//this->getControllableEntity()->lookAt(this->targetPosition_);
	 			//It says move, but really it only turns
	 			this->aimAtTargetRot();
	 			if(this->isLookingAtTargetNew(Degree(5).valueRadians()))
	 			{
	 				orxout() << 42 << endl;
	 			}
	 		}
 	}
 }