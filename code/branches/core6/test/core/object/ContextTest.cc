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
                SubclassContext() { RegisterRootObject(SubclassContext); }
        };
    }

    TEST(ContextTest, CanCreateContext)
    {
        Context context;
    }

    TEST(ContextTest, CanCreateSubclassContext)
    {
        SubclassContext context;
    }

    TEST(ContextTest, ContextIsItsOwnContext)
    {
        Context context;
        EXPECT_EQ(&context, context.getContext());
    }

    TEST(ContextTest, SubclassContextIsItsOwnContext)
    {
        SubclassContext context;
        EXPECT_EQ(&context, context.getContext());
    }

    TEST(ContextTest, SubclassAddsToItsOwnObjectList)
    {
        SubclassContext context;
        EXPECT_EQ(&context, context.getContext());
        EXPECT_EQ(1u, context.getObjectList<SubclassContext>()->size());
    }
}
