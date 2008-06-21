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

#ifndef _OverlayGroup_H__
#define _OverlayGroup_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <OgrePrerequisites.h>
#include "core/BaseObject.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport OverlayGroup : public BaseObject
    {
    public:
        OverlayGroup();
        ~OverlayGroup();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        static void toggleVisibility(const std::string& name);
        static void scaleGroup(const std::string& name, float scale);
        static void scrollGroup(const std::string& name, const Vector2& scroll);
        static void rotateGroup(const std::string& name, Radian angle);

    private:
        void scale(const Vector2& scale) { this->setScale(scale * this->scale_); }
        void setScale(const Vector2& scale);
        Vector2 getScale() const { return this->scale_; }

        void scroll(const Vector2& scroll) { this->setScroll(scroll + this->scroll_); }
        void setScroll(const Vector2& scroll);
        Vector2 getScroll() const { return this->scale_; }

        void addElement(OrxonoxOverlay* element);
        OrxonoxOverlay* getElement(unsigned int index);

        std::map<std::string, OrxonoxOverlay*> hudElements_;
        Vector2 scale_;
        Vector2 scroll_;
    };
}

#endif /* _OverlayGroup_H__ */
