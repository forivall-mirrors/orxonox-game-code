#include <gtest/gtest.h>
#include "core/object/SmartPtr.h"

namespace orxonox
{
    namespace
    {
        class DestroyableTest : public Destroyable
        {
            public:
                DestroyableTest(bool& destroyed) : destroyed_(destroyed) { destroyed_ = false; }
                virtual ~DestroyableTest() { destroyed_ = true; }

            private:
                bool& destroyed_;
        };
    }

    TEST(SmartPtrTest, CanReferenceObject)
    {
        bool bla;
        DestroyableTest* test = new DestroyableTest(bla);
        SmartPtr<DestroyableTest> smartPtr = test;
        EXPECT_EQ(test, smartPtr.get());
        test->destroy();
    }

    TEST(SmartPtrTest, IncreasesReferenceCount)
    {
        bool bla;
        DestroyableTest* test = new DestroyableTest(bla);
        EXPECT_EQ(0u, test->getReferenceCount());
        {
            SmartPtr<DestroyableTest> smartPtr = test;
            EXPECT_EQ(1u, test->getReferenceCount());
        }
        EXPECT_EQ(0u, test->getReferenceCount());
        test->destroy();
    }

    TEST(SmartPtrTest, DestroyDeletesInstance)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        EXPECT_FALSE(destroyed);
        test->destroy();
        EXPECT_TRUE(destroyed);
    }

    TEST(SmartPtrTest, PreventsDestruction)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        EXPECT_FALSE(destroyed);
        SmartPtr<DestroyableTest> smartPtr = test;
        test->destroy();
        EXPECT_FALSE(destroyed);
    }

    TEST(SmartPtrTest, DestroysIfSmartPtrRemoved)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        EXPECT_FALSE(destroyed);
        {
            SmartPtr<DestroyableTest> smartPtr = test;
            test->destroy();
            EXPECT_FALSE(destroyed);
        }
        EXPECT_TRUE(destroyed);
    }

    TEST(SmartPtrTest, DestroysIfAllSmartPtrsRemoved)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        EXPECT_FALSE(destroyed);
        {
            SmartPtr<DestroyableTest> smartPtr1 = test;
            {
                SmartPtr<DestroyableTest> smartPtr2 = test;
                {
                    SmartPtr<DestroyableTest> smartPtr3 = test;
                    test->destroy();
                    EXPECT_FALSE(destroyed);
                }
                EXPECT_FALSE(destroyed);
            }
            EXPECT_FALSE(destroyed);
        }
        EXPECT_TRUE(destroyed);
    }
}
