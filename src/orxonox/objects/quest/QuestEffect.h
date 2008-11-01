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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#ifndef _QuestEffect_H__
#define _QuestEffect_H__

#include "OrxonoxPrereqs.h"

#include <list>

#include "core/BaseObject.h"

namespace orxonox {

    class Player; //Forward declaration, remove when fully integrated into the objecthirarchy.

    /**
    @brief
        Handles effects for quests.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestEffect : public BaseObject
    {
        public:
            QuestEffect(BaseObject* creator);
            virtual ~QuestEffect();

            virtual bool invoke(Player* player) = 0; //!< Invokes the effect.
            static bool invokeEffects(Player* player, std::list<QuestEffect*> & effects); //!< Invokes all effects in the list.


    };

}

#endif /* _QuestEffect_H__ */
