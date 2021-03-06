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

#ifndef _AIController_H__
#define _AIController_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "ArtificialController.h"

namespace orxonox
{
    class _OrxonoxExport AIController : public ArtificialController, public Tickable
    {
        public:
            AIController(Context* context);
            virtual ~AIController();

            virtual void tick(float dt); //<! Carrying out the targets set in action().

        protected:
            virtual void action(); //<! action() is called in regular intervals managing the bot's behaviour ~ setting targets.
            void defaultBehaviour(float maxrand); //<! Helper function for code reusage. Some concrete commands for a bot.

        private:
            static const float ACTION_INTERVAL;

            Timer actionTimer_; //<! Regularly calls action().
    };
}

#endif /* _AIController_H__ */
