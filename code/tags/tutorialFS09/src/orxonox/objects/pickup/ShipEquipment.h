#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
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
    class BaseItem;
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
	bool insert(BaseItem* item);
	bool erase (BaseItem* item);
	//void print(std::multimap<std::string, BaseItem*> eut);
	void eraseAll();
	//bool checkSlot(BaseItem* item);
	BaseItem* checkSlot(BaseItem* item);
//	const std::multimap<std::string, BaseItem*>& getEquipment() const { return this->Equipment; }
	inline std::multimap<std::string, BaseItem*>& getEquipment() {return this->Equipment;}
	inline std::multimap<std::string, BaseItem*>& getUsable() {return this->Usable;}
	inline std::multimap<std::string, BaseItem*>& getTrunk() {return this->Trunk;}
	inline Pawn* getPlayer() {return this->player ;}
	inline void setPlayer(Pawn* setplayer)
	{this->player = setplayer;}
        private:
            Pawn* player;//!< Is a pointer to the belonging player
            std::multimap<std::string, BaseItem*> Equipment;//!< the Equipment for permanent Items
            std::multimap<std::string, BaseItem*> Usable;//!< Where Usables are stored
            std::multimap<std::string, BaseItem*> Trunk;//!< Every other Item is stored here
    };
}














#endif
