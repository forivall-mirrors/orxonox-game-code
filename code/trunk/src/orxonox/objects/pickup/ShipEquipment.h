
#ifndef _ShipEquipment_H__
#define _ShipEquipment_H__
#include <string>
#include <multimap>


/*
multimap<std::string, Item*> equipment_;
equipment_.insert(pair<std::string, Item*>("Weapon", new Item()));*/


namespace orxonox{

	class _OrxonoxExport ShipEquipment{
	public:
		void AddItem(Shipitem toAddItem);
		void RemoveItem(Shipitem toRemoveItem);
		bool CheckifValid(Shipitem toBeChecked);
		int
	private:
		std::multimap<std::string, Item*> Equipment;
	};
}


















#endif
