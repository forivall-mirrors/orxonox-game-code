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

#ifndef _MemoryArchive_H__
#define _MemoryArchive_H__

#include "CorePrereqs.h"
#include <OgrePrerequisites.h>

#if OGRE_VERSION < 0x010600

#include <map>
#include <OgreArchive.h>
#include <OgreArchiveFactory.h>
#include <boost/shared_array.hpp>

namespace orxonox
{
    /**
    @brief
        Provides a simple mechanism of 'memory mapped IO', but compatible for Ogre 1.4

        Files are stored as shared_array<char> in a static map with the archive names
        (imaging the different archives as resource locations). This map then contains
        yet another map with the actual files.
        If you want to know why we make such a fuss, contact GraphicsManager::upgradeToGraphics()
        for more information.
    */
    class MemoryArchive : public Ogre::Archive
    {
    public:
        MemoryArchive(const Ogre::String& name, const Ogre::String& archiveType) : Ogre::Archive(name, archiveType) { }
        ~MemoryArchive() { }

        bool isCaseSensitive(void) const { return true; }

        void load();
        void unload() { }

        Ogre::DataStreamPtr open(const Ogre::String& filename) const;

        Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);
        Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

        Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);
        Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);

        bool exists(const Ogre::String& filename);

        static void addFile(const std::string& archiveName, const std::string& filename, shared_array<char> content, size_t size)
            { archives_s[archiveName][filename] = std::make_pair(content, size); }

    private:
        void findFiles(const Ogre::String& pattern, bool bRecursive,
            bool bDirs, Ogre::StringVector* simpleList, Ogre::FileInfoList* detailList);

        typedef std::map<std::string, std::pair<shared_array<char>, size_t> > FileMap;
        typedef std::map<std::string, FileMap> ArchiveMap;
        static ArchiveMap archives_s;
    };

    //! Specialisation of ArchiveFactory for MemoryArchive.
    class _CoreExport MemoryArchiveFactory : public Ogre::ArchiveFactory
    {
    public:
        const Ogre::String& getType(void) const;
        Ogre::Archive* createInstance(const Ogre::String& name) { return new MemoryArchive(name, "Memory"); }
        void destroyInstance(Ogre::Archive* arch)               { delete arch; }
    };
}

#endif /* OGRE_VERSION < 0x010600 */

#endif /* _MemoryArchive_H__ */
