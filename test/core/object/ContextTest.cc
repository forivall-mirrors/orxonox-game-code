#include <gtest/gtest.h>
#include "core/object/Context.h"
#include "core/class/OrxonoxClass.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    namespace
    {
        class SubclassContext : public OrxonoxClass, public Context
        {
            public:
                SubclassContext() : Context(NULL) { RegisterRootObject(SubclassContext); }
        };

        // Fixture
        class ContextTest : public ::testing::Test
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

    TEST_F(ContextTest, CanCreateContext)
    {
        Context context(NULL);
    }

    TEST_F(ContextTest, CanCreateSubclassContext)
    {
        SubclassContext context;
    }

    TEST_F(ContextTest, ContextIsItsOwnContext)
    {
        Context context(NULL);
        EXPECT_EQ(&context, context.getContext());
    }

    TEST_F(ContextTest, SubclassContextIsItsOwnContext)
    {
        SubclassContext context;
        EXPECT_EQ(&context, context.getContext());
    }

    TEST_F(ContextTest, SubclassAddsToItsOwnObjectList)
    {
        SubclassContext context;
        EXPECT_EQ(&context, context.getContext());
        EXPECT_EQ(1u, context.getObjectList<SubclassContext>()->size());
    }
}
