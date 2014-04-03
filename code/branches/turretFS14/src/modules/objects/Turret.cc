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
 *      Marian Runo
 *   Co-authors:
 *      ...
 *
 */

#include "Turret.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace orxonox
{
    RegisterClass(Turret);



    /**
     * @brief Constructor
     */
    Turret::Turret(Context* context) : Pawn(context)
    {
        RegisterObject(Turret);
        this->startOrientInv_ = Quaternion::IDENTITY;
        this->maxPitch_ = 0;
        this->maxYaw_ = 0;
        this->gotOrient_ = false;
        this->rotationThrust_ = 50;

        this->localAngularAcceleration_.setValue(0, 0, 0);
    }

    /**
     * @brief Destructor
     */
    Turret::~Turret()
    {

    }


    void Turret::rotatePitch(const Vector2& value)
    {
        if (this->maxPitch_ == 0)
        {
            return;
        }
        if (this->maxPitch_ >= 180) //no need to check, if the limit too big
        {
            this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x*0.8f);
            return;
        }

        Quaternion drot = startOrientInv_ * this->getOrientation();

        Ogre::Real val = boundBetween(drot.getPitch(false).valueDegrees(), -180, 180);
        Ogre::Real offset = boundBetween(Degree(value.x).valueDegrees(), -180, 180);
        Ogre::Real lowerBound = offset - this->maxPitch_;
        Ogre::Real upperBound = offset + this->maxPitch_;
        if (lowerBound < -180) //Avoid wrapping around of the boundaries
        {
            lowerBound += this->maxPitch_;
            upperBound += this->maxPitch_;
            val = boundBetween(val + this->maxPitch_, -180, 180); //val might wrap around here
        }
        else if (upperBound >= 180) //Avoid wrapping around of the boundaries (the other side)
        {
            lowerBound -= this->maxPitch_;
            upperBound -= this->maxPitch_;
            val = boundBetween(val-this->maxPitch_, -180, 180); //val might wrap around here
        }
        if ((val >= lowerBound || value.x > 0) && (val <= upperBound || value.x < 0)) 
        {
            this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x*0.8f);
        }
        return;
    }

    void Turret::rotateYaw(const Vector2& value)
    {
        if (this->maxPitch_ == 0)
        {
            return;
        }
        if (this->maxPitch_ >= 180) //no need to check, if the limit too big
        {
            this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x*0.8f);
            return;
        }

        Quaternion drot = startOrientInv_ * this->getOrientation();

        Ogre::Real val = boundBetween(drot.getYaw(false).valueDegrees(), -180, 180);
        Ogre::Real offset = boundBetween(Degree(value.x).valueDegrees(), -180, 180);
        Ogre::Real lowerBound = offset - this->maxPitch_;
        Ogre::Real upperBound = offset + this->maxPitch_;
        if (lowerBound < -180) //Avoid wrapping around of the boundaries
        {
            lowerBound += this->maxPitch_;
            upperBound += this->maxPitch_;
            val = boundBetween(val + this->maxPitch_, -180, 180); //val might wrap around here
        }
        else if (upperBound >= 180) //Avoid wrapping around of the boundaries (the other side)
        {
            lowerBound -= this->maxPitch_;
            upperBound -= this->maxPitch_;
            val = boundBetween(val-this->maxPitch_, -180, 180); //val might wrap around here
        }
        if ((val >= lowerBound || value.x > 0) && (val <= upperBound || value.x < 0)) 
        {
           this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x*0.8f);
        }
        return;
    }

    void Turret::rotateRoll(const Vector2& value)
    {
        return; //Standard turrets don't roll
    }

    void Turret::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(Turret, "maxPitch", setMaxPitch, getMaxPitch, xmlelement, mode);
        XMLPortParam(Turret, "maxYaw", setMaxYaw, getMaxYaw, xmlelement, mode);
        SUPER(Turret, XMLPort, xmlelement, mode);
    }

    void Turret::tick(float dt)
    {
        SUPER(Turret, tick, dt);

        if(!gotOrient_)
        {
            startOrientInv_ = this->getOrientation().Inverse();
            gotOrient_ = true;
        }
        Quaternion drot = startOrientInv_ * this->getOrientation();
        orxout() << "Pitch: " << drot.getPitch(false).valueDegrees() << "\tYaw: " << drot.getYaw(false).valueDegrees() << "\tRoll: " << drot.getRoll(false).valueDegrees() << endl;
        
        this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
        this->localAngularAcceleration_.setValue(0, 0, 0);
    }


    Ogre::Real Turret::boundBetween(Ogre::Real val, Ogre::Real lowerBound, Ogre::Real upperBound)
    {
        if (lowerBound > upperBound){ std::swap(lowerBound, upperBound); }
        val -= lowerBound; //adjust to 0
        Ogre::Real rangeSize = upperBound - lowerBound;
        if (rangeSize == 0){ return upperBound; } //avoid dividing by 0
        return val - (rangeSize * std::floor(val / rangeSize)) + lowerBound;
    }

}
