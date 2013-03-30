#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/object/ObjectListIterator.h"
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
                MOCK_METHOD0(test, void());
        };
    }

    TEST(ObjectListIteratorTest, CanCreateIterator)
    {
        ObjectListIterator<ListableTest> it;
    }

    TEST(ObjectListIteratorTest, CanAssignIterator)
    {
        ObjectListIterator<ListableTest> it = ObjectList<ListableTest>::begin();
    }

    TEST(ObjectListIteratorTest, CanIterateOverEmptyList)
    {
        size_t i = 0;
        for (ObjectListIterator<ListableTest> it = ObjectList<ListableTest>::begin(); it != ObjectList<ListableTest>::end(); ++it)
            ++i;
        EXPECT_EQ(0u, i);
    }

    TEST(ObjectListIteratorTest, CanIterateOverFullList)
    {
        ListableTest test1;
        ListableTest test2;
        ListableTest test3;

        size_t i = 0;
        for (ObjectListIterator<ListableTest> it = ObjectList<ListableTest>::begin(); it != ObjectList<ListableTest>::end(); ++it)
        {
            ++i;
            if (i == 1u) EXPECT_EQ(&test1, *it);
            if (i == 2u) EXPECT_EQ(&test2, *it);
            if (i == 3u) EXPECT_EQ(&test3, *it);
        }
        EXPECT_EQ(3u, i);
    }

    TEST(ObjectListIteratorTest, CanIterateReverseOverFullList)
    {
        ListableTest test1;
        ListableTest test2;
        ListableTest test3;

        size_t i = 0;
        for (ObjectListIterator<ListableTest> it = ObjectList<ListableTest>::rbegin(); it != ObjectList<ListableTest>::rend(); --it)
        {
            ++i;
            if (i == 1u) EXPECT_EQ(&test3, *it);
            if (i == 2u) EXPECT_EQ(&test2, *it);
            if (i == 3u) EXPECT_EQ(&test1, *it);
        }
        EXPECT_EQ(3u, i);
    }

    TEST(ObjectListIteratorTest, CanCallObjects)
    {
        ListableTest test1;
        ListableTest test2;
        ListableTest test3;

        EXPECT_CALL(test1, test());
        EXPECT_CALL(test2, test());
        EXPECT_CALL(test3, test());

        for (ObjectListIterator<ListableTest> it = ObjectList<ListableTest>::begin(); it != ObjectList<ListableTest>::end(); ++it)
            it->test();
    }
}
