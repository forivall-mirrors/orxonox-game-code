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

#ifndef _ResourceCollection_H__
#define _ResourceCollection_H__

#include "tools/ToolsPrereqs.h"

#include <string>
#include <vector>
#include "core/BaseObject.h"

namespace orxonox
{
    class _ToolsExport ResourceCollection : public BaseObject
    {
    public:
        ResourceCollection(Context* context);
        virtual ~ResourceCollection();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        void setResourceGroup(const std::string& resourceGroup);
        const std::string& getResourceGroup() const { return resourceGroup_; }

        void addResourceLocation(ResourceLocation* location);
        void removeResourceLocation(ResourceLocation* location);
        ResourceLocation* getResourceLocation(unsigned int index) const;

    private:
        ResourceCollection(const ResourceCollection&);

        std::string resourceGroup_;
        std::vector<ResourceLocation*> resourceLocations_;
    };
}

#endif /* _ResourceCollection_H__ */
