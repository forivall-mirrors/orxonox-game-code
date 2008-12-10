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

#ifndef _CollisionShape_H__
#define _CollisionShape_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "core/BaseObject.h"
#include "network/Synchronisable.h"

namespace orxonox
{
    class _OrxonoxExport CollisionShape : public BaseObject, public network::Synchronisable
    {
        public:
            CollisionShape(BaseObject* creator);
            virtual ~CollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline void setPosition(const Vector3& position)
                { this->position_ = position; }
            inline const Vector3& getPosition() const
                { return this->position_; }

            inline void setOrientation(const Quaternion& orientation)
                { this->orientation_ = orientation; }
            inline const Quaternion& getOrientation() const
                { return this->orientation_; }

            void yaw(const Degree& angle)   { this->orientation_ = this->orientation_ * Quaternion(angle, Vector3::UNIT_Y); }
            void pitch(const Degree& angle) { this->orientation_ = this->orientation_ * Quaternion(angle, Vector3::UNIT_X); }
            void roll(const Degree& angle)  { this->orientation_ = this->orientation_ * Quaternion(angle, Vector3::UNIT_Z); }

            virtual void setScale3D(const Vector3& scale);
            virtual void setScale(float scale);
            inline const Vector3& getScale3D(void) const
                { return this->scale_; }

            virtual inline btCollisionShape* getCollisionShape() const
                { return this->collisionShape_; }

            bool hasTransform() const;

            inline void setParent(CompoundCollisionShape* shape, unsigned int ID)
                { this->parent_ = shape; this->parentID_ = ID; }

        protected:
            btCollisionShape* collisionShape_;

        private:
            void updateParent();

            Vector3           position_;
            Quaternion        orientation_;
            Vector3           scale_;
            CompoundCollisionShape* parent_;
            unsigned int      parentID_;
    };
}

#endif /* _CollisionShape_H__ */
