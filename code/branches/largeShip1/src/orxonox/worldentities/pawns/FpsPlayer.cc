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
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "items/Engine.h"
#include "Scene.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSlot.h"
#include "weaponsystem/Weapon.h"

#include <cmath>

namespace orxonox
{
    const float orientationGain_ = 100;
    const float jumpValue_ = 300;
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

        //this->weaponNode = this->cameraPositionRootNode_;
        this->weaponNode_ = this->getScene()->getRootSceneNode()->createChildSceneNode();
        this->attachNode(this->weaponNode_);
    }

    FpsPlayer::~FpsPlayer()
    {
        if (this->isInitialized())
        {
            if (this->mesh_.getEntity())
                this->detachOgreObject(this->mesh_.getEntity());

            if (this->weaponNode_ && this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroySceneNode(this->weaponNode_->getName());
        }
    }

    void FpsPlayer::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FpsPlayer, XMLPort, xmlelement, mode);

        XMLPortParamVariable(FpsPlayer, "primaryThrust",  primaryThrust_,  xmlelement, mode);
        XMLPortParamVariable(FpsPlayer, "auxilaryThrust", auxilaryThrust_, xmlelement, mode);
        XMLPortParamVariable(FpsPlayer, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParam(FpsPlayer, "weapon", setMeshSource, getMeshSource, xmlelement, mode);
    }

    void FpsPlayer::registerVariables()
    {
        registerVariable(this->primaryThrust_,  VariableDirection::ToClient);
        registerVariable(this->auxilaryThrust_, VariableDirection::ToClient);
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
        registerVariable(this->weaponMashName_, VariableDirection::ToClient);
    }



    void FpsPlayer::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = targetting down).");
    }

    bool FpsPlayer::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type != WorldEntity::Dynamic)
        {
            orxout(internal_warning) << "Cannot tell a FpsPlayer not to be dynamic! Ignoring." << endl;
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

            this->thisTickBoost_ = false;

            float localSpeedSquared = this->localVelocity_.squaredLength();
            float localSpeed;
            if (localSpeedSquared > 1.0)
                localSpeed = this->speed_ / sqrtf(localSpeedSquared);
            else
                localSpeed = this->speed_;

            this->localVelocity_.x *= localSpeed;
            this->localVelocity_.z *= localSpeed;
            Vector3 temp = this->getOrientation() * this->localVelocity_;
            if (localVelocity_.y == jumpValue_)
                this->setVelocity(Vector3(temp.x, temp.y + this->getVelocity().y, temp.z));
            else
                this->setVelocity(Vector3(temp.x, this->getVelocity().y, temp.z));
            this->localVelocity_.x = 0;
            this->localVelocity_.y = 0;
            this->localVelocity_.z = 0;

            if (!this->isInMouseLook())
            {
                this->yaw(Radian(this->yaw_ * this->getMouseLookSpeed()), WorldEntity::Parent);

                Radian pitch = this->cameraPositionRootNode_->getOrientation().getPitch();
                if (pitch < Radian(math::pi_2) && pitch > Radian(-math::pi_2))
                {
                    this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
                }
                else if (pitch < Radian(-math::pi_2))
                {
                    if (this->pitch_ > 0.0f)
                        this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
                    else if (pitch < Radian(-math::pi_2))
                        this->cameraPositionRootNode_->pitch(-pitch + Radian(-math::pi_2));
                }
                else if (pitch > Radian(math::pi_2))
                {
                    if (this->pitch_ < 0.0f)
                        this->cameraPositionRootNode_->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
                    else if (pitch > Radian(math::pi_2))
                        this->cameraPositionRootNode_->pitch(-pitch + Radian(math::pi_2));
                }
                this->weaponNode_->setOrientation(this->cameraPositionRootNode_->getOrientation());
            }

            this->yaw_ = this->pitch_ = this->roll_ = 0;

            this->setAngularVelocity(0.0, 0.0, 0.0);
            this->savedOrientation_ = this->getOrientation();
        }

        SUPER(FpsPlayer, tick, dt);
    }

    void FpsPlayer::changedMesh()
    {
        if (GameMode::showsGraphics())
        {
            if (this->mesh_.getEntity())
                this->weaponNode_->detachObject(this->mesh_.getEntity());

            this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

            if (this->mesh_.getEntity())
            {
                this->weaponNode_->attachObject(this->mesh_.getEntity());
            }
        }
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

    void FpsPlayer::boost(bool bBoost) //acctually jump
    {
        if (this->isFloor_)
        {
            if (!this->thisTickBoost_)
                this->localVelocity_.y = jumpValue_;
            //this->physicalBody_->applyCentralImpulse(btVector3(0, jumpvalue, 0));
            this->thisTickBoost_ = true;
            this->isFloor_ = false;
        }
    }

    bool FpsPlayer::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (contactPoint.m_normalWorldOnB.y() > 0.6)
            this->isFloor_ = true;
        else
            this->isFloor_ = false;

        return false;
    }

    void FpsPlayer::addedWeaponPack(WeaponPack* wPack)
    {
        for (size_t i = 0; i < wPack->getNumWeapons(); ++i)
        {
            Weapon* weapon = wPack->getWeapon(i);
            if (weapon->getWeaponSlot())
            {
                weapon->getWeaponSlot()->removeWeapon();
                weapon->detachFromParent();
                weapon->attachToNode(this->weaponNode_);
            }
        }
    }
}
