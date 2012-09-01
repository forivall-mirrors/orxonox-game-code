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

/**
    @file PongBot.h
    @brief Declaration of the PongBot class.
    @ingroup Pong
*/

#ifndef _PongBot_H__
#define _PongBot_H__

#include "pong/PongPrereqs.h"
#include "infos/Bot.h"

namespace orxonox
{

    /**
    @brief
        A bot especially for @ref orxonox::Pong "Pong".

        Uses the @ref orxonox::PongAI "PongAI".

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport PongBot : public Bot
    {
        public:
            PongBot(BaseObject* creator);
            virtual ~PongBot() {}
    };
}

#endif /* _PongBot_H__ */
