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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "tinyxml/tinyxml.h"
#include "misc/Tokenizer.h"
#include "misc/String2Number.h"
#include "../core/CoreIncludes.h"
#include "../Orxonox.h"

#include "Model.h"

namespace orxonox
{
    CreateFactory(Model);

    Model::Model()
    {
        RegisterObject(Model);
    }

    Model::~Model()
    {
    }

    void Model::loadParams(TiXmlElement* xmlElem)
    {
        WorldEntity::loadParams(xmlElem);

        if (xmlElem->Attribute("mesh"))
        {
            meshSrc_ = xmlElem->Attribute("mesh");
        }
        create();
    }

    bool Model::create(){
      if(meshSrc_.compare("")!=0){
        this->mesh_.setMesh(meshSrc_);
        this->attachObject(this->mesh_.getEntity());
        COUT(4) << "Loader: Created model" << std::endl;
      }
      registerAllVariables();
      return true;
    }

    void Model::registerAllVariables(){
      registerVar(&meshSrc_, meshSrc_.length()+1, network::STRING);
    }
}
