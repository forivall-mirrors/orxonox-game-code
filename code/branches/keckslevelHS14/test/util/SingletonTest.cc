#include <gtest/gtest.h>
#include "util/Singleton.h"

namespace orxonox
{
    namespace
    {
        class TestSingleton : public Singleton<TestSingleton>
        {
            friend class Singleton<TestSingleton>;

            public:
                TestSingleton() : value_(MAGIC_VALUE) {}
                virtual ~TestSingleton() {}

                size_t getValue() const { return this->value_; }

                static const size_t MAGIC_VALUE;

            private:
                size_t value_;
                static TestSingleton* singletonPtr_s;
        };

        TestSingleton* TestSingleton::singletonPtr_s = NULL;
        const size_t TestSingleton::MAGIC_VALUE = 0xCAFEBABE;
    }

    TEST(DISABLED_SingletonDeathTest, MustBeUnique)
    {
        // create first instance
        TestSingleton* instance = new TestSingleton();
        // create second instance, must abort
        EXPECT_DEATH(new TestSingleton(), ".*");

        delete instance;
    }

    TEST(DISABLED_SingletonDeathTest, MustBeCreated)
    {
        // no instance created, must abort
        EXPECT_DEATH(TestSingleton::getInstance(), ".*");
    }

    TEST(DISABLED_SingletonDeathTest, MustBeReset)
    {
        // create instance
        TestSingleton* instance = new TestSingleton();
        // getInstance() must return newly created instance
        EXPECT_EQ(instance, &TestSingleton::getInstance());

        // delete instance
        delete instance;
        // must abort
        EXPECT_DEATH(TestSingleton::getInstance(), ".*");
    }

    TEST(Singleton, MustBeTheSame)
    {
        // create instance
        TestSingleton* instance = new TestSingleton();

        EXPECT_EQ(instance, &TestSingleton::getInstance());
        EXPECT_EQ(TestSingleton::MAGIC_VALUE, TestSingleton::getInstance().getValue());

        // delete instance
        delete instance;
    }

    TEST(Singleton, Exists)
    {
        EXPECT_FALSE(TestSingleton::exists());

        // create instance
        TestSingleton* instance = new TestSingleton();

        EXPECT_TRUE(TestSingleton::exists());

        // delete instance
        delete instance;

        EXPECT_FALSE(TestSingleton::exists());
    }
}
