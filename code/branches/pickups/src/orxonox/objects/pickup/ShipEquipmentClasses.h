
#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
#include "SpaceShip.h"
#include <map>


/*
multimap<std::string, Item*> equipment_;
equipment_.insert(pair<std::string, Item*>("Weapon", new Item()));*/


/*				std::map<std::itemtype, Item*> EQClasses;
           	EQClasses["jacke"] = 0;
            Item* item = itemMap_["jacke"];
           
           if (itemMap_["jacke"])
           if (itemMap_.find("jacke") != itemMap_.end()) */
namespaces orxonox{
	
	class _OrxonoxExport ShipEquipment{
	public:
		void AddItem(Shipitem toAddItem);
		void RemoveItem(Shipitem toRemoveItem);
		bool CheckifValid(Shipitem toBeChecked);
		int 
	private:
		std::map<std::string, Item*> Shipitems;
		
				
	















#endif