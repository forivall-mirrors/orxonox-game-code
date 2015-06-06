#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class IdentifiableClass : public Identifiable
        {
            public:
                IdentifiableClass() { RegisterObject(IdentifiableClass); }
        };

        RegisterClassNoArgs(IdentifiableClass);

        // Fixture
        class IdentifiableTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                }

                virtual void TearDown()
                {
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                }
        };
    }

    TEST_F(IdentifiableTest, CanCreate)
    {
        IdentifiableClass* test = new IdentifiableClass();
        ASSERT_TRUE(test != NULL);
        delete test;
    }

    TEST_F(IdentifiableTest, HasIdentifierAssigned)
    {
        IdentifiableClass test;
        EXPECT_TRUE(test.getIdentifier());
    }

    TEST_F(IdentifiableTest, CanBeIdentified)
    {
        IdentifiableClass test;
        EXPECT_TRUE(test.isA(Class(IdentifiableClass)));
    }
}
