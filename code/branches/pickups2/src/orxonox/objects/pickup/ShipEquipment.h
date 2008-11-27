
#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
#include <map>
#include "Item.h"


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
	inline int getSpace()
	{
	return Usable.size()+Trunk.size();
	};
	bool insert(Item* item);
	bool erase (Item* item);
	bool checkSlot(Item* item) const;
//	const std::multimap<std::string, Item*>& getEquipment() const { return this->Equipment; }

        private:
            std::multimap<std::string, Item*> Equipment;
            std::multimap<std::string, Item*> Usable;
            std::multimap<std::string, Item*> Trunk;
    };
}














#endif
