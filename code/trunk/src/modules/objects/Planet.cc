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
 *      Marian Runo
 *   Co-authors:
 *      ...
 *
 */

#include "Planet.h"

#include <OgreEntity.h>
#include <OgreBillboardSet.h>
#include <OgreProgressiveMesh.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"
#include "graphics/Camera.h"
#include "CameraManager.h"

namespace orxonox
{
    CreateFactory(Planet);

    /**
     * @brief Constructor
     */
    Planet::Planet(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(Planet);
        this->registerVariables();
    }

    /**
     * @brief Destructor
     */
    Planet::~Planet()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->detachOgreObject(this->mesh_.getEntity());
    }

    void Planet::tick(float dt)
    {
        if (!this->isVisible())
            return;

        if (GameMode::showsGraphics())
        {
            Camera* activeCamera = CameraManager::getInstance().getActiveCamera();
            if (activeCamera && this->billboard_.getBillboardSet())
            {
                float distance = this->getPosition().distance( activeCamera->getWorldPosition() );
                //             orxout(internal_warning) << distance << endl;
                float planetRadius = this->getScale();

                float newScale = 2 * distance / sqrt(distance*distance - planetRadius*planetRadius);
                float tempTest = newScale*(1+float(this->atmosphereSize)/float(this->imageSize));
                newScale = tempTest;

                this->billboard_.getBillboardSet()->setDefaultDimensions(newScale, newScale);
            }
        }

        SUPER(Planet, tick, dt);
    }

    void Planet::init()
    {
    }

    void Planet::changedMesh()
    {
        if( GameMode::showsGraphics() )
        {
            if (this->mesh_.getEntity())
                this->detachOgreObject(this->mesh_.getEntity());

            this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

            if (this->mesh_.getEntity())
            {
                this->attachOgreObject(this->mesh_.getEntity());
                this->mesh_.getEntity()->setCastShadows(this->bCastShadows_);
                this->mesh_.setVisible(this->isVisible());

                float scaleFactor = this->getScale();

    #if OGRE_VERSION >= 0x010700
                Ogre::Mesh::LodValueList distList;
    #else
                Ogre::Mesh::LodDistanceList distList;
    #endif

                distList.push_back(10.0f*scaleFactor);
                distList.push_back(19.0f*scaleFactor);
                distList.push_back(27.0f*scaleFactor);
                distList.push_back(34.0f*scaleFactor);
                distList.push_back(40.0f*scaleFactor);
                distList.push_back(45.0f*scaleFactor);
                distList.push_back(49.0f*scaleFactor);
                distList.push_back(52.0f*scaleFactor);
                distList.push_back(54.0f*scaleFactor);
                distList.push_back(55.0f*scaleFactor);

                float reductionValue = 0.2f;

                this->mesh_.getEntity()->getMesh()->generateLodLevels(distList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, reductionValue);
                billboard_.setBillboardSet(this->getScene()->getSceneManager(), this->atmosphere_, Vector3(0,0,0));

                this->attachOgreObject(this->billboard_.getBillboardSet());
                this->billboard_.getBillboardSet()->setUseAccurateFacing(true);
                this->setCastShadows(true);
                this->billboard_.getBillboardSet()->setRenderQueueGroup(this->mesh_.getEntity()->getRenderQueueGroup());
                this->mesh_.setCastShadows(true);
            }
        }
    }

    void Planet::changedShadows()
    {
        this->mesh_.setCastShadows(this->bCastShadows_);
    }

    void Planet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Planet, XMLPort, xmlelement, mode);

        XMLPortParam(Planet, "atmosphere", setAtmosphere, getAtmosphere, xmlelement, mode).defaultValues("planet/Atmosphere");
        XMLPortParam(Planet, "atmospheresize", setAtmosphereSize, getAtmosphereSize, xmlelement,mode);
        XMLPortParam(Planet, "imagesize", setImageSize, getImageSize, xmlelement,mode);
        XMLPortParam(Planet, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
        XMLPortParam(Planet, "shadow", setCastShadows, getCastShadows, xmlelement, mode).defaultValues(true);
    }

    void Planet::registerVariables()
    {
        registerVariable(this->atmosphere_, VariableDirection::ToClient);
        registerVariable(this->atmosphereSize, VariableDirection::ToClient);
        registerVariable(this->imageSize, VariableDirection::ToClient);
        // Note: the meshSrc should be synchronised after atmosphere and other values, because the meshSrc callback setts the atmosphere billboards
        registerVariable(this->meshSrc_, VariableDirection::ToClient, new NetworkCallback<Planet>(this, &Planet::changedMesh));
        registerVariable(this->bCastShadows_, VariableDirection::ToClient, new NetworkCallback<Planet>(this, &Planet::changedShadows));
    }

    void Planet::changedVisibility()
    {
        SUPER(Planet, changedVisibility);
        if (this->isInitialized())
        {
            this->mesh_.setVisible(this->isVisible());
            this->billboard_.setVisible(this->isVisible());
        }
    }
}
