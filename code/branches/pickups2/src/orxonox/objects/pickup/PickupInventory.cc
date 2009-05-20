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
#include "PassiveItem.h"
#include "UsableItem.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"

#include "gui/GUIManager.h"
#include "objects/controllers/HumanController.h"
#include "objects/worldentities/pawns/Pawn.h"

#include <CEGUIImage.h>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
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

    unsigned int PickupInventory::getCurrentUsableIndex()
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn && pawn->getPickups().getCurrentUsable())
        {
            UsableItem* use = pawn->getPickups().getCurrentUsable();
            std::deque<UsableItem*> items = pawn->getPickups().getUsableItems();
            for(unsigned int i = 0; i < items.size(); i++)
            {
                if(items.at(i) == use)
                    return i;
            }
        }

        return 0;
    }
    bool PickupInventory::isCurrentUsable(const BaseItem* item)
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
            return (pawn->getPickups().getCurrentUsable() == item);
        else
            return false;
    }
    void PickupInventory::selectUsable(unsigned int i)
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
        {
            std::deque<UsableItem*> items = pawn->getPickups().getUsableItems();
            if(i < items.size())
                pawn->getPickups().setCurrentUsable(items.at(i));
        }
    }

    unsigned int PickupInventory::getEquipmentCount()
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
            return pawn->getPickups().getEquipmentItems().size();
        else
            return 0;
    }
    unsigned int PickupInventory::getUsableCount()
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
            return pawn->getPickups().getUsableItems().size();
        else
            return 0;
    }
    unsigned int PickupInventory::getPassiveCount()
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
            return pawn->getPickups().getPassiveItems().size();
        else
            return 0;
    }
    BaseItem* PickupInventory::getEquipmentItem(unsigned int i)
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
        {
            std::deque<EquipmentItem*> l = pawn->getPickups().getEquipmentItems();
            if (i >= l.size()) { return NULL; }
            return l.at(i);
        }
        else
            return NULL;
    }
    BaseItem* PickupInventory::getUsableItem(unsigned int i)
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
        {
            std::deque<UsableItem*> l = pawn->getPickups().getUsableItems();
            if (i >= l.size()) { return NULL; }
            return l.at(i);
        }
        else
            return NULL;
    }
    BaseItem* PickupInventory::getPassiveItem(unsigned int i)
    {
        Pawn* pawn = HumanController::getLocalControllerEntityAsPawn();
        if(pawn)
        {
            std::deque<PassiveItem*> l = pawn->getPickups().getPassiveItems();
            if (i >= l.size()) { return NULL; }
            return l.at(i);
        }
        else
            return NULL;
    }

    std::string PickupInventory::getImageForItem(const BaseItem* item)
    {
        if(!item)
            return "";

        std::string name = "pickup_" + item->getGUIImage();

        if(!CEGUI::ImagesetManager::getSingletonPtr()->isImagesetPresent(name))
        {
            CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile(name, item->getGUIImage(), "");
        }

        return "set:" + name + " image:full_image";
    }

    void PickupInventory::clearInventory(CEGUI::WindowManager* winMgr, int equipCount, int usableCount)
    {
        for(int i = 0; i < equipCount; i++)
        {
            std::ostringstream id;
            id << i;

            winMgr->destroyWindow("orxonox/Inventory/Frame/equ/" + id.str());
            winMgr->destroyWindow("orxonox/Inventory/Title/equ/" + id.str());
            winMgr->destroyWindow("orxonox/Inventory/Items/equ/" + id.str());
        }
        for(int i = 0; i < usableCount; i++)
        {
            std::ostringstream id;
            id << i;

            std::string s = id.str();
            winMgr->destroyWindow("orxonox/Inventory/Frame/use/" + id.str());
            winMgr->destroyWindow("orxonox/Inventory/Title/use/" + id.str());
            winMgr->destroyWindow("orxonox/Inventory/Items/use/" + id.str());
        }
    }
    void PickupInventory::updateTabs(CEGUI::WindowManager *winMgr, CEGUI::Window *equipWindow, CEGUI::Window *usableWindow)
    {
        PickupInventory::updateEquipment(winMgr, equipWindow);
        PickupInventory::updateUsable(winMgr, usableWindow);
    }

    void PickupInventory::updateEquipment(CEGUI::WindowManager* winMgr, CEGUI::Window* target)
    {
        Pawn* pawn;
        if(pawn = HumanController::getLocalControllerEntityAsPawn())
        {
            std::deque<EquipmentItem*> items = pawn->getPickups().getEquipmentItems();
            for(unsigned int i = 0; i < items.size(); i++)
            {
                std::ostringstream id;
                id << "equ/" << i;

                EquipmentItem* item = items.at(i);

                PickupInventory::addItem(winMgr, target, id.str(), item, "FFFFFFFF", i % 5, i / 5);
            }
        }
    }
    void PickupInventory::updateUsable(CEGUI::WindowManager* winMgr, CEGUI::Window* target)
    {
        Pawn* pawn;
        if(pawn = HumanController::getLocalControllerEntityAsPawn())
        {
            std::deque<UsableItem*> items = pawn->getPickups().getUsableItems();
            for(unsigned int i = 0; i < items.size(); i++)
            {
                std::ostringstream id;
                id << "use/" << i;

                UsableItem* item = items.at(i);
                std::string colour;

                if(PickupInventory::isCurrentUsable(item))
                    colour = "FFFF5555";
                else
                    colour = "FFFFFFFF";

                PickupInventory::addItem(winMgr, target, id.str(), item, colour, i % 5, i / 5);
            }
        }
    }

    void PickupInventory::addItem(CEGUI::WindowManager* winMgr, CEGUI::Window* target, const std::string& id, BaseItem* item, const std::string& titleColour, int x, int y)
    { 
        if(!winMgr || !target || !item) { return; }

        std::string image = PickupInventory::getImageForItem(item);

        CEGUI::Window* frame = winMgr->createWindow("TaharezLook/StaticImage", "orxonox/Inventory/Frame/" + id);
        frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 5 + x * 70), CEGUI::UDim(0, 5 + y * 90)));
        frame->setSize(CEGUI::UVector2(CEGUI::UDim(0, 65), CEGUI::UDim(0, 65)));

        CEGUI::Window* text = winMgr->createWindow("TaharezLook/StaticText", "orxonox/Inventory/Title/" + id);
        text->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 5 + x * 70), CEGUI::UDim(0, 70 + y * 90)));
        text->setSize(CEGUI::UVector2(CEGUI::UDim(0, 65), CEGUI::UDim(0, 20)));
        text->setProperty("Text", item->getGUIText());
        text->setProperty("FrameEnabled", "False");
        text->setProperty("BackgroundEnabled", "False");
        text->setProperty("HorzFormatting", "HorzCentred");
        text->setProperty("VertFormatting", "VertCentred");
        text->setProperty("TextColours", "tl:" + titleColour + " tr:" + titleColour + " bl:" + titleColour + " br:" + titleColour + "");

        CEGUI::Window* btn = winMgr->createWindow("TaharezLook/Button", "orxonox/Inventory/Items/" + id);
        btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 8 + x * 70), CEGUI::UDim(0, 8 + y * 90)));
        btn->setSize(CEGUI::UVector2(CEGUI::UDim(0, 59), CEGUI::UDim(0, 59)));
        btn->setProperty("NormalImage", image);
        btn->setProperty("HoverImage", image);
        btn->setProperty("PushedImage", image);
        btn->setProperty("DisabledImage", image);
        btn->setProperty("Tooltip", item->getGUIText());
        btn->subscribeScriptedEvent("Clicked", "itemClicked");

        target->addChildWindow(text);
        target->addChildWindow(frame);
        target->addChildWindow(btn);
    }
}
