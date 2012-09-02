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

#ifndef _FadingBillboard_H__
#define _FadingBillboard_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "Billboard.h"

namespace orxonox
{
    class _OrxonoxExport FadingBillboard : public Billboard, public Tickable
    {
        public:
            FadingBillboard(BaseObject* creator);
            virtual ~FadingBillboard();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);
            virtual void changedActivity();
            virtual void changedVisibility();

            inline void setTurnOnTime(float turnontime)
                { this->turnontime_ = turnontime; }
            inline float getTurnOnTime() const
                { return this->turnontime_; }

            inline void setTurnOffTime(float turnofftime)
                { this->turnofftime_ = turnofftime; }
            inline float getTurnOffTime() const
                { return this->turnofftime_; }

            inline const ColourValue& getFadedColour() const
                { return this->fadedColour_; }

        protected:
            void registerVariables();
            virtual void startturnonoff();
            virtual void stopturnonoff();
            virtual void poststopturnonoff();
            virtual void changedColour();

            float turnontime_;
            float turnofftime_;
            float postprocessingtime_;
            Timer turnonofftimer_;
            char changedirection_;
            ColourValue fadedColour_;
    };
}

#endif /* _FadingBillboard_H__ */
