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

#ifndef _CompoundCollisionShape_H__
#define _CompoundCollisionShape_H__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "CollisionShape.h"

namespace orxonox
{
    class _OrxonoxExport CompoundCollisionShape : public CollisionShape
    {
        public:
            CompoundCollisionShape(BaseObject* creator);
            virtual ~CompoundCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void addChildShape(CollisionShape* shape);
            CollisionShape* getChildShape(unsigned int index) const;

            virtual btCollisionShape* getCollisionShape() const;

            inline bool empty() const
                { return this->childShapes_.size() == 0; }

        private:
            btCompoundShape*             compoundShape_;
            std::vector<CollisionShape*> childShapes_;
    };
}

#endif /* _CompoundCollisionShape_H__ */
