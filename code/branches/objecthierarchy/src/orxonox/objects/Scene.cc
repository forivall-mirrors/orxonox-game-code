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
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Scene.h"

#include <OgreRoot.h>
#include <OgreSceneManagerEnumerator.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>

#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(Scene);

    Scene::Scene(BaseObject* creator) : BaseObject(creator), network::Synchronisable(creator)
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

        // test test test
        if (Core::showsGraphics() && this->sceneManager_)
        {
            Ogre::Light* light;
            light = this->sceneManager_->createLight("Light0");
            light->setType(Ogre::Light::LT_DIRECTIONAL);
            light->setDiffuseColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setSpecularColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setDirection(1, -0.2, 0.2);
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
//                this->sceneManager_->destroySceneNode(this->rootSceneNode_->getName()); // TODO: remove getName() for newer versions of Ogre
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

        XMLPortObjectExtended(Scene, BaseObject, "", addObject, getObject, xmlelement, mode, true, false);
    }

    void Scene::registerVariables()
    {
        REGISTERSTRING(this->skybox_,     network::direction::toclient, new network::NetworkCallback<Scene>(this, &Scene::networkcallback_applySkybox));
        REGISTERDATA(this->ambientLight_, network::direction::toclient, new network::NetworkCallback<Scene>(this, &Scene::networkcallback_applyAmbientLight));
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
}
