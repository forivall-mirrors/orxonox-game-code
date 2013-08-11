#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/class/OrxonoxClass.h"
#include "core/class/OrxonoxInterface.h"

namespace orxonox
{
    namespace
    {
        class Interface : public OrxonoxInterface
            { public: Interface() { RegisterRootObject(Interface); } };

        class BaseClass : public OrxonoxClass
            { public: BaseClass() { RegisterRootObject(BaseClass); } };

        class RealClass : public BaseClass, public Interface
            { public: RealClass() { RegisterObject(RealClass); } };

        // Fixture
        class IdentifierSimpleClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("Interface", new ClassFactoryNoArgs<Interface>());
                    registerClass("BaseClass", new ClassFactoryNoArgs<BaseClass>());
                    registerClass("RealClass", new ClassFactoryNoArgs<RealClass>());

                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyAllIdentifiers();
                }
        };
    }

    TEST(IdentifierSimpleClassHierarchyTest_NoFixture, NoInitialization)
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

    TEST_F(IdentifierSimpleClassHierarchyTest, TestInterface)
    {
        Identifier* identifier = Class(Interface);

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_EQ(0u, identifier->getParents().size());
    }

    TEST_F(IdentifierSimpleClassHierarchyTest, BaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_EQ(0u, identifier->getParents().size());
    }

    TEST_F(IdentifierSimpleClassHierarchyTest, RealClass)
    {
        Identifier* identifier = Class(RealClass);

        EXPECT_EQ(0u, identifier->getChildren().size());
        EXPECT_EQ(2u, identifier->getParents().size());
    }
}
