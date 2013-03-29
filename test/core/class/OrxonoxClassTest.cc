#include <gtest/gtest.h>
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public OrxonoxClass
        {
        };
    }

    TEST(OrxonoxClassTest, CanCreate)
    {
        TestClass* test = new TestClass();
        ASSERT_TRUE(test != NULL);
        delete test;
    }
}
