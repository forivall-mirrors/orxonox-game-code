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
}
