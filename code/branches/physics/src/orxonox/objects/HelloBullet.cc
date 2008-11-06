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
 *      Martin Stypinski
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "HelloBullet.h"

#include <OgreStaticGeometry.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "ogreode/OgreOde_Core.h"
#include "ogreode/OgreOdeGeometry.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"

#include "util/Sleep.h"


namespace orxonox
{
    CreateFactory(HelloBullet);
    
    HelloBullet::HelloBullet(BaseObject* creator)
        : BaseObject(creator)
    {
 	   RegisterObject(HelloBullet);
  	   COUT(0) << "HelloBullet loaded" << std::endl ;
           int maxProxies = 1024;


  //      btVector3 worldAabbMin(-10000,-10000,-10000);
  //      btVector3 worldAabbMax(10000,10000,10000);
  //      btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

  //      btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  //      btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

  //      btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  //      dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

	dynamicsWorld =  creator->getScene()->getPhysicalWorld();
        dynamicsWorld-> setGravity(btVector3(0,-10,0));


        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),10);

        btCollisionShape* fallShape = new btSphereShape(1);


        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
        btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        dynamicsWorld->addRigidBody(groundRigidBody);


        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,100,0)));
        btScalar mass = 1000;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
        fallRigidBody = new btRigidBody(fallRigidBodyCI);
        dynamicsWorld->addRigidBody(fallRigidBody);

 

//load floor mash
        Ogre::SceneManager* sceneMgr = creator->getScene()->getSceneManager();

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
                entity->setCastShadows(false);
                floor->addEntity(entity, Ogre::Vector3(x,0,z));
            }
        }       

        floor->build();


// crate

        entity_ = sceneMgr->createEntity("crate","crate.mesh");
        entity_->setQueryFlags (1<<2);
        sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode("crate");
        sceneNode_->attachObject(entity_);
        entity_->setNormaliseNormals(true);
        entity_->setCastShadows(true);
	sceneNode_ -> setPosition(Vector3(0,100,0));


      


    }

    HelloBullet::~HelloBullet()
    {
     //  dynamicsWorld->removeRigidBody(fallRigidBody);
     //   delete fallRigidBody->getMotionState();
     //   delete fallRigidBody;

     //   dynamicsWorld->removeRigidBody(groundRigidBody);
     //   delete groundRigidBody->getMotionState();
     //   delete groundRigidBody;


     //   delete fallShape;

     //   delete groundShape;


     //   delete dynamicsWorld;
     //   delete solver;
     //   delete collisionConfiguration;
     //   delete dispatcher;
     //   delete broadphase;

    }

    void HelloBullet::setConfigValues()
    {

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
    void HelloBullet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
      SUPER(HelloBullet, XMLPort, xmlelement, mode);
    }

    void HelloBullet::tick(float dt)
    {
                dynamicsWorld->stepSimulation(dt,10);
                btTransform trans;
                fallRigidBody->getMotionState()->getWorldTransform(trans);
	       // COUT(0) << "sphere height: " << trans.getOrigin().getY() << std::endl;
		sceneNode_ -> setPosition(Vector3(0,trans.getOrigin().getY(),0));
	//	msleep(20);
		
		
    }

}
