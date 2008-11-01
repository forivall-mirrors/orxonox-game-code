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
}
