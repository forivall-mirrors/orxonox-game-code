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
        //This is a failed attempt at limiting the turret's rotation. It's handled in the controller (for now?)
        /*
        Vector3 currentDir = getTransformedVector(this->getOrientation() * WorldEntity::FRONT, this->localX_, this->localY_, this->localZ_);
        Vector3 currentDirProjected = currentDir;
        currentDirProjected.x = 0;
        Vector3 startDirProjected = this->startDir_;
        startDirProjected.x = 0;     
        Ogre::Real angle = startDirProjected.angleBetween(currentDirProjected).valueDegrees();
        //orxout() << "Pitch: " << angle << endl;   
        //if(angle < this->maxPitch_ || (currentDirProjected.y <= 0 && value.x > 0) || (currentDirProjected.y > 0 && value.x < 0) )
        {
            this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x*0.8f);
        }
        */
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x*0.8f);
    }

    void Turret::rotateYaw(const Vector2& value)
    {
        //This is a failed attempt at limiting the turret's rotation. It's handled in the controller (for now?)
        /*
        Vector3 currentDir = getTransformedVector(this->getOrientation() * WorldEntity::FRONT, this->localX_, this->localY_, this->localZ_);
        Vector3 currentDirProjected = currentDir;
        currentDirProjected.y = 0;
        Vector3 startDirProjected = this->startDir_;
        startDirProjected.y = 0;
        Ogre::Real angle = startDirProjected.angleBetween(currentDirProjected).valueDegrees();
        orxout() << "Yaw: " << angle << endl;
        if(angle < this->maxYaw_ || (currentDirProjected.x <= 0 && value.x < 0) || (currentDirProjected.x > 0 && value.x > 0))
        {
            this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x*0.8f);
        }
        */
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x*0.8f);
    }

    void Turret::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x*0.8f);
    }

    void Turret::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Turret, XMLPort, xmlelement, mode);
    }

    void Turret::tick(float dt)
    {
        SUPER(Turret, tick, dt);

        this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
        this->localAngularAcceleration_ = physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_;

        //physics don't work when attached :(
        //this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);

        pitch(Degree(localAngularAcceleration_.x()*dt/1000), WorldEntity::World);
        yaw(Degree(localAngularAcceleration_.y()*dt/1000), WorldEntity::World);
        roll(Degree(localAngularAcceleration_.z()*dt/1000), WorldEntity::World);

        this->localAngularAcceleration_.setValue(0, 0, 0);
    }
}