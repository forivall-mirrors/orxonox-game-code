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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "PhysicsTest.h"

#include <OgreStaticGeometry.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "ogreode/OgreOde_Core.h"
#include "ogreode/OgreOdeGeometry.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/XMLPort.h"
#include "GraphicsEngine.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(PhysicsTest);

    PhysicsTest::PhysicsTest(BaseObject* creator)
        : BaseObject(creator)
        , odeWorld_(0)
        , odeSpace_(0)
        , odeStepper_(0)
        , odeGround_(0)   
        , odeBody_(0)
        , odeGeom_(0)
        , sceneNode_(0)
        , entity_(0)
        , bRunning_(false)
    {
        RegisterObject(PhysicsTest);
        setConfigValues();
        ModifyConfigValue(bRunning_, tset, false);
    }

    PhysicsTest::~PhysicsTest()
    {
    }

    void PhysicsTest::setConfigValues()
    {
        SetConfigValue(bRunning_, false);
    }

    /**
    @brief
        XML loading and saving.
    @param
        xmlelement The XML-element
    @param
        loading Loading (true) or saving (false)
    @return
        The XML-element
    */
    void PhysicsTest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PhysicsTest, XMLPort, xmlelement, mode);

        Ogre::SceneManager* sceneMgr = this->getScene()->getSceneManager();

        // set up OgreOde

        odeWorld_ = new OgreOde::World(sceneMgr);
        odeWorld_->setGravity(Vector3(0,-9.80665,0));
        odeWorld_->setCFM(10e-5);
        odeWorld_->setERP(0.8);
        odeWorld_->setAutoSleep(true);
        odeWorld_->setAutoSleepAverageSamplesCount(10);
        odeWorld_->setContactCorrectionVelocity(1.0);
        odeSpace_ = odeWorld_->getDefaultSpace();


        // set up stepper

        const Ogre::Real _time_step = 0.5;http://isg.ee.ethz.ch/
        const Ogre::Real time_scale = Ogre::Real(1.7);
        const Ogre::Real max_frame_time = Ogre::Real(1.0 / 4);
        odeStepper_ = new OgreOde::StepHandler(odeWorld_, OgreOde::StepHandler::QuickStep, _time_step,
            max_frame_time, time_scale);


        // create a plane in x-z dimensions.

        odeGround_ = new OgreOde::InfinitePlaneGeometry(Ogre::Plane(Ogre::Vector3(0,1,0),0),
            odeWorld_, odeWorld_->getDefaultSpace());

        CollidingObject* collidingObject = new CollidingObject();
        odeGround_->setUserObject(static_cast<CollisionTestedObject*>(collidingObject));

        // Use a load of meshes to represent the floor
        int i = 0;
        Ogre::StaticGeometry* floor;
        floor = sceneMgr->createStaticGeometry("StaticFloor");
        floor->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
        // Set the region origin so the center is at 0 world
        floor->setOrigin(Ogre::Vector3::ZERO);
        for (Real z = -80.0; z <= 80.0; z += 20.0)
        {
            for (Real x = -80.0; x <= 80.0; x += 20.0)
            {
                std::string name = std::string("Ground") + convertToString(i++);
                Ogre::Entity* entity = sceneMgr->createEntity(name, "plane.mesh");
                entity->setQueryFlags (1<<4);
                entity->setUserObject(odeGround_);
                entity->setCastShadows(false);
                floor->addEntity(entity, Ogre::Vector3(x,0,z));
            }
        }       

        floor->build();


        // create a hanging crate

        entity_ = sceneMgr->createEntity("crate","crate.mesh");
        entity_->setQueryFlags (1<<2);
        sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode("crate");
        sceneNode_->attachObject(entity_);
        entity_->setNormaliseNormals(true);
        entity_->setCastShadows(true);

        odeBody_ = new OgreOde::Body(odeWorld_);
        sceneNode_->attachObject(odeBody_);

        
        // set size and mass of the crate

        Vector3 size(10.0, 10.0, 10.0);
        odeMass_ = OgreOde::BoxMass(0.5, size);
        odeMass_.setDensity(5.0, size);
        odeGeom_ = new OgreOde::BoxGeometry(size, odeWorld_, odeSpace_);
        sceneNode_->setScale(size.x * 0.1, size.y * 0.1, size.z * 0.1);
        odeBody_->setMass(odeMass_);
        odeGeom_->setBody(odeBody_);
        entity_->setUserObject(odeGeom_);
        odeGeom_->setUserObject(static_cast<CollisionTestedObject*>(this));


        odeBody_->setOrientation(Quaternion(Degree(30.0), Vector3(0,0,0)));
        odeBody_->setPosition(Vector3(0,120,-20));

    }

    void PhysicsTest::tick(float dt)
    {
        // only update physics in a certain interval
        if (this->bRunning_ && odeStepper_->step(dt))
            odeWorld_->synchronise();
    }

    bool PhysicsTest::collision(OgreOde::Contact* contact)
    {
        // Check for collisions between things that are connected and ignore them
        OgreOde::Geometry * const g1 = contact->getFirstGeometry();
        OgreOde::Geometry * const g2 = contact->getSecondGeometry();

        if (g1 && g2)
        {
            const OgreOde::Body * const b1 = g2->getBody();
            const OgreOde::Body * const b2 = g1->getBody();
            if (b1 && b2 && OgreOde::Joint::areConnected(b1, b2)) 
                return false; 
        }

        //set contact parameters:
        contact->setBouncyness(1.0);
        contact->setCoulombFriction(OgreOde::Utility::Infinity);
        contact->setForceDependentSlip(1.0);
        contact->setAdditionalFDS(1.0);

        // we have 2 collidable objects from our object system, if one of the Collide function returns false, e return false in this method, too, else we return true, so ode computes a normal collision.
        // true means ode will treat this like a normal collison => rigid body behavior
        // false means ode will not treat this collision at all => objects ignore each other

        bool res = true;

        if (g1->getUserObject())
            if (!static_cast<CollisionTestedObject*>(g1->getUserObject())->collide(true, contact))
                res = false;

        if (g2->getUserObject())
            if (!static_cast<CollisionTestedObject*>(g2->getUserObject())->collide(false, contact))
                res = false;

        return res;
    }

    bool CollidingObject::Collide(bool MineIsFirst, OgreOde::Contact* contact)
    {
        contact->setForceDependentSlip(contact->getForceDependentSlip() * ForceDependentSlip);
        contact->setAdditionalFDS(contact->getForceDependentSlip2() * ForceDependentSlip);
        contact->setCoulombFriction(contact->getCoulombFrictionMu() * Friction);
        contact->setBouncyness(contact->getBouncyness() * Bouncyness, contact->getBouncynessVelocity() * BounceVelocity);
        return true;
    }

}
