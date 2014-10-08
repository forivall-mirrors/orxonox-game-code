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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of ConfigFileSection.
*/

#include "ConfigFileSection.h"

#include "ConfigFileEntryValue.h"
#include "ConfigFileEntryVectorValue.h"

namespace orxonox
{
    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    /**
        @brief Destructor: Deletes all entries.
    */
    ConfigFileSection::~ConfigFileSection()
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
            delete (*(it++));
    }

    /**
        @brief Deletes all elements of a config vector if their index is greater or equal to @a startindex.

        @param name         The name of the vector
        @param startindex   The index of the first element that will be deleted
    */
    void ConfigFileSection::deleteVectorEntries(const std::string& name, unsigned int startindex)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() >= startindex))
            {
                delete (*it);
                this->entries_.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

    /**
        @brief Returns the size of a config vector.
        @param name     The name of the vector
    */
    unsigned int ConfigFileSection::getVectorSize(const std::string& name) const
    {
        unsigned int size = 0;
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if ((*it)->getName() == name)
                if ((*it)->getIndex() >= size)
                    size = (*it)->getIndex() + 1;
        return size;
    }

    /**
        @brief Returns the title and comment of the section as it will be stored in the file.
    */
    std::string ConfigFileSection::getFileEntry() const
    {
        if (this->additionalComment_.empty())
            return ('[' + this->name_ + ']');
        else
            return ('[' + this->name_ + "] " + this->additionalComment_);
    }

    /**
        @brief Returns the entry with given name (or NULL if it doesn't exist).

        @param name     The name of the entry
    */
    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if ((*it)->getName() == name)
                return *it;
        }
        return NULL;
    }

    /**
        @brief Returns the entry of a vector element with given name and index (or NULL if it doesn't exist).

        @param name     The name of the vector
        @param index    The index of the element in the vector
    */
    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name, unsigned int index) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
                return *it;
        }
        return NULL;
    }

    /**
        @brief Returns the iterator to the entry with given name. If the entry doesn't exist, it is created using the fallback value.

        @param name     The name of the entry
        @param fallback The value that will be used if the entry doesn't exist
        @param bString  If true, the value is treated as string which means some special treatment of special characters.
    */
    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getOrCreateEntryIterator(const std::string& name, const std::string& fallback, bool bString)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                (*it)->setString(bString);
                return it;
            }
        }

        this->bUpdated_ = true;

        return this->entries_.insert(this->entries_.end(), new ConfigFileEntryValue(name, fallback, bString));
    }

    /**
        @brief Returns the iterator to the entry of a vector element with given name and index. If the entry doesn't exist, it is created using the fallback value.

        @param name     The name of the vector
        @param index    The index of the element in the vector
        @param fallback The value that will be used if the entry doesn't exist
        @param bString  If true, the value is treated as string which means some special treatment of special characters.
    */
    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getOrCreateEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
            {
                (*it)->setString(bString);
                return it;
            }
        }

        this->bUpdated_ = true;

        if (index == 0)
            return this->entries_.insert(this->entries_.end(), new ConfigFileEntryVectorValue(name, index, fallback, bString));
        else
            return this->entries_.insert(++this->getOrCreateEntryIterator(name, index - 1, "", bString), new ConfigFileEntryVectorValue(name, index, fallback, bString));
    }
}
