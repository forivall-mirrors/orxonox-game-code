#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/object/Iterator.h"
#include "core/class/OrxonoxClass.h"
#include "core/class/OrxonoxInterface.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    namespace
    {
        class TestInterface : virtual public OrxonoxInterface
        {
            public:
            TestInterface() { RegisterRootObject(TestInterface); }
        };

        class TestClass : public OrxonoxClass, public TestInterface
        {
            public:
                TestClass() { RegisterRootObject(TestClass); }
                MOCK_METHOD0(test, void());
        };
    }

    TEST(IteratorTest, CanCreateIterator)
    {
        Iterator<TestInterface> it;
    }

    TEST(IteratorTest, CanAssignIterator)
    {
        Iterator<TestInterface> it = ObjectList<TestInterface>::begin();
    }

    TEST(IteratorTest, CanIterateOverEmptyList)
    {
        size_t i = 0;
        for (Iterator<TestInterface> it = ObjectList<TestInterface>::begin(); it != ObjectList<TestInterface>::end(); ++it)
            ++i;
        EXPECT_EQ(0u, i);
    }

    TEST(IteratorTest, CanCallObjects)
    {
        TestClass test1;
        TestClass test2;
        TestClass test3;

        EXPECT_CALL(test1, test());
        EXPECT_CALL(test2, test());
        EXPECT_CALL(test3, test());

        // iterate over interfaces but use a TestClass iterator - now we can call TestClass::test()
        for (Iterator<TestClass> it = ObjectList<TestInterface>::begin(); it != ObjectList<TestInterface>::end(); ++it)
            it->test();
    }
}
