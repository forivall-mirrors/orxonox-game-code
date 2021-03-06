#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/class/OrxonoxClass.h"
#include "core/class/OrxonoxInterface.h"

namespace orxonox
{
    //                 +------ Class0
    // BaseClass <-----+---------------------+--- Class1
    //                 +---------------------+-+- Class2a
    //                 +---------------------+-+- Class2b
    //                 +-+-+-- Class3        | |
    //                   | |                 | |
    // BaseInterface1 <--+---- Interface1 <--� |
    //                     |                   |
    // BaseInterface2 <----+-- Interface2 <----�

    namespace
    {
        class BaseInterface1 : public OrxonoxInterface
        {
            public:
                BaseInterface1()
                {
                    RegisterObject(BaseInterface1);
                }

                virtual void test1() = 0;
        };

        class BaseInterface2 : public OrxonoxInterface
        {
            public:
                BaseInterface2()
                {
                    RegisterObject(BaseInterface2);
                }

                virtual void test2() = 0;
        };

        class Interface1 : public BaseInterface1
        {
            public:
                Interface1()
                {
                    RegisterObject(Interface1);
                }
        };

        class Interface2 : public BaseInterface2
        {
            public:
                Interface2()
                {
                    RegisterObject(Interface2);
                }
        };

        class BaseClass : public OrxonoxClass
        {
            public:
                BaseClass()
                {
                    RegisterObject(BaseClass);
                }
        };

        class Class0 : public BaseClass
        {
            public:
                Class0()
                {
                    RegisterObject(Class0);
                }
        };

        class Class1 : public BaseClass, public Interface1
        {
            public:
                Class1()
                {
                    RegisterObject(Class1);
                }

                virtual void test1() {}
        };

        class Class2a : public BaseClass, public Interface1, Interface2
        {
            public:
                Class2a()
                {
                    RegisterObject(Class2a);
                }

                virtual void test1() {}
                virtual void test2() {}
        };

        class Class2b : public BaseClass, public Interface2, Interface1
        {
            public:
                Class2b()
                {
                    RegisterObject(Class2b);
                }

                virtual void test1() {}
                virtual void test2() {}
        };

        class Class3 : public BaseClass, public BaseInterface1, BaseInterface2
        {
            public:
                Class3()
                {
                    RegisterObject(Class3);
                }

                virtual void test1() {}
                virtual void test2() {}
        };

        // Fixture
        class IdentifierClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("Context", new ClassFactoryWithContext<Context>());
                    registerClass("Listable", new ClassFactoryWithContext<Listable>());
                    registerClass("Configurable", new ClassFactoryNoArgs<Configurable>());
                    registerClass("OrxonoxInterface", new ClassFactoryNoArgs<OrxonoxInterface>());
                    registerClass("OrxonoxClass", new ClassFactoryNoArgs<OrxonoxClass>());
                    registerClass("BaseInterface1", static_cast<ClassFactory<BaseInterface1>*>(NULL), false).inheritsFrom(Class(OrxonoxInterface));
                    registerClass("BaseInterface2", static_cast<ClassFactory<BaseInterface2>*>(NULL), false).inheritsFrom(Class(OrxonoxInterface));
                    registerClass("Interface1", static_cast<ClassFactory<Interface1>*>(NULL), false).inheritsFrom(Class(BaseInterface1));
                    registerClass("Interface2", static_cast<ClassFactory<Interface2>*>(NULL), false).inheritsFrom(Class(BaseInterface2));
                    registerClass("BaseClass", new ClassFactoryNoArgs<BaseClass>());
                    registerClass("Class0", new ClassFactoryNoArgs<Class0>());
                    registerClass("Class1", new ClassFactoryNoArgs<Class1>());
                    registerClass("Class2a", new ClassFactoryNoArgs<Class2a>());
                    registerClass("Class2b", new ClassFactoryNoArgs<Class2b>());
                    registerClass("Class3", new ClassFactoryNoArgs<Class3>());

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

    TEST(IdentifierClassHierarchyTest_NoFixture, NoInitialization)
    {
        {
            Identifier* identifier = Class(BaseInterface1);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(BaseInterface2);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Interface1);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Interface2);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(BaseClass);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Class0);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Class1);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Class2a);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Class2b);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
        {
            Identifier* identifier = Class(Class3);
            EXPECT_EQ(0u, identifier->getChildren().size());
            EXPECT_EQ(0u, identifier->getParents().size());
        }
    }

    TEST_F(IdentifierClassHierarchyTest, TestBaseInterface1)
    {
        Identifier* identifier = Class(BaseInterface1);

        EXPECT_EQ(2u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class3)));

        EXPECT_EQ(5u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class1)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2b)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxInterface)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestBaseInterface2)
    {
        Identifier* identifier = Class(BaseInterface2);

        EXPECT_EQ(2u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Interface2)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class3)));

        EXPECT_EQ(4u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Interface2)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2b)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxInterface)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestInterface1)
    {
        Identifier* identifier = Class(Interface1);

        EXPECT_EQ(3u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class1)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2b)));

        EXPECT_EQ(3u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class1)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2b)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseInterface1)));

        EXPECT_EQ(5u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface1)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestInterface2)
    {
        Identifier* identifier = Class(Interface2);

        EXPECT_EQ(2u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2b)));

        EXPECT_EQ(2u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2b)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseInterface2)));

        EXPECT_EQ(5u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface2)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestBaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(5u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class0)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class1)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2b)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class3)));

        EXPECT_EQ(5u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class0)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class1)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2a)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2b)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxClass)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestClass0)
    {
        Identifier* identifier = Class(Class0);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));

        EXPECT_EQ(5u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestClass1)
    {
        Identifier* identifier = Class(Class1);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(2u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface1)));

        EXPECT_EQ(8u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface1)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestClass2a)
    {
        Identifier* identifier = Class(Class2a);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(3u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface2)));

        EXPECT_EQ(10u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface2)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface2)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestClass2b)
    {
        Identifier* identifier = Class(Class2b);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(3u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Interface2)));

        EXPECT_EQ(10u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface2)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Interface2)));
    }

    TEST_F(IdentifierClassHierarchyTest, TestClass3)
    {
        Identifier* identifier = Class(Class3);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());

        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(3u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseInterface1)));
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseInterface2)));

        EXPECT_EQ(8u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxInterface)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface1)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseInterface2)));
    }
}
