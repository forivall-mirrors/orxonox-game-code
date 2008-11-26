
#include "ShipEquipment.h"

namespace orxonox
{
	ShipEquipmentClass::int getSpace()
	{
	return Usable.size()+Trunk.size();
	}
}
