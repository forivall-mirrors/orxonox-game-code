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

#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>

#include "util/Convert.h"
#include "util/Math.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Iterator.h"
#include "core/input/InputManager.h"
#include "core/XMLPort.h"
#include "core/ConsoleCommand.h"
#include "tools/ParticleInterface.h"
#include "network/Client.h"
#include "Backlight.h"
#include "CameraHandler.h"
#include "ParticleSpawner.h"
#include "Settings.h"
#include "RotatingProjectile.h"
#include "ParticleProjectile.h"
#include "GraphicsEngine.h"
#include "SpaceShipAI.h"

namespace orxonox{

SetConsoleCommand(SpaceShip, setMaxSpeedTest, false).accessLevel(AccessLevel::Debug);
    SetConsoleCommand(SpaceShip, whereAmI, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(SpaceShip, moveLongitudinal, true).accessLevel(AccessLevel::User).defaultValue(0, 1.0f).axisParamIndex(0).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveLateral, true).accessLevel(AccessLevel::User).defaultValue(0, 1.0f).axisParamIndex(0).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveYaw, true).accessLevel(AccessLevel::User).defaultValue(0, 1.0f).axisParamIndex(0).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, movePitch, true).accessLevel(AccessLevel::User).defaultValue(0, 1.0f).axisParamIndex(0).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, moveRoll, true).accessLevel(AccessLevel::User).defaultValue(0, 1.0f).axisParamIndex(0).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(SpaceShip, fire, true).accessLevel(AccessLevel::User).keybindMode(KeybindMode::OnHold);
    SetConsoleCommandAliasMulti(SpaceShip, setMaxSpeedTest, "setMaxSpeed", 1, false).accessLevel(AccessLevel::Debug);
    SetConsoleCommandAliasMulti(SpaceShip, setMaxSpeedTest, "setMaxBlubber", 2, false).accessLevel(AccessLevel::Debug);
    SetConsoleCommandAliasMulti(SpaceShip, setMaxSpeedTest, "setRofl", 3, false).accessLevel(AccessLevel::Debug);



    CreateFactory(SpaceShip);

    SpaceShip* SpaceShip::instance_s;


    SpaceShip *SpaceShip::getLocalShip(){
      ObjectList<SpaceShip>::iterator it;
      for(it = ObjectList<SpaceShip>::begin(); it; ++it){
	assert(it->isA(Class(SpaceShip)));
        if( (it)->myShip_ || (network::Host::running() && network::Host::getShipID()==(it)->objectID) ){
//	  COUT(1) << "^^^^^^^^^^ myShip_:" << *it << " classname: " << (*it)->getIdentifier()->getName() << " objectid: " << (*it)->objectID << std::endl;
          return *it;
	}
      }
      return 0;
    }


bool SpaceShip::create()
    {
        if (!myShip_)
        {
            if (network::Host::running())
                //COUT(3) << "this id: " << this->objectID << " myShipID: " << network::Host::getShipID() << std::endl;
                if (network::Host::running() && objectID == network::Host::getShipID())
                {
                    if (!network::Host::isServer())
                        setObjectMode(0x3);
                    myShip_ = true;
                }
        }
        else
            this->setRadarObjectColour(this->getProjectileColour());
        assert(Model::create());
        this->init();
        return true;
    }




    void SpaceShip::registerAllVariables()
    {
        registerVar( &camName_, camName_.length()+1, network::STRING, 0x1 );
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



void SpaceShip::changedVisibility()
    {
        SUPER(SpaceShip, changedVisibility);

        this->tt1_->setEnabled(this->isVisible());
        this->tt2_->setEnabled(this->isVisible());
        this->redBillboard_.setVisible(this->isVisible());
        this->greenBillboard_.setVisible(this->isVisible());
        this->crosshairNear_.setVisible(this->isVisible());
        this->crosshairFar_.setVisible(this->isVisible());
        this->rightThrusterFlare_.setVisible(this->isVisible());
        this->leftThrusterFlare_.setVisible(this->isVisible());
        this->smoke_->setVisible(this->isVisible());
        this->fire_->setVisible(this->isVisible());
        this->backlight_->setVisible(this->isVisible());
    }

    void SpaceShip::changedActivity()
    {
        SUPER(SpaceShip, changedActivity);

        this->tt1_->setEnabled(this->isVisible());
        this->tt2_->setEnabled(this->isVisible());
        this->redBillboard_.setVisible(this->isVisible());
        this->greenBillboard_.setVisible(this->isVisible());
        this->crosshairNear_.setVisible(this->isVisible());
        this->crosshairFar_.setVisible(this->isVisible());
        this->rightThrusterFlare_.setVisible(this->isVisible());
        this->leftThrusterFlare_.setVisible(this->isVisible());
    }

    void SpaceShip::setCamera(const std::string& camera)
    {
      camName_=camera;
      // change camera attributes here, if you want to ;)
    }

    void SpaceShip::getFocus(){
      COUT(4) << "requesting focus" << std::endl;
      //if(!network::Host::running() || network::Host::getShipID()==objectID)
      if(myShip_)
        CameraHandler::getInstance()->requestFocus(cam_);

    }

    Camera* SpaceShip::getCamera(){
        return cam_;
    }

    void SpaceShip::createCamera(){
//       COUT(4) << "begin camera creation" << std::endl;
      this->camNode_ = this->getNode()->createChildSceneNode(camName_);
      COUT(4) << "position: (this)" << this->getNode()->getPosition() << std::endl;
      this->camNode_->setPosition(Vector3(-25,0,5));
//      Quaternion q1 = Quaternion(Radian(Degree(90)),Vector3(0,-1,0));
//      Quaternion q2 = Quaternion(Radian(Degree(90)),Vector3(0,0,-1));
//      camNode_->setOrientation(q1*q2);
      COUT(4) << "position: (cam)" << this->camNode_->getPosition() << std::endl;
      cam_ = new Camera(this->camNode_);

      cam_->setTargetNode(this->getNode());
//        cam->setPosition(Vector3(0,-350,0));
//      Quaternion q1 = Quaternion(Radian(Degree(90)),Vector3(0,0,1));
//      Quaternion q2 = Quaternion(Radian(Degree(90)),Vector3(0,1,0));
      Quaternion q1 = Quaternion(Radian(Degree(90)),Vector3(0,-1,0));
      Quaternion q2 = Quaternion(Radian(Degree(90)),Vector3(1,0,0));

       this->camNode_->setOrientation(q2*q1);
      //if(!network::Host::running() || network::Host::getShipID()==objectID){ //TODO: check this
      if(myShip_){
        COUT(4) << "requesting focus for camera" << std::endl;
        //CameraHandler::getInstance()->requestFocus(cam_);
        if(this->isExactlyA(Class(SpaceShip))){
          getFocus();
          COUT(4) << "getting focus for obj id: " << objectID << std::endl;
        }else
          COUT(4) << "not getting focus (not exactly spaceship) for obj id: " << objectID << std::endl;
      }else
        COUT(4) << "not getting focus (not my ship) for obj id: " << objectID << std::endl;
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


    std::string SpaceShip::whereAmI() {
        return getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().x)
        + "  " + getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().y)
        + "  " + getConvertedValue<float, std::string>(SpaceShip::getLocalShip()->getPosition().z);
    }








}
