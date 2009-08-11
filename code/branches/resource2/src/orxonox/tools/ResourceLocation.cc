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

#include "ResourceLocation.h"

#include <OgreResourceGroupManager.h>
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    ResourceLocation::ResourceLocation(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(ResourceLocation);

        // Default values
        archiveType_ = "FileSystem";
        bRecursive_  = false;
    }

    ResourceLocation::~ResourceLocation()
    {
    }

    void ResourceLocation::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        XMLPortParam(ResourceLocation, "path",        setPath,        getPath,        xmlElement, mode);
        XMLPortParam(ResourceLocation, "archiveType", setArchiveType, getArchiveType, xmlElement, mode);
        XMLPortParam(ResourceLocation, "recursive",   setRecursive,   getRecursive,   xmlElement, mode);
        if (path_.empty())
            ThrowException(AbortLoading, "ResourceLocation: No path given.");
    }

    void ResourceLocation::load(const std::string& resourceGroup)
    {
        if (path_.empty())
            ThrowException(InitialisationFailed, "ResourceLocation: Trying to add one without the path being set");
        // Add it to the Ogre paths
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            this->getPath(), this->getArchiveType(), resourceGroup, this->getRecursive());
        resourceGroup_ = resourceGroup;
    }

    void ResourceLocation::unload()
    {
        // Remove from Ogre paths
        resourceGroup_.erase();
        try
        {
            Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(
                this->getPath(), this->getResourceGroup());
        }
        catch (const std::exception& ex)
        {
            COUT(1) << "Removing of a ResourceLocation failed: " << ex.what() << std::endl;
        }
    }
}
