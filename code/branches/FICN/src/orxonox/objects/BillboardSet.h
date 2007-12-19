#include <string>

#include "OgreBillboardSet.h"

#ifndef _BillboardSet_H__
#define _BillboardSet_H__

namespace orxonox
{
    class BillboardSet
    {
        public:
            BillboardSet();
            ~BillboardSet();
            void setBillboardSet(const std::string& file, const Ogre::ColourValue& colour = Ogre::ColourValue(1.0, 1.0, 1.0), int count = 1, const Ogre::Vector3& position = Ogre::Vector3::ZERO);

            inline Ogre::BillboardSet* getBillboardSet()
                { return this->billboardSet_; }

            inline const std::string& getName() const
                { return this->billboardSet_->getName(); }

        private:
            static unsigned int billboardSetCounter_s;
            Ogre::BillboardSet* billboardSet_;
    };
}

#endif
