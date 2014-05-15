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
#include "Scene.h"
#include <OgreSceneQuery.h>


namespace orxonox
{
    RegisterClass(Turret);



    /**
        @brief 
        Sets default values for all variables. Also hides the turret from the radar.

        @param context
        The context
     */
    Turret::Turret(Context* context) : Pawn(context)
    {
        RegisterObject(Turret);
        this->rotationThrust_ = 50;
        this->startDir_ = Vector3::ZERO;
        this->localZ_ = Vector3::UNIT_Z;
        this->localY_ = Vector3::UNIT_Y;
        this->localX_ = Vector3::UNIT_X;
        this->maxAttackRadius_ = 200;
        this->minAttackRadius_ = 0;
        this->maxPitch_ = 90;
        this->maxYaw_ = 90;
        this->once_ = false;
        this->rotation_ = Quaternion::IDENTITY;

        this->setRadarVisibility(false);
    }

    /**
        @brief 
        Destructor. Nothing to see here.
     */
    Turret::~Turret()
    {

    }

    /**
        @brief
        Checks, if a WorldEntity is inside the turret's range.

        This function is safe to use on turrets that are attached, rotated, etc.
        The turret's range is determined with the maxPitch, maxYaw, and the two attackRadius.

        @param target
        The WorldEntity to check

        @return
        The squared distance to the position. -1, if it's ouside of range
    */
    float Turret::isInRange(const WorldEntity* target ) const
    {
        //Check distance
        Vector3 distance = target->getWorldPosition() - this->getWorldPosition();
        float distanceVal = distance.squaredLength();
        if(distanceVal > (this->maxAttackRadius_ * this->maxAttackRadius_) || distanceVal < (this->minAttackRadius_ * this->minAttackRadius_))
        {
            return -1.f;
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
            return -1.f;
        }

        //Check yaw
        dirProjected = dir;
        dirProjected.y = 0;
        startDirProjected = this->startDir_;
        startDirProjected.y = 0;
        angle = startDirProjected.angleBetween(dirProjected).valueDegrees();
        if(angle > this->maxYaw_)
        {
            return -1.f;
        }

        Ogre::SceneManager* scenemanager = this->getScene()->getSceneManager();
        Ogre::Ray ray = Ogre::Ray(this->getWorldPosition(), distance);
        Ogre::DefaultRaySceneQuery rayscenequery = Ogre::DefaultRaySceneQuery(scenemanager);
        rayscenequery.setRay(ray);

        return distanceVal;
    }

    /**
        @brief
        Rotates the turret to make it aim at a certain position.

        @note
        There are no checks, if the position is valid (i.e. if the turret is allowed to aim there).
        This function must be called again for every tick, or the turret will stop rotating.

        @param position
        The position to aim at
    */
    void Turret::aimAtPosition(const Vector3& position)
    {
        Vector3 currDir = this->getWorldOrientation() * WorldEntity::FRONT;
        Vector3 targetDir = position - this->getWorldPosition();

        this->rotation_ = currDir.getRotationTo(targetDir);
    }

    /**
        @brief
        Does currently nothing.

        Should rotate the turret with the specified pitch. Contains a failed attempt at limiting said rotation.
    */
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

    /**
        @brief
        Does currently nothing.

        Should rotate the turret with the specified yaw. Contains a failed attempt at limiting said rotation.
    */
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

    /**
        @brief
        Does currently nothing.

        May be used to limit turret's rotation in the future.
    */
    void Turret::rotateRoll(const Vector2& value)
    {
    }

    /**
        @brief
        Loads parameters from xml

        Parameters loaded are: rotationThrust, maxAttackRadius, minAttackRadius, maxYaw, maxPitch
    */
    void Turret::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Turret, XMLPort, xmlelement, mode);
        
        XMLPortParamVariable(Turret, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParam(Turret, "maxAttackRadius", setMaxAttackRadius, getMaxAttackRadius, xmlelement, mode);
        XMLPortParam(Turret, "minAttackRadius", setMinAttackRadius, getMinAttackRadius, xmlelement, mode);
        XMLPortParam(Turret, "maxYaw", setMaxYaw, getMaxYaw, xmlelement, mode);
        XMLPortParam(Turret, "maxPitch", setMaxPitch, getMaxPitch, xmlelement, mode);
    }

    /**
        @brief
        The turret's actions are done here.

        Every tick, the turret gets rotated if it should, and the local axes get updated with the parent's rotation.
    
        @param dt
        Duration of the tick
    */
    void Turret::tick(float dt)
    {
        SUPER(Turret, tick, dt);

        //Stuff isn't properly initialized in the c'tor, so we have to do it like this
        if(!this->once_)
        {
            //Account for rotations done in xml
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
            //Don't make the rotation instantaneous. Use an arbitrary interpolation, not that great...
            //TODO: make the rotation better (constant velocity etc.). At the moment, the turret rotates
            //      slower the closer it is to the destination
            Quaternion drot = Quaternion::nlerp(dt*this->rotationThrust_/20.f, Quaternion::IDENTITY, this->rotation_);
            this->rotate(drot, WorldEntity::World);
            this->rotation_ = Quaternion::IDENTITY;
        }

    }
}