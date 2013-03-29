#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public Identifiable
        {
            public:
                TestClass() { RegisterRootObject(TestClass); }
        };

        class TestSubclass : public TestClass
        {
            public:
                TestSubclass() { RegisterObject(TestSubclass); }
        };
    }

    TEST(IdentifierTest, IdentifierExistsOfClass)
    {
        TestClass test;

        Identifier* identifier = Class(TestClass);
        EXPECT_TRUE(identifier != NULL);
    }

    TEST(IdentifierTest, IdentifierExistsOfSubclass)
    {
        TestSubclass test;

        Identifier* identifier = Class(TestSubclass);
        EXPECT_TRUE(identifier != NULL);
    }

    TEST(IdentifierTest, HasNameOfClass)
    {
        TestClass test;

        Identifier* identifier = Class(TestClass);
        EXPECT_EQ("TestClass", identifier->getName());
    }

    TEST(IdentifierTest, HasNameOfSubClass)
    {
        TestSubclass test;

        Identifier* identifier = Class(TestSubclass);
        EXPECT_EQ("TestSubclass", identifier->getName());
    }
}
