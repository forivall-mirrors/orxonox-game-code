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

#ifndef _PongAI_H__
#define _PongAI_H__

#include "OrxonoxPrereqs.h"

#include "Controller.h"
#include "objects/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport PongAI : public Controller, public Tickable
    {
        public:
            PongAI(BaseObject* creator);
            virtual ~PongAI() {}

            void setConfigValues();

            virtual void tick(float dt);

            void setPongBall(PongBall* ball)
                { this->ball_ = ball; }

        protected:
            void calculateRandomOffset();

            PongBall* ball_;
            float randomOffset_;
            float relHysteresisOffset_;
            float strength_;
    };
}

#endif /* _PongAI_H__ */
