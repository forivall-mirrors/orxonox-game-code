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

#ifndef _OverlayText_H__
#define _OverlayText_H__

#include "overlays/OverlaysPrereqs.h"

#include <string>
#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport OverlayText : public OrxonoxOverlay
    {
    public:
        enum Alignment
        {
            Left,
            Right,
            Center
        };

        OverlayText(Context* context);
        virtual ~OverlayText();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        void setCaption(const std::string& caption);
        std::string getCaption() const;

        void setFont(const std::string& font);
        const std::string& getFont() const;

        void setSpaceWidth(float width);
        float getSpaceWidth() const;

        void setColour(const ColourValue& colour);
        const ColourValue& getColour() const;

        void setAlignment(OverlayText::Alignment alignment);
        OverlayText::Alignment getAlignment() const;

        void setAlignmentString(const std::string& alignment);
        std::string getAlignmentString() const;

        inline void setTextSize(float size) { this->setSize(Vector2(size, size)); }
        inline float getTextSize() const    { return this->getSize().y; }

    protected:
        virtual void sizeChanged();
        virtual void changedColour() {}
        virtual void changedCaption() {}

        Ogre::TextAreaOverlayElement* text_;
    };
}
#endif /* _OverlayText_H__ */
