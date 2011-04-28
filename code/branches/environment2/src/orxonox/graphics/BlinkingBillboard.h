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

#ifndef _BlinkingBillboard_H__
#define _BlinkingBillboard_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "tools/interfaces/Tickable.h"
#include "Billboard.h"

namespace orxonox
{
    class _OrxonoxExport BlinkingBillboard : public Billboard, public Tickable
    {
        public:
            BlinkingBillboard(BaseObject* creator);
            virtual ~BlinkingBillboard();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);

            inline void setAmplitude(float amplitude)
                { this->amplitude_ = amplitude; }
            inline float getAmplitude() const
                { return this->amplitude_; }

            inline void setFrequency(float frequency)
                { this->frequency_ = frequency; }
            inline float getFrequency() const
                { return this->frequency_; }

            inline void setPhase(const Degree& phase)
                { this->phase_ = phase; }
            inline const Degree& getPhase() const
                { return this->phase_; }

            inline void setQuadratic(bool bQuadratic)
                { this->bQuadratic_ = bQuadratic; }
            inline bool isQuadratic() const
                { return this->bQuadratic_; }

        private:
            void registerVariables();
            
            float amplitude_;
            float frequency_;
            Degree phase_;
            bool bQuadratic_;
            float time_;
    };
}

#endif /* _BlinkingBillboard_H__ */
