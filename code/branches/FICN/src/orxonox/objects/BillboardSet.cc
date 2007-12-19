#include <sstream>

#include "BillboardSet.h"
#include "../Orxonox.h"

namespace orxonox
{
    unsigned int BillboardSet::billboardSetCounter_s = 0;

    BillboardSet::BillboardSet()
    {
        this->billboardSet_ = 0;
    }

    void BillboardSet::setBillboardSet(const std::string& file, const Ogre::ColourValue& colour, int count, const Ogre::Vector3& position)
    {
        std::ostringstream name;
        name << (BillboardSet::billboardSetCounter_s++);
        this->billboardSet_ = Orxonox::getSingleton()->getSceneManager()->createBillboardSet("Billboard" + name.str(), count);
        this->billboardSet_->createBillboard(position, colour);
        this->billboardSet_->setMaterialName(file);
    }

    BillboardSet::~BillboardSet()
    {
        if (this->billboardSet_)
            Orxonox::getSingleton()->getSceneManager()->destroyBillboardSet(this->billboardSet_);
    }
}
