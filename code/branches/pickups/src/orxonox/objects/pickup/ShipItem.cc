#include "Shipitem.h"

namespace orxonox
{
    itemtype ShipItem::CheckType()
    {
        return this.type;
    }

    subItemTypePermanent CheckSubType()
    {
        return this.subtype_permanent;
    }

    subItemTypePowerups CheckSubType()
    {
        return this.subtype_powerups;
    }
    subItemTypeUsable CheckSubType()
    {
        return this.subtype_usables;
    }

}
