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

/**
@file
@brief Declaration of the OverlayGroup class.
*/

#ifndef _OverlayGroup_H__
#define _OverlayGroup_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/BaseObject.h"

namespace orxonox
{
    /**
    @brief
        OverlayGroup does almost exactly what it says: It groups OrxonoxOverlays together.
        You can scroll the entire group by a certain amount. Scale however works differently
        than expected: Each OrxonoxOverlay scales individually. That's quite useful when you
        create your HUD with an OverlayGroup and then want to alter its size.
    */
    class _OrxonoxExport OverlayGroup : public BaseObject
    {
    public:
        OverlayGroup(Context* context);
        //! Empty destructor.
        ~OverlayGroup();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        static void toggleVisibility(const std::string& name);
        static void show(const std::string& name);
        static void scaleGroup(const std::string& name, float scale);
        static void scrollGroup(const std::string& name, const Vector2& scroll);

        inline const std::set< SmartPtr<OrxonoxOverlay> >& getOverlays() const
            { return this->hudElements_; }

        virtual void changedVisibility();
        virtual void changedGametype();

        void setOwner(BaseObject* owner);
        inline BaseObject* getOwner() const
            { return this->owner_; }

        //! Scales each OrxonoxOverlay individually by scale.
        void scale(const Vector2& scale) { this->setScale(scale * this->scale_); }
        void setScale(const Vector2& scale);
        //! Returns the current size of the group.
        Vector2 getScale() const { return this->scale_; }

        //! Scrolls each OrxonoxOverlay individually by scroll.
        void scroll(const Vector2& scroll) { this->setScroll(scroll + this->scroll_); }
        void setScroll(const Vector2& scroll);
        //! Returns the current scrolling offset of the group.
        Vector2 getScroll() const { return this->scale_; }

        void addElement(OrxonoxOverlay* element);
        bool removeElement(OrxonoxOverlay* element);
        OrxonoxOverlay* getElement(unsigned int index);

    private:
        std::set< SmartPtr<OrxonoxOverlay> > hudElements_;    //!< Contains all the OrxonoxOverlays of the this group.
        Vector2 scale_;                            //!< Current scale (independent of the elements).
        Vector2 scroll_;                           //!< Current scrolling offset.
        BaseObject* owner_;                        //!< The owner of this OverlayGroup
    };
}

#endif /* _OverlayGroup_H__ */
