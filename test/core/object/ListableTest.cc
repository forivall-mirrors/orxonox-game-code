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

    TEST(ListableTest, CanChangeContext)
    {
        Context* rootContext = Context::getRootContext();
        Context newContext;
        ListableSubclassTest test;

        EXPECT_EQ(1u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        EXPECT_EQ(0u, newContext.getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, &newContext));
        EXPECT_EQ(0u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &newContext));

        test.setContext(&newContext);

        EXPECT_EQ(0u, rootContext->getObjectList<ListableTest>()->size());
        EXPECT_FALSE(objectListContains<ListableTest>(&test, rootContext));
        EXPECT_EQ(0u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, rootContext));

        EXPECT_EQ(1u, newContext.getObjectList<ListableTest>()->size());
        EXPECT_TRUE(objectListContains<ListableTest>(&test, &newContext));
        EXPECT_EQ(1u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &newContext));
    }
}
