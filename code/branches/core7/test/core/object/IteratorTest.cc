#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/object/Iterator.h"
#include "core/class/OrxonoxClass.h"
#include "core/class/OrxonoxInterface.h"
#include "core/CoreIncludes.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class TestInterface : virtual public OrxonoxInterface
        {
            public:
            TestInterface() { RegisterObject(TestInterface); }
        };

        class TestClass : public OrxonoxClass, public TestInterface
        {
            public:
                TestClass() { RegisterObject(TestClass); }
                MOCK_METHOD0(test, void());
        };

        RegisterClassNoArgs(TestInterface);
        RegisterClassNoArgs(TestClass);

        // Fixture
        class IteratorTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    Context::setRootContext(new Context(NULL));
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances();
                }

                virtual void TearDown()
                {
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances();
                    Context::setRootContext(NULL);
                }
        };
    }

    TEST_F(IteratorTest, CanCreateIterator)
    {
        Iterator<TestInterface> it;
    }

    TEST_F(IteratorTest, CanAssignIterator)
    {
        Iterator<TestInterface> it = ObjectList<TestInterface>::begin();
    }

    TEST_F(IteratorTest, CanIterateOverEmptyList)
    {
        size_t i = 0;
        for (Iterator<TestInterface> it = ObjectList<TestInterface>::begin(); it != ObjectList<TestInterface>::end(); ++it)
            ++i;
        EXPECT_EQ(0u, i);
    }

    TEST_F(IteratorTest, CanCallObjects)
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
