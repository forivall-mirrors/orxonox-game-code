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
                SubclassContext() : Context(NULL) { RegisterObject(SubclassContext); }
        };

        RegisterClassNoArgs(SubclassContext);

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

    TEST_F(ContextTest, ContextIsAddedToItsOwnObjectList)
    {
        Context context(NULL);
        ASSERT_EQ(1u, context.getObjectList<Context>()->size());
        EXPECT_TRUE(context.getObjectList<Context>()->begin()->objectBase_ == static_cast<Listable*>(&context));
    }

    TEST_F(ContextTest, ContextIsAddedToObjectListOfBaseContext)
    {
        Context baseContext(NULL);
        Context subContext(&baseContext);

        ASSERT_EQ(1u, subContext.getObjectList<Context>()->size());
        EXPECT_TRUE(subContext.getObjectList<Context>()->begin()->objectBase_ == static_cast<Listable*>(&subContext));

        ASSERT_EQ(2u, baseContext.getObjectList<Context>()->size());
        EXPECT_TRUE(baseContext.getObjectList<Context>()->begin()->objectBase_ == static_cast<Listable*>(&baseContext));
        EXPECT_TRUE(baseContext.getObjectList<Context>()->begin()->next_->objectBase_ == static_cast<Listable*>(&subContext));
    }
}
