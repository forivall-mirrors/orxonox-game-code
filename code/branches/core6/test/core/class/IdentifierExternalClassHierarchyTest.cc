#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"

namespace orxonox
{
    namespace
    {
        class Interface : virtual public Identifiable
            { public: Interface() { RegisterRootObject(Interface); } };

        class BaseClass : virtual public Identifiable
            { public: BaseClass() { RegisterRootObject(BaseClass); } };

        class RealClass : public BaseClass, public Interface
            { public: RealClass() { RegisterObject(RealClass); } };

        // Fixture
        class IdentifierExternalClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("Interface", new ClassFactoryNoArgs<Interface>());
                    registerClass("BaseClass", new ClassFactoryNoArgs<BaseClass>());
                    registerClass("RealClass", new ClassFactoryNoArgs<RealClass>());
                    IdentifierManager::createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::destroyAllIdentifiers();
                }
        };
    }

    TEST(IdentifierExternalClassHierarchyTest_NoFixture, NoInitialization)
    {
        {
            Identifier* identifier = Class(Interface);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(BaseClass);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(RealClass);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
    }

    TEST_F(IdentifierExternalClassHierarchyTest, TestInterface)
    {
        Identifier* identifier = Class(Interface);

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_EQ(0u, identifier->getParents().size());
    }

    TEST_F(IdentifierExternalClassHierarchyTest, BaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_EQ(0u, identifier->getParents().size());
    }

    TEST_F(IdentifierExternalClassHierarchyTest, RealClass)
    {
        Identifier* identifier = Class(RealClass);

        EXPECT_EQ(0u, identifier->getChildren().size());
        EXPECT_EQ(2u, identifier->getParents().size());
    }
}
