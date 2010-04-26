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
 *      Jan Bernegger
 *   Co-authors:
 *      ...
 *
 */

#include "MeshLodInformation.h"

#include <OgreEntity.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(MeshLodInformation);

    MeshLodInformation::MeshLodInformation(BaseObject* creator) 
	: BaseObject(creator), lodLevel_(-1)
    {
        RegisterObject(MeshLodInformation);
    }

    MeshLodInformation::~MeshLodInformation()
    {    }

    void MeshLodInformation::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MeshLodInformation, XMLPort, xmlelement, mode);

        XMLPortParam(MeshLodInformation, "lodQuality", setLodLevel, getLodLevel, xmlelement, mode).defaultValues(5);
        XMLPortParam(MeshLodInformation, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
    }
    
    std::string MeshLodInformation::getMeshName()
    {
		return MeshLodInformation::getMeshSource();
    }
	
	void MeshLodInformation::setLodLevel(unsigned int lodLevel)
	{
		lodLevel_=lodLevel;
	}
	int MeshLodInformation::getLodLevel()
	{
		return lodLevel_;
	} 
	void MeshLodInformation::setMeshSource(std::string meshSource)
	{
		meshSource_ = meshSource;
	}
	std::string MeshLodInformation::getMeshSource()
	{
		return meshSource_;
	}
 
}