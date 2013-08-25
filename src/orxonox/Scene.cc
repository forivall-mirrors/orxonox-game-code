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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#include "Scene.h"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneManagerEnumerator.h>
#include <OgreSceneNode.h>

#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"
#include "Radar.h"
#include "worldentities/WorldEntity.h"
#include "Level.h"

namespace orxonox
{
    RegisterClass(Scene);

    Scene::Scene(Context* context) : BaseObject(context), Synchronisable(context), Context(context)
    {
        RegisterObject(Scene);

        this->setScene(SmartPtr<Scene>(this, false), OBJECTID_UNKNOWN);
        this->bShadows_ = true;
        this->soundReferenceDistance_ = 20.0;

        if (GameMode::showsGraphics())
        {
            assert(Ogre::Root::getSingletonPtr());
            this->sceneManager_ = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);
            this->rootSceneNode_ = this->sceneManager_->getRootSceneNode();

            this->radar_ = new Radar();
        }
        else
        {
            // create a dummy SceneManager of our own since we don't have Ogre::Root.
            this->sceneManager_ = new Ogre::DefaultSceneManager("");
            this->rootSceneNode_ = this->sceneManager_->getRootSceneNode();

            this->radar_ = 0;
        }

        // No physics yet, XMLPort will do that.
        const int defaultMaxWorldSize = 100000;
        this->negativeWorldRange_ = Vector3::UNIT_SCALE * -defaultMaxWorldSize;
        this->positiveWorldRange_ = Vector3::UNIT_SCALE *  defaultMaxWorldSize;
        this->gravity_ = Vector3::ZERO;
        this->physicalWorld_   = 0;
        this->solver_          = 0;
        this->dispatcher_      = 0;
        this->collisionConfig_ = 0;
        this->broadphase_      = 0;

        this->registerVariables();
    }

    Scene::~Scene()
    {
        if (this->isInitialized())
        {
            if (GameMode::showsGraphics())
                Ogre::Root::getSingleton().destroySceneManager(this->sceneManager_);
            else
                delete this->sceneManager_;

            if (this->radar_)
                this->radar_->destroy();

            this->setPhysicalWorld(false);
        }
    }

    void Scene::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Scene, XMLPort, xmlelement, mode);

        XMLPortParam(Scene, "skybox", setSkybox, getSkybox, xmlelement, mode);
        XMLPortParam(Scene, "ambientlight", setAmbientLight, getAmbientLight, xmlelement, mode).defaultValues(ColourValue(0.2f, 0.2f, 0.2f, 1.0f));
        XMLPortParam(Scene, "shadow", setShadow, getShadow, xmlelement, mode).defaultValues(true);
        XMLPortParam(Scene, "soundReferenceDistance", setSoundReferenceDistance, getSoundReferenceDistance, xmlelement, mode);

        XMLPortParam(Scene, "gravity", setGravity, getGravity, xmlelement, mode);
        XMLPortParam(Scene, "negativeWorldRange", setNegativeWorldRange, getNegativeWorldRange, xmlelement, mode);
        XMLPortParam(Scene, "positiveWorldRange", setPositiveWorldRange, getPositiveWorldRange, xmlelement, mode);
        XMLPortParam(Scene, "hasPhysics", setPhysicalWorld, hasPhysics, xmlelement, mode).defaultValues(true);

        XMLPortObjectExtended(Scene, BaseObject, "", addObject, getObject, xmlelement, mode, true, false);
    }

    void Scene::registerVariables()
    {
        registerVariable(this->skybox_,             VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_applySkybox));
        registerVariable(this->ambientLight_,       VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_applyAmbientLight));
        registerVariable(this->negativeWorldRange_, VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_negativeWorldRange));
        registerVariable(this->positiveWorldRange_, VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_positiveWorldRange));
        registerVariable(this->gravity_,            VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_gravity));
        registerVariable(this->bHasPhysics_,        VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_hasPhysics));
        registerVariable(this->bShadows_,           VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::networkcallback_applyShadows));
        registerVariable(this->getLevel(),          VariableDirection::ToClient, new NetworkCallback<Scene>(this, &Scene::changedLevel));
    }

    void Scene::setNegativeWorldRange(const Vector3& range)
    {
        if (range.length() < 10.0f)
        {
            orxout(internal_warning) << "Setting the negative world range to a very small value: "
                                     << multi_cast<std::string>(range) << endl;
        }
        if (this->hasPhysics())
        {
            orxout(internal_warning) << "Attempting to set the physical world range at run time. "
                                     << "This causes a complete physical reload which might take some time." << endl;
            this->setPhysicalWorld(false);
            this->negativeWorldRange_ = range;
            this->setPhysicalWorld(true);
        }
        else
            this->negativeWorldRange_ = range;
    }

    void Scene::setPositiveWorldRange(const Vector3& range)
    {
        if (range.length() < 10.0f)
        {
            orxout(internal_warning) << "Setting the positive world range to a very small value: "
                                     << multi_cast<std::string>(range) << endl;
        }
        if (this->hasPhysics())
        {
            orxout(internal_warning) << "Attempting to set the physical world range at run time. "
                                     << "This causes a complete physical reload which might take some time." << endl;
            this->setPhysicalWorld(false);
            this->positiveWorldRange_ = range;
            this->setPhysicalWorld(true);
        }
        else
            this->positiveWorldRange_ = range;
    }

    void Scene::setGravity(const Vector3& gravity)
    {
        this->gravity_ = gravity;
        if (this->hasPhysics())
            this->physicalWorld_->setGravity(multi_cast<btVector3>(this->gravity_));
    }

    void Scene::setPhysicalWorld(bool wantPhysics)
    {
        this->bHasPhysics_ = wantPhysics;
        if (wantPhysics && !hasPhysics())
        {
            // Note: These are all little known default classes and values.
            //       It would require further investigation to properly dertermine the right choices.
            this->broadphase_      = new bt32BitAxisSweep3(
                multi_cast<btVector3>(this->negativeWorldRange_), multi_cast<btVector3>(this->positiveWorldRange_));
            this->collisionConfig_ = new btDefaultCollisionConfiguration();
            this->dispatcher_      = new btCollisionDispatcher(this->collisionConfig_);
            this->solver_          = new btSequentialImpulseConstraintSolver();

            this->physicalWorld_   = new btDiscreteDynamicsWorld(this->dispatcher_, this->broadphase_, this->solver_, this->collisionConfig_);
            this->physicalWorld_->setGravity(multi_cast<btVector3>(this->gravity_));

            // also set the collision callback variable.
            // Note: This is a global variable which we assign a static function.
            // TODO: Check whether this (or anything about Bullet) works with multiple physics engine instances.
            gContactAddedCallback = &Scene::collisionCallback;
        }
        else if (!wantPhysics && hasPhysics())
        {
            // Remove all WorldEntities and shove them to the queue since they would still like to be in a physical world.
            for (std::set<WorldEntity*>::const_iterator it = this->physicalObjects_.begin();
                it != this->physicalObjects_.end(); ++it)
            {
                this->physicalWorld_->removeRigidBody((*it)->physicalBody_);
                this->physicalObjectQueue_.insert(*it);
            }
            this->physicalObjects_.clear();

            delete this->physicalWorld_;
            delete this->solver_;
            delete this->dispatcher_;
            delete this->collisionConfig_;
            delete this->broadphase_;
            this->physicalWorld_   = 0;
            this->solver_          = 0;
            this->dispatcher_      = 0;
            this->collisionConfig_ = 0;
            this->broadphase_      = 0;
        }
    }

    void Scene::tick(float dt)
    {
        if (!GameMode::showsGraphics())
        {
            // We need to update the scene nodes if we don't render
            this->rootSceneNode_->_update(true, false);
        }
        if (this->hasPhysics())
        {
            // TODO: This here is bad practice! It will slow down the first tick() by ages.
            //       Rather have an initialise() method as well, called by the Level after everything has been loaded.
            if (this->physicalObjectQueue_.size() > 0)
            {
                // Add all scheduled WorldEntities
                for (std::set<WorldEntity*>::const_iterator it = this->physicalObjectQueue_.begin();
                    it != this->physicalObjectQueue_.end(); ++it)
                {
                    this->physicalWorld_->addRigidBody((*it)->physicalBody_);
                    this->physicalObjects_.insert(*it);
                }
                this->physicalObjectQueue_.clear();
            }

            // Note: 60 means that Bullet will do physics correctly down to 1 frames per seconds.
            //       Under that mark, the simulation will "loose time" and get unusable.
            physicalWorld_->stepSimulation(dt, 60);
        }
    }

    void Scene::setSkybox(const std::string& skybox)
    {
        try
        {
            if (GameMode::showsGraphics() && this->sceneManager_)
                this->sceneManager_->setSkyBox(true, skybox);
        }
        catch (const Ogre::Exception&)
        {
            orxout(internal_error) << "Could not load skybox '" << skybox << "':" << endl;
            orxout(internal_error) << Exception::handleMessage() << endl;
        }

        this->skybox_ = skybox;
    }

    void Scene::setAmbientLight(const ColourValue& colour)
    {
        if (GameMode::showsGraphics() && this->sceneManager_)
            this->sceneManager_->setAmbientLight(colour);

        this->ambientLight_ = colour;
    }

    void Scene::setShadow(bool bShadow)
    {
        if (GameMode::showsGraphics() && this->sceneManager_)
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
        object->setScene(this, this->getObjectID());
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

    void Scene::addPhysicalObject(WorldEntity* object)
    {
        if (object)
        {
            std::set<WorldEntity*>::iterator it = this->physicalObjects_.find(object);
            if (it != this->physicalObjects_.end())
                return;

            this->physicalObjectQueue_.insert(object);
        }
    }

    void Scene::removePhysicalObject(WorldEntity* object)
    {
        // check queue first
        std::set<WorldEntity*>::iterator it = this->physicalObjectQueue_.find(object);
        if (it != this->physicalObjectQueue_.end())
        {
            this->physicalObjectQueue_.erase(it);
            return;
        }

        it = this->physicalObjects_.find(object);
        if (it == this->physicalObjects_.end())
            return;
        this->physicalObjects_.erase(it);

        if (this->hasPhysics())
            this->physicalWorld_->removeRigidBody(object->physicalBody_);
    }

    /*static*/ bool Scene::collisionCallback(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0,
                                             int index0, const btCollisionObject* colObj1, int partId1, int index1)
    {
        // get the WorldEntity pointers
        SmartPtr<WorldEntity> object0 = static_cast<WorldEntity*>(colObj0->getUserPointer());
        SmartPtr<WorldEntity> object1 = static_cast<WorldEntity*>(colObj1->getUserPointer());

        // false means that bullet will assume we didn't modify the contact
        bool modified = false;
        if (object0->isCollisionCallbackActive())
            modified |= object0->collidesAgainst(object1, cp);
        if (object1->isCollisionCallbackActive())
            modified |= object1->collidesAgainst(object0, cp);

        return modified;
    }
}
