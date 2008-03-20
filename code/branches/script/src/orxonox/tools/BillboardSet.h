#ifndef _BillboardSet_H__
#define _BillboardSet_H__

#include <string>

#include <OgreBillboardSet.h>

#include "../OrxonoxPrereqs.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport BillboardSet
    {
        public:
            BillboardSet();
            ~BillboardSet();
            void setBillboardSet(const std::string& file, const ColourValue& colour = ColourValue(1.0, 1.0, 1.0), int count = 1, const Vector3& position = Vector3::ZERO);

            inline Ogre::BillboardSet* getBillboardSet()
                { return this->billboardSet_; }

            inline const std::string& getName() const
                { return this->billboardSet_->getName(); }

        private:
            static unsigned int billboardSetCounter_s;
            Ogre::BillboardSet* billboardSet_;
    };
}

#endif /* _BillboardSet_H__ */
