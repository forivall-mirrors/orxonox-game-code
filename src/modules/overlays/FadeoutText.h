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

#ifndef _FadeoutText_H__
#define _FadeoutText_H__

#include "overlays/OverlaysPrereqs.h"

#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport FadeoutText : public OverlayText, public Tickable
    {
        public:
            FadeoutText(Context* context);
            virtual ~FadeoutText() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            inline void setDelay(float delay)
                { this->delay_ = delay; }
            inline float getDelay() const
                { return this->delay_; }

            inline void setFadeouttime(float fadeouttime)
                { this->fadeouttime_ = fadeouttime; }
            inline float getFadeouttime() const
                { return this->fadeouttime_; }

        private:
            virtual void changedColour();
            virtual void changedCaption();

            void fadeout();
            void reset();

            float delay_;
            float fadeouttime_;

            bool bFadingOut_;
            Timer fadeouttimer_;

            float initialAlpha_;
    };
}
#endif /* _FadeoutText_H__ */
