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
 *      Reto Grieder
 *   Co-authors:
 *      Adrian Friedli
 *
 */

#ifndef _GSClient_H__
#define _GSClient_H__

#include "SpecialConfig.h"
#include "OrxonoxPrereqs.h"
#include "network/NetworkPrereqs.h"
#include "GSLevel.h"
#include "GSGraphics.h"

namespace orxonox
{
#ifdef GGZMOD_FOUND
    class _OrxonoxExport GGZClient;
#endif /* GGZMOD_FOUND */

    class _OrxonoxExport GSClient : public GameState<GSGraphics>, public GSLevel
    {
    public:
        GSClient();
        ~GSClient();

    private:
        void enter();
        void leave();
        void ticked(const Clock& time);

        Client* client_;
#ifdef GGZMOD_FOUND
        GGZClient* ggzClient;
#endif /* GGZMOD_FOUND */
    };
}

#endif /* _GSClient_H__ */
