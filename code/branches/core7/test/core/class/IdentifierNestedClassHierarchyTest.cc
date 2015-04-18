#include <gtest/gtest.h>
#include "core/CoreIncludes.h"
#include "core/class/Identifiable.h"
#include "core/class/OrxonoxClass.h"
#include "core/class/OrxonoxInterface.h"

namespace orxonox
{
    // Tests the class hierarchy with classes that contain other classes as members.
    //
    //                    +------ Nested1         +------ Class1_notNested
    //                    +------ Nested2         +------ Class2_oneNested
    // OrxonoxClass <-----+------ BaseClass <-----+------ Class3_twoNested <----------- Class3_Child
    //                                            +------ Class4_class1Nested
    //                                            +------ Class5_class2Nested
    //                                            +------ Class6_class3Nested <-------- Class6_Child

    namespace
    {
        class Nested1 : public OrxonoxClass
        {
            public:
                Nested1() { RegisterObject(Nested1); }
        };

        class Nested2 : public OrxonoxClass
        {
            public:
                Nested2() { RegisterObject(Nested2); }
        };

        class BaseClass : public OrxonoxClass
        {
            public:
                BaseClass() { RegisterObject(BaseClass); }
        };

        class Class1_notNested : public BaseClass
        {
            public:
                Class1_notNested() { RegisterObject(Class1_notNested); }
        };

        class Class2_oneNested : public BaseClass
        {
            public:
                Class2_oneNested() { RegisterObject(Class2_oneNested); }
                Nested1 nested1_;
        };

        class Class3_twoNested : public BaseClass
        {
            public:
                Class3_twoNested() { RegisterObject(Class3_twoNested); }
                Nested1 nested1_;
                Nested2 nested2_;
        };

        class Class4_class1Nested : public BaseClass
        {
            public:
                Class4_class1Nested() { RegisterObject(Class4_class1Nested); }
                Class1_notNested nested_;
        };

        class Class5_class2Nested : public BaseClass
        {
            public:
                Class5_class2Nested() { RegisterObject(Class5_class2Nested); }
                Class2_oneNested nested_;
        };

        class Class6_class3Nested : public BaseClass
        {
            public:
                Class6_class3Nested() { RegisterObject(Class6_class3Nested); }
                Class3_twoNested nested_;
        };

        class Class3_Child : public Class3_twoNested
        {
            public:
                Class3_Child() { RegisterObject(Class3_Child); }
        };

        class Class6_Child : public Class6_class3Nested
        {
            public:
                Class6_Child() { RegisterObject(Class6_Child); }
        };

        // Fixture
        class IdentifierNestedClassHierarchyTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("Context", new ClassFactoryWithContext<Context>());
                    registerClass("Listable", new ClassFactoryWithContext<Listable>());
                    registerClass("Configurable", new ClassFactoryNoArgs<Configurable>());
                    registerClass("OrxonoxClass", new ClassFactoryNoArgs<OrxonoxClass>());
                    registerClass("Nested1", new ClassFactoryNoArgs<Nested1>());
                    registerClass("Nested2", new ClassFactoryNoArgs<Nested2>());
                    registerClass("BaseClass", new ClassFactoryNoArgs<BaseClass>());
                    registerClass("Class1_notNested", new ClassFactoryNoArgs<Class1_notNested>());
                    registerClass("Class2_oneNested", new ClassFactoryNoArgs<Class2_oneNested>());
                    registerClass("Class3_twoNested", new ClassFactoryNoArgs<Class3_twoNested>());
                    registerClass("Class4_class1Nested", new ClassFactoryNoArgs<Class4_class1Nested>());
                    registerClass("Class5_class2Nested", new ClassFactoryNoArgs<Class5_class2Nested>());
                    registerClass("Class6_class3Nested", new ClassFactoryNoArgs<Class6_class3Nested>());
                    registerClass("Class3_Child", new ClassFactoryNoArgs<Class3_Child>());
                    registerClass("Class6_Child", new ClassFactoryNoArgs<Class6_Child>());

                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyAllIdentifiers();
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

    TEST_F(IdentifierNestedClassHierarchyTest, TestNested1)
    {
        Identifier* identifier = Class(Nested1);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());
        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxClass)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestNested2)
    {
        Identifier* identifier = Class(Nested2);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());
        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxClass)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestBaseClass)
    {
        Identifier* identifier = Class(BaseClass);

        EXPECT_EQ(6u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class1_notNested)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class2_oneNested)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class3_twoNested)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class4_class1Nested)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class5_class2Nested)));
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class6_class3Nested)));

        EXPECT_EQ(8u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class1_notNested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class2_oneNested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3_twoNested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class4_class1Nested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class5_class2Nested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class6_class3Nested)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3_Child)));
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class6_Child)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(OrxonoxClass)));

        EXPECT_EQ(4u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass1_notNested)
    {
        Identifier* identifier = Class(Class1_notNested);

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

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass2_oneNested)
    {
        Identifier* identifier = Class(Class2_oneNested);

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

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass3_twoNested)
    {
        Identifier* identifier = Class(Class3_twoNested);

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class3_Child)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class3_Child)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));

        EXPECT_EQ(5u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass4_class1Nested)
    {
        Identifier* identifier = Class(Class4_class1Nested);

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

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass5_class2Nested)
    {
        Identifier* identifier = Class(Class5_class2Nested);

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

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass6_class3Nested)
    {
        Identifier* identifier = Class(Class6_class3Nested);

        EXPECT_EQ(1u, identifier->getDirectChildren().size());
        EXPECT_TRUE(contains(identifier->getDirectChildren(), Class(Class6_Child)));

        EXPECT_EQ(1u, identifier->getChildren().size());
        EXPECT_TRUE(contains(identifier->getChildren(), Class(Class6_Child)));

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(BaseClass)));

        EXPECT_EQ(5u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass3_Child)
    {
        Identifier* identifier = Class(Class3_Child);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());
        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Class3_twoNested)));

        EXPECT_EQ(6u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Class3_twoNested)));
    }

    TEST_F(IdentifierNestedClassHierarchyTest, TestClass6_Child)
    {
        Identifier* identifier = Class(Class6_Child);

        EXPECT_EQ(0u, identifier->getDirectChildren().size());
        EXPECT_EQ(0u, identifier->getChildren().size());

        EXPECT_EQ(1u, identifier->getDirectParents().size());
        EXPECT_TRUE(contains(identifier->getDirectParents(), Class(Class6_class3Nested)));

        EXPECT_EQ(6u, identifier->getParents().size());
        EXPECT_TRUE(contains(identifier->getParents(), Class(Identifiable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Listable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Configurable)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(OrxonoxClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(BaseClass)));
        EXPECT_TRUE(contains(identifier->getParents(), Class(Class6_class3Nested)));
    }
}
