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
        , parent_(0)
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

    /**
    @brief
        Adds a child to the current tree. The Child can contain children of its own.
        But you cannot a state tree that already has an active state.
    @param state
        The state to be added.
    */
    void GameState::addChild(GameState* state)
    {
        assert(state != NULL);

        std::map<std::string, GameState*>::const_iterator it = this->children_.find(state->getName());
        if (it == this->children_.end())
        {
            this->children_[state->getName()] = state;
            // mark us as parent
            state->setParent(this);
        }
        else
        {
            ThrowException(GameState, "Cannot add two children with the same name");
        }
    }

    /**
    @brief
        Removes a child by instance. This splits the tree in two parts,
        each of them functional on its own.
    @param state
        GameState by instance pointer
    */
    void GameState::removeChild(GameState* state)
    {
        assert(state != NULL);

        std::map<std::string, GameState*>::iterator it = this->children_.find(state->getName());
        if (it != this->children_.end())
            this->children_.erase(it);
        else
        {
            ThrowException(GameState, "Game state '" + this->getName() + "' doesn't have a child named '"
                + state->getName() + "'.");
        }
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
        this->activate();
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
