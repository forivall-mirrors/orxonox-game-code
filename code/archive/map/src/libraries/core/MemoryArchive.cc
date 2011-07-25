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

#include "MemoryArchive.h"

#if OGRE_VERSION < 0x010600

#include <OgreException.h>
#include <boost/filesystem.hpp>
#include <boost/version.hpp>

// Boost 1.36 has some issues with deprecated functions that have been omitted
#if (BOOST_VERSION == 103600)
#  define BOOST_HAS_BRANCH_PATH_FUNCTION has_parent_path
#else
#  define BOOST_HAS_BRANCH_PATH_FUNCTION has_branch_path
#endif

namespace orxonox
{
    using namespace Ogre;

    MemoryArchive::ArchiveMap MemoryArchive::archives_s;

    void MemoryArchive::load()
    {
        if (archives_s.find(this->getName()) == archives_s.end())
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, this->getName() + " - MemoryArchive not found.", "MemoryArchive");
    }

    DataStreamPtr MemoryArchive::open(const String& filename) const
    {
        const FileMap& files = archives_s[this->getName()];
        FileMap::const_iterator itFile = files.find(filename);
        if (itFile == files.end())
            return DataStreamPtr();
        else
            return MemoryDataStreamPtr(new MemoryDataStream(itFile->second.first.get(), itFile->second.second));
    }

    void MemoryArchive::findFiles(const String& pattern, bool bRecursive,
        bool bDirs, StringVector* simpleList, FileInfoList* detailList)
    {
        const FileMap& files = archives_s[this->getName()];

        for (FileMap::const_iterator it = files.begin(); it != files.end(); ++it)
        {
            boost::filesystem::path file = it->first;
            // Check pattern
            if (!StringUtil::match(file.string(), pattern, true))
                continue;
            if (bDirs)
                file = file.branch_path();
            if (file.empty())
                continue;
            if (file.BOOST_HAS_BRANCH_PATH_FUNCTION() && !bRecursive)
                continue;
            if (simpleList)
                simpleList->push_back(file.string());
            if (detailList)
            {
                FileInfo fi;
                fi.archive = this;
                fi.filename = file.string();
                fi.basename = file.leaf();
                fi.path = file.branch_path().string();
                fi.compressedSize = it->second.second;
                fi.uncompressedSize = it->second.second;
                detailList->push_back(fi);
            }
        }
    }
    StringVectorPtr MemoryArchive::list(bool recursive, bool dirs)
    {
        StringVectorPtr ret(new StringVector());
        findFiles("*", recursive, dirs, ret.getPointer(), 0);
        return ret;
    }

    FileInfoListPtr MemoryArchive::listFileInfo(bool recursive, bool dirs)
    {
        FileInfoListPtr ret(new FileInfoList());
        findFiles("*", recursive, dirs, 0, ret.getPointer());
        return ret;
    }

    StringVectorPtr MemoryArchive::find(const String& pattern,
                                            bool recursive, bool dirs)
    {
        StringVectorPtr ret(new StringVector());
        findFiles(pattern, recursive, dirs, ret.getPointer(), 0);
        return ret;
    }

    FileInfoListPtr MemoryArchive::findFileInfo(const String& pattern,
        bool recursive, bool dirs)
    {
        FileInfoListPtr ret(new FileInfoList());
        findFiles(pattern, recursive, dirs, 0, ret.getPointer());
        return ret;
    }

    bool MemoryArchive::exists(const String& filename)
    {
        const FileMap& files = archives_s[this->getName()];
        return files.find(filename) != files.end();
    }

    const Ogre::String& MemoryArchiveFactory::getType(void) const
    {
        static std::string result("Memory");
        return result;
    }
}

#endif /* OGRE_VERSION < 0x010600 */
