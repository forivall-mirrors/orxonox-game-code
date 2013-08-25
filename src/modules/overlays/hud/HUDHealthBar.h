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

#ifndef _HUDHealthBar_H__
#define _HUDHealthBar_H__

#include "overlays/OverlaysPrereqs.h"

#include "util/Math.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"
#include "HUDBar.h"

namespace orxonox
{
    class _OverlaysExport HUDHealthBar : public HUDBar, public Tickable
    {
        public:
            HUDHealthBar(Context* context);
            virtual ~HUDHealthBar();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            virtual void changedOwner();
            virtual void changedOverlayGroup();
            virtual void changedVisibility();
            virtual void changedName();

            inline void setTextFont(const std::string& font)
                { if (!font.empty()) { this->textoverlay_->setFont(font); } }
            inline const std::string& getTextFont() const
                { return this->textoverlay_->getFont(); }

            void setTextColour(const ColourValue& colour);
            inline const ColourValue& getTextColour() const
                { return this->textoverlay_->getColour(); }

            void setTextUseBarColour(bool bUseBarColour);
            inline bool getTextUseBarColour() const
                { return this->bUseBarColour_; }

            inline void setTextAlignment(OverlayText::Alignment alignment)
                { this->textoverlay_->setAlignment(alignment); }
            inline OverlayText::Alignment getTextAlignment() const
                { return this->textoverlay_->getAlignment(); }

            void setTextAlignmentString(const std::string& alignment)
                { this->textoverlay_->setAlignmentString(alignment); }
            std::string getTextAlignmentString() const
                { return this->textoverlay_->getAlignmentString(); }

            inline void setTextSize(float size)
                { this->textoverlay_->setTextSize(size); }
            inline float getTextSize() const
                { return this->textoverlay_->getTextSize(); }

            inline void setTextVisible(bool bVisible)
                { this->textoverlay_->setVisible(bVisible); }
            inline bool getTextVisible() const
                { return this->textoverlay_->isVisible(); }

            inline void setTextPickPoint(const Vector2& pickpoint)
                { this->textoverlay_->setPickPoint(pickpoint); }
            inline Vector2 getTextPickPoint() const
                { return this->textoverlay_->getPickPoint(); }

            inline void setTextOffset(const Vector2& position)
                { this->textoverlay_->setPosition(this->getPosition() + (position - this->getPickPoint()) * this->getSize()); }
            inline Vector2 getTextOffset() const
                { return (this->textoverlay_->getPosition() - this->getPosition()) / this->getSize() + this->getPickPoint(); }

            inline void setTextAspectCorrection(bool correct)
                { this->textoverlay_->setAspectCorrection(correct); }
            inline bool getTextAspectCorrection() const
                { return this->textoverlay_->getAspectCorrection(); }

            inline void setTextRotation(const Degree& angle)
                { this->textoverlay_->setRotation(angle); }
            inline const Degree& getTextRotation() const
                { return this->textoverlay_->getRotation(); }

            inline void setTextSpaceWidth(float width)
                { this->textoverlay_->setSpaceWidth(width); }
            inline float getTextSpaceWidth() const
                { return this->textoverlay_->getSpaceWidth(); }

            inline void setHealthBarOwner(Pawn* owner)
                { this->owner_ = owner; }

        private:
            WeakPtr<Pawn> owner_;
            SmartPtr<OverlayText> textoverlay_;
            bool bUseBarColour_;
            ColourValue textColour_;
    };
}
#endif /* _HUDHealthBar_H__ */
