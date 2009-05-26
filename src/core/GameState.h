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
#include <map>
#include "CorePrereqs.h"

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
        friend class Game;

    public:
        /**
        @brief
            Gives information about what the GameState is currently doing
        */
        struct State
        {
            unsigned active       : 1;
            unsigned activating   : 1;
            unsigned deactivating : 1;
            unsigned updating     : 1;
            unsigned suspended    : 1;
            unsigned topState     : 1;
        };

    public:
        GameState(const std::string& name, bool countTicktime = true);
        virtual ~GameState();

        const std::string& getName() const { return name_; }
        State getActivity()          const { return this->activity_; }
        GameState* getParent()       const { return this->parent_; }

        bool getCountTickTime()      const { return this->bCountTickTime_; }

        void addChild(GameState* state);
        void removeChild(GameState* state);

    protected:
        virtual void activate() = 0;
        virtual void deactivate() = 0;
        virtual void update(const Clock& time) = 0;

    private:
        void setParent(GameState* state) { this->parent_ = state; }
        void setActivity(State activity);
        void activateInternal();
        void deactivateInternal();
        void updateInternal(const Clock& time);

        const std::string                        name_;
        State                                    activity_;
        const bool                               bCountTickTime_;
        GameState*                               parent_;
        std::map<std::string, GameState*>        children_;
    };
}

#endif /* _GameState_H__ */
