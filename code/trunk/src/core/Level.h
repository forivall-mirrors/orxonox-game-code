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

#include "CorePrereqs.h"
#include "ClassTreeMask.h"

namespace orxonox
{
    class _CoreExport Level
    {
        public:
            Level(const std::string& file) : file_(file) {}
            Level(const std::string& file, const ClassTreeMask& mask) : file_(file), mask_(mask) {};

            const std::string& getFile() const { return this->file_; }
            const ClassTreeMask& getMask() const { return this->mask_; }

        private:
            std::string file_;
            ClassTreeMask mask_;
    };
}

#endif /* _Level_H__ */
