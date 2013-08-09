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

#include "Bot.h"

#include "util/Math.h"
#include "core/GameMode.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "gametypes/Gametype.h"
#include "controllers/AIController.h"

namespace orxonox
{
    CreateFactory(Bot);

    Bot::Bot(Context* context) : PlayerInfo(context)
    {
        RegisterObject(Bot);

        this->bHumanPlayer_ = false;
        this->bLocalPlayer_ = GameMode::isMaster();
        this->bSetUnreadyAfterSpawn_ = false;
        this->setReadyToSpawn(true);
        this->defaultController_ = Class(AIController);

        this->setConfigValues();

        this->setName(this->names_[static_cast<size_t>(rnd() * this->names_.size())]);

        if (this->getGametype())
            this->getGametype()->playerEntered(this);

        this->createController();
    }

    Bot::~Bot()
    {
    }

    void Bot::setConfigValues()
    {
        static const std::string names[] =
        {
            "Dr. Julius No",
            "Rosa Klebb",
            "Auric Goldfinger",
            "Emilio Largo",
            "Ernst Stavro Blofeld",
            "Dr. Kananga",
            "Francisco Scaramanga",
            "Karl Stromberg",
            "Sir Hugo Drax",
            "Aris Kristatos",
            "Kamal Khan",
            "General Orlov",
            "Max Zorin",
            "Brad Whitaker",
            "General Georgi Koskov",
            "Franz Sanchez",
            "Alec Trevelyan",
            "Elliot Carver",
            "Elektra King",
            "Viktor Zokas",
            "Gustav Graves",
            "Le Chiffre",
            "Mr. White",
            "Dominic Greene"
        };
        static std::vector<std::string> defaultnames(names, names + sizeof(names) / sizeof(std::string));

        SetConfigValue(names_, defaultnames);
    }
}
