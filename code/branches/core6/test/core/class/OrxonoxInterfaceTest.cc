#include <gtest/gtest.h>
#include "core/class/OrxonoxInterface.h"
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    namespace
    {
        class Interface1 : virtual public OrxonoxInterface
        {
        };
        class Interface2 : virtual public OrxonoxInterface
        {
        };
        class Interface3 : virtual public OrxonoxInterface
        {
        };

        class TestClass1 : public Interface1, public Interface2, public Interface3
        {
        };

        class TestClass2 : public OrxonoxClass, public Interface1, public Interface2, public Interface3
        {
        };
    }

    TEST(OrxonoxInterfaceTest, CanCreate1)
    {
        TestClass1* test = new TestClass1();
        ASSERT_TRUE(test != NULL);
        delete test;
    }

    TEST(OrxonoxInterfaceTest, CanCreate2)
    {
        TestClass2* test = new TestClass2();
        ASSERT_TRUE(test != NULL);
        delete test;
    }
}
