#include <gtest/gtest.h>
#include "core/object/Listable.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    namespace
    {
        class ListableTest : public Listable
        {
            public:
                ListableTest() { RegisterRootObject(ListableTest); }
        };

        class ListableSubclassTest : public ListableTest
        {
            public:
                ListableSubclassTest() { RegisterObject(ListableSubclassTest); }
        };

        template <class T>
        bool objectListContains(T* element, Context* context = Context::getRootContext())
        {
            for (typename ObjectList<T>::iterator it = static_cast<ObjectListElement<T>*>(context->getObjectList<T>()->begin());
                    it != static_cast<ObjectListElement<T>*>(context->getObjectList<T>()->end()); ++it)
                if (*it == element)
                    return true;
            return false;
        }
    }

    TEST(ListableTest, CanCreate)
    {
        ListableTest* test = new ListableTest();
        ASSERT_TRUE(test != NULL);
        delete test;
    }

    TEST(ListableTest, AddsToObjectList)
    {
        ListableTest test;
        EXPECT_EQ(1u, ObjectList<ListableTest>::size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test));
    }

    TEST(ListableTest, AddsToAllObjectLists)
    {
        ListableSubclassTest test;
        EXPECT_EQ(1u, ObjectList<ListableTest>::size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test));
        EXPECT_EQ(1u, ObjectList<ListableSubclassTest>::size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test));
    }

    TEST(ListableTest, RemovesFromObjectList)
    {
        EXPECT_EQ(0u, ObjectList<ListableTest>::size());
        {
            ListableTest test;
            EXPECT_EQ(1u, ObjectList<ListableTest>::size());
            EXPECT_TRUE(objectListContains<ListableTest>(&test));
        }
        EXPECT_EQ(0u, ObjectList<ListableTest>::size());
    }

    TEST(ListableTest, RemovesFromAllObjectLists)
    {
        EXPECT_EQ(0u, ObjectList<ListableTest>::size());
        EXPECT_EQ(0u, ObjectList<ListableSubclassTest>::size());
        {
            ListableSubclassTest test;
            EXPECT_EQ(1u, ObjectList<ListableTest>::size());
            EXPECT_TRUE(objectListContains<ListableTest>(&test));
            EXPECT_EQ(1u, ObjectList<ListableSubclassTest>::size());
            EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test));
        }
        EXPECT_EQ(0u, ObjectList<ListableTest>::size());
        EXPECT_EQ(0u, ObjectList<ListableSubclassTest>::size());
    }

    TEST(ListableTest, CanAddObjectToContext)
    {
        Context context(NULL);
        ListableSubclassTest test;

        test.setContext(&context);

        // object is in new context
        EXPECT_EQ(1u, context.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &context));
        EXPECT_EQ(1u, context.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &context));
    }

    TEST(ListableTest, CanAddObjectToSubContext)
    {
        Context baseContext(NULL);
        Context subContext(&baseContext);
        ListableSubclassTest test;

        test.setContext(&subContext);

        // object is in both contexts
        EXPECT_EQ(1u, baseContext.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &baseContext));
        EXPECT_EQ(1u, baseContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &baseContext));

        EXPECT_EQ(1u, subContext.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &subContext));
        EXPECT_EQ(1u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext));
    }

    TEST(ListableTest, CanChangeContext)
    {
        Context* rootContext = Context::getRootContext();
        Context newContext(NULL);
        ListableSubclassTest test;

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is NOT in new context
        EXPECT_EQ(0u, newContext.getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, &newContext));
        EXPECT_EQ(0u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &newContext));

        test.setContext(&newContext);

        // object was removed from root context
        EXPECT_EQ(0u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(0u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is now in new context
        EXPECT_EQ(1u, newContext.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &newContext));
        EXPECT_EQ(1u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &newContext));
    }

    TEST(ListableTest, CanChangeToSubContext)
    {
        Context* rootContext = Context::getRootContext();
        Context subContext(rootContext);
        ListableSubclassTest test;

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is NOT in sub context
        EXPECT_EQ(0u, subContext.getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, &subContext));
        EXPECT_EQ(0u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext));

        test.setContext(&subContext);

        // object is still in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is now also in sub context
        EXPECT_EQ(1u, subContext.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &subContext));
        EXPECT_EQ(1u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext));
    }

    TEST(ListableTest, CanChangeFromSubcontextToSubContext)
    {
        Context* rootContext = Context::getRootContext();
        Context subContext1(rootContext);
        Context subContext2(rootContext);
        ListableSubclassTest test;

        test.setContext(&subContext1);

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is also in sub context 1
        EXPECT_EQ(1u, subContext1.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &subContext1));
        EXPECT_EQ(1u, subContext1.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext1));

        // object is NOT in sub context 2
        EXPECT_EQ(0u, subContext2.getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, &subContext2));
        EXPECT_EQ(0u, subContext2.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext2));

        test.setContext(&subContext2);

        // object is still in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object was removed from sub context 1
        EXPECT_EQ(0u, subContext1.getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, &subContext1));
        EXPECT_EQ(0u, subContext1.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext1));

        // object is now in sub context 2
        EXPECT_EQ(1u, subContext2.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &subContext2));
        EXPECT_EQ(1u, subContext2.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext2));
    }
}
