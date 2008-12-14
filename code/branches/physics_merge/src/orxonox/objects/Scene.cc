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
 *      Reto Grieder (physics)
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Scene.h"

#include <OgreRoot.h>
#include <OgreSceneManagerEnumerator.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"
#include "objects/worldentities/WorldEntity.h"

namespace orxonox
{
    CreateFactory(Scene);

    Scene::Scene(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(Scene);

        this->setScene(this);
        this->bShadows_ = false;

        if (Core::showsGraphics())
        {
            if (Ogre::Root::getSingletonPtr())
            {
                this->sceneManager_ = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);
                this->rootSceneNode_ = this->sceneManager_->getRootSceneNode();
            }
            else
            {
                this->sceneManager_ = 0;
                this->rootSceneNode_ = 0;
            }
        }
        else
        {
            // create a dummy SceneManager of our own since we don't have Ogre::Root.
            this->sceneManager_ = new Ogre::DefaultSceneManager("");
            this->rootSceneNode_ = this->sceneManager_->getRootSceneNode();
        }

        // No physics for default
        this->physicalWorld_ = 0;

        // test test test
        if (Core::showsGraphics() && this->sceneManager_)
        {
            Ogre::Light* light;
            light = this->sceneManager_->createLight("Light-1");
            light->setType(Ogre::Light::LT_DIRECTIONAL);
            light->setDiffuseColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setSpecularColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setDirection(1, -0.3, 0.3);
        }
        // test test test

        this->registerVariables();
    }

    Scene::~Scene()
    {
        if (this->isInitialized())
        {
            if (Ogre::Root::getSingletonPtr())
            {
                Ogre::Root::getSingleton().destroySceneManager(this->sceneManager_);
            }
            else if (!Core::showsGraphics())
            {
                delete this->sceneManager_;
            }
        }
    }

    void Scene::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Scene, XMLPort, xmlelement, mode);

        XMLPortParam(Scene, "skybox", setSkybox, getSkybox, xmlelement, mode);
        XMLPortParam(Scene, "ambientlight", setAmbientLight, getAmbientLight, xmlelement, mode).defaultValues(ColourValue(0.2, 0.2, 0.2, 1));
        XMLPortParam(Scene, "shadow", setShadow, getShadow, xmlelement, mode).defaultValues(true);

        //const int defaultMaxWorldSize = 100000;
        //Vector3 worldAabbMin(-defaultMaxWorldSize, -defaultMaxWorldSize, -defaultMaxWorldSize);
        //Vector3 worldAabbMax( defaultMaxWorldSize,  defaultMaxWorldSize,  defaultMaxWorldSize);
        //XMLPortParamVariable(Scene, "negativeWorldRange", worldAabbMin, xmlelement, mode);
        //XMLPortParamVariable(Scene, "positiveWorldRange", worldAabbMax, xmlelement, mode);
        XMLPortParam(Scene, "hasPhysics", setPhysicalWorld, hasPhysics, xmlelement, mode).defaultValue(0, true);//.defaultValue(1, worldAabbMin).defaultValue(2, worldAabbMax);

        XMLPortObjectExtended(Scene, BaseObject, "", addObject, getObject, xmlelement, mode, true, false);
    }

    void Scene::registerVariables()
    {
        registerVariable(this->skybox_,       variableDirection::toclient, new NetworkCallback<Scene>(this, &Scene::networkcallback_applySkybox));
        registerVariable(this->ambientLight_, variableDirection::toclient, new NetworkCallback<Scene>(this, &Scene::networkcallback_applyAmbientLight));
        registerVariable(this->bHasPhysics_,  variableDirection::toclient, new NetworkCallback<Scene>(this, &Scene::networkcallback_hasPhysics));
    }

    void Scene::setPhysicalWorld(bool wantPhysics)//, const Vector3& worldAabbMin, const Vector3& worldAabbMax)
    {
        this->bHasPhysics_ = wantPhysics;
        if (wantPhysics && !hasPhysics())
        {
            //float x = worldAabbMin.x;
            //float y = worldAabbMin.y;
            //float z = worldAabbMin.z;
            btVector3 worldAabbMin(-100000, -100000, -100000);
            //x = worldAabbMax.x;
            //y = worldAabbMax.y;
            //z = worldAabbMax.z;
            btVector3 worldAabbMax(100000, 100000, 100000);

            btDefaultCollisionConfiguration*     collisionConfig = new btDefaultCollisionConfiguration();
            btCollisionDispatcher*               dispatcher      = new btCollisionDispatcher(collisionConfig);
            bt32BitAxisSweep3*                   broadphase      = new bt32BitAxisSweep3(worldAabbMin,worldAabbMax);
            btSequentialImpulseConstraintSolver* solver          = new btSequentialImpulseConstraintSolver;

            this->physicalWorld_ =  new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

            // Disable Gravity for space
            this->physicalWorld_->setGravity(btVector3(0,0,0));
        }
        else
        {
            // TODO: Destroy Bullet physics
        }
    }

    void Scene::tick(float dt)
    {
        if (!Core::showsGraphics())
        {
            // We need to update the scene nodes if we don't render
            this->rootSceneNode_->_update(true, false);
        }
        if (physicalWorld_)
        {
            if (this->physicsQueue_.size() > 0)
            {
                // Add all scheduled WorldEntities
                for (std::set<btRigidBody*>::const_iterator it = this->physicsQueue_.begin();
                    it != this->physicsQueue_.end(); ++it)
                {
                    if (!(*it)->isInWorld())
                    {
                        //COUT(0) << "body position: " << omni_cast<Vector3>((*it)->getWorldTransform().getOrigin()) << std::endl;
                        //COUT(0) << "body velocity: " << omni_cast<Vector3>((*it)->getLinearVelocity()) << std::endl;
                        //COUT(0) << "body orientation: " << omni_cast<Quaternion>((*it)->getWorldTransform().getRotation()) << std::endl;
                        //COUT(0) << "body angular: " << omni_cast<Vector3>((*it)->getAngularVelocity()) << std::endl;
                        //COUT(0) << "body mass: " << omni_cast<float>((*it)->getInvMass()) << std::endl;
                        //COUT(0) << "body inertia: " << omni_cast<Vector3>((*it)->getInvInertiaDiagLocal()) << std::endl;
                        this->physicalWorld_->addRigidBody(*it);
                    }
                }
                this->physicsQueue_.clear();
            }

            // TODO: This is not stable! If physics cannot be calculated real time anymore,
            //       framerate will drop exponentially.
            physicalWorld_->stepSimulation(dt,(int)(dt/0.0166666f + 1.0f));
        }
    }

    void Scene::setSkybox(const std::string& skybox)
    {
        if (Core::showsGraphics() && this->sceneManager_)
            this->sceneManager_->setSkyBox(true, skybox);

        this->skybox_ = skybox;
    }

    void Scene::setAmbientLight(const ColourValue& colour)
    {
        if (Core::showsGraphics() && this->sceneManager_)
            this->sceneManager_->setAmbientLight(colour);

        this->ambientLight_ = colour;
    }

    void Scene::setShadow(bool bShadow)
    {
        if (Core::showsGraphics() && this->sceneManager_)
        {
            if (bShadow)
                this->sceneManager_->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
            else
                this->sceneManager_->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
        }

        this->bShadows_ = bShadow;
    }

    void Scene::addObject(BaseObject* object)
    {
        this->objects_.push_back(object);
        object->setScene(this);
    }

    BaseObject* Scene::getObject(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<BaseObject*>::const_iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void Scene::addRigidBody(btRigidBody* body)
    {
        if (!this->physicalWorld_)
            COUT(1) << "Error: Cannot add WorldEntity body to physical Scene: No physics." << std::endl;
        else if (body)
            this->physicsQueue_.insert(body);
    }

    void Scene::removeRigidBody(btRigidBody* body)
    {
        if (!this->physicalWorld_)
            COUT(1) << "Error: Cannot remove WorldEntity body from physical Scene: No physics." << std::endl;
        else if (body)
        {
            this->physicalWorld_->removeRigidBody(body);
            // Also check queue
            std::set<btRigidBody*>::iterator it = this->physicsQueue_.find(body);
            if (it != this->physicsQueue_.end())
                this->physicsQueue_.erase(it);
        }
    }
}
