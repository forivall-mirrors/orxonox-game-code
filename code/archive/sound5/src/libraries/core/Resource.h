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
#include <OgreStringVector.h>

namespace orxonox
{
    // Import the Ogre::DataStreamList
    using Ogre::DataStreamList;
    using Ogre::DataStreamListPtr;
    using Ogre::StringVector;
    using Ogre::StringVectorPtr;

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

    /** Provides simple functions to easily access the Ogre::ResourceGroupManager.
        The wrapper functions also avoid having to deal with resource groups.
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
        @return
            Shared pointer to data stream containing the data. Will be
            destroyed automatically when no longer referenced.
        */
        static DataStreamPtr open(const std::string& name);

        //! Similar to open(string, string, bool), but with a fileInfo struct
        static DataStreamPtr open(shared_ptr<ResourceInfo> fileInfo)
        {
            return open(fileInfo->filename);
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
        @return
            Shared pointer to a data stream list , will be
            destroyed automatically when no longer referenced
        */
        static DataStreamListPtr openMulti(const std::string& pattern);

        /**
            Find out if the named file exists.
        @param filename
            Fully qualified name of the file to test for
        */
        static bool exists(const std::string& name);

        /**
            Get struct with information about path and size.
        @param filename
            Fully qualified name of the file to test for
        */
        static shared_ptr<ResourceInfo> getInfo(const std::string& name);

        /**
            Retrieves a list with all resources matching a certain pattern.
        @param pattern
            The pattern to look for. If resource locations have been
            added recursively, subdirectories will be searched too so this
            does not need to be fully qualified.
        */
        static StringVectorPtr findResourceNames(const std::string& pattern);

        //! Name of the default resource group (usually "General")
        static std::string DEFAULT_GROUP;

    private:
        Resource();
        ~Resource();
        Resource(const Resource& instance);
    };
}

#endif /* _Core_Resource_H__ */
