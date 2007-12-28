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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include <string>

#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "tinyxml/tinyxml.h"
#include "misc/String2Number.h"
#include "../core/CoreIncludes.h"
#include "../Orxonox.h"
#include "../particle/ParticleInterface.h"
#include "weapon_system/AmmunitionDump.h"
#include "weapon_system/BarrelGun.h"

#include "Fighter.h"

namespace orxonox
{
    CreateFactory(Fighter);

    Fighter::Fighter()
    {
        RegisterObject(Fighter);

        this->setConfigValues();

        this->setMouseEventCallback_ = false;

        this->w = NULL;
        this->tt = NULL;
        this->ammoDump_ = NULL;
        this->mainWeapon_ = NULL;
        this->rightButtonPressed_ = false;
        this->leftButtonPressed_ = false;

        this->moveForward_ = 0;
        this->rotateUp_ = 0;
        this->rotateDown_ = 0;
        this->rotateRight_ = 0;
        this->rotateLeft_ = 0;
        this->loopRight_ = 0;
        this->loopLeft_ = 0;
        this->brakeForward_ = 0;
        this->brakeRotate_ = 0;
        this->brakeLoop_ = 0;
        this->speedForward_ = 0;
        this->speedRotateUpDown_ = 0;
        this->speedRotateRightLeft_ = 0;
        this->speedLoopRightLeft_ = 0;
        this->maxSpeedForward_ = 0;
        this->maxSpeedRotateUpDown_ = 0;
        this->maxSpeedRotateRightLeft_ = 0;
        this->maxSpeedLoopRightLeft_ = 0;
        this->accelerationForward_ = 0;
        this->accelerationRotateUpDown_ = 0;
        this->accelerationRotateRightLeft_ = 0;
        this->accelerationLoopRightLeft_ = 0;

        this->speed = 250;
        this->loop = 100;
        this->rotate = 10;
        this->mouseX = 0;
        this->mouseY = 0;
        this->maxMouseX = 0;
        this->minMouseX = 0;
        this->moved = false;

        this->brakeRotate(rotate*10);
        this->brakeLoop(loop);

        COUT(3) << "Info: Fighter was loaded" << std::endl;
    }

    Fighter::~Fighter()
    {
        if (w)
            delete w;
        if (tt)
            delete tt;
    }

    void Fighter::setConfigValues()
    {
        SetConfigValue(bInvertMouse_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
    }

    void Fighter::setMaxSpeedValues(float maxSpeedForward, float maxSpeedRotateUpDown, float maxSpeedRotateRightLeft, float maxSpeedLoopRightLeft)
    {
        this->maxSpeedForward_ = maxSpeedForward;
        this->maxSpeedRotateUpDown_ = maxSpeedRotateUpDown;
        this->maxSpeedRotateRightLeft_ = maxSpeedRotateRightLeft;
        this->maxSpeedLoopRightLeft_ = maxSpeedLoopRightLeft;
    }

    void Fighter::loadParams(TiXmlElement* xmlElem)
    {
        Model::loadParams(xmlElem);

#if 0
        w = new particle::ParticleInterface(Orxonox::getSingleton()->getSceneManager(),"schuss" + this->getName(),"Orxonox/schuss");
        w->getParticleSystem()->setParameter("local_space","true");
        w->newEmitter();
/*
        w->setDirection(Vector3(0,0,1));
        w->setPositionOfEmitter(0, Vector3(10,10,0));
        w->setPositionOfEmitter(1, Vector3(-10,10,0));
*/
        w->setDirection(Vector3(1,0,0));
        w->setPositionOfEmitter(0, Vector3(0,10,10));
        w->setPositionOfEmitter(1, Vector3(0,-10,10));

        emitterRate_ = w->getRate();

        Ogre::SceneNode* node1 = this->getNode()->createChildSceneNode(this->getName() + "particle1");
        node1->setInheritScale(false);
        w->addToSceneNode(node1);
#endif

        tt = new ParticleInterface(Orxonox::getSingleton()->getSceneManager(),"twinthruster" + this->getName(),"Orxonox/engineglow");
        tt->getParticleSystem()->setParameter("local_space","true");
        tt->newEmitter();
/*
        tt->setDirection(Vector3(0,0,1));
        tt->setPositionOfEmitter(0, Vector3(20,-1,-15));
        tt->setPositionOfEmitter(1, Vector3(-20,-1,-15));
*/
        tt->setDirection(Vector3(-1,0,0));
        tt->setPositionOfEmitter(0, Vector3(-15,20,-1));
        tt->setPositionOfEmitter(1, Vector3(-15,-20,-1));
        tt->setVelocity(50);

        Ogre::SceneNode* node2 = this->getNode()->createChildSceneNode(this->getName() + "particle2");
        node2->setInheritScale(false);
        tt->addToSceneNode(node2);

        // add weapon

        ammoDump_ = new AmmunitionDump();
        ammoDump_->setDumpSize("Barrel", 1000);
        ammoDump_->store("Barrel", 420);

        mainWeapon_ = new BarrelGun();
        mainWeapon_->setAmmoDump(ammoDump_);
        Orxonox::getSingleton()->getSceneManager()->getRootSceneNode()->removeChild(mainWeapon_->getNode());
        getNode()->addChild(mainWeapon_->getNode());

        if (xmlElem->Attribute("forward") && xmlElem->Attribute("rotateupdown") && xmlElem->Attribute("rotaterightleft") && xmlElem->Attribute("looprightleft"))
        {
            std::string forwardStr = xmlElem->Attribute("forward");
            std::string rotateupdownStr = xmlElem->Attribute("rotateupdown");
            std::string rotaterightleftStr = xmlElem->Attribute("rotaterightleft");
            std::string looprightleftStr = xmlElem->Attribute("looprightleft");

            String2Number<float>(this->maxSpeedForward_, forwardStr);
            String2Number<float>(this->maxSpeedRotateUpDown_, rotateupdownStr);
            String2Number<float>(this->maxSpeedRotateRightLeft_, rotaterightleftStr);
            String2Number<float>(this->maxSpeedLoopRightLeft_, looprightleftStr);

            COUT(4) << "Loader: Initialized spaceship steering with values " << maxSpeedForward_ << " " << maxSpeedRotateUpDown_ << " " << maxSpeedRotateRightLeft_ << " " << maxSpeedLoopRightLeft_ << " " << std::endl;
    	}

    	if (xmlElem->Attribute("camera"))
    	{
            Ogre::Camera *cam = Orxonox::getSingleton()->getSceneManager()->createCamera("ShipCam");
            Ogre::SceneNode *node = this->getNode()->createChildSceneNode("CamNode");
/*
//            node->setInheritOrientation(false);
            cam->setPosition(Vector3(0,50,-150));
            cam->lookAt(Vector3(0,20,0));
            cam->roll(Degree(0));
*/

            cam->setPosition(Vector3(-150,0,50));
//            cam->setPosition(Vector3(0,-350,0));
            cam->lookAt(Vector3(0,0,20));
            cam->roll(Degree(-90));

            node->attachObject(cam);
            Orxonox::getSingleton()->getOgrePointer()->getRoot()->getAutoCreatedWindow()->addViewport(cam);
    	}
    }

    bool Fighter::mouseMoved(const OIS::MouseEvent &e)
    {
        this->mouseX += e.state.X.rel;
        //if (this->bInvertMouse_)
            //this->mouseY += e.state.Y.rel;
        //else
            this->mouseY -= e.state.Y.rel;

//        if(mouseX>maxMouseX) maxMouseX = mouseX;
//        if(mouseX<minMouseX) minMouseX = mouseX;
//        cout << "mouseX: " << mouseX << "\tmouseY: " << mouseY << endl;

        this->moved = true;

        return true;
    }

    bool Fighter::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
    {
      if (id == OIS::MB_Left)
      {
        this->leftButtonPressed_ = true;
      }
      else if (id == OIS::MB_Right)
        this->rightButtonPressed_ = true;
      return true;
    }

    bool Fighter::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
    {
      if (id == OIS::MB_Left)
      {
        this->leftButtonPressed_ = false;
      }
      else if (id == OIS::MB_Right)
        this->rightButtonPressed_ = false;
      return true;
    }

    void Fighter::tick(float dt)
    {
        if (!this->setMouseEventCallback_)
        {
            if (Orxonox::getSingleton()->getMouse())
            {
                Orxonox::getSingleton()->getMouse()->setEventCallback(this);
                this->setMouseEventCallback_ = true;
            }
        }

        WorldEntity::tick(dt);

        OIS::Keyboard* mKeyboard = Orxonox::getSingleton()->getKeyboard();
        OIS::Mouse* mMouse = Orxonox::getSingleton()->getMouse();

        mKeyboard->capture();
        mMouse->capture();

        if (leftButtonPressed_)
            mainWeapon_->primaryFireRequest();
        if (rightButtonPressed_)
            mainWeapon_->secondaryFireRequest();

        if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
            this->moveForward(speed);
        else
            this->moveForward(0);

        if(mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
            this->brakeForward(speed);
        else
            this->brakeForward(speed/10);

        if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
            this->loopRight(loop);
        else
            this->loopRight(0);

        if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
            this->loopLeft(loop);
        else
            this->loopLeft(0);

        if (mKeyboard->isKeyDown(OIS::KC_G))
            this->mainWeapon_->addAction(BaseWeapon::RELOAD);
        else
            this->loopLeft(0);

        if(moved)
        {
            if (mouseY<=0)
                this->rotateUp(-mouseY*rotate);
            if (mouseY>0)
                this->rotateDown(mouseY*rotate);
            if (mouseX>0)
                this->rotateRight(mouseX*rotate);
            if (mouseX<=0)
                this->rotateLeft(-mouseX*rotate);

            mouseY = 0;
            mouseX = 0;
            moved = false;
        }
        else
        {
            this->rotateUp(0);
            this->rotateDown(0);
            this->rotateRight(0);
            this->rotateLeft(0);
        }

        if(moveForward_ > 0)
        {
            accelerationForward_ = moveForward_;
            if(speedForward_ < maxSpeedForward_)
                speedForward_ += accelerationForward_*dt;
            if(speedForward_ > maxSpeedForward_)
                speedForward_ = maxSpeedForward_;
        }

        if(moveForward_ <= 0)
        {
            accelerationForward_ = -brakeForward_;
            if(speedForward_ > 0)
                speedForward_ += accelerationForward_*dt;
            if(speedForward_ < 0)
                speedForward_ = 0;
        }

        if(rotateUp_ > 0)
        {
            accelerationRotateUpDown_ = rotateUp_;
            if(speedRotateUpDown_ < maxSpeedRotateUpDown_)
                speedRotateUpDown_ += accelerationRotateUpDown_*dt;
            if(speedRotateUpDown_ > maxSpeedRotateUpDown_)
            speedRotateUpDown_ = maxSpeedRotateUpDown_;
        }

        if(rotateDown_ > 0)
        {
            accelerationRotateUpDown_ = rotateDown_;
            if(speedRotateUpDown_ > -maxSpeedRotateUpDown_)
                speedRotateUpDown_ -= accelerationRotateUpDown_*dt;
            if(speedRotateUpDown_ < -maxSpeedRotateUpDown_)
                speedRotateUpDown_ = -maxSpeedRotateUpDown_;
        }

        if(rotateUp_ == 0 && rotateDown_ == 0)
        {
            accelerationRotateUpDown_ = brakeRotate_;
            if(speedRotateUpDown_ > 0)
                speedRotateUpDown_ -= accelerationRotateUpDown_*dt;
            if(speedRotateUpDown_ < 0)
                speedRotateUpDown_ += accelerationRotateUpDown_*dt;
            if(fabs(speedRotateUpDown_) < accelerationRotateUpDown_*dt)
                speedRotateUpDown_ = 0;
        }

        if(rotateRight_ > 0)
        {
            accelerationRotateRightLeft_ = rotateRight_;
            if(speedRotateRightLeft_ > -maxSpeedRotateRightLeft_)
                speedRotateRightLeft_ -= accelerationRotateRightLeft_*dt;
            if(speedRotateRightLeft_ < -maxSpeedRotateRightLeft_)
                speedRotateRightLeft_ = -maxSpeedRotateRightLeft_;
        }

        if(rotateLeft_ > 0)
        {
            accelerationRotateRightLeft_ = rotateLeft_;
            if(speedRotateRightLeft_ < maxSpeedRotateRightLeft_)
                speedRotateRightLeft_ += accelerationRotateRightLeft_*dt;
            if(speedRotateRightLeft_ > maxSpeedRotateRightLeft_)
                speedRotateRightLeft_ = maxSpeedRotateRightLeft_;
        }

        if(rotateRight_ == 0 && rotateLeft_ == 0)
        {
            accelerationRotateRightLeft_ = brakeRotate_;
            if(speedRotateRightLeft_ > 0)
                speedRotateRightLeft_ -= accelerationRotateRightLeft_*dt;
            if(speedRotateRightLeft_ < 0)
                speedRotateRightLeft_ += accelerationRotateRightLeft_*dt;
            if(fabs(speedRotateRightLeft_) < accelerationRotateRightLeft_*dt)
                speedRotateRightLeft_ = 0;
        }

        if(loopRight_ > 0)
        {
            accelerationLoopRightLeft_ = loopRight_;
            if(speedLoopRightLeft_ < maxSpeedLoopRightLeft_)
                speedLoopRightLeft_ += accelerationLoopRightLeft_*dt;
            if(speedLoopRightLeft_ > maxSpeedLoopRightLeft_)
                speedLoopRightLeft_ = maxSpeedLoopRightLeft_;
        }

        if(loopLeft_ > 0)
        {
            accelerationLoopRightLeft_ = loopLeft_;
            if(speedLoopRightLeft_ > -maxSpeedLoopRightLeft_)
                speedLoopRightLeft_ -= accelerationLoopRightLeft_*dt;
            if(speedLoopRightLeft_ < -maxSpeedLoopRightLeft_)
                speedLoopRightLeft_ = -maxSpeedLoopRightLeft_;
        }

        if(loopLeft_ == 0 && loopRight_ == 0)
        {
            accelerationLoopRightLeft_ = brakeLoop_;
            if(speedLoopRightLeft_ > 0)
                speedLoopRightLeft_ -= accelerationLoopRightLeft_*dt;
            if(speedLoopRightLeft_ < 0)
                speedLoopRightLeft_ += accelerationLoopRightLeft_*dt;
            if(fabs(speedLoopRightLeft_) < accelerationLoopRightLeft_*dt)
                speedLoopRightLeft_ = 0;
        }

        Vector3 transVector = Vector3::ZERO;
/*
        transVector.z = 1;
        this->translate(transVector*speedForward_*dt, Ogre::Node::TS_LOCAL);
        this->pitch(Degree(speedRotateUpDown_*dt), Ogre::Node::TS_LOCAL);
        this->yaw(Degree(speedRotateRightLeft_*dt), Ogre::Node::TS_LOCAL);
        this->roll(Degree(speedLoopRightLeft_*dt), Ogre::Node::TS_LOCAL);
*/

        transVector.x = 1;
        this->translate(transVector*speedForward_*dt, Ogre::Node::TS_LOCAL);
        this->yaw(Degree(speedRotateUpDown_*dt), Ogre::Node::TS_LOCAL);
        this->roll(Degree(speedRotateRightLeft_*dt), Ogre::Node::TS_LOCAL);
        this->pitch(Degree(speedLoopRightLeft_*dt), Ogre::Node::TS_LOCAL);

        if (accelerationForward_ > 25.0)
        {
          this->tt->setRate(emitterRate_);
        }
        else
        {
          this->tt->setRate(0);
        }

    }

    void Fighter::moveForward(float moveForward) {
        moveForward_ = moveForward;
    }

    void Fighter::rotateUp(float rotateUp) {
        rotateUp_ = rotateUp;
    }

    void Fighter::rotateDown(float rotateDown) {
        rotateDown_ = rotateDown;
    }

    void Fighter::rotateLeft(float rotateLeft) {
        rotateLeft_ = rotateLeft;
    }

    void Fighter::rotateRight(float rotateRight) {
        rotateRight_ = rotateRight;
    }

    void Fighter::loopLeft(float loopLeft) {
        loopLeft_ = loopLeft;
    }

    void Fighter::loopRight(float loopRight) {
        loopRight_ = loopRight;
    }

    void Fighter::brakeForward(float brakeForward) {
        brakeForward_ = brakeForward;
    }

    void Fighter::brakeRotate(float brakeRotate) {
        brakeRotate_ = brakeRotate;
    }

    void Fighter::brakeLoop(float brakeLoop) {
        brakeLoop_ = brakeLoop;
    }

    void Fighter::maxSpeedForward(float maxSpeedForward) {
        maxSpeedForward_ = maxSpeedForward;
    }

    void Fighter::maxSpeedRotateUpDown(float maxSpeedRotateUpDown) {
        maxSpeedRotateUpDown_ = maxSpeedRotateUpDown;
    }

    void Fighter::maxSpeedRotateRightLeft(float maxSpeedRotateRightLeft) {
        maxSpeedRotateRightLeft_ = maxSpeedRotateRightLeft;
    }

    void Fighter::maxSpeedLoopRightLeft(float maxSpeedLoopRightLeft) {
        maxSpeedLoopRightLeft_ = maxSpeedLoopRightLeft;
    }
}
