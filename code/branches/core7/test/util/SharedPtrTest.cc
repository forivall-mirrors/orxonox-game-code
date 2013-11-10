#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/SharedPtr.h"

namespace orxonox
{
    namespace
    {
        class TestClass
        {
            public:
                TestClass() : value_(0) {}
                virtual ~TestClass() {}

                void setValue(int value) { this->value_ = value; }
                int getValue() const { return this->value_; }

            private:
                int value_;
        };

        class TestChildClass : public TestClass
        {
            public:
                TestChildClass() {}
                virtual ~TestChildClass() {}
        };

        class TestClassMock : public TestClass
        {
            public:
                TestClassMock() {}
                ~TestClassMock() { objectDestroyed(); }

                MOCK_METHOD0(objectDestroyed, void());
        };
    }

    TEST(SharedPtr, ConstructorDefault)
    {
        SharedPtr<TestClass> test;
        EXPECT_EQ(0, test.get());
    }

    TEST(SharedPtr, ConstructorPointer)
    {
        TestClass* pointer = new TestClass();

        SharedPtr<TestClass> test = pointer;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, ConstructorChildPointer)
    {
        TestChildClass* pointer = new TestChildClass();

        SharedPtr<TestClass> test = pointer;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, ConstructorOther)
    {
        TestClass* pointer = new TestClass();

        SharedPtr<TestClass> other = pointer;
        EXPECT_EQ(pointer, other.get());

        SharedPtr<TestClass> test = other;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, ConstructorOtherChild)
    {
        TestChildClass* pointer = new TestChildClass();

        SharedPtr<TestChildClass> other = pointer;
        EXPECT_EQ(pointer, other.get());

        SharedPtr<TestClass> test = other;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, Assign)
    {
        TestClass* pointer = new TestClass();

        SharedPtr<TestClass> other = pointer;
        EXPECT_EQ(pointer, other.get());

        SharedPtr<TestClass> test;
        EXPECT_EQ(0, test.get());

        test = other;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, AssignChild)
    {
        TestChildClass* pointer = new TestChildClass();

        SharedPtr<TestChildClass> other = pointer;
        EXPECT_EQ(pointer, other.get());

        SharedPtr<TestClass> test;
        EXPECT_EQ(0, test.get());

        test = other;
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, Cast)
    {
        TestChildClass* pointer = new TestChildClass();

        SharedPtr<TestChildClass> other = pointer;
        EXPECT_EQ(pointer, other.get());

        SharedPtr<TestClass> test = other.cast<TestClass>();
        EXPECT_EQ(pointer, test.get());
    }

    TEST(SharedPtr, Access)
    {
        TestClass* pointer = new TestClass();

        SharedPtr<TestClass> test = pointer;
        EXPECT_EQ(pointer, test.get());

        EXPECT_EQ(0, test->getValue());
        test->setValue(5);
        EXPECT_EQ(5, test->getValue());
        EXPECT_EQ(5, (*test).getValue());
    }

    TEST(SharedPtr, Boolean)
    {
        SharedPtr<TestClass> test;
        EXPECT_EQ(0, test.get());
        EXPECT_FALSE(test);

        TestClass* pointer = new TestClass();

        test = pointer;
        EXPECT_EQ(pointer, test.get());
        EXPECT_TRUE(test);
    }

    TEST(SharedPtr, Swap)
    {
        TestClass* pointer1 = new TestClass();
        TestClass* pointer2 = new TestClass();

        SharedPtr<TestClass> test1 = pointer1;
        SharedPtr<TestClass> test2 = pointer2;

        EXPECT_EQ(pointer1, test1.get());
        EXPECT_EQ(pointer2, test2.get());

        test1.swap(test2);

        EXPECT_EQ(pointer2, test1.get());
        EXPECT_EQ(pointer1, test2.get());
    }

    TEST(SharedPtr, ObjectDestroyedOnePointer)
    {
        TestClassMock* pointer = new TestClassMock();
        SharedPtr<TestClass> test = pointer;

        EXPECT_CALL(*pointer, objectDestroyed()).Times(1);
    }

    TEST(SharedPtr, ObjectDestroyedManyPointers)
    {
        TestClassMock* pointer = new TestClassMock();

        SharedPtr<TestClass> test = pointer;
        std::vector<SharedPtr<TestClass> > tests;
        for (size_t i = 0; i < 100; ++i)
            tests.push_back(SharedPtr<TestClass>(test));

        EXPECT_CALL(*pointer, objectDestroyed()).Times(1);
    }
}
