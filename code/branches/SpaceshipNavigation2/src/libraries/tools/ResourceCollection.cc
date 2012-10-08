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

#include "ResourceCollection.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/Resource.h"
#include "core/XMLPort.h"
#include "ResourceLocation.h"

namespace orxonox
{
    CreateFactory(ResourceCollection);

    ResourceCollection::ResourceCollection(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(ResourceCollection);

        // Default group is "General"
        this->setResourceGroup(Resource::getDefaultResourceGroup());
    }

    ResourceCollection::~ResourceCollection()
    {
    }

    void ResourceCollection::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(ResourceCollection, "resourceGroup", setResourceGroup, getResourceGroup, xmlelement, mode);
        XMLPortObject(ResourceCollection, ResourceLocation, "", addResourceLocation, getResourceLocation, xmlelement, mode);
    }

    void ResourceCollection::setResourceGroup(const std::string& resourceGroup)
    {
        if (resourceGroup.empty())
        {
            orxout(internal_warning) << "\"\" is not a valid resource group." << endl;
            return;
        }
        if (!resourceLocations_.empty())
            ThrowException(InitialisationFailed, "ResourceGroup: Group change not allowed with locations already added!");
        resourceGroup_ = resourceGroup;
    }

    void ResourceCollection::addResourceLocation(ResourceLocation* location)
    {
        location->load(resourceGroup_);
        this->resourceLocations_.push_back(location);
    }

    void ResourceCollection::removeResourceLocation(ResourceLocation* location)
    {
        for (std::vector<ResourceLocation*>::iterator it = resourceLocations_.begin(); it != resourceLocations_.end(); )
        {
            if (*it == location)
            {
                it = resourceLocations_.erase(it);
                location->unload(); // doesn't throw
            }
            else
                ++it;
        }
    }

    ResourceLocation* ResourceCollection::getResourceLocation(unsigned int index) const
    {
        if (index >= resourceLocations_.size())
            return NULL;
        else
            return resourceLocations_[index];
    }
}
