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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *      Benjamin Knecht
 *
 */

#ifndef _HUDBar_H__
#define _HUDBar_H__

#include "overlays/OverlaysPrereqs.h"

#include <map>
#include <vector>

#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/BaseObject.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport BarColour : public BaseObject
    {
    public:
        BarColour(Context* context);
        virtual ~BarColour() { }

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        void setColour(const ColourValue& colour) { this->colour_ = colour; }
        const ColourValue& getColour() const { return this->colour_; }

        void setPosition(float pos) { this->position_ = pos; }
        float getPosition() const    { return this->position_; }

    private:
        ColourValue colour_;
        float position_;
    };


    class _OverlaysExport HUDBar : public OrxonoxOverlay
    {
    public:
        HUDBar(Context* context);
        virtual ~HUDBar();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        void clearColours();

        inline void setRightToLeft(bool r2l)
        {
            if (r2l != this->right2Left_)
            {
                this->right2Left_ = r2l;
                this->valueChanged();
            }
        }
        inline bool getRightToLeft() const
            { return this->right2Left_; }

        inline void setValue(float value)
        {
            float temp = clamp(value, 0.0f, 1.0f);
            if (temp != this->value_)
            {
                this->value_ = temp;
                this->valueChanged();
            }
        }
        inline float getValue() const
            { return this->value_; }

        inline void setAutoColour(bool val)
        {
            if (val != this->autoColour_)
            {
                this->autoColour_ = val;
                this->valueChanged();

                if (!val)
                    this->currentColour_ = ColourValue::White;
            }
        }
        inline bool getAutoColour() const
            { return this->autoColour_; }

        void setBarTexture(const std::string& texture);
        const std::string& getBarTexture() const;

        inline const ColourValue& getCurrentBarColour() const
            { return this->currentColour_; }

    protected:
        virtual void valueChanged();

    private:
        void addColour(BarColour* colour);
        BarColour* getColour(unsigned int index);

        bool right2Left_;
        bool autoColour_;                   //!< whether bar changes colour automatically
        float value_;                       //!< progress of bar
        ColourValue currentColour_;

        Ogre::PanelOverlayElement* bar_;
        Ogre::TextureUnitState* textureUnitState_;
        std::map<float, ColourValue> colours_;
        std::vector<BarColour*> barColours_;

        static unsigned int materialcount_s;
    };
}
#endif /* _HUDBar_H__ */
