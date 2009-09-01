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

#include "Resource.h"
#include <OgreResourceGroupManager.h>

namespace orxonox
{
    std::string Resource::DEFAULT_GROUP(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    DataStreamPtr Resource::open(const std::string& name, const std::string& group, bool bSearchGroupsIfNotFound)
    {
        return Ogre::ResourceGroupManager::getSingleton().openResource(name, group, bSearchGroupsIfNotFound);
    }

    DataStreamListPtr Resource::openMulti(const std::string& pattern, const std::string& group)
    {
        return Ogre::ResourceGroupManager::getSingleton().openResources(pattern, group);
    }

    bool Resource::exists(const std::string& name, const std::string& group)
    {
        return Ogre::ResourceGroupManager::getSingleton().resourceExists(group, name);
    }

    shared_ptr<ResourceInfo> Resource::getInfo(const std::string& name, const std::string& group)
    {
        Ogre::FileInfoListPtr infos = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(group, name);
        for (std::vector<Ogre::FileInfo>::const_iterator it = infos->begin(); it != infos->end(); ++it)
        {
            if (it->filename == name)
            {
                shared_ptr<ResourceInfo> ptr(new ResourceInfo());
                ptr->filename = name;
                ptr->path = it->path;
                ptr->basename = it->basename;
                ptr->group = group;
                ptr->size = it->uncompressedSize;
                return ptr;
            }
        }
        return shared_ptr<ResourceInfo>();
    }
}
