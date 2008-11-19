
#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
#include "SpaceShip.h"
#include <map>


/*
multimap<std::string, Item*> equipment_;
equipment_.insert(pair<std::string, Item*>("Weapon", new Item()));*/


/*          std::map<std::itemtype, Item*> EQClasses;
            EQClasses["jacke"] = 0;
            Item* item = itemMap_["jacke"];

           if (itemMap_["jacke"])
           if (itemMap_.find("jacke") != itemMap_.end()) */
namespace orxonox
{
    class _OrxonoxExport ShipEquipment
    {
        public:
            /*void AddItem(Shipitem* toAddItem);
            void RemoveItem(Shipitem* toRemoveItem);
            bool CheckifValid(Shipitem* toBeChecked);
            bool CheckifSpace();
            void SwitchItem(Permanent* toSwitchItem);
            string GetNameofPermanent (subItemTypePermanent NametoGet);*/ //holt den Namen des getragenen Items im jeweiligen Slot.

        private:
            multimap<std::string, Item*> Equipment;
            multimap<std::string, Item*> Usable;
            multimap<std::string, Item*> Trunk;
    };
}














#endif
