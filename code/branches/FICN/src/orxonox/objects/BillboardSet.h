#ifndef _BillboardSet_H__
#define _BillboardSet_H__

#include <OgreBillboardSet.h>

#include "../OrxonoxPrereqs.h"

#include "misc/String.h"
#include "../core/CoreIncludes.h"
#include "misc/ColourValue.h"
#include "misc/Vector3.h"

namespace orxonox
{
    class BillboardSet
    {
        public:
            BillboardSet();
            ~BillboardSet();
            void setBillboardSet(const String& file, const ColourValue& colour = ColourValue(1.0, 1.0, 1.0), int count = 1, const Vector3& position = Vector3::ZERO);

            inline Ogre::BillboardSet* getBillboardSet()
                { return this->billboardSet_; }

            inline const String& getName() const
                { return this->billboardSet_->getName(); }

        private:
            static unsigned int billboardSetCounter_s;
            Ogre::BillboardSet* billboardSet_;
    };
}

#endif /* _BillboardSet_H__ */
