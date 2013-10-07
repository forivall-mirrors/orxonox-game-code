#include <gtest/gtest.h>
#include "core/class/OrxonoxClass.h"
#include "core/object/Context.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public OrxonoxClass
        {
        };

        // Fixture
        class OrxonoxClassTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    Context::setRootContext(new Context(NULL));
                }

                virtual void TearDown()
                {
                    Context::setRootContext(NULL);
                }
        };
    }

    TEST_F(OrxonoxClassTest, CanCreate)
    {
        TestClass* test = new TestClass();
        ASSERT_TRUE(test != NULL);
        delete test;
    }
}
