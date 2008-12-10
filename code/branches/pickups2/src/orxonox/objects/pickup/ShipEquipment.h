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
/**
@brief
    ShipEquipment is the inventory of a player. It's part of the Pawn class.

*/
    class _OrxonoxExport ShipEquipment
    {
        public:
	inline int getSpace()
	{
	return Usable.size()+Trunk.size();
	};
	bool insert(Item* item);
	bool erase (Item* item);
	//void print(std::multimap<std::string, Item*> eut);
	void eraseAll();
	//bool checkSlot(Item* item);
	Item* checkSlot(Item* item);
//	const std::multimap<std::string, Item*>& getEquipment() const { return this->Equipment; }
	inline std::multimap<std::string, Item*>& getEquipment() {return this->Equipment;}
	inline std::multimap<std::string, Item*>& getUsable() {return this->Usable;}
	inline std::multimap<std::string, Item*>& getTrunk() {return this->Trunk;}
	inline Pawn* getPlayer() {return this->player ;}
	inline void setPlayer(Pawn* setplayer)
	{this->player = setplayer;}
        private:
            Pawn* player;//!< Is a pointer to the belonging player
            std::multimap<std::string, Item*> Equipment;//!< the Equipment for permanent Items
            std::multimap<std::string, Item*> Usable;//!< Where Usables are stored
            std::multimap<std::string, Item*> Trunk;//!< Every other Item is stored here
    };
}














#endif
