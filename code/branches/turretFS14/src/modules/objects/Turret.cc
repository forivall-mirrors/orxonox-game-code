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
 *      Marian Runo, Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

#include "Turret.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

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
        this->startDir_ = Vector3::ZERO;
        this->localZ_ = Vector3::UNIT_Z;
        this->localY_ = Vector3::UNIT_Y;
        this->localX_ = Vector3::UNIT_X;
        this->attackRadius_ = 200;
        this->maxPitch_ = 90;
        this->maxYaw_ = 90;
        this->once_ = false;
        this->rotation_ = Quaternion::IDENTITY;

        this->setRadarVisibility(false);
    }

    /**
     * @brief Destructor
     */
    Turret::~Turret()
    {

    }


    bool Turret::isInRange(const Vector3 &position)
    {
        //Check distance
        Vector3 distance = position - this->getWorldPosition();
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

    void Turret::aimAtPosition(const Vector3& position)
    {
        Vector3 currDir = this->getWorldOrientation() * WorldEntity::FRONT;
        Vector3 targetDir = position - this->getWorldPosition();

        this->rotation_ = currDir.getRotationTo(targetDir);

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
    }

    void Turret::rotateRoll(const Vector2& value)
    {
    }

    void Turret::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Turret, XMLPort, xmlelement, mode);
        
        XMLPortParamVariable(Turret, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParam(Turret, "attackRadius", setAttackRadius, getAttackRadius, xmlelement, mode);
        XMLPortParam(Turret, "maxYaw", setMaxYaw, getMaxYaw, xmlelement, mode);
        XMLPortParam(Turret, "maxPitch", setMaxPitch, getMaxPitch, xmlelement, mode);
    }

    void Turret::tick(float dt)
    {
        SUPER(Turret, tick, dt);


        if(!this->once_)
        {

            Quaternion startOrient = this->getOrientation();
            this->localXStart_ = startOrient * this->localX_;
            this->localXStart_.normalise();
            this->localX_ = this->localXStart_;
            this->localYStart_ = startOrient * this->localY_;
            this->localYStart_.normalise();
            this->localY_ = this->localYStart_;
            this->localZStart_ = startOrient * this->localZ_;
            this->localZStart_.normalise();
            this->localZ_ = this->localZStart_;

            //startDir should always be (0,0,-1)
            this->startDir_ = getTransformedVector(startOrient * WorldEntity::FRONT, this->localX_, this->localY_, this->localZ_);

            this->once_ = true;

        }

        //Adjust local axes to parent's rotation
        WorldEntity* parent = this->getParent();
        if(parent)
        {
            Quaternion parentrot = parent->getWorldOrientation();
            this->localX_ = parentrot * this->localXStart_;
            this->localY_ = parentrot * this->localYStart_;
            this->localZ_ = parentrot * this->localZStart_;
        }

        //rotate
        if(this->rotation_ != Quaternion::IDENTITY)
        {
            //Don't make the rotation instantaneous
            Quaternion drot = Quaternion::Slerp(dt*this->rotationThrust_/20.f, Quaternion::IDENTITY, this->rotation_);
            this->rotate(drot, WorldEntity::World);
            this->rotation_ = Quaternion::IDENTITY;
        }

    }
}