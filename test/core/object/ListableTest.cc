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
        bool objectListContains(T* element)
        {
            for (typename ObjectList<T>::iterator it = ObjectList<T>::begin(); it != ObjectList<T>::end(); ++it)
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
        {
            ListableTest test;
            EXPECT_EQ(1u, ObjectList<ListableTest>::size());
            EXPECT_TRUE(objectListContains<ListableTest>(&test));
        }
        EXPECT_EQ(0u, ObjectList<ListableTest>::size());
    }

    TEST(ListableTest, RemovesFromAllObjectLists)
    {
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
}
