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

/**
    @file
    @brief Implementation of the QuestEffect class.
*/

#include "QuestEffect.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Creates a new QuestEffect.
        Is not meant to be invoked directly, since this is only an interface.
    */
    QuestEffect::QuestEffect(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestEffect);
    }

    /**
    @brief
        Destructor.
    */
    QuestEffect::~QuestEffect()
    {

    }

    /**
    @brief
        Static method. Invoke all QuestEffects in an QuestEffect-list on a given player.
    @param player
        The player the QuestEffects are invoked on.
    @param effects
        A list of all the QuestEffects to be invoked.
    @return
        Returns false if there was an error, view console of log for further detail.
    */
    /*static*/ bool QuestEffect::invokeEffects(PlayerInfo* player, std::list<QuestEffect*> & effects)
    {
        bool check = true;

        COUT(4) << "Invoking QuestEffects on player: " << player << " ."  << std::endl;

        for (std::list<QuestEffect*>::iterator effect = effects.begin(); effect != effects.end(); effect++)
        {
            check = check && (*effect)->invoke(player);
        }
        return check;
    }

}
