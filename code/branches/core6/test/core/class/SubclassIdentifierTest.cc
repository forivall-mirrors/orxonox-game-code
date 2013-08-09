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

        CreateFactory(TestClass);
        CreateFactory(TestSubclass);
    }

    TEST(SubclassIdentifierTest, CanCreateIdentifier)
    {
        TestSubclass test;

        SubclassIdentifier<TestClass> subclassIdentifier;
    }

    TEST(SubclassIdentifierTest, DefaultsToNormalIdentifier)
    {
        TestSubclass test;

        SubclassIdentifier<TestClass> subclassIdentifier;
        EXPECT_EQ(Class(TestClass), subclassIdentifier.getIdentifier());
    }

    TEST(SubclassIdentifierTest, CanAssignIdentifierOfSubclass)
    {
        TestSubclass test;
        IdentifierManager::createClassHierarchy();

        SubclassIdentifier<TestClass> subclassIdentifier;
        subclassIdentifier = Class(TestSubclass);
        EXPECT_EQ(Class(TestSubclass), subclassIdentifier.getIdentifier());
    }

    TEST(SubclassIdentifierTest, CanCreateSubclass)
    {
        TestSubclass test;
        IdentifierManager::createClassHierarchy();

        SubclassIdentifier<TestClass> subclassIdentifier;
        subclassIdentifier = Class(TestSubclass);

        TestClass* instance = subclassIdentifier.fabricate(NULL);
        ASSERT_TRUE(instance != NULL);
        EXPECT_EQ(Class(TestSubclass), instance->getIdentifier());
        delete instance;
    }
}
