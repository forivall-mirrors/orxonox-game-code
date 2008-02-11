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

#ifndef _Level_H__
#define _Level_H__

#include <string>
#include <vector>

#include "CorePrereqs.h"

namespace orxonox
{
    class _CoreExport Level
    {
        public:
            Level(const std::string& file);
            Level(const std::string& file, const Identifier* include);
            Level(const std::string& file, const std::vector<const Identifier*>& includes);
            Level(const std::string& file, const Identifier* include, const Identifier* exclude);
            Level(const std::string& file, const std::vector<const Identifier*>& includes, const Identifier* exclude);
            Level(const std::string& file, const Identifier* include, std::vector<const Identifier*> excludes);
            Level(const std::string& file, const std::vector<const Identifier*>& includes, std::vector<const Identifier*> excludes);

            void setFile(const std::string& file);
            void setIncludes(const Identifier* include);
            void setIncludes(const std::vector<const Identifier*>& includes);
            void setExcludes(const Identifier* exclude);
            void setExcludes(const std::vector<const Identifier*>& excludes);

            void addIncludes(const Identifier* include);
            void addIncludes(const std::vector<const Identifier*>& includes);
            void addExcludes(const Identifier* exclude);
            void addExcludes(const std::vector<const Identifier*>& excludes);

            void removeIncludes(const Identifier* include);
            void removeIncludes(const std::vector<const Identifier*>& includes);
            void removeExcludes(const Identifier* exclude);
            void removeExcludes(const std::vector<const Identifier*>& excludes);

            inline const std::string& getFile() const { return this->file_; }
            inline const std::vector<const Identifier*>& getIncludes() const { return this->includes_; }
            inline const std::vector<const Identifier*>& getExcludes() const { return this->excludes_; }

        private:
            std::string file_;
            std::vector<const Identifier*> includes_;
            std::vector<const Identifier*> excludes_;
    };
}

#endif
