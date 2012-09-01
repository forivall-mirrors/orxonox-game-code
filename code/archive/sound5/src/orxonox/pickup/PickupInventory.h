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

/**
    @file
    @brief Declaration of static class for the inventory GUI window.
*/

#ifndef _PickupInventory_H__
#define _PickupInventory_H__

#include "OrxonoxPrereqs.h"

#include <CEGUIForwardRefs.h>
#include "core/BaseObject.h"

// tolua_begin
namespace orxonox
{
    /**
        @brief Static class for the inventory GUI window.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport PickupInventory
    {
// tolua_end
    public:
        PickupInventory();
        virtual ~PickupInventory();

        static PickupInventory* getSingleton(); // tolua_export

        static void toggleInventory(); // tolua_export

        static unsigned int getEquipmentCount(); // tolua_export
        static unsigned int getUsableCount(); // tolua_export
        static unsigned int getPassiveCount(); // tolua_export

        static unsigned int getCurrentUsableIndex(); // tolua_export
        static bool isCurrentUsable(const BaseItem* item); // tolua_export
        static void selectUsable(unsigned int i); // tolua_export

        static BaseItem* getEquipmentItem(unsigned int i); // tolua_export
        static BaseItem* getUsableItem(unsigned int i); // tolua_export
        static BaseItem* getPassiveItem(unsigned int i); // tolua_export

        static std::string getImageForItem(const BaseItem* item); // tolua_export

        void clearInventory(CEGUI::WindowManager* winMgr, CEGUI::Window* equipPane, CEGUI::Window* usablePane); // tolua_export
        void updateTabs(CEGUI::WindowManager* winMgr, CEGUI::Window* equipWindow, CEGUI::Window* usableWindow); // tolua_export

        void updateEquipment(CEGUI::WindowManager* winMgr, CEGUI::Window* target);
        void updateUsable(CEGUI::WindowManager* winMgr, CEGUI::Window* target);

        static void createItemWindows(CEGUI::WindowManager* winMgr, const std::string& id, int x, int y);
        static void setWindowProperties(CEGUI::WindowManager* winMgr, CEGUI::Window* target, const std::string& id, const BaseItem* item, const std::string& textColour);

        const bool isVisible() const
            { return this->bInventoryVisible_; }
        void setVisible(bool visible)
            { this->bInventoryVisible_ = visible; }
    private:
        bool bInventoryVisible_;
        unsigned int createdEquipmentWindows_;
        unsigned int createdUsableWindows_;
        unsigned int visibleEquipmentWindows_;
        unsigned int visibleUsableWIndows_;

        static PickupInventory* pickupInventory_s;
    }; // tolua_export
} // tolua_export

#endif /* _PickupInventory_H__ */
