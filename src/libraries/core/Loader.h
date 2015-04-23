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
    @defgroup Loader Loader
    @ingroup XML
*/

/**
    @file
    @ingroup XML Loader
*/

#ifndef _Loader_H__
#define _Loader_H__

#include "CorePrereqs.h"

#include <map>
#include <vector>

#include "util/Singleton.h"
#include "ClassTreeMask.h"

namespace orxonox
{
    class _CoreExport Loader : public Singleton<Loader>
    {
        friend class Singleton<Loader>;

        public:
            bool open(const XMLFile* file, const ClassTreeMask& mask = ClassTreeMask(), bool bVerbose = true);
            void close();
            void close(const XMLFile* file);

            void add(const XMLFile* file, const ClassTreeMask& mask = ClassTreeMask());
            void remove(const XMLFile* file);

            bool load(const ClassTreeMask& mask = ClassTreeMask(), bool bVerbose = true);
            void unload(const ClassTreeMask& mask = ClassTreeMask());
            bool reload(const ClassTreeMask& mask = ClassTreeMask(), bool bVerbose = true);

            bool load(const XMLFile* file, const ClassTreeMask& mask = ClassTreeMask(),
                      bool bVerbose = true, bool bRemoveLuaTags = false);
            void unload(const XMLFile* file, const ClassTreeMask& mask = ClassTreeMask());
            bool reload(const XMLFile* file, const ClassTreeMask& mask = ClassTreeMask(), bool bVerbose = true);

            static std::string replaceLuaTags(const std::string& text);
            static std::string removeLuaTags(const std::string& text);

            ClassTreeMask currentMask_;

        private:
            static bool getLuaTags(const std::string& text, std::map<size_t, bool>& luaTags);

            std::vector<std::pair<const XMLFile*, ClassTreeMask> > files_;

            static Loader* singletonPtr_s;
    };
}

#endif /* _Loader_H__ */
