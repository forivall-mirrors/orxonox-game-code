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

#include "core/BaseObject.h"

namespace CEGUI { class Window; }

// tolua_begin
namespace orxonox
{
    class EquipmentItem;
    class PassiveItem;
    class UsableItem;

    /**
        @brief Static class for the inventory GUI window.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport PickupInventory
    {
// tolua_end
    public:
        //PickupInventory(BaseObject* creator);
        //virtual ~PickupInventory();

        /**
            @brief Toggle visiblity of inventory.
        */
        static void toggleInventory(); // tolua_export

        static void tabChanged(CEGUI::Window* tab); // tolua_export

        static unsigned int getEquipmentCount(); // tolua_export
        static unsigned int getUsableCount(); // tolua_export
        static unsigned int getPassiveCount(); // tolua_export

        static EquipmentItem* getEquipmentItem(unsigned int i); // tolua_export
        static UsableItem* getUsableItem(unsigned int i); // tolua_export
        static PassiveItem* getPassiveItem(unsigned int i); // tolua_export

        static std::string getImagesetForEquipment(unsigned int i); // tolua_export
        static std::string getImagesetForUsable(unsigned int i); // tolua_export 
        static std::string getImagesetForPassive(unsigned int i); // tolua_export
    }; // tolua_export
} // tolua_export

#endif /* _PickupInventory_H__ */
