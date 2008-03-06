/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"

#include <string>

#include <OIS/OIS.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>

#include "util/tinyxml/tinyxml.h"
#include "util/String2Number.h"
#include "util/Math.h"
#include "../core/CoreIncludes.h"
#include "../core/Debug.h"
#include "../Orxonox.h"
#include "../particle/ParticleInterface.h"
#include "Projectile.h"

#include "SpaceShip.h"

namespace orxonox
{
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip()
    {
        RegisterObject(SpaceShip);

        this->setConfigValues();

        this->setMouseEventCallback_ = false;
        this->bLMousePressed_ = false;
        this->bRMousePressed_ = false;
        this->mouseX_ = 0;
        this->mouseY_ = 0;

        this->camNode_ = 0;

        this->tt_ = 0;
        this->redNode_ = 0;
        this->greenNode_ = 0;
        this->blinkTime_ = 0;

        this->timeToReload_ = 0;

        this->maxSpeed_ = 0;
        this->maxSideAndBackSpeed_ = 0;
        this->maxRotation_ = 0;
        this->translationAcceleration_ = 0;
        this->rotationAcceleration_ = 0;
        this->translationDamping_ = 0;
        this->rotationDamping_ = 0;

        this->maxRotationRadian_ = 0;
        this->rotationAccelerationRadian_ = 0;
        this->rotationDampingRadian_ = 0;
        this->zeroRadian_ = Radian(0);

        this->setRotationAxis(1, 0, 0);
        this->setStatic(false);

        COUT(3) << "Info: SpaceShip was loaded" << std::endl;
    }

    SpaceShip::~SpaceShip()
    {
        if (this->tt_)
            delete this->tt_;
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
        SetConfigValue(reloadTime_, 0.125).description("The reload time of the weapon in seconds");
        SetConfigValue(projectileName_, "Projectile").description("Projectile of the ship-weapon.");
    }

    void SpaceShip::loadParams(TiXmlElement* xmlElem)
    {
        Model::loadParams(xmlElem);

        // START CREATING THRUSTER
        this->tt_ = new ParticleInterface(Orxonox::getSingleton()->getSceneManager(),"twinthruster" + this->getName(),"Orxonox/engineglow");
        this->tt_->getParticleSystem()->setParameter("local_space","true");
        this->tt_->newEmitter();

        this->tt_->setDirection(Vector3(-1,0,0));
        this->tt_->setPositionOfEmitter(0, Vector3(-15,20,-1));
        this->tt_->setPositionOfEmitter(1, Vector3(-15,-20,-1));
        this->tt_->setVelocity(50);

        emitterRate_ = tt_->getRate();

        Ogre::SceneNode* node2 = this->getNode()->createChildSceneNode(this->getName() + "particle2");
        node2->setInheritScale(false);
        tt_->addToSceneNode(node2);
        // END CREATING THRUSTER

        // START CREATING BLINKING LIGHTS
        this->redBillboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
        this->greenBillboard_.setBillboardSet("Examples/Flare", ColourValue(0.0, 1.0, 0.0), 1);

        this->redNode_ = this->getNode()->createChildSceneNode(this->getName() + "red", Vector3(0.3, 4.7, -0.3));
        this->redNode_->setInheritScale(false);
        this->greenNode_ = this->getNode()->createChildSceneNode(this->getName() + "green", Vector3(0.3, -4.7, -0.3));
        this->greenNode_->setInheritScale(false);

        this->redNode_->attachObject(this->redBillboard_.getBillboardSet());
        this->redNode_->setScale(0.3, 0.3, 0.3);

        this->greenNode_->attachObject(this->greenBillboard_.getBillboardSet());
        this->greenNode_->setScale(0.3, 0.3, 0.3);
        // END CREATING BLINKING LIGHTS

        // START of testing crosshair
        this->crosshairNear_.setBillboardSet("Orxonox/Crosshair", ColourValue(1.0, 1.0, 0.0), 1);
        this->crosshairFar_.setBillboardSet("Orxonox/Crosshair", ColourValue(1.0, 1.0, 0.0), 1);

        this->chNearNode_ = this->getNode()->createChildSceneNode(this->getName() + "near", Vector3(50.0, 0.0, 0.0));
        this->chNearNode_->setInheritScale(false);
        this->chFarNode_ = this->getNode()->createChildSceneNode(this->getName() + "far", Vector3(200.0, 0.0, 0.0));
        this->chFarNode_->setInheritScale(false);

        this->chNearNode_->attachObject(this->crosshairNear_.getBillboardSet());
        this->chNearNode_->setScale(0.2, 0.2, 0.2);

        this->chFarNode_->attachObject(this->crosshairFar_.getBillboardSet());
        this->chFarNode_->setScale(0.4, 0.4, 0.4);

        // END of testing crosshair

        if (xmlElem->Attribute("maxSpeed") && xmlElem->Attribute("maxSideAndBackSpeed") && xmlElem->Attribute("maxRotation") && xmlElem->Attribute("transAcc") && xmlElem->Attribute("rotAcc") && xmlElem->Attribute("transDamp") && xmlElem->Attribute("rotDamp"))
        {

            std::string msStr = xmlElem->Attribute("maxSpeed");
            std::string msabsStr = xmlElem->Attribute("maxSideAndBackSpeed");
            std::string mrStr = xmlElem->Attribute("maxRotation");
            std::string taStr = xmlElem->Attribute("transAcc");
            std::string raStr = xmlElem->Attribute("rotAcc");
            std::string tdStr = xmlElem->Attribute("transDamp");
            std::string rdStr = xmlElem->Attribute("rotDamp");

            String2Number<float>(this->maxSpeed_, msStr);
            String2Number<float>(this->maxSideAndBackSpeed_, msabsStr);
            String2Number<float>(this->maxRotation_, mrStr);
            String2Number<float>(this->translationAcceleration_, taStr);
            String2Number<float>(this->rotationAcceleration_, raStr);
            String2Number<float>(this->translationDamping_, tdStr);
            String2Number<float>(this->rotationDamping_, rdStr);

            this->maxRotationRadian_ = Radian(this->maxRotation_);
            this->rotationAccelerationRadian_ = Radian(this->rotationAcceleration_);
            this->rotationDampingRadian_ = Radian(this->rotationDamping_);

            COUT(4) << "Loader: Initialized SpaceShip" << std::endl;
    	}

    	if (xmlElem->Attribute("camera"))
    	{
            Ogre::Camera *cam = Orxonox::getSingleton()->getSceneManager()->createCamera("ShipCam");
            this->camNode_ = this->getNode()->createChildSceneNode("CamNode");

            cam->setPosition(Vector3(-200,0,35));
            cam->lookAt(Vector3(0,0,35));
            cam->roll(Degree(-90));

            this->camNode_->attachObject(cam);
            Orxonox::getSingleton()->getOgrePointer()->getRoot()->getAutoCreatedWindow()->addViewport(cam);
    	}
    }

    int sgn(float x)
    {
        if (x >= 0)
            return 1;
        else
            return -1;
    }

    bool SpaceShip::mouseMoved(const OIS::MouseEvent &e)
    {
        if (this->bRMousePressed_)
        {
            this->camNode_->roll(Degree(-e.state.X.rel * 0.10));
            this->camNode_->yaw(Degree(e.state.Y.rel * 0.10));
        }
        else
        {
            float minDimension = e.state.height;
            if (e.state.width < minDimension)
                minDimension = e.state.width;

            this->mouseX_ += e.state.X.rel;
            if (this->mouseX_ < -minDimension)
                this->mouseX_ = -minDimension;
            if (this->mouseX_ > minDimension)
                this->mouseX_ = minDimension;

            this->mouseY_ += e.state.Y.rel;
            if (this->mouseY_ < -minDimension)
                this->mouseY_ = -minDimension;
            if (this->mouseY_ > minDimension)
                this->mouseY_ = minDimension;

            float xRotation = this->mouseX_ / minDimension;
            xRotation = xRotation*xRotation * sgn(xRotation);
            xRotation *= -this->rotationAcceleration_;
            if (xRotation > this->maxRotation_)
                xRotation = this->maxRotation_;
            if (xRotation < -this->maxRotation_)
                xRotation = -this->maxRotation_;
            this->mouseXRotation_ = Radian(xRotation);

            float yRotation = this->mouseY_ / minDimension;
            yRotation = yRotation*yRotation * sgn(yRotation);
            yRotation *= this->rotationAcceleration_;
            if (yRotation > this->maxRotation_)
                yRotation = this->maxRotation_;
            if (yRotation < -this->maxRotation_)
                yRotation = -this->maxRotation_;
            this->mouseYRotation_ = Radian(yRotation);
        }

        return true;
    }

    bool SpaceShip::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (id == OIS::MB_Left)
            this->bLMousePressed_ = true;
        else if (id == OIS::MB_Right)
            this->bRMousePressed_ = true;

        return true;
    }

    bool SpaceShip::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (id == OIS::MB_Left)
            this->bLMousePressed_ = false;
        else if (id == OIS::MB_Right)
        {
            this->bRMousePressed_ = false;
            this->camNode_->resetOrientation();
        }

        return true;
    }

    void SpaceShip::tick(float dt)
    {
        if (!this->setMouseEventCallback_)
        {
            if (Orxonox::getSingleton()->getMouse())
            {
                Orxonox::getSingleton()->getMouse()->setEventCallback(this);
                this->setMouseEventCallback_ = true;
            }
        }

        if (this->redNode_ && this->greenNode_)
        {
            this->blinkTime_ += dt;
            float redScale = 0.15 + 0.15 * sin(this->blinkTime_ * 10.0);
            float greenScale = 0.15 - 0.15 * sin(this->blinkTime_ * 10.0);
            this->redNode_->setScale(redScale, redScale, redScale);
            this->greenNode_->setScale(greenScale, greenScale, greenScale);
        }

        if (this->timeToReload_ > 0)
            this->timeToReload_ -= dt;
        else
            this->timeToReload_ = 0;

        if (this->bLMousePressed_ && this->timeToReload_ <= 0)
        {
            SubclassIdentifier<Projectile> projectile = ID(this->projectileName_);
            Projectile* newProjectile = projectile.fabricate();
            newProjectile->setOwner(this);

            this->timeToReload_ = this->reloadTime_;
        }

        OIS::Keyboard* mKeyboard = Orxonox::getSingleton()->getKeyboard();
        OIS::Mouse* mMouse = Orxonox::getSingleton()->getMouse();

        mKeyboard->capture();
        mMouse->capture();


        // #####################################
        // ############# STEERING ##############
        // #####################################

        if (this->velocity_.x > this->maxSpeed_)
            this->velocity_.x = this->maxSpeed_;
        if (this->velocity_.x < -this->maxSideAndBackSpeed_)
            this->velocity_.x = -this->maxSideAndBackSpeed_;
        if (this->velocity_.y > this->maxSideAndBackSpeed_)
            this->velocity_.y = this->maxSideAndBackSpeed_;
        if (this->velocity_.y < -this->maxSideAndBackSpeed_)
            this->velocity_.y = -this->maxSideAndBackSpeed_;
        if (this->rotationRate_ > this->maxRotationRadian_)
            this->rotationRate_ = this->maxRotationRadian_;
        if (this->rotationRate_ < -this->maxRotationRadian_)
            this->rotationRate_ = -this->maxRotationRadian_;

        if (this->acceleration_.x == 0)
        {
            if (this->velocity_.x > 0)
            {
                this->velocity_.x -= (this->translationDamping_ * dt);
                if (this->velocity_.x < 0)
                    this->velocity_.x = 0;
            }
            else if (this->velocity_.x < 0)
            {
                this->velocity_.x += (this->translationDamping_ * dt);
                if (this->velocity_.x > 0)
                    this->velocity_.x = 0;
            }
        }

        if (this->acceleration_.y == 0)
        {
            if (this->velocity_.y > 0)
            {
                this->velocity_.y -= (this->translationDamping_ * dt);
                if (this->velocity_.y < 0)
                    this->velocity_.y = 0;
            }
            else if (this->velocity_.y < 0)
            {
                this->velocity_.y += (this->translationDamping_ * dt);
                if (this->velocity_.y > 0)
                    this->velocity_.y = 0;
            }
        }

        if (this->momentum_ == this->zeroRadian_)
        {
            if (this->rotationRate_ > this->zeroRadian_)
            {
                this->rotationRate_ -= (this->rotationDampingRadian_ * dt);
                if (this->rotationRate_ < this->zeroRadian_)
                    this->rotationRate_ = 0;
            }
            else if (this->rotationRate_ < this->zeroRadian_)
            {
                this->rotationRate_ += (this->rotationDampingRadian_ * dt);
                if (this->rotationRate_ > this->zeroRadian_)
                    this->rotationRate_ = 0;
            }
        }

        if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
            this->acceleration_.x = this->translationAcceleration_;
        else if(mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
            this->acceleration_.x = -this->translationAcceleration_;
        else
            this->acceleration_.x = 0;

        if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
            this->acceleration_.y = -this->translationAcceleration_;
        else if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
            this->acceleration_.y = this->translationAcceleration_;
        else
            this->acceleration_.y = 0;

        if (mKeyboard->isKeyDown(OIS::KC_DELETE) || mKeyboard->isKeyDown(OIS::KC_Q))
            this->momentum_ = Radian(-this->rotationAccelerationRadian_);
        else if (mKeyboard->isKeyDown(OIS::KC_PGDOWN) || mKeyboard->isKeyDown(OIS::KC_E))
            this->momentum_ = Radian(this->rotationAccelerationRadian_);
        else
            this->momentum_ = 0;

        WorldEntity::tick(dt);

        this->roll(this->mouseXRotation_ * dt);
        if (this->bInvertYAxis_)
            this->yaw(Radian(-this->mouseYRotation_ * dt));
        else
            this->yaw(Radian(this->mouseYRotation_ * dt));

        if (this->acceleration_.x > 0)
            this->tt_->setRate(emitterRate_);
        else
            this->tt_->setRate(0);
    }
}
