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
 *      ...
 *
 */

/**
@file
@brief
    Implementation of GameState class.
*/

#include "GameState.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "util/OrxAssert.h"
#include "Game.h"

namespace orxonox
{
    /**
    @brief
        Constructor only initialises variables and sets the name permanently.
    */
    GameState::GameState(const GameStateInfo& info)
        : info_(info)
    {
        this->activity_.activating   = false;
        this->activity_.active       = false;
        this->activity_.deactivating = false;
        this->activity_.suspended    = false;
        this->activity_.topState     = false;
        this->activity_.updating     = false;
    }

    /**
    @brief
        Destructor only checks that we don't delete an active state.
    */
    GameState::~GameState()
    {
        OrxAssert(this->activity_.active == false, "Deleting an active GameState is a very bad idea..");
    }

    const std::string& GameState::getName() const
    {
        return info_.stateName;
    }

    void GameState::activateInternal()
    {
        this->activity_.activating = true;
        this->activate();
        this->activity_.activating = false;
        this->activity_.active = true;
    }

    void GameState::deactivateInternal()
    {
        this->activity_.active = false;
        this->activity_.deactivating = true;
        this->deactivate();
        this->activity_.deactivating = false;
        this->activity_.suspended = false;
        this->activity_.updating = false;
    }

    void GameState::updateInternal(const Clock& time)
    {
        this->activity_.updating = true;
        this->update(time);
        this->activity_.updating = false;
    }
}
