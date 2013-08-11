#include <gtest/gtest.h>
#include "core/object/ClassFactory.h"
#include "core/BaseObject.h"

namespace orxonox
{
    TEST(ClassFactoryTest, CanFabricateObject)
    {
        Factory* factory = new ClassFactoryWithContext<BaseObject>("BaseObject");
        Identifiable* object = factory->fabricate(NULL);
        ASSERT_TRUE(object != NULL);
        BaseObject* baseObject = dynamic_cast<BaseObject*>(object);
        EXPECT_TRUE(baseObject != NULL);
        delete object;
        // don't delete factory - it remains in the identifier
    }
}
