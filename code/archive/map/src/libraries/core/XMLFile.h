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

#ifndef _XMLFile_H__
#define _XMLFile_H__

#include "CorePrereqs.h"

#include <string>
#include "ClassTreeMask.h"

namespace orxonox
{
    class _CoreExport XMLFile
    {
        public:
            XMLFile(const std::string& filename)
                : filename_(filename)
                , bLuaSupport_(true)
            { }
            XMLFile(const ClassTreeMask& mask, const std::string& filename)
                : filename_(filename)
                , mask_(mask)
                , bLuaSupport_(true)
            { }

            void setLuaSupport(bool val) { bLuaSupport_ = val; }

            const std::string& getFilename() const { return this->filename_; }
            const ClassTreeMask& getMask() const { return this->mask_; }
            bool getLuaSupport() const { return this->bLuaSupport_; }

        private:
            std::string filename_;
            ClassTreeMask mask_;
            bool bLuaSupport_; // Default is true
    };
}

#endif /* _XMLFile_H__ */
