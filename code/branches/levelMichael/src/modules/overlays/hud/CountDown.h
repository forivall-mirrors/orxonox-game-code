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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#ifndef _CountDown_H__
#define _CountDown_H__

#include "overlays/OverlaysPrereqs.h"

#include "tools/interfaces/TimeFactorListener.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport CountDown : public OverlayText, public Tickable
    {
        public:
            CountDown(Context* context);
            virtual ~CountDown();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();
            virtual void tick(float dt);

            inline void setCounter(float value)
                { this->counter_ = value + 0.99f; }
            inline float getCounter() const
                { return this->counter_; }
        protected:

        private:
            PlayerInfo* owner_;
            float counter_;
            bool hasStopped_;
    };
}
#endif /* _CountDown_H__ */
