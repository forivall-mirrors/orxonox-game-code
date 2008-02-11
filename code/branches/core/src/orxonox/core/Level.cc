/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "Level.h"
#include "Identifier.h"
#include "BaseObject.h"

namespace orxonox
{
    Level::Level(const std::string& file)
    {
        this->setFile(file);
        this->setIncludes(ClassIdentifier<BaseObject>::getIdentifier());
        this->setExcludes(0);
    }

    Level::Level(const std::string& file, const Identifier* include)
    {
        this->setFile(file);
        this->setIncludes(include);
        this->setExcludes(0);
    }

    Level::Level(const std::string& file, const std::vector<const Identifier*>& includes)
    {
        this->setFile(file);
        this->setIncludes(includes);
        this->setExcludes(0);
    }

    Level::Level(const std::string& file, const Identifier* include, const Identifier* exclude)
    {
        this->setFile(file);
        this->setIncludes(include);
        this->setExcludes(exclude);
    }

    Level::Level(const std::string& file, const std::vector<const Identifier*>& includes, const Identifier* exclude)
    {
        this->setFile(file);
        this->setIncludes(includes);
        this->setExcludes(exclude);
    }

    Level::Level(const std::string& file, const Identifier* include, std::vector<const Identifier*> excludes)
    {
        this->setFile(file);
        this->setIncludes(include);
        this->setExcludes(excludes);
    }

    Level::Level(const std::string& file, const std::vector<const Identifier*>& includes, std::vector<const Identifier*> excludes)
    {
        this->setFile(file);
        this->setIncludes(includes);
        this->setExcludes(excludes);
    }



    void Level::setFile(const std::string& file)
    {
        this->file_ = file;
    }

    void Level::setIncludes(const Identifier* include)
    {
        this->includes_ = std::vector<const Identifier*>(1, include);
    }

    void Level::setIncludes(const std::vector<const Identifier*>& includes)
    {
        this->includes_ = includes;

        for (std::vector<const Identifier*>::iterator it1 = this->includes_.begin(); it1 < this->includes_.end(); ++it1)
            for (std::vector<const Identifier*>::iterator it2 = ++it1; it2 < this->includes_.end(); ++it2)
                if ((*it1) == (*it2))
                    this->includes_.erase(it2++);
    }

    void Level::setExcludes(const Identifier* exclude)
    {
        this->excludes_ = std::vector<const Identifier*>(1, exclude);
    }

    void Level::setExcludes(const std::vector<const Identifier*>& excludes)
    {
        this->excludes_ = excludes;

        for (std::vector<const Identifier*>::iterator it1 = this->excludes_.begin(); it1 < this->excludes_.end(); ++it1)
            for (std::vector<const Identifier*>::iterator it2 = ++it1; it2 < this->excludes_.end(); ++it2)
                if ((*it1) == (*it2))
                    this->excludes_.erase(it2++);
    }



    void Level::addIncludes(const Identifier* include)
    {
        for (std::vector<const Identifier*>::const_iterator it = this->includes_.begin(); it < this->includes_.end(); ++it)
            if ((*it) == include)
                return;

        this->includes_.insert(this->includes_.end(), include);
    }

    void Level::addIncludes(const std::vector<const Identifier*>& includes)
    {
        for (std::vector<const Identifier*>::const_iterator it = includes.begin(); it < includes.end(); ++it)
            this->addIncludes(*it);
    }

    void Level::addExcludes(const Identifier* exclude)
    {
        for (std::vector<const Identifier*>::const_iterator it = this->excludes_.begin(); it < this->excludes_.end(); ++it)
            if ((*it) == exclude)
                return;

        this->excludes_.insert(this->excludes_.end(), exclude);
    }

    void Level::addExcludes(const std::vector<const Identifier*>& excludes)
    {
        for (std::vector<const Identifier*>::const_iterator it = excludes.begin(); it < excludes.end(); ++it)
            this->addExcludes(*it);
    }



    void Level::removeIncludes(const Identifier* include)
    {
        for (std::vector<const Identifier*>::iterator it = this->includes_.begin(); it < this->includes_.end(); ++it)
            if ((*it) == include)
                this->includes_.erase(it);
    }

    void Level::removeIncludes(const std::vector<const Identifier*>& includes)
    {
        for (std::vector<const Identifier*>::const_iterator it = includes.begin(); it < includes.end(); ++it)
            this->removeIncludes(*it);
    }

    void Level::removeExcludes(const Identifier* exclude)
    {
        for (std::vector<const Identifier*>::iterator it = this->excludes_.begin(); it < this->excludes_.end(); ++it)
            if ((*it) == exclude)
                this->excludes_.erase(it);
    }

    void Level::removeExcludes(const std::vector<const Identifier*>& excludes)
    {
        for (std::vector<const Identifier*>::const_iterator it = excludes.begin(); it < excludes.end(); ++it)
            this->removeExcludes(*it);
    }
}
