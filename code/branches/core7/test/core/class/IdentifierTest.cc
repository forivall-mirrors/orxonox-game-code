#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public Identifiable
        {
            public:
                TestClass() { RegisterObject(TestClass); }
        };

        class TestSubclass : public TestClass
        {
            public:
                TestSubclass() { RegisterObject(TestSubclass); }
        };

        RegisterClassNoArgs(TestClass);
        RegisterClassNoArgs(TestSubclass);

        // Fixture
        class IdentifierTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    new IdentifierManager();
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                }

                virtual void TearDown()
                {
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                    delete &IdentifierManager::getInstance();
                }
        };
    }

    TEST_F(IdentifierTest, IdentifierExistsOfClass)
    {
        TestClass test;

        Identifier* identifier = Class(TestClass);
        EXPECT_TRUE(identifier != NULL);
    }

    TEST_F(IdentifierTest, IdentifierExistsOfSubclass)
    {
        TestSubclass test;

        Identifier* identifier = Class(TestSubclass);
        EXPECT_TRUE(identifier != NULL);
    }

    TEST_F(IdentifierTest, HasNameOfClass)
    {
        TestClass test;

        Identifier* identifier = Class(TestClass);
        EXPECT_EQ("TestClass", identifier->getName());
    }

    TEST_F(IdentifierTest, HasNameOfSubClass)
    {
        TestSubclass test;

        Identifier* identifier = Class(TestSubclass);
        EXPECT_EQ("TestSubclass", identifier->getName());
    }
}
