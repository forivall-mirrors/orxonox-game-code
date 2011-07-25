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

#include <OgreException.h>
#include <OgreResourceGroupManager.h>

namespace orxonox
{
    std::string Resource::DEFAULT_GROUP(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    DataStreamPtr Resource::open(const std::string& name)
    {
        return Ogre::ResourceGroupManager::getSingleton().openResource(name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    }

    DataStreamListPtr Resource::openMulti(const std::string& pattern)
    {
        DataStreamListPtr resources(new Ogre::DataStreamList());
        const Ogre::StringVector& groups = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        for (Ogre::StringVector::const_iterator it = groups.begin(); it != groups.end(); ++it)
        {
            DataStreamListPtr temp = Ogre::ResourceGroupManager::getSingleton().openResources(pattern, *it);
            resources->insert(resources->end(), temp->begin(), temp->end());
        }
        return resources;
    }

    bool Resource::exists(const std::string& name)
    {
        try
        {
            Ogre::ResourceGroupManager::getSingleton().findGroupContainingResource(name);
            return true;
        }
        catch (const Ogre::Exception&)
        {
            return false;
        }
    }

    shared_ptr<ResourceInfo> Resource::getInfo(const std::string& name)
    {
        std::string group;
        try
        {
            group = Ogre::ResourceGroupManager::getSingleton().findGroupContainingResource(name);
        }
        catch (const Ogre::Exception&)
        {
            return shared_ptr<ResourceInfo>();
        }
        Ogre::FileInfoListPtr infos = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(group, name);
        for (Ogre::FileInfoList::const_iterator it = infos->begin(); it != infos->end(); ++it)
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

    StringVectorPtr Resource::findResourceNames(const std::string& pattern)
    {
        StringVectorPtr resourceNames(new Ogre::StringVector());
        const Ogre::StringVector& groups = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        for (Ogre::StringVector::const_iterator it = groups.begin(); it != groups.end(); ++it)
        {
            StringVectorPtr temp = Ogre::ResourceGroupManager::getSingleton().findResourceNames(*it, pattern);
            resourceNames->insert(resourceNames->end(), temp->begin(), temp->end());
        }
        return resourceNames;
    }
}
