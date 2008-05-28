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
#include "Model.h"

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
    CreateFactory(Model);

    Model::Model()
    {
        RegisterObject(Model);
        registerAllVariables();
    }

    Model::~Model()
    {
    }

    /**
        @brief XML loading and saving.
    @p
    aram xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Model::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        WorldEntity::XMLPort(xmlelement, mode);

        XMLPortParamLoadOnly(Model, "mesh", setMesh, xmlelement, mode);

        Model::create();
    }

    void Model::setMesh(const std::string& meshname)
    {
        this->meshSrc_ = meshname;
    }

    bool Model::create(){
      if(!WorldEntity::create())
        return false;
      if ((this->meshSrc_ != "") && (this->meshSrc_.size() > 0))
      {
        this->mesh_.setMesh(meshSrc_);
        this->attachObject(this->mesh_.getEntity());

        //HACK!!
        if ((this->meshSrc_ == "assff.mesh") || (this->meshSrc_ == "ast1.mesh") || (this->meshSrc_ == "ast2.mesh") || (this->meshSrc_ == "ast3.mesh") || (this->meshSrc_ == "ast4.mesh") ||(this->meshSrc_ == "ast5.mesh") || (this->meshSrc_ == "ast6.mesh"))
        {
          Ogre::MeshPtr pMesh = this->mesh_.getEntity()->getMesh();
          //set Mesh to tangentspace 
          unsigned short src, dest;
          if (!pMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
          {
            pMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
          }
          if ((this->meshSrc_ == "assff.mesh"))
          {
            (this->mesh_.getEntity())->setMaterialName("Assff/BumpMap");
          }
          else
          {
            (this->mesh_.getEntity())->setMaterialName("Asteroid/BumpMap");
          }


        }
        COUT(4) << "Loader (Model.cc): Created model" << std::endl;
      }
      return true;
    }

    void Model::registerAllVariables(){
//       WorldEntity::registerAllVariables();
      COUT(5) << "Model.cc:registering new meshsrc with size: " << meshSrc_.length()+1 << " this: " << this << std::endl;
      registerVar(&meshSrc_, meshSrc_.length() + 1, network::STRING);
    }
}
