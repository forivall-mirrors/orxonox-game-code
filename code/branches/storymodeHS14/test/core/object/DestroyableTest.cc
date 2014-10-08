#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/object/Destroyable.h"

namespace orxonox
{
    namespace
    {
        class DestroyableTest : public Destroyable
        {
            public:
                DestroyableTest(bool& destroyed) : destroyed_(destroyed) { destroyed_ = false; }
                virtual ~DestroyableTest() { destroyed_ = true; }
                MOCK_METHOD0(preDestroy, void());

            private:
                bool& destroyed_;
        };

        class DestructionListenerMock : public DestructionListener
        {
            public:
                MOCK_METHOD0(objectDeleted, void());
                inline void registerAsDestructionListener(Destroyable* object)
                    { this->DestructionListener::registerAsDestructionListener(object); }
        };
    }

    TEST(DestroyableTest, DestroyDeletesInstance)
    {
        bool destroyed = false;
        DestroyableTest* test = new DestroyableTest(destroyed);
        EXPECT_FALSE(destroyed);
        EXPECT_TRUE(test != NULL);
        test->destroy();
        EXPECT_TRUE(destroyed);
    }

    TEST(DestroyableTest, DestroyCallsPreDestroy)
    {
        bool bla;
        DestroyableTest* test = new DestroyableTest(bla);
        EXPECT_CALL(*test, preDestroy());
        test->destroy();
    }

    TEST(DestroyableTest, DestroyCallsListener)
    {
        bool bla;
        DestroyableTest* test = new DestroyableTest(bla);
        DestructionListenerMock listener;
        listener.registerAsDestructionListener(test);
        EXPECT_CALL(listener, objectDeleted());
        test->destroy();
    }

    TEST(DestroyableTest, ReferenceCounterIsZero)
    {
        bool bla;
        DestroyableTest* test = new DestroyableTest(bla);
        EXPECT_EQ(0u, test->getReferenceCount());
        test->destroy();
    }
}
