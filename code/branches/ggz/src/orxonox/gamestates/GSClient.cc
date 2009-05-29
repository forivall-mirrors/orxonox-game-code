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
 *      Fabian 'x3n' Landau
 *      Adrian Friedli
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSClient.h"
#include "SpecialConfig.h"

#include "core/input/InputManager.h"
#include "core/CommandLine.h"
#include "core/Core.h"
#include "network/Client.h"
#ifdef GGZMOD_FOUND
#include "GGZClient.h"
#endif /* GGZMOD_FOUND */

namespace orxonox
{
    SetCommandLineArgument(ip, "127.0.0.1").information("#.#.#.#");

    GSClient::GSClient()
        : GameState<GSGraphics>("client"),
        client_(0),
        ggzClient(0)
    {
    }

    GSClient::~GSClient()
    {
    }

    void GSClient::enter()
    {
        Core::setIsClient(true);

#ifdef GGZMOD_FOUND
        if (GGZClient::isActive())
        {
            this->ggzClient = new GGZClient(this);
            return;
        }
#endif /* GGZMOD_FOUND */

        this->connect(CommandLine::getValue("ip").getString(), CommandLine::getValue("port"));
    }

    void GSClient::connect(const std::string& address, int port)
    {
        this->client_ = new Client(address, port);

        if(!client_->establishConnection())
            ThrowException(InitialisationFailed, "Could not establish connection with server.");

        GSLevel::enter(this->getParent()->getViewport());

        client_->tick(0);
    }

    void GSClient::leave()
    {
        GSLevel::leave();

        client_->closeConnection();

        if (ggzClient)
        {
            delete ggzClient;
        }

        // destroy client
        delete this->client_;

        Core::setIsClient(false);
    }

    void GSClient::ticked(const Clock& time)
    {
        GSLevel::ticked(time);
        if(client_)
        {
            client_->tick(time.getDeltaTime());
        }

        this->tickChild(time);
    }
}
