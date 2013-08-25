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

#ifndef _TeamSpawnPoint_H__
#define _TeamSpawnPoint_H__

#include "OrxonoxPrereqs.h"

#include "worldentities/pawns/Pawn.h"
#include "SpawnPoint.h"

namespace orxonox
{
    class _OrxonoxExport TeamSpawnPoint : public SpawnPoint
    {
        public:
            TeamSpawnPoint(Context* context);
            virtual ~TeamSpawnPoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void setTeamNumber(unsigned int team)
                { this->teamNumber_ = team; }
            unsigned int getTeamNumber() const
                { return this->teamNumber_; }

        private:
            unsigned int teamNumber_;
    };
}

#endif /* _TeamSpawnPoint_H__ */
