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
#include <cassert>
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
        , bPauseParent_(false)
        //, bActive_(false)
        //, bSuspended_(false)
        //, bRunning_(false)
        , scheduledTransition_(0)
        , parent_(0)
        , activeChild_(0)
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
        if (this->operation_.active)
        {
            if (this->parent_)
                this->requestState(this->parent_->getName());
            else
                this->requestState("");
        }
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
            if (this->checkState(it->second->getName()))
            {
                ThrowException(GameState, "Cannot add a GameState to the hierarchy twice.");
                return;
            }
        }
        if (this->checkState(state->name_))
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
        {
            this->allChildren_[it->second->getName()] = it->second;
            this->grandchildrenToChildren_[it->second] = state;
            if (this->parent_)
                this->parent_->grandchildAdded(this, it->second);
        }
        // merge 'state' into this tree
        this->allChildren_[state->name_] = state;
        this->grandchildrenToChildren_[state] = state;
        if (this->parent_)
            this->parent_->grandchildAdded(this, state);

        // mark us as parent
        state->parent_ = this;
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
            if (state->getOperation().active)
            {
                ThrowException(GameState, "Cannot remove active game state child '"
                    + state->getName() + "' from '" + name_ + "'.");
                //COUT(2) << "Warning: Cannot remove active game state child '" << state->getName()
                //    << "' from '" << name_ << "'." << std::endl;
            }
            else
            {
                for (std::map<GameState*, GameState*>::const_iterator it = state->grandchildrenToChildren_.begin();
                    it != state->grandchildrenToChildren_.end(); ++it)
                {
                    this->grandchildRemoved(it->first);
                }
                this->grandchildRemoved(state);
            }
        }
        else
        {
            ThrowException(GameState, "Game state '" + name_ + "' doesn't have a child named '"
                + state->getName() + "'. Removal skipped.");
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
        GameState* state = checkState(name);
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
    void GameState::grandchildAdded(GameState* child, GameState* grandchild)
    {
        // fill the two maps correctly.
        this->allChildren_[grandchild->getName()] = grandchild;
        this->grandchildrenToChildren_[grandchild] = child;
        if (this->parent_)
            this->parent_->grandchildAdded(this, grandchild);
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
    void GameState::grandchildRemoved(GameState* grandchild)
    {
        // adjust the two maps correctly.
        this->allChildren_.erase(grandchild->getName());
        this->grandchildrenToChildren_.erase(grandchild);
        if (this->parent_)
            this->parent_->grandchildRemoved(grandchild);
    }

    /**
    @brief
        Checks whether a specific game states exists in the hierarchy.
    @remarks
        Remember that the every node has a map with all its child nodes.
    */
    GameState* GameState::checkState(const std::string& name)
    {
        if (this->parent_)
            return this->parent_->checkState(name);
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
            if (this->parent_)
                return this->parent_->getCurrentState();
            else
                return 0;
        }
    }

    /**
    @brief
        Returns the root node of the tree.
    */
    GameState* GameState::getRootNode()
    {
        if (this->parent_)
            return this->parent_->getRootNode();
        else
            return this;
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
        GameState* current = getCurrentState();
        if (current != 0 && (current->getOperation().entering || current->getOperation().leaving))
        {
            ThrowException(GameState, "Making state transitions during enter()/leave() is forbidden.");
        }
        //if (name == "")
        //{
        //    // user would like to leave every state.
        //    if (current)
        //    {
        //        // Deactivate all states but root
        //        GameState* root = getRootNode();
        //        current->makeTransition(root);
        //        //// Kick root too
        //        //assert(!(root->getOperation().entering || root->getOperation().leaving));
        //        //if (root->operation_.running)
        //        //    root->scheduledTransition_ = 0;
        //        //else
        //        //    root->deactivate();
        //    }
        //}
        else
        {
            GameState* request = checkState(name);
            if (request)
            {
                if (current)
                {
                    // There is already an active state
                    current->makeTransition(request);
                }
                else
                {
                    // no active state --> we have to activate the root node first.
                    GameState* root = getRootNode();
                    root->activate();
                    root->makeTransition(request);
                }
            }
            else
            {
                COUT(2) << "Warning: GameState '" << name << "' doesn't exist." << std::endl;
            }
        }
    }

    /**
    @brief
        Internal method that actually makes the state transition. Since it is internal,
        the method can assume certain things to be granted (like 'this' is always active).
    */
    void GameState::makeTransition(GameState* state)
    {
        // we're always already active
        assert(this->operation_.active);

        if (state == this)
            return;

        // Check for 'state' in the children map first
        std::map<GameState*, GameState*>::const_iterator it = this->grandchildrenToChildren_.find(state);
        if (it != this->grandchildrenToChildren_.end())
        {
            // child state. Don't use 'state', might be a grandchild!
            it->second->activate();
            it->second->makeTransition(state);
        }
        else
        {
            // parent. We can be sure of this.
            assert(this->parent_ != 0);

            // only do the transition if we're not currently running
            if (this->operation_.running)
            {
                //this->bDeactivationScheduled_ = true;
                this->scheduledTransition_ = state;
            }
            else
            {
                this->deactivate();
                this->parent_->makeTransition(state);
            }

        }
    }

    /**
    @brief
        Activates the state. Only sets bActive_ to true and notifies the parent.
    */
    void GameState::activate()
    {
        if (this->parent_)
            this->parent_->activeChild_ = this;
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
        if (this->parent_)
            this->parent_->activeChild_ = 0;
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
    void GameState::tick(float dt)
    {
        this->operation_.running = true;
        this->ticked(dt);
        this->operation_.running = false;

        if (this->scheduledTransition_)
        {
            // state was requested to be deactivated when ticked.
            this->makeTransition(this->scheduledTransition_);
            this->scheduledTransition_ = 0;
            this->deactivate();
        }
    }

}
