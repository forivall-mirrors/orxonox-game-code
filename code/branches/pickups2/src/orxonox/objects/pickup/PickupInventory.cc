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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      ...
 *
 */

#include "PickupInventory.h"

#include "EquipmentItem.h"
#include "UsableItem.h"
#include "PassiveItem.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"

#include "gui/GUIManager.h"
#include "objects/controllers/HumanController.h"
#include "objects/worldentities/pawns/SpaceShip.h"

#include <CEGUIImagesetManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUITabControl.h>

namespace orxonox
{
    orxonox::ConsoleCommand& function_PickupInventory_ToggleInventory_0 =
        orxonox::CommandExecutor::addConsoleCommandShortcut(orxonox::createConsoleCommand(orxonox::createFunctor(&PickupInventory::toggleInventory), "toggleInventory"), true);

    static bool bInventoryVisible_ = false;
    void PickupInventory::toggleInventory()
    {
        if(bInventoryVisible_) {
            GUIManager::getInstancePtr()->executeCode("hideGUI(\"PickupInventory\")");
            GUIManager::getInstancePtr()->executeCode("hideCursor()");
            InputManager::getInstance().requestLeaveState("guiMouseOnly");
        }
        else
        {
            GUIManager::getInstancePtr()->showGUI("PickupInventory");
            GUIManager::getInstancePtr()->executeCode("showCursor()");
            InputManager::getInstance().requestEnterState("guiMouseOnly");
        }
        bInventoryVisible_ = !bInventoryVisible_;
    }
    void PickupInventory::tabChanged(CEGUI::Window *tab)
    {
        CEGUI::TabControl* tabCtrl = dynamic_cast<CEGUI::TabControl*>(tab);
        if(tabCtrl != NULL)
        {
            COUT(1) << "tabChanged() tab index: " << tabCtrl->getSelectedTabIndex() << std::endl;
        }
        else
        {
            COUT(1) << "tabChanged() argument is no CEGUI::TabControl!" << std::endl;
        }
    }
    unsigned int PickupInventory::getEquipmentCount()
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
                return pawn->getPickups().getEquipmentItems().size();

        }
        return 0;
    }
    unsigned int PickupInventory::getUsableCount()
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
                return pawn->getPickups().getUsableItems().size();

        }
        return 0;
    }
    unsigned int PickupInventory::getPassiveCount()
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
                return pawn->getPickups().getPassiveItems().size();

        }
        return 0;
    }
    EquipmentItem* PickupInventory::getEquipmentItem(unsigned int i)
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
            {
                std::deque<EquipmentItem*> l = pawn->getPickups().getEquipmentItems();
                if (i >= l.size()) { return NULL; }
                return l.at(i);
            }

        }
        return NULL;
    }
    UsableItem* PickupInventory::getUsableItem(unsigned int i)
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
            {
                std::deque<UsableItem*> l = pawn->getPickups().getUsableItems();
                if (i >= l.size()) { return NULL; }
                return l.at(i);
            }

        }
        return NULL;
    }
    PassiveItem* PickupInventory::getPassiveItem(unsigned int i)
    {
        if (HumanController::getLocalControllerSingleton() && HumanController::getLocalControllerSingleton()->getControllableEntity())
        {
            Pawn* pawn = dynamic_cast<Pawn*>(HumanController::getLocalControllerSingleton()->getControllableEntity());
            if(pawn)
            {
                std::deque<PassiveItem*> l = pawn->getPickups().getPassiveItems();
                if (i >= l.size()) { return NULL; }
                return l.at(i);
            }

        }
        return NULL;
    }
    std::string PickupInventory::getImagesetForEquipment(unsigned int i)
    {
        EquipmentItem* item = PickupInventory::getEquipmentItem(i);
        if(!item)
            return "";

        std::string name = "pickup_" + item->getGUIImage();

        if(!CEGUI::ImagesetManager::getSingletonPtr()->isImagesetPresent(name)) {
            CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile(name, item->getGUIImage(), "pickups");
        }

        return name;
    }
    std::string PickupInventory::getImagesetForUsable(unsigned int i)
    {
        UsableItem* item = PickupInventory::getUsableItem(i);
        if(!item)
            return "";

        std::string name = "pickup_" + item->getGUIImage();

        if(!CEGUI::ImagesetManager::getSingletonPtr()->isImagesetPresent(name)) {
            CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile(name, item->getGUIImage(), "pickups");
        }

        return name;
    }
    std::string PickupInventory::getImagesetForPassive(unsigned int i)
    {
        PassiveItem* item = PickupInventory::getPassiveItem(i);
        if(!item)
            return "";

        std::string name = "pickup_" + item->getGUIImage();

        if(!CEGUI::ImagesetManager::getSingletonPtr()->isImagesetPresent(name)) {
            CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile(name, item->getGUIImage(), "pickups");
        }

        return name;
    }
}
