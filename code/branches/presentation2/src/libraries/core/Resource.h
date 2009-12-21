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

#ifndef _Core_Resource_H__
#define _Core_Resource_H__

#include "CorePrereqs.h"

#include <boost/shared_ptr.hpp>
#include <OgreDataStream.h>

namespace orxonox
{
    // Import the Ogre::DataStreamList
    using Ogre::DataStreamList;
    using Ogre::DataStreamListPtr;

    //! Stores basic information about a Resource from Ogre
    struct ResourceInfo
    {
        //! The file's fully qualified name
        std::string filename;
        //! Path name; separated by '/' and ending with '/'
        std::string path;
        //! Base filename
        std::string basename;
        //! Resource group the file is in
        std::string group;
        //! Uncompressed size
        size_t size;
    };

    /**
    @brief
        Provides simple functions to easily access the Ogre::ResourceGroupManager
    */
    class _CoreExport Resource
    {
        // Docs by Ogre::ResourceGroupManager.h
    public:
        /**
        @brief
            Open a single resource by name and return a DataStream
            pointing at the source of the data.
        @param name
            The name of the resource to locate.
            Even if resource locations are added recursively, you
            must provide a fully qualified name to this method.
        @param groupName
            The name of the resource group; this determines which
            locations are searched.
        @param searchGroupsIfNotFound
            If true, if the resource is not found in
            the group specified, other groups will be searched.
        @return
            Shared pointer to data stream containing the data. Will be
            destroyed automatically when no longer referenced.
        */
        static DataStreamPtr open(const std::string& name,
            const std::string& group = Resource::DEFAULT_GROUP,
            bool bSearchGroupsIfNotFound = false);

        //! Similar to open(string, string, bool), but with a fileInfo struct
        static DataStreamPtr open(shared_ptr<ResourceInfo> fileInfo,
            bool bSearchGroupsIfNotFound = false)
        {
            return open(fileInfo->filename, fileInfo->group, bSearchGroupsIfNotFound);
        }

        /**
        @brief
            Open all resources matching a given pattern (which can contain
            the character '*' as a wildcard), and return a collection of
            DataStream objects on them.
        @param pattern
            The pattern to look for. If resource locations have been
            added recursively, subdirectories will be searched too so this
            does not need to be fully qualified.
        @param groupName
            The resource group; this determines which locations
            are searched.
        @return
            Shared pointer to a data stream list , will be
            destroyed automatically when no longer referenced
        */
        static DataStreamListPtr openMulti(const std::string& pattern, const std::string& group = Resource::DEFAULT_GROUP);

        /**
            Find out if the named file exists in a group.
        @param filename
            Fully qualified name of the file to test for
        @param group
            The name of the resource group
        */
        static bool exists(const std::string& name, const std::string& group = Resource::DEFAULT_GROUP);

        /**
            Get struct with information about group, path and size.
        @param filename
            Fully qualified name of the file to test for
        @param group
            The name of the resource group
        */
        static shared_ptr<ResourceInfo> getInfo(const std::string& name, const std::string& group = Resource::DEFAULT_GROUP);

        //! Name of the default resource group (usually "General")
        static std::string DEFAULT_GROUP;

    private:
        Resource();
        ~Resource();
        Resource(const Resource& instance);
    };
}

#endif /* _Core_Resource_H__ */
