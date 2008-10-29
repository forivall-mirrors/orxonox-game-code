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


#include "OrxonoxStableHeaders.h"
#include "Planet.h"

#include "tinyxml/tinyxml.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreMeshManager.h>


namespace orxonox
{
    CreateFactory(Planet);

    /**
     * @brief Constructor
     */
    Planet::Planet()
    {
        RegisterObject(Planet);
        registerAllVariables();
    }

    /**
     * @brief Destructor
     */
    Planet::~Planet()
    {
        if (this->isInitialized() && (this->meshSrc_ != "") && (this->meshSrc_.size() > 0))
            this->detachObject(this->mesh_.getEntity());
    }
    
    void Planet::init(){
        //std::vector<Real> newList;
        //this->distList = &newList;
        
        //this->distList.push_back(3000.0);
        //this->distList.push_back(6000.0);
        //this->distList.push_back(48000.0);
        this->distList.push_back(100000.0);
        this->distList.push_back(125000.0);
        this->distList.push_back(150000.0);
        this->distList.push_back(175000.0);
        this->distList.push_back(200000.0);
        
        Real reductionValue = 0.1;
        
        this->mesh_.getEntity()->getMesh()->generateLodLevels(distList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, reductionValue);
        //billboard_.setBillboardSet("Orxonox/Atmosphere", this->getPosition(), 1);
        //this->getNode()->attachObject(this->billboard_.getBillboardSet());
        //this->billboard_.getBillboardSet()->setDefaultDimensions(Real(30000), Real(30000));

    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Planet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Planet, XMLPort, xmlelement, mode);

        XMLPortParam(Planet, "mesh", setMesh, getMesh, xmlelement, mode);

        Planet::create();
    }

    bool Planet::create(){
      if(!WorldEntity::create())
        return false;
      if ((this->meshSrc_ != "") && (this->meshSrc_.size() > 0))
      {
        this->mesh_.setMesh(meshSrc_);
        this->attachObject(this->mesh_.getEntity());

        COUT(2) << "Loader (Planet.cc): Created model" << std::endl;
      }
      Planet::init();
      if(this->isExactlyA(Class(Planet)))
        setObjectFrequency(300); //sync all 10 seconds (this only applies to asteroids and other isExactlyA(Model)'s
      return true;
    }

    void Planet::registerAllVariables(){
//       WorldEntity::registerAllVariables();
      COUT(5) << "Planet.cc:registering new meshsrc with size: " << meshSrc_.length()+1 << " this: " << this << std::endl;
      registerVar(&meshSrc_, meshSrc_.length() + 1, network::STRING);
    }

    void Planet::changedVisibility()
    {
        SUPER(Planet, changedVisibility);
        if (this->isInitialized())
            this->mesh_.setVisible(this->isVisible());
            this->billboard_.setVisible(this->isVisible());
    }
}
