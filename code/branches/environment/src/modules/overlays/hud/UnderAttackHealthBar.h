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

#ifndef _UnderAttackHealthBar_H__
#define _UnderAttackHealthBar_H__

#include "overlays/OverlaysPrereqs.h"

#include "util/Math.h"
#include "tools/Timer.h"
#include "HUDHealthBar.h"

namespace orxonox
{
    class _OverlaysExport UnderAttackHealthBar : public HUDHealthBar
    {
        public:
            UnderAttackHealthBar(BaseObject* creator);
            ~UnderAttackHealthBar();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();
            virtual void changedOverlayGroup();

            inline void setDescriptionPickPoint(const Vector2& pickpoint)
                { this->text_->setPickPoint(pickpoint); }
            inline Vector2 getDescriptionPickPoint() const
                { return this->text_->getPickPoint(); }

            inline void setDescriptionOffset(const Vector2& position)
                { this->text_->setPosition(this->getPosition() + (position - this->getPickPoint()) * this->getSize()); }
            inline Vector2 getDescriptionOffset() const
                { return (this->text_->getPosition() - this->getPosition()) / this->getSize() + this->getPickPoint(); }

        private:
            void init();

            PlayerInfo* owner_;
            SmartPtr<OverlayText> text_;
            Timer inittimer_;
    };
}
#endif /* _UnderAttackHealthBar_H__ */
