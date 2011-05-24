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
    @file Dock.cc
    @brief Docking system main class
*/

#include "Dock.h"

#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/GUIManager.h"
#include "infos/HumanPlayer.h"
#include "worldentities/pawns/Pawn.h"
#include "interfaces/PlayerTrigger.h"
#include "core/command/ConsoleCommand.h"

#include "ToluaBindDocking.h"

namespace orxonox
{
    // Register tolua_open function when loading the library
    DeclareToluaInterface(Docking);

    CreateFactory(Dock);

    SetConsoleCommand("Dock", "dock",    &Dock::cmdDock).addShortcut().setAsInputCommand();
    SetConsoleCommand("Dock", "undock",  &Dock::cmdUndock).addShortcut().setAsInputCommand();

    Dock::Dock(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Dock);
    }

    Dock::~Dock()
    {
    }


    void Dock::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLPort, xmlelement, mode);

        XMLPortObject(Dock, DockingEffect, "effects", addEffect, getEffect, xmlelement, mode);
        XMLPortObject(Dock, DockingAnimation, "animations", addAnimation, getAnimation, xmlelement, mode);
        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);
    }

    void Dock::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);
    }


    bool Dock::execute(bool bTriggered, BaseObject* trigger)
    {
        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        Pawn* pawn = NULL;

        // Check whether it is a player trigger and extract pawn from it
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer()) {  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                COUT(4) << "Docking:execute PlayerTrigger was not triggered by a player.." << std::endl;
                return false;
            }
            pawn = pTrigger->getTriggeringPlayer();
        }
        else
        {
            COUT(4) << "Docking::execute Not a player trigger, can't extract pawn from it.." << std::endl;
            return false;
        }
        if(pawn == NULL)
        {
            COUT(4) << "Docking::execute Can't retrieve Pawn from Trigger. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }

        // Extract the PlayerInfo from the Pawn.
        PlayerInfo* player = pawn->getPlayer();
        if(player == NULL)
        {
            COUT(2) << "Docking::execute The PlayerInfo* is NULL." << std::endl;
            return false;
        }

        if(bTriggered)
        {
            // Add player to this Docks candidates
            candidates.insert(player);

            // Show docking dialog
            GUIManager::showGUI("DockingDialog");
        }
        else
        {
            // Remove player from candidates list
            candidates.erase(player);
        }

        return true;
    }


    void Dock::cmdDock()
    {
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->dock(player))
                break;
        }
    }

    void Dock::cmdUndock()
    {
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->undock(player))
                break;
        }
    }


    bool Dock::dock(PlayerInfo* player)
    {
        // Check if player is a candidate
        if(candidates.find(player) == candidates.end())
        {
            COUT(2) << "Dock::dock Player is not a candidate!" << std::endl;
            return false;
        }

        candidates.erase(player);
        docked.insert(player);

        if (animations.empty())
            return dockingAnimationFinished(player);
        else
            DockingAnimation::invokeAnimation(true, player, animations);

        return true;
    }

    bool Dock::dockingAnimationFinished(PlayerInfo* player)
    {
        if(docked.find(player) == docked.end())
        {
            COUT(2) << "Dock::dockingAnimationFinished Player is not currently docked." << std::endl;
            return false;
        }

        DockingEffect::invokeEffect(true, player, effects);
        return true;
    }

    bool Dock::undock(PlayerInfo* player)
    {
        // Check if player is docked to this Dock
        if(docked.find(player) == docked.end())
        {
            COUT(2) << "Dock::undock Player is not docked to this Dock." << std::endl;
            return false;
        }

        docked.erase(player);
        candidates.insert(player);

        DockingEffect::invokeEffect(false, player, effects);

        if (animations.empty())
            return undockingAnimationFinished(player);
        else
            DockingAnimation::invokeAnimation(false, player, animations);

        return true;
    }

    bool Dock::undockingAnimationFinished(PlayerInfo* player) {
        COUT(4) << "Dock::undockingAnimationFinished executed" << std::endl;
        return true;
    }


    unsigned int Dock::getNumberOfActiveDocks()
    {
        int i = 0;
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->candidates.find(player) != it->candidates.end())
                i++;
        }
        return i;
    }

    Dock* Dock::getActiveDockAtIndex(unsigned int index)
    {
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->candidates.find(player) != it->candidates.end())
            {
                if(index == 0)
                    return *it;
                index--;
            }
        }
        return NULL;
    }


    bool Dock::addEffect(DockingEffect* effect)
    {
        assert(effect);
        effects.push_back(effect);
        return true;
    }

    const DockingEffect* Dock::getEffect(unsigned int i) const
    {
        for (std::list<DockingEffect*>::const_iterator effect = this->effects.begin(); effect != this->effects.end(); ++effect)
        {
            if(i == 0)
               return *effect;
            i--;
        }
        return NULL;
    }

    bool Dock::addAnimation(DockingAnimation* animation)
    {
        assert(animation);
        animation->setParent(this);
        animations.push_back(animation);
        return true;
    }

    const DockingAnimation* Dock::getAnimation(unsigned int i) const
    {
        for (std::list<DockingAnimation*>::const_iterator animation = this->animations.begin(); animation != this->animations.end(); ++animation)
        {
            if(i == 0)
               return *animation;
            i--;
        }
        return NULL;
    }
}

