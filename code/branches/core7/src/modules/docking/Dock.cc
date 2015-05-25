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
#include "core/command/ConsoleCommandIncludes.h"
#include "network/NetworkFunctionIncludes.h"

#include "infos/HumanPlayer.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    RegisterClass(Dock);

    SetConsoleCommand("Dock", "dock",    &Dock::cmdDock).addShortcut().setAsInputCommand();
    SetConsoleCommand("Dock", "undock",  &Dock::cmdUndock).addShortcut().setAsInputCommand();

    registerStaticNetworkFunction(Dock::showDockingDialog);

    Dock::Dock(Context* context) : StaticEntity(context)
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
        XMLPortEventSink(Dock, BaseObject, "undocking", undocking, xmlelement, mode);

    }

    void Dock::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);

        XMLPortEventSink(Dock, BaseObject, "undocking", undocking, xmlelement, mode);
    }


    bool Dock::undocking(bool bTriggered, BaseObject* trigger)
    {

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        PlayerInfo* player = NULL;

        // Check whether it is a player trigger and extract pawn from it
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer()) {  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
            orxout(verbose, context::docking) << "Docking:execute PlayerTrigger was not triggered by a player.." << endl;
            return false;
            }
            player = pTrigger->getTriggeringPlayer();
        }
        else
        {
            orxout(verbose, context::docking) << "Docking::execute Not a player trigger, can't extract pawn from it.." << endl;
            return false;
        }
        if(player == NULL)
        {
            orxout(verbose, context::docking) << "Docking::execute Can't retrieve PlayerInfo from Trigger. (" << trigger->getIdentifier()->getName() << ")" << endl;
            return false;
        }

        if(bTriggered)
        {
            // Add player to this Docks candidates
            candidates_.insert(player);

            // Show docking dialog
            this->showUndockingDialogHelper(player);
        }
        else
        {
            // Remove player from candidates list
            candidates_.erase(player);
        }

        return true;
    }



    bool Dock::execute(bool bTriggered, BaseObject* trigger)
    {
        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        PlayerInfo* player = NULL;

        // Check whether it is a player trigger and extract pawn from it
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer()) {  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                orxout(verbose, context::docking) << "Docking:execute PlayerTrigger was not triggered by a player.." << endl;
                return false;
            }
            player = pTrigger->getTriggeringPlayer();
        }
        else
        {
            orxout(verbose, context::docking) << "Docking::execute Not a player trigger, can't extract pawn from it.." << endl;
            return false;
        }
        if(player == NULL)
        {
            orxout(verbose, context::docking) << "Docking::execute Can't retrieve PlayerInfo from Trigger. (" << trigger->getIdentifier()->getName() << ")" << endl;
            return false;
        }

        if(bTriggered)
        {
            // Add player to this Docks candidates
            candidates_.insert(player);

            // Show docking dialog
            this->showDockingDialogHelper(player);
        }
        else
        {
            // Remove player from candidates list
            candidates_.erase(player);
        }

        return true;
    }


    void Dock::showUndockingDialogHelper(PlayerInfo* player)
        {
            assert(player);

            if(!player->isHumanPlayer())
                return;

            if(GameMode::isMaster())
            {
                if(GameMode::showsGraphics())
                    GUIManager::showGUI("UndockingDialog");
            }
            else
                callStaticNetworkFunction(Dock::showDockingDialog, player->getClientID());

        }

    void Dock::showDockingDialogHelper(PlayerInfo* player)
    {
        assert(player);
        
        if(!player->isHumanPlayer())
            return;
        
        if(GameMode::isMaster())
        {
            if(GameMode::showsGraphics())
                GUIManager::showGUI("DockingDialog");
        }
        else
            callStaticNetworkFunction(Dock::showDockingDialog, player->getClientID());

    }

    /*static*/ void Dock::showDockingDialog()
    {
        if(GameMode::showsGraphics())
            GUIManager::showGUI("DockingDialog");
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
        if(candidates_.find(player) == candidates_.end())
        {
            orxout(internal_warning, context::docking) << "Dock::dock Player is not a candidate!" << endl;
            return false;
        }

        candidates_.erase(player);
        docked_.insert(player);

        if (animations_.empty())
            return dockingAnimationFinished(player);

        else
            DockingAnimation::invokeAnimation(true, player, animations_);

        return true;
    }

    bool Dock::dockingAnimationFinished(PlayerInfo* player)
    {
        if(docked_.find(player) == docked_.end())
        {
            orxout(internal_warning, context::docking) << "Dock::dockingAnimationFinished Player is not currently docked." << endl;
            return false;
        }

        DockingEffect::invokeEffect(true, player, effects_);
        return true;
    }

    bool Dock::undock(PlayerInfo* player)
    {
        // Check if player is docked to this Dock
        if(docked_.find(player) == docked_.end())
        {
            orxout(internal_warning, context::docking) << "Dock::undock Player is not docked to this Dock." << endl;
            return false;
        }

        docked_.erase(player);
        candidates_.insert(player);

        DockingEffect::invokeEffect(false, player, effects_);

        if (animations_.empty())
            return undockingAnimationFinished(player);
        else
            DockingAnimation::invokeAnimation(false, player, animations_);

        return true;
    }

    bool Dock::undockingAnimationFinished(PlayerInfo* player) {
        orxout(verbose, context::docking) << "Dock::undockingAnimationFinished executed" << endl;
        return true;
    }

    unsigned int Dock::getNumberOfActiveDocks()
    {
        int i = 0;
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->candidates_.find(player) != it->candidates_.end())
                i++;
        }
        return i;
    }

    Dock* Dock::getActiveDockAtIndex(unsigned int index)
    {
        PlayerInfo* player = HumanController::getLocalControllerSingleton()->getPlayer();
        for(ObjectList<Dock>::iterator it = ObjectList<Dock>::begin(); it != ObjectList<Dock>::end(); ++it)
        {
            if(it->candidates_.find(player) != it->candidates_.end())
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
        effects_.push_back(effect);
        return true;
    }

    const DockingEffect* Dock::getEffect(unsigned int i) const
    {
        for (std::list<DockingEffect*>::const_iterator effect = this->effects_.begin(); effect != this->effects_.end(); ++effect)
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
        animations_.push_back(animation);
        return true;
    }

    const DockingAnimation* Dock::getAnimation(unsigned int i) const
    {
        for (std::list<DockingAnimation*>::const_iterator animation = this->animations_.begin(); animation != this->animations_.end(); ++animation)
        {
            if(i == 0)
               return *animation;
            i--;
        }
        return NULL;
    }
}
