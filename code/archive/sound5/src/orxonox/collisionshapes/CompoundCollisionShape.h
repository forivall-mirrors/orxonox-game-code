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

#include <cassert>
#include <map>
#include "CollisionShape.h"

namespace orxonox
{
    class _OrxonoxExport CompoundCollisionShape : public CollisionShape
    {
        public:
            CompoundCollisionShape(BaseObject* creator);
            virtual ~CompoundCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void attach(CollisionShape* shape);
            void detach(CollisionShape* shape);
            void detachAll();
            CollisionShape* getAttachedShape(unsigned int index) const;

            void updateAttachedShape(CollisionShape* shape);

        private:
            void updatePublicShape();
            inline virtual btCollisionShape* createNewShape() const
                { assert(false); return 0; }

            btCompoundShape* compoundShape_;
            std::map<CollisionShape*, btCollisionShape*> attachedShapes_;
    };
}

#endif /* _CompoundCollisionShape_H__ */
