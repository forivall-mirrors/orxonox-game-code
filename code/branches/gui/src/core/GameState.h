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
    Definition of GameState class.
*/

#ifndef _GameState_H__
#define _GameState_H__

#include "CorePrereqs.h"

#include <string>
#include <vector>
#include <map>
#include "util/Integers.h"
#include "Clock.h"

namespace orxonox
{
    /**
    @brief
        An implementation of a tree to manage game states.
        This leads to a certain hierarchy that is created at runtime.
        To actually use the structure, you will have to derive from it and
        implement 'enter', 'leave' and 'tick'. The latter corresponds to an
        update function.

        Internally the tree is managed by two maps per node: One stores all its
        children, grandchildren, etc. by name of the state. The other maps these
        children states to the actual child of the node.
        An example: Foo is a grandchildren of Bar and Foofoo is the Foo's parent.
        Then Bar stores Foo in map by its name. The other one then maps Foo to Foofoo.
    */
    class _CoreExport GameState
    {
        friend class RootGameState;

    public:
        /**
        @brief
            Gives information about what the GameState is currently doing
        */
        struct Operations
        {
            unsigned active    : 1;
            unsigned entering  : 1;
            unsigned leaving   : 1;
            unsigned running   : 1;
            unsigned suspended : 1;
        };

    public:
        GameState(const std::string& name);
        virtual ~GameState();

        const std::string& getName() const { return name_; }
        const Operations getOperation() const { return this->operation_; }
        bool isInSubtree(GameState* state) const;

        GameState* getState(const std::string& name);
        GameState* getRoot();
        GameState* getParent() const { return this->parent_; }
        //! Returns the currently active game state
        virtual GameState* getCurrentState();

        virtual void requestState(const std::string& name);

        void addChild(GameState* state);
        void removeChild(GameState* state);
        void removeChild(const std::string& name);

    protected:
        virtual void enter() = 0;
        virtual void leave() = 0;
        virtual void ticked(const Clock& time) = 0;

        GameState* getActiveChild() { return this->activeChild_; }

        void tickChild(const Clock& time) { if (this->getActiveChild()) this->getActiveChild()->tick(time); }

    private:
        //! Performs a transition to 'destination'
        virtual void makeTransition(GameState* source, GameState* destination);

        void grandchildAdded(GameState* child, GameState* grandchild);
        void grandchildRemoved(GameState* grandchild);

        void tick(const Clock& time);
        void activate();
        void deactivate();

        const std::string                   name_;
        Operations                          operation_;
        GameState*                          parent_;
        GameState*                          activeChild_;
        //bool                                bPauseParent_;
        std::map<std::string, GameState*>   allChildren_;
        std::map<GameState*, GameState*>    grandchildrenToChildren_;
    };
}

#endif /* _GameState_H__ */
