#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/object/WeakPtr.h"

namespace orxonox
{
    namespace
    {
        class DestroyableTest : public Destroyable
        {
        };

        class Callback
        {
            public:
                virtual ~Callback() {}
                MOCK_METHOD0(callback, void());
        };
    }

    TEST(WeakPtrTest, CanReferenceObject)
    {
        DestroyableTest* test = new DestroyableTest();
        WeakPtr<DestroyableTest> weakPtr = test;
        EXPECT_EQ(test, weakPtr.get());
        test->destroy();
    }

    TEST(WeakPtrTest, DestroyRemovesReference)
    {
        DestroyableTest* test = new DestroyableTest();
        WeakPtr<DestroyableTest> weakPtr = test;
        EXPECT_EQ(test, weakPtr.get());
        test->destroy();
        EXPECT_EQ(NULL, weakPtr.get());
    }

    TEST(WeakPtrTest, DestroyCallsCallback)
    {
        DestroyableTest* test = new DestroyableTest();
        WeakPtr<DestroyableTest> weakPtr = test;
        Callback callback;
        weakPtr.setCallback(createFunctor(&Callback::callback, &callback));
        EXPECT_CALL(callback, callback());
        test->destroy();
    }

    void isNull(const WeakPtr<DestroyableTest> weakPtr)
    {
        EXPECT_TRUE(weakPtr == NULL);
        EXPECT_TRUE(weakPtr == 0);
        EXPECT_TRUE(!weakPtr);
        EXPECT_FALSE(weakPtr != NULL);
        EXPECT_FALSE(weakPtr != 0);
        EXPECT_FALSE(weakPtr);
    }

    TEST(WeakPtrTest, IsNull)
    {
        {
            WeakPtr<DestroyableTest> weakPtr;
            isNull(weakPtr);
        }
        {
            WeakPtr<DestroyableTest> weakPtr = NULL;
            isNull(weakPtr);
        }
        {
            WeakPtr<DestroyableTest> weakPtr;
            weakPtr = NULL;
            isNull(weakPtr);
        }
        {
            WeakPtr<DestroyableTest> weakPtr = 0;
            isNull(weakPtr);
        }
        {
            WeakPtr<DestroyableTest> weakPtr;
            weakPtr = 0;
            isNull(weakPtr);
        }
    }

    TEST(WeakPtrTest, IsNotNull)
    {
        DestroyableTest* test = new DestroyableTest();
        WeakPtr<DestroyableTest> weakPtr = test;
        EXPECT_FALSE(weakPtr == NULL);
        EXPECT_FALSE(weakPtr == 0);
        EXPECT_FALSE(!weakPtr);
        EXPECT_TRUE(weakPtr != NULL);
        EXPECT_TRUE(weakPtr != 0);
        EXPECT_TRUE(weakPtr);
        test->destroy();
    }
}
