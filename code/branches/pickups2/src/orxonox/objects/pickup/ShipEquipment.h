#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
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
    class Item;

    class _OrxonoxExport ShipEquipment
    {
        public:
	inline int getSpace()
	{
	return Usable.size()+Trunk.size();
	};
	bool insert(Item* item);
	bool erase (Item* item);
	void eraseAll();
	bool checkSlot(Item* item);
//	const std::multimap<std::string, Item*>& getEquipment() const { return this->Equipment; }
	inline std::multimap<std::string, Item*>& getEquipment() {return this->Equipment;}
	inline std::multimap<std::string, Item*>& getUsable() {return this->Usable;}
	inline std::multimap<std::string, Item*>& getTrunk() {return this->Trunk;}
	inline Pawn* getPlayer() {return this->player ;}
	inline void setPlayer(Pawn* setplayer)
	{this->player = setplayer;}

        private:
            Pawn* player;
            std::multimap<std::string, Item*> Equipment;
            std::multimap<std::string, Item*> Usable;
            std::multimap<std::string, Item*> Trunk;
    };
}














#endif
