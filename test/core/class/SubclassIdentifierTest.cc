#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/class/SubclassIdentifier.h"
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public OrxonoxClass
        {
            public:
                TestClass(Context* context = NULL) { RegisterRootObject(TestClass); }
        };

        class TestSubclass : public TestClass
        {
            public:
                TestSubclass(Context* context = NULL) { RegisterObject(TestSubclass); }
        };

        // Fixture
        class SubclassIdentifierTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("OrxonoxClass", new ClassFactoryNoArgs<OrxonoxClass>());
                    registerClass("TestClass", new ClassFactoryWithContext<TestClass>());
                    registerClass("TestSubclass", new ClassFactoryWithContext<TestSubclass>());

                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyAllIdentifiers();
                }
        };
    }

    TEST_F(SubclassIdentifierTest, CanCreateIdentifier)
    {
        TestSubclass test;

        SubclassIdentifier<TestClass> subclassIdentifier;
    }

    TEST_F(SubclassIdentifierTest, DefaultsToNormalIdentifier)
    {
        TestSubclass test;

        SubclassIdentifier<TestClass> subclassIdentifier;
        EXPECT_EQ(Class(TestClass), subclassIdentifier.getIdentifier());
    }

    TEST_F(SubclassIdentifierTest, CanAssignIdentifierOfSubclass)
    {
        TestSubclass test;
        SubclassIdentifier<TestClass> subclassIdentifier;
        subclassIdentifier = Class(TestSubclass);
        EXPECT_EQ(Class(TestSubclass), subclassIdentifier.getIdentifier());
    }

    TEST_F(SubclassIdentifierTest, CanCreateSubclass)
    {
        TestSubclass test;
        SubclassIdentifier<TestClass> subclassIdentifier;
        subclassIdentifier = Class(TestSubclass);

        TestClass* instance = subclassIdentifier.fabricate(NULL);
        ASSERT_TRUE(instance != NULL);
        EXPECT_EQ(Class(TestSubclass), instance->getIdentifier());
        delete instance;
    }
}
