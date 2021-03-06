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
        {
            public:
                Interface()
                {
                    RegisterObject(Interface);
                }

                virtual void test() = 0;
        };

        class BaseClass : public OrxonoxClass
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

        // Fixture
        class IdentifierSimpleClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("Context", new ClassFactoryWithContext<Context>());
                    registerClass("Listable", new ClassFactoryWithContext<Listable>());
                    registerClass("Configurable", new ClassFactoryNoArgs<Configurable>());
                    registerClass("OrxonoxInterface", new ClassFactoryNoArgs<OrxonoxInterface>());
                    registerClass("OrxonoxClass", new ClassFactoryNoArgs<OrxonoxClass>());
                    registerClass("Interface", static_cast<ClassFactory<Interface>*>(NULL), false).inheritsFrom(Class(OrxonoxInterface));
                    registerClass("BaseClass", new ClassFactoryNoArgs<BaseClass>());
                    registerClass("RealClass", new ClassFactoryNoArgs<RealClass>());

                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyAllIdentifiers();
                }
        };

        bool contains(const std::set<const Identifier*> identifiers, Identifier* identifier)
        {
            return identifiers.find(identifier) != identifiers.end();
        }
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

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxInterface)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
    }

    TEST_F(IdentifierSimpleClassHierarchyTest, BaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(RealClass)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxClass)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
    }

    TEST_F(IdentifierSimpleClassHierarchyTest, RealClass)
    {
        Identifier* identifier = Class(RealClass);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(2u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));

        EXPECT_EQ(7u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
    }
}
