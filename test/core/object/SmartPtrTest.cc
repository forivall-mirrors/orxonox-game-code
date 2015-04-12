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

    void isNull(const SmartPtr<DestroyableTest> smartPtr)
    {
        EXPECT_TRUE(smartPtr == NULL);
        EXPECT_TRUE(smartPtr == 0);
        EXPECT_TRUE(!smartPtr);
        EXPECT_FALSE(smartPtr != NULL);
        EXPECT_FALSE(smartPtr != 0);
        EXPECT_FALSE(smartPtr);
    }

    TEST(SmartPtrTest, IsNull)
    {
        {
            SmartPtr<DestroyableTest> smartPtr;
            isNull(smartPtr);
        }
        {
            SmartPtr<DestroyableTest> smartPtr = NULL;
            isNull(smartPtr);
        }
        {
            SmartPtr<DestroyableTest> smartPtr;
            smartPtr = NULL;
            isNull(smartPtr);
        }
        {
            SmartPtr<DestroyableTest> smartPtr = 0;
            isNull(smartPtr);
        }
        {
            SmartPtr<DestroyableTest> smartPtr;
            smartPtr = 0;
            isNull(smartPtr);
        }
    }

    TEST(SmartPtrTest, IsNotNull)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        SmartPtr<DestroyableTest> smartPtr = test;
        EXPECT_FALSE(smartPtr == NULL);
        EXPECT_FALSE(smartPtr == 0);
        EXPECT_FALSE(!smartPtr);
        EXPECT_TRUE(smartPtr != NULL);
        EXPECT_TRUE(smartPtr != 0);
        EXPECT_TRUE(smartPtr);
        test->destroy();
    }
}
