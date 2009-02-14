
#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
#include "SpaceShip.h"
#include <map>


/*
multimap<std::string, BaseItem*> equipment_;
equipment_.insert(pair<std::string, BaseItem*>("Weapon", new BaseItem()));*/


/*          std::map<std::itemtype, BaseItem*> EQClasses;
            EQClasses["jacke"] = 0;
            BaseItem* item = itemMap_["jacke"];

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
            multimap<std::string, BaseItem*> Equipment;
            multimap<std::string, BaseItem*> Usable;
            multimap<std::string, BaseItem*> Trunk;
    };
}














#endif
