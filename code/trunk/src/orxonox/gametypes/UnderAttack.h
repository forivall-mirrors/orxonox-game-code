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
 *      Matthias Mock
 *   Co-authors:
 *      ...
 *
 */

#ifndef _UnderAttack_H__
#define _UnderAttack_H__

#include "OrxonoxPrereqs.h"

#include "TeamDeathmatch.h"

namespace orxonox
{
    class _OrxonoxExport UnderAttack : public TeamDeathmatch
    {
        public:
            UnderAttack(BaseObject* creator);
            virtual ~UnderAttack() {}

            void setConfigValues();
            void tick (float dt);
            void addDestroyer(Destroyer* destroyer);
            inline Destroyer* getDestroyer() const
                { return this->destroyer_; }

            virtual bool allowPawnHit(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0);

        protected:
            virtual void killedDestroyer();

            WeakPtr<Destroyer> destroyer_;
            unsigned int teams_;
            float gameTime_;
            int timesequence_;
            bool gameEnded_;
    };
}

#endif /* _TeamDeathmatch_H__ */
