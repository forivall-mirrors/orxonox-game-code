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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

#ifndef _JumpProjectile_H__
#define _JumpProjectile_H__

#include "jump/JumpPrereqs.h"
#include "util/Math.h"
#include "worldentities/MovableEntity.h"


namespace orxonox
{
    class _JumpExport JumpProjectile : public MovableEntity
    {
        public:
            JumpProjectile(Context* context);
            virtual ~JumpProjectile();

            virtual void tick(float dt);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }

            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }

            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }

            void setFigure(WeakPtr<JumpFigure> figure);

        protected:
            float fieldWidth_;
            float fieldHeight_;
            WeakPtr<JumpFigure> figure_;
    };
}

#endif /* _JumpProjectile_H__ */
