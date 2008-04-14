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

#ifndef _Loader_H__
#define _Loader_H__

#include <vector>

#include "ClassTreeMask.h"
#include "Level.h"

#include "CorePrereqs.h"

namespace orxonox
{
    class _CoreExport Loader
    {
        public:
            static bool open(const Level* level, const ClassTreeMask& mask = ClassTreeMask());
            static void close();
            static void close(const Level* level);

            static void add(const Level* level, const ClassTreeMask& mask = ClassTreeMask());
            static void remove(const Level* level);

            static bool load(const ClassTreeMask& mask = ClassTreeMask());
            static void unload(const ClassTreeMask& mask = ClassTreeMask());
            static bool reload(const ClassTreeMask& mask = ClassTreeMask());

            static bool load(const Level* level, const ClassTreeMask& mask = ClassTreeMask());
            static void unload(const Level* level, const ClassTreeMask& mask = ClassTreeMask());
            static bool reload(const Level* level, const ClassTreeMask& mask = ClassTreeMask());

            static ClassTreeMask currentMask_s;

        private:
            static std::vector<std::pair<const Level*, ClassTreeMask> > levels_s;
    };
}

#endif /* _Loader_H__ */
