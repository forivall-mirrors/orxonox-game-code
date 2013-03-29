#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"

namespace orxonox
{
    namespace
    {
        class IdentifiableTest : public Identifiable
        {
            public:
                IdentifiableTest() { RegisterRootObject(IdentifiableTest); }
        };
    }

    TEST(IdentifiableTest, CanCreate)
    {
        IdentifiableTest* test = new IdentifiableTest();
        ASSERT_TRUE(test != NULL);
        delete test;
    }

    TEST(IdentifiableTest, HasIdentifierAssigned)
    {
        IdentifiableTest test;
        EXPECT_TRUE(test.getIdentifier());
    }

    TEST(IdentifiableTest, CanBeIdentified)
    {
        IdentifiableTest test;
        EXPECT_TRUE(test.isA(Class(IdentifiableTest)));
    }
}
