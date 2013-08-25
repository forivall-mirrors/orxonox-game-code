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

#ifndef _ResourceLocation_H__
#define _ResourceLocation_H__

#include "tools/ToolsPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{
    class _ToolsExport ResourceLocation : public BaseObject
    {
        // for load/unload
        friend class ResourceCollection;

    public:
        ResourceLocation(Context* context);
        virtual ~ResourceLocation();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        void setPath(const std::string& path) { path_ = path; }
        const std::string& getPath() const { return path_; }

        void setArchiveType(const std::string& archiveType) { archiveType_ = archiveType; }
        const std::string& getArchiveType() const { return archiveType_; }

        void setRecursive(bool bRecursive) { bRecursive_ = bRecursive; }
        bool getRecursive() const { return bRecursive_; }

        const std::string& getResourceGroup() const { return resourceGroup_; }

    private:
        ResourceLocation(const ResourceLocation&);

        void load(const std::string& resourceGroup);
        void unload();

        std::string path_;
        std::string archiveType_;
        std::string resourceGroup_;
        bool bRecursive_;
    };
}

#endif /* _ResourceLocation_H__ */
