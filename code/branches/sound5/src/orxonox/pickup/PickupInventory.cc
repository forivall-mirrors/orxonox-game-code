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

#include <CEGUIImage.h>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUITabControl.h>

#include "core/ConsoleCommand.h"
#include "core/GUIManager.h"
#include "core/input/InputManager.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"

#include "EquipmentItem.h"
#include "PassiveItem.h"
#include "UsableItem.h"


namespace orxonox
{
    SetConsoleCommandShortcut(PickupInventory, toggleInventory);

    PickupInventory* PickupInventory::pickupInventory_s = NULL;
    PickupInventory* PickupInventory::getSingleton()
    {
        if(!PickupInventory::pickupInventory_s)
            PickupInventory::pickupInventory_s = new PickupInventory();

        return PickupInventory::pickupInventory_s;
    }

    PickupInventory::PickupInventory()
    {
        this->bInventoryVisible_ = false;
        this->visibleEquipmentWindows_ = this->visibleUsableWIndows_ = 0;

        // Create some windows to avoid creating them while playing
        CEGUI::WindowManager* winMgr = CEGUI::WindowManager::getSingletonPtr();
        for(int i = 0; i < 10; i++)
        {
            std::ostringstream id;
            id << i;

            PickupInventory::createItemWindows(winMgr, "equ/" + id.str(), i % 5, i / 5);
            PickupInventory::createItemWindows(winMgr, "use/" + id.str(), i % 5, i / 5);
        }
        this->createdEquipmentWindows_ = this->createdUsableWindows_ = 10;
    }
    PickupInventory::~PickupInventory()
    {
    }



    void PickupInventory::toggleInventory()
    {
        if(PickupInventory::getSingleton()->isVisible()) {
            GUIManager::hideGUI("PickupInventory");
        }
        else
        {
            GUIManager::showGUI("PickupInventory");
        }
        PickupInventory::getSingleton()->setVisible(!PickupInventory::getSingleton()->isVisible());
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

        const std::string& name = "pickup_" + item->getGUIImage();

        if(!CEGUI::ImagesetManager::getSingletonPtr()->isImagesetPresent(name))
        {
            CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile(name, item->getGUIImage(), "");
        }

        return ("set:" + name + " image:full_image");
    }

    void PickupInventory::clearInventory(CEGUI::WindowManager* winMgr, CEGUI::Window* equipPane, CEGUI::Window* usablePane)
    {
        for(unsigned int i = 0; i < this->visibleEquipmentWindows_; i++)
        {
            std::ostringstream id;
            id << i;

            winMgr->getWindow("orxonox/Inventory/Frame/equ/" + id.str())->setVisible(false);
            winMgr->getWindow("orxonox/Inventory/Title/equ/" + id.str())->setVisible(false);
            winMgr->getWindow("orxonox/Inventory/Items/equ/" + id.str())->setVisible(false);

            /*equipPane->removeChildWindow("orxonox/Inventory/Frame/equ/" + id.str());
            equipPane->removeChildWindow("orxonox/Inventory/Title/equ/" + id.str());
            equipPane->removeChildWindow("orxonox/Inventory/Items/equ/" + id.str());*/
        }
        for(unsigned int i = 0; i < this->visibleUsableWIndows_; i++)
        {
            std::ostringstream id;
            id << i;

            winMgr->getWindow("orxonox/Inventory/Frame/use/" + id.str())->setVisible(false);
            winMgr->getWindow("orxonox/Inventory/Title/use/" + id.str())->setVisible(false);
            winMgr->getWindow("orxonox/Inventory/Items/use/" + id.str())->setVisible(false);

            /*usablePane->removeChildWindow("orxonox/Inventory/Frame/use/" + id.str());
            usablePane->removeChildWindow("orxonox/Inventory/Title/use/" + id.str());
            usablePane->removeChildWindow("orxonox/Inventory/Items/use/" + id.str());*/
        }
    }
    void PickupInventory::updateTabs(CEGUI::WindowManager *winMgr, CEGUI::Window *equipWindow, CEGUI::Window *usableWindow)
    {
        this->updateEquipment(winMgr, equipWindow);
        this->updateUsable(winMgr, usableWindow);
    }

    void PickupInventory::updateEquipment(CEGUI::WindowManager* winMgr, CEGUI::Window* target)
    {
        Pawn* pawn;
        if((pawn = HumanController::getLocalControllerEntityAsPawn()))
        {
            std::deque<EquipmentItem*> items = pawn->getPickups().getEquipmentItems();
            for(unsigned int i = 0; i < items.size(); i++)
            {
                std::ostringstream id;
                id << "equ/" << i;

                EquipmentItem* item = items.at(i);

                if(this->createdEquipmentWindows_ <= i)
                {
                    PickupInventory::createItemWindows(winMgr, id.str(), i % 5, i / 5);
                    this->createdEquipmentWindows_++;
                }

                PickupInventory::setWindowProperties(winMgr, target, id.str(), item, "FFFFFFFF");
            }
            this->visibleEquipmentWindows_ = items.size();
        }
    }
    void PickupInventory::updateUsable(CEGUI::WindowManager* winMgr, CEGUI::Window* target)
    {
        Pawn* pawn;
        if((pawn = HumanController::getLocalControllerEntityAsPawn()))
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

                if(this->createdUsableWindows_ <= i)
                {
                    PickupInventory::createItemWindows(winMgr, id.str(), i % 5, i / 5);
                    this->createdUsableWindows_++;
                }

                PickupInventory::setWindowProperties(winMgr, target, id.str(), item, colour);
            }
            this->visibleUsableWIndows_ = items.size();
        }
    }

    void PickupInventory::createItemWindows(CEGUI::WindowManager* winMgr, const std::string& id, int x, int y)
    {
        if(!winMgr) { return; }

        CEGUI::Window* frame = winMgr->createWindow("TaharezLook/StaticImage", "orxonox/Inventory/Frame/" + id);
        frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 5.0f + x * 70), CEGUI::UDim(0, 5.0f + y * 90)));
        frame->setSize(CEGUI::UVector2(CEGUI::UDim(0, 65), CEGUI::UDim(0, 65)));
        frame->setRiseOnClickEnabled(false);
        frame->setVisible(false);

        CEGUI::Window* text = winMgr->createWindow("TaharezLook/StaticText", "orxonox/Inventory/Title/" + id);
        text->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 5.0f + x * 70), CEGUI::UDim(0, 70.0f + y * 90)));
        text->setSize(CEGUI::UVector2(CEGUI::UDim(0, 65), CEGUI::UDim(0, 20)));
        text->setProperty("FrameEnabled", "False");
        text->setProperty("BackgroundEnabled", "False");
        text->setProperty("HorzFormatting", "HorzCentred");
        text->setProperty("VertFormatting", "VertCentred");
        text->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
        text->setVisible(false);

        CEGUI::Window* btn = winMgr->createWindow("TaharezLook/Button", "orxonox/Inventory/Items/" + id);
        btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 8.0f + x * 70), CEGUI::UDim(0, 8.0f + y * 90)));
        btn->setSize(CEGUI::UVector2(CEGUI::UDim(0, 59), CEGUI::UDim(0, 59)));
        btn->subscribeScriptedEvent("Clicked", "PickupInventory.itemClicked");
        btn->setVisible(false);
    }
    void PickupInventory::setWindowProperties(CEGUI::WindowManager* winMgr, CEGUI::Window* target, const std::string& id, const BaseItem* item, const std::string& textColour)
    {
        CEGUI::Window* txt = winMgr->getWindow("orxonox/Inventory/Title/" + id);
        CEGUI::Window* btn = winMgr->getWindow("orxonox/Inventory/Items/" + id);
        CEGUI::Window* frm = winMgr->getWindow("orxonox/Inventory/Frame/" + id);

        frm->setVisible(true);

        txt->setVisible(true);
        txt->setProperty("Text", item->getGUIText());
        txt->setProperty("TextColours", "tl:" + textColour + " tr:" + textColour + " bl:" + textColour + " br:" + textColour);

        const std::string& image = PickupInventory::getImageForItem(item);
        btn->setVisible(true);
        btn->setProperty("NormalImage", image);
        btn->setProperty("HoverImage", image);
        btn->setProperty("PushedImage", image);
        btn->setProperty("DisabledImage", image);
        btn->setProperty("Tooltip", item->getGUIText());

        target->addChildWindow(frm);
        target->addChildWindow(txt);
        target->addChildWindow(btn);
    }
}
