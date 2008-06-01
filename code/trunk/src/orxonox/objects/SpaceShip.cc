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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Benjamin Knecht
 *
 */

#include "OrxonoxStableHeaders.h"
#include "SpaceShip.h"

#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>

#include "CameraHandler.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Debug.h"
#include "GraphicsEngine.h"
#include "core/InputManager.h"
#include "particle/ParticleInterface.h"
#include "Projectile.h"
#include "RotatingProjectile.h"
#include "core/XMLPort.h"
#include "core/ConsoleCommand.h"
#include "network/Client.h"
#include "hud/HUD.h"

namespace orxonox
{
    SetConsoleCommand(SpaceShip, setMaxSpeedTest, false).setAccessLevel(AccessLevel::Debug);
    SetConsoleCommand(SpaceShip, whereAmI, true).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(SpaceShip, moveLongitudinal, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, 1.0f).setAxisParamIndex(0).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveLateral, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, 1.0f).setAxisParamIndex(0).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveYaw, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, 1.0f).setAxisParamIndex(0).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, movePitch, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, 1.0f).setAxisParamIndex(0).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveRoll, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, 1.0f).setAxisParamIndex(0).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, fire, true).setAccessLevel(AccessLevel::User).setKeybindMode(KeybindMode::OnHold);
    SetConsoleCommandGeneric(test1, SpaceShip, createConsoleCommand(createFunctor(&SpaceShip::setMaxSpeedTest), "setMaxSpeed"), false).setAccessLevel(AccessLevel::Debug);
    SetConsoleCommandGeneric(test2, SpaceShip, createConsoleCommand(createFunctor(&SpaceShip::setMaxSpeedTest), "setMaxBlubber"), false).setAccessLevel(AccessLevel::Debug);
    SetConsoleCommandGeneric(test3, SpaceShip, createConsoleCommand(createFunctor(&SpaceShip::setMaxSpeedTest), "setRofl"), false).setAccessLevel(AccessLevel::Debug);

    CreateFactory(SpaceShip);

    SpaceShip* SpaceShip::instance_s;

    SpaceShip *SpaceShip::getLocalShip(){
      Iterator<SpaceShip> it;
      for(it = ObjectList<SpaceShip>::start(); it; ++it){
        if( (it)->myShip_ )
          return *it;
      }
      return NULL;
    }

    SpaceShip::SpaceShip() :
      //testvector_(0,0,0),
      //bInvertYAxis_(false),
      setMouseEventCallback_(false),
      bLMousePressed_(false),
      bRMousePressed_(false),
      camNode_(0),
      cam_(0),
      camName_("CamNode"),
      tt_(0),
      redNode_(0),
      greenNode_(0),
      blinkTime_(0.0f),
      chNearNode_(0),
      chFarNode_(0),
      timeToReload_(0.0f),
      //reloadTime_(0.0f),
      maxSideAndBackSpeed_(0.0f),
      maxSpeed_(0.0f),
      maxRotation_(0.0f),
      translationAcceleration_(0.0f),
      rotationAcceleration_(0.0f),
      translationDamping_(0.0f),
      rotationDamping_(0.0f),
      maxRotationRadian_(0),
      rotationAccelerationRadian_(0),
      rotationDampingRadian_(0),
      zeroRadian_(0),
      mouseXRotation_(0),
      mouseYRotation_(0),
      mouseX_(0.0f),
      mouseY_(0.0f),
      emitterRate_(0.0f),
      myShip_(false),
      teamNr_(0),
      health_(100)
    {
        RegisterObject(SpaceShip);
        this->registerAllVariables();

        SpaceShip::instance_s = this;

        this->setConfigValues();

        initialDir_ = Vector3(1.0, 0.0, 0.0);
        currentDir_ = initialDir_;
        initialOrth_ = Vector3(0.0, 0.0, 1.0);
        currentOrth_ = initialOrth_;

        this->camName_ = this->getName() + "CamNode";

        this->setRotationAxis(1, 0, 0);
        this->setStatic(false);

        COUT(3) << "Info: SpaceShip was loaded" << std::endl;
    }

    SpaceShip::~SpaceShip()
    {
        if (this->tt_)
            delete this->tt_;
        if(setMouseEventCallback_)
          InputManager::removeMouseHandler("SpaceShip");
        if (this->cam_)
          delete this->cam_;
        if (!Identifier::isCreatingHierarchy() && !myShip_ && &HUD::getSingleton()!=NULL)
          //remove the radar object
          HUD::getSingleton().removeRadarObject(this->getNode());
    }

    bool SpaceShip::create(){
      if(!myShip_){
        if(network::Client::getSingleton() && objectID == network::Client::getSingleton()->getShipID())
          myShip_=true;
        else
          HUD::getSingleton().addRadarObject(this->getNode(), 3);
      }
      if(Model::create())
        this->init();
      else
        return false;
      return true;
    }

    void SpaceShip::registerAllVariables(){
      registerVar( &camName_, camName_.length()+1, network::STRING, 0x1);
      registerVar( &maxSpeed_, sizeof(maxSpeed_), network::DATA, 0x1);
      registerVar( &maxSideAndBackSpeed_, sizeof(maxSideAndBackSpeed_), network::DATA, 0x1);
      registerVar( &maxRotation_, sizeof(maxRotation_), network::DATA, 0x1);
      registerVar( &translationAcceleration_, sizeof(translationAcceleration_), network::DATA, 0x1);
      registerVar( &rotationAcceleration_, sizeof(rotationAcceleration_), network::DATA, 0x1);
      registerVar( &rotationAccelerationRadian_, sizeof(rotationAccelerationRadian_), network::DATA, 0x1);
      registerVar( &translationDamping_, sizeof(translationDamping_), network::DATA, 0x1);
      registerVar( &rotationDamping_, sizeof(rotationDamping_), network::DATA, 0x1);
      registerVar( &rotationDampingRadian_, sizeof(rotationDampingRadian_), network::DATA, 0x1);

    }

    void SpaceShip::init()
    {
        // START CREATING THRUSTER
        this->tt_ = new ParticleInterface(GraphicsEngine::getSingleton().getSceneManager(),"twinthruster" + this->getName(),"Orxonox/engineglow");
        this->tt_->getParticleSystem()->setParameter("local_space","true");
        this->tt_->newEmitter();
/*
        this->tt_->setDirection(Vector3(0,0,1));
        this->tt_->setPositionOfEmitter(0, Vector3(20,-1,-15));
        this->tt_->setPositionOfEmitter(1, Vector3(-20,-1,-15));
*/
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

        this->redNode_ = this->getNode()->createChildSceneNode(this->getName() + "red", Vector3(0.3, 4.0, -0.3));
        this->redNode_->setInheritScale(false);
        this->greenNode_ = this->getNode()->createChildSceneNode(this->getName() + "green", Vector3(0.3, -4.0, -0.3));
        this->greenNode_->setInheritScale(false);

        this->redNode_->attachObject(this->redBillboard_.getBillboardSet());
        this->redNode_->setScale(0.3, 0.3, 0.3);

        this->greenNode_->attachObject(this->greenBillboard_.getBillboardSet());
        this->greenNode_->setScale(0.3, 0.3, 0.3);
        // END CREATING BLINKING LIGHTS

        if (this->isExactlyA(Class(SpaceShip)))
        {
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
        }

        createCamera();
        // END of testing crosshair
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
        SetConfigValue(reloadTime_, 0.125).description("The reload time of the weapon in seconds");
        SetConfigValue(testvector_, Vector3()).description("asdfblah");
    }

    void SpaceShip::setCamera(const std::string& camera)
    {
      camName_=camera;
      // change camera attributes here, if you want to ;)
    }

    void SpaceShip::getFocus(){
      COUT(4) << "requesting focus" << std::endl;
      if(network::Client::getSingleton()==0 || network::Client::getSingleton()->getShipID()==objectID)
        CameraHandler::getInstance()->requestFocus(cam_);

    }

    Camera* SpaceShip::getCamera(){
        return cam_;
    }

    void SpaceShip::createCamera(){
//       COUT(4) << "begin camera creation" << std::endl;
      this->camNode_ = this->getNode()->createChildSceneNode(camName_);
      COUT(4) << "position: (this)" << this->getNode()->getPosition() << std::endl;
      this->camNode_->setPosition(Vector3(-50,0,10));
//      Quaternion q1 = Quaternion(Radian(Degree(90)),Vector3(0,-1,0));
//      Quaternion q2 = Quaternion(Radian(Degree(90)),Vector3(0,0,-1));
//      camNode_->setOrientation(q1*q2);
      COUT(4) << "position: (cam)" << this->camNode_->getPosition() << std::endl;
      cam_ = new Camera(this->camNode_);

      cam_->setTargetNode(this->getNode());
//        cam->setPosition(Vector3(0,-350,0));
      Quaternion q1 = Quaternion(Radian(Degree(90)),Vector3(0,-1,0));
      Quaternion q2 = Quaternion(Radian(Degree(90)),Vector3(1,0,0));
      camNode_->setOrientation(q2*q1);
      if(network::Client::getSingleton()!=0 && network::Client::getSingleton()->getShipID()==objectID){
        this->setBacksync(true);
        CameraHandler::getInstance()->requestFocus(cam_);
      }

    }

    void SpaceShip::setMaxSpeed(float value)
    { this->maxSpeed_ = value; }
    void SpaceShip::setMaxSideAndBackSpeed(float value)
    { this->maxSideAndBackSpeed_ = value; }
    void SpaceShip::setMaxRotation(float value)
    { this->maxRotation_ = value; this->maxRotationRadian_ = Radian(value); }
    void SpaceShip::setTransAcc(float value)
    { this->translationAcceleration_ = value; }
    void SpaceShip::setRotAcc(float value)
    { this->rotationAcceleration_ = value; this->rotationAccelerationRadian_ = Radian(value); }
    void SpaceShip::setTransDamp(float value)
    { this->translationDamping_ = value; }
    void SpaceShip::setRotDamp(float value)
    { this->rotationDamping_ = value; this->rotationDampingRadian_ = Radian(value); }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        Model::XMLPort(xmlelement, mode);

        XMLPortParamLoadOnly(SpaceShip, "camera", setCamera, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "maxSpeed", setMaxSpeed, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "maxSideAndBackSpeed", setMaxSideAndBackSpeed, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "maxRotation", setMaxRotation, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "transAcc", setTransAcc, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "rotAcc", setRotAcc, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "transDamp", setTransDamp, xmlelement, mode);
        XMLPortParamLoadOnly(SpaceShip, "rotDamp", setRotDamp, xmlelement, mode);
        myShip_=true; // TODO: this is only a hack

        SpaceShip::create();
        if (this->isExactlyA(Class(SpaceShip)))
            getFocus();
    }

    int sgn(float x)
    {
        if (x >= 0)
            return 1;
        else
            return -1;
    }

    std::string SpaceShip::whereAmI() {
	return getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().x)
	+ "  " + getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().y)
	+ "  " + getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().z);
    }

    Vector3 SpaceShip::getDir() {
        return currentDir_;
    }

    Vector3 SpaceShip::getOrth(){
        return currentOrth_;
    }

    float SpaceShip::getMaxSpeed() { return maxSpeed_; }

    void SpaceShip::tick(float dt)
    {
        currentDir_ = getOrientation()*initialDir_;
		currentOrth_ = getOrientation()*initialOrth_;

        if (this->cam_)
            this->cam_->tick(dt);

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

            Projectile *p;
            if (this->isExactlyA(Class(SpaceShip)))
                p = new RotatingProjectile(this);
            else
                p = new Projectile(this);
            p->setColour(this->getProjectileColour());
            p->create();
            if(p->classID==0)
              COUT(3) << "generated projectile with classid 0" <<  std::endl; // TODO: remove this output

            p->setBacksync(true);
            this->timeToReload_ = this->reloadTime_;
        }


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

        if( myShip_ )
        {
          COUT(4) << "steering our ship: " << objectID << std::endl;
          this->acceleration_.x = 0;
          this->acceleration_.y = 0;
          this->momentum_ = 0;
          this->mouseXRotation_ = Radian(0);
          this->mouseYRotation_ = Radian(0);
          this->bLMousePressed_ = false;
        }/*else
          COUT(4) << "not steering ship: " << objectID << " our ship: " << network::Client::getSingleton()->getShipID() << std::endl;*/
    }

    void SpaceShip::movePitch(float val)
    {   getLocalShip()->setMovePitch(val);   }
    void SpaceShip::moveYaw(float val)
    {   getLocalShip()->setMoveYaw(val);   }
    void SpaceShip::moveRoll(float val)
    {   getLocalShip()->setMoveRoll(val);   }
    void SpaceShip::moveLongitudinal(float val)
    {   getLocalShip()->setMoveLongitudinal(val);   }
    void SpaceShip::moveLateral(float val)
    {   getLocalShip()->setMoveLateral(val);   }
    void SpaceShip::fire()
    {   getLocalShip()->doFire();   }

    void SpaceShip::setMovePitch(float val)
    {
        val = -val * val * sgn(val) * this->rotationAcceleration_;
        if (val > this->maxRotation_)
            val = this->maxRotation_;
        if (val < -this->maxRotation_)
            val = -this->maxRotation_;
        this->mouseYRotation_ = Radian(val);
    }

    void SpaceShip::setMoveYaw(float val)
    {
        val = -val * val * sgn(val) * this->rotationAcceleration_;
        if (val > this->maxRotation_)
            val = this->maxRotation_;
        if (val < -this->maxRotation_)
            val = -this->maxRotation_;
        this->mouseXRotation_ = Radian(val);
    }

    void SpaceShip::setMoveRoll(float val)
    {
        this->momentum_ = Radian(-this->rotationAccelerationRadian_ * val);
        //COUT(3) << "rotating val: " << val << " acceleration: " << this->rotationAccelerationRadian_.valueDegrees() << std::endl;
    }

    void SpaceShip::setMoveLongitudinal(float val)
    {
        this->acceleration_.x = this->translationAcceleration_ * val;
    }

    void SpaceShip::setMoveLateral(float val)
    {
        this->acceleration_.y = -this->translationAcceleration_ * val;
    }

    void SpaceShip::doFire()
    {
        this->bLMousePressed_ = true;
    }
}
