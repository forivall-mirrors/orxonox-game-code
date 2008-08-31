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
#include "Debug.h"
#include "Exception.h"

namespace orxonox
{
    /**
    @brief
        Constructor only initialises variables and sets the name permanently.
    */
    GameState::GameState(const std::string& name)
        : name_(name)
        //, parent_(0)
        , activeChild_(0)
        //, bPausegetParent()(false)
    {
        Operations temp = {false, false, false, false, false};
        this->operation_ = temp;
    }

    /**
    @brief
        Destructor only checks that we don't delete an active state.
    */
    GameState::~GameState()
    {
        OrxAssert(!isInSubtree(getCurrentState()), "Deleting an active GameState is a very bad idea..");
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
        if (!state)
            return;
        // check if the state/tree to be added has states in it that already exist in this tree.
        for (std::map<std::string, GameState*>::const_iterator it = state->allChildren_.begin();
            it != state->allChildren_.end(); ++it)
        {
            if (this->getState(it->second->getName()))
            {
                ThrowException(GameState, "Cannot add a GameState to the hierarchy twice.");
                return;
            }
        }
        if (this->getState(state->name_))
        {
            ThrowException(GameState, "Cannot add a GameState to the hierarchy twice.");
            return;
        }
        // Make sure we don't add a tree that already has an active state.
        if (state->getCurrentState())
        {
            ThrowException(GameState, "Cannot merge a tree that is already active.");
            return;
        }

        // merge the child's children into this tree
        for (std::map<std::string, GameState*>::const_iterator it = state->allChildren_.begin();
            it != state->allChildren_.end(); ++it)
            this->grandchildAdded(state, it->second);
        // merge 'state' into this tree
        this->grandchildAdded(state, state);

        // mark us as parent
        state->setParent(this);
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
        std::map<GameState*, GameState*>::iterator it = this->grandchildrenToChildren_.find(state);
        if (it != this->grandchildrenToChildren_.end())
        {
            if (state->isInSubtree(getCurrentState()))
            {
                ThrowException(GameState, "Cannot remove an active game state child '"
                    + state->getName() + "' from '" + name_ + "'.");
                //COUT(2) << "Warning: Cannot remove an active game state child '" << state->getName()
                //    << "' from '" << name_ << "'." << std::endl;
            }
            else
            {
                for (std::map<GameState*, GameState*>::const_iterator it = state->grandchildrenToChildren_.begin();
                    it != state->grandchildrenToChildren_.end(); ++it)
                    this->grandchildRemoved(it->first);
                this->grandchildRemoved(state);
            }
        }
        else
        {
            ThrowException(GameState, "Game state '" + name_ + "' doesn't have a child named '"
                + state->getName() + "'.");
            //COUT(2) << "Warning: Game state '" << name_ << "' doesn't have a child named '"
            //    << state->getName() << "'. Removal skipped." << std::endl;
        }
    }

    /**
    @brief
        Removes a child by name. This splits the tree in two parts,
        each of them functional on its own.
    @param state
        GameState by name
    */

    void GameState::removeChild(const std::string& name)
    {
        GameState* state = getState(name);
        if (state)
        {
            removeChild(state);
        }
        else
        {
            ThrowException(GameState, "GameState '" + name + "' doesn't exist.");
            //COUT(2) << "Warning: GameState '" << name << "' doesn't exist." << std::endl;
        }
    }

    /**
    @brief
        Tells a state that one of its children has added a child. This is necessary
        to fill the internal maps correctly.
    @param child
        The child who notices this state.
    @param grandchild
        The child that has been added.
    */
    inline void GameState::grandchildAdded(GameState* child, GameState* grandchild)
    {
        // fill the two maps correctly.
        this->allChildren_[grandchild->getName()] = grandchild;
        this->grandchildrenToChildren_[grandchild] = child;
        if (this->getParent())
            this->getParent()->grandchildAdded(this, grandchild);
    }

    /**
    @brief
        Tells a state that one of its children has removed a child. This is necessary
        to fill the internal maps correctly.
    @param child
        The child who notices this state.
    @param grandchild
        The child that has been removed.
    */
    inline void GameState::grandchildRemoved(GameState* grandchild)
    {
        // adjust the two maps correctly.
        this->allChildren_.erase(grandchild->getName());
        this->grandchildrenToChildren_.erase(grandchild);
        if (this->getParent())
            this->getParent()->grandchildRemoved(grandchild);
    }

    /**
    @brief
        Checks whether a specific game states exists in the hierarchy.
    @remarks
        Remember that the every node has a map with all its child nodes.
    */
    GameState* GameState::getState(const std::string& name)
    {
        if (this->getParent())
            return this->getParent()->getState(name);
        else
        {
            // The map only contains children, so check ourself first
            if (name == this->name_)
                return this;
            // Search in the map. If there is no entry, we can be sure the state doesn't exist.
            std::map<std::string, GameState*>::const_iterator it = this->allChildren_.find(name);
            return (it!= this->allChildren_.end() ? it->second : 0);
        }
    }

    /**
    @brief
        Returns the root node of the tree.
    */
    GameState* GameState::getRoot()
    {
        if (this->getParent())
            return this->getParent()->getRoot();
        else
            return this;
    }

    /**
    @brief
        Returns the current active state.
    @remarks
        Remember that the current active state is the one that does not
        have active children itself. Many states can be active at once.
    */
    GameState* GameState::getCurrentState()
    {
        if (this->operation_.active)
        {
            if (this->activeChild_)
                return this->activeChild_->getCurrentState();
            else
                return this;
        }
        else
        {
            if (this->getParent())
                return this->getParent()->getCurrentState();
            else
                return 0;
        }
    }

    /**
    @brief
        Determines whether 'state' is in this subtree, including this node.
    */
    bool GameState::isInSubtree(GameState* state) const
    {
        return (grandchildrenToChildren_.find(state) != grandchildrenToChildren_.end()
                || state == this);
    }

    /**
    @brief
        Makes a state transition according to the state tree. You can choose any state
        in the tree to do the call. The function finds the current state on its own.
    @param state
        The state to be entered, has to exist in the tree.
    */
    void GameState::requestState(const std::string& name)
    {
        assert(getRoot());
        getRoot()->requestState(name);
    }

    /**
    @brief
        Internal method that actually makes the state transition. Since it is internal,
        the method can assume certain things to be granted (like 'this' is always active).
    */
    void GameState::makeTransition(GameState* source, GameState* destination)
    {
        if (source == this->getParent())
        {
            // call is from the parent
            this->activate();
        }
        else if (source == 0)
        {
            // call was just started by root
            // don't do anyting yet
        }
        else
        {
            // call is from a child
            this->activeChild_ = 0;
        }

        if (destination == this)
            return;

        // Check for 'destination' in the children map first
        std::map<GameState*, GameState*>::const_iterator it
            = this->grandchildrenToChildren_.find(destination);
        if (it != this->grandchildrenToChildren_.end())
        {
            // child state. Don't use 'state', might be a grandchild!
            this->activeChild_ = it->second;
            it->second->makeTransition(this, destination);
        }
        else
        {
            // parent. We can be sure of this.
            assert(this->getParent() != 0);

            this->deactivate();
            this->getParent()->makeTransition(this, destination);
        }
    }

    /**
    @brief
        Activates the state. Only sets bActive_ to true and notifies the parent.
    */
    void GameState::activate()
    {
        this->operation_.active = true;
        this->operation_.entering = true;
        this->enter();
        this->operation_.entering = false;
    }

    /**
        Activates the state. Only sets bActive_ to false and notifies the parent.
    */
    void GameState::deactivate()
    {
        this->operation_.leaving = true;
        this->leave();
        this->operation_.leaving = false;
        this->operation_.active = false;
    }

    /**
    @brief
        Update method that calls ticked() with enclosed bRunning_ = true
        If there was a state transition request within ticked() then this
        method will transition in the end.
    @param dt Delta time
    @note
        This method is not virtual! You cannot override it therefore.
    */
    void GameState::tick(const Clock& time)
    {
        this->operation_.running = true;
        this->ticked(time);
        this->operation_.running = false;
    }
}
