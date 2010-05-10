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
 *      Cyrill Frei
 *   Co-authors:
 *      ...
 *
 */

#include "FpsPlayer.h"

#include <OgreSceneNode.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btVector3.h>
#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>


#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "items/Engine.h"

#include <cmath>

namespace orxonox
{
    const float orientationGain = 100;
    const float jumpvalue = 300;
    CreateFactory(FpsPlayer);

    FpsPlayer::FpsPlayer(BaseObject* creator) : Pawn(creator)
    {
        RegisterObject(FpsPlayer);
        this->speed_ = 200;
        this->localVelocity_ = Vector3::ZERO;
/* 
 *        this->primaryThrust_  = 100;
 *        this->auxilaryThrust_ =  30;
 *        this->rotationThrust_ =  10;
 *
 *        this->localLinearAcceleration_.setValue(0, 0, 0);
 *        this->localAngularAcceleration_.setValue(0, 0, 0);
 *        this->bBoost_ = false;
 *        this->bPermanentBoost_ = false;
 *        this->steering_ = Vector3::ZERO;
*/      


        this->bInvertYAxis_ = false;

        this->setDestroyWhenPlayerLeft(true);

        // FpsPlayer is always a physical object per default
        // Be aware of this call: The collision type legality check will not reach derived classes!
        this->setCollisionType(WorldEntity::Dynamic);
        // Get notification about collisions
        this->enableCollisionCallback();

        this->setConfigValues();
        this->registerVariables();
	
    }

    FpsPlayer::~FpsPlayer()
    {
    }

    void FpsPlayer::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FpsPlayer, XMLPort, xmlelement, mode);
        
        XMLPortParamVariable(FpsPlayer, "primaryThrust",  primaryThrust_,  xmlelement, mode);
        XMLPortParamVariable(FpsPlayer, "auxilaryThrust", auxilaryThrust_, xmlelement, mode);
        XMLPortParamVariable(FpsPlayer, "rotationThrust", rotationThrust_, xmlelement, mode);
    }

    void FpsPlayer::registerVariables()
    {
        registerVariable(this->primaryThrust_,  VariableDirection::ToClient);
        registerVariable(this->auxilaryThrust_, VariableDirection::ToClient);
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
    }

    void FpsPlayer::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = targetting down).");
    }

    bool FpsPlayer::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type != WorldEntity::Dynamic)
        {
            CCOUT(1) << "Error: Cannot tell a FpsPlayer not to be dynamic! Ignoring." << std::endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void FpsPlayer::tick(float dt)
    {
        if (this->hasLocalController())
        {
            this->setOrientation(savedOrientation_);
	    
	    thistickboost=false;
	    
	    float localSpeedSquared = this->localVelocity_.squaredLength();
            float localSpeed;
            if (localSpeedSquared > 1.0)
                localSpeed = this->speed_ / sqrtf(localSpeedSquared);
            else
                localSpeed = this->speed_;

            this->localVelocity_.x *= localSpeed;
            this->localVelocity_.z *= localSpeed;
	    Vector3 temp = this->getOrientation() * this->localVelocity_;
	    if(localVelocity_.y==jumpvalue) this->setVelocity(Vector3(temp.x, temp.y + this->getVelocity().y, temp.z));
	    else this->setVelocity(Vector3(temp.x, this->getVelocity().y, temp.z));
            this->localVelocity_.x = 0;
            this->localVelocity_.y = 0;
            this->localVelocity_.z = 0;

            if (!this->isInMouseLook())
            {
                this->yaw(Radian(this->yaw_ * this->getMouseLookSpeed()),WorldEntity::Parent);
                //this->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
		Radian pitch=this->cameraPositionRootNode_->getOrientation().getPitch();
		if( pitch<Radian(1.5707) && pitch>Radian(-1.5707) ) this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
		else if(pitch<Radian(-1.5707)){
			if(this->pitch_>0.0) this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
			else if(pitch<Radian(-1.571)) this->cameraPositionRootNode_->pitch(-pitch+Radian(-1.570796));
		}
		else if(pitch>Radian(1.5707)){
			if(this->pitch_<0.0) this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
			else if(pitch>Radian(1.571)) this->cameraPositionRootNode_->pitch(-pitch+Radian(1.570796));
		}
		 
		
 //               this->roll(Radian(this->roll_ * this->getMouseLookSpeed()));
            }

            this->yaw_ = this->pitch_ = this->roll_ = 0;
	    
	    //Quaternion q=this->getOrientation();
	    //if( q.y<0.99 ) this->setOrientation(q.w, q.x, 1.0, q.z);
	    this->setAngularVelocity(0.0, 0.0, 0.0);
	    savedOrientation_=this->getOrientation();
        }

        SUPER(FpsPlayer, tick, dt);
    }

    void FpsPlayer::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

//        this->setSyncMode(ObjectDirection::ToClient);
    }

    void FpsPlayer::startLocalHumanControl()
    {
        ControllableEntity::startLocalHumanControl();
    }
    
    void FpsPlayer::moveFrontBack(const Vector2& value)
    {
        this->localVelocity_.z -= value.x;
    }


    void FpsPlayer::moveRightLeft(const Vector2& value)
    {
        this->localVelocity_.x += value.x;
    }

    void FpsPlayer::moveUpDown(const Vector2& value)
    {
        //this->localVelocity_.y += value.x;
    }

    void FpsPlayer::rotateYaw(const Vector2& value)
    {
        this->yaw_ += value.y;

        ControllableEntity::rotateYaw(value);
    }

    void FpsPlayer::rotatePitch(const Vector2& value)
    {
        this->pitch_ += value.y;

        ControllableEntity::rotatePitch(value);
    }

    void FpsPlayer::rotateRoll(const Vector2& value)
    {
        this->roll_ += value.y;

        ControllableEntity::rotateRoll(value);
    }

    void FpsPlayer::fire()
    {
    }
    
    void FpsPlayer::boost()					//acctually jump
    {
        if(isfloor) { 
		if(!thistickboost) this->localVelocity_.y = jumpvalue;
		//this->physicalBody_->applyCentralImpulse(btVector3(0, jumpvalue, 0));
		thistickboost=true;
		isfloor=false;
	}
    }

    bool FpsPlayer::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
	if(contactPoint.m_normalWorldOnB.y() > 0.6) isfloor=true;
	else isfloor=false;
	
	return false;
    }
    
}
