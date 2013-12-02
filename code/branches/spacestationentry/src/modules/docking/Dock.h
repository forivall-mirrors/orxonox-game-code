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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Dock.h
    @brief Definition of Dock class
    @ingroup Docking
*/

#ifndef _Dock_H__
#define _Dock_H__

#include "DockingPrereqs.h"

#include <set>

#include "core/EventIncludes.h"
#include "core/XMLPort.h"

#include "controllers/HumanController.h"

#include "DockingAnimation.h"
#include "DockingEffect.h"

#include "worldentities/StaticEntity.h"

namespace orxonox // tolua_export
{  // tolua_export

    class _DockingExport Dock  // tolua_export
        : public StaticEntity
    { // tolua_export
        public:
            Dock(Context* context);
            virtual ~Dock();

            // Trigger interface
            bool execute(bool bTriggered, BaseObject* trigger);
            bool undocking(bool bTriggered, BaseObject* trigger);

            // XML interface
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            // XML functions
            bool addEffect(DockingEffect* effect); //!< Add a DockingEffect to the Dock.
            const DockingEffect* getEffect(unsigned int index) const; //!< Get the DockingEffect at a given index.
            bool addAnimation(DockingAnimation* animation); //!< Add a DockingAnimation to the Dock.
            const DockingAnimation* getAnimation(unsigned int index) const; //!< Get the DockingAnimation at a given index.

            // Docking/undocking logic, checks conditions and invokes the DockingAnimations
            bool dock(PlayerInfo* player); //!< Returns true if given player docked successfully (player must be a candidate)
            bool undock(PlayerInfo* player); //!< Undocks a player (player must be docked)

            // Animation logic
            bool dockingAnimationFinished(PlayerInfo* player); //!< Called when a docking animation finished
            bool undockingAnimationFinished(PlayerInfo* player); //!< Called when a undocking animation finished

            // LUA interface
            // tolua_begin
            void dock()
                { this->dock(HumanController::getLocalControllerSingleton()->getPlayer()); }
            static unsigned int getNumberOfActiveDocks();
            static Dock* getActiveDockAtIndex(unsigned int index);
            // tolua_end

            // Console commands
            static void cmdDock();
            static void cmdUndock();

            // Network functions
            void showDockingDialogHelper(PlayerInfo* player);
            static void showDockingDialog();

        private:
            std::set<PlayerInfo*> candidates_; //!< A set of all players which are allowed to dock using the console command.
            std::set<PlayerInfo*> docked_; //!< A set of all docked players

            std::list<DockingEffect*> effects_; //!< The list of DockingEffects to be executed when a player docks.
            std::list<DockingAnimation*> animations_; //!< The list of DockingAnimations to be executed before a player docks
    }; // tolua_export
} // tolua_export

#endif /* _Dock_H__ */
