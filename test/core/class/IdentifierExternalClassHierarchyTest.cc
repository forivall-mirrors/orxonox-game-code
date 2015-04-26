#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class Interface : virtual public Identifiable
        {
            public:
                Interface()
                {
                    RegisterObject(Interface);
                }

                virtual void test() = 0;
        };

        class BaseClass : virtual public Identifiable
        {
            public:
                BaseClass()
                {
                    RegisterObject(BaseClass);
                }
        };

        class RealClass : public BaseClass, public Interface
        {
            public:
                RealClass()
                {
                    RegisterObject(RealClass);
                }

                virtual void test() {}
        };

        RegisterAbstractClass(Interface).inheritsFrom<Identifiable>();
        RegisterClassNoArgs(BaseClass);
        RegisterClassNoArgs(RealClass);

        // Fixture
        class IdentifierExternalClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances();
                    ModuleInstance::setCurrentModuleInstance(new ModuleInstance()); // overwrite ModuleInstance because the old one is now loaded and shouln't be used anymore. TODO: better solution?
                    Identifier::initConfigValues_s = false; // TODO: hack!
                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyClassHierarchy();
                }
        };

        bool contains(const std::list<const Identifier*> identifiers, Identifier* identifier)
        {
            return std::find(identifiers.begin(), identifiers.end(), identifier) != identifiers.end();
        }

        bool contains(const std::set<const Identifier*> identifiers, Identifier* identifier)
        {
            return identifiers.find(identifier) != identifiers.end();
        }
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

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Identifiable)));

        EXPECT_EQ(1u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
    }

    TEST_F(IdentifierExternalClassHierarchyTest, BaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Identifiable)));

        EXPECT_EQ(1u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
    }

    TEST_F(IdentifierExternalClassHierarchyTest, RealClass)
    {
        Identifier* identifier = Class(RealClass);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(2u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));

        EXPECT_EQ(3u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
    }
}
