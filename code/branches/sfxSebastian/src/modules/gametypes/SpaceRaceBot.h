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
 *  Created on: Oct 8, 2012
 *      Author: purgham
 */

#ifndef SPACERACEBOT_H_
#define SPACERACEBOT_H_

#include "infos/Bot.h"
#include "gametypes/GametypesPrereqs.h"
#include "gametypes/SpaceRaceController.h"


namespace orxonox
{

    /**

    */
    class _GametypesExport SpaceRaceBot: public Bot
    {
        public:
        SpaceRaceBot(Context* context);
            virtual ~SpaceRaceBot() {}
    };
}


#endif /* SPACERACEBOT_H_ */
