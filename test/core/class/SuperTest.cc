#include <gtest/gtest.h>
#include <tinyxml/tinyxml.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "core/class/Super.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public BaseObject
        {
            public:
                TestClass(Context* context = NULL) : BaseObject(context), changedNameBase_(false), xmlPortBase_(false), modeBase_(XMLPort::NOP)
                {
                    RegisterObject(TestClass);
                }

                virtual void changedName()
                {
                    this->changedNameBase_ = true;
                }

                virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode)
                {
                    this->xmlPortBase_ = true;
                    this->modeBase_ = mode;
                }

                bool changedNameBase_;
                bool xmlPortBase_;
                XMLPort::Mode modeBase_;
        };

        class TestSubclass : public TestClass
        {
            public:
                TestSubclass(Context* context = NULL) : TestClass(context), changedNameSubclass_(false), xmlPortSubclass_(false), modeSubclass_(XMLPort::NOP)
                {
                    RegisterObject(TestSubclass);
                }

                virtual void changedName()
                {
                    this->changedNameSubclass_ = true;

                    SUPER(TestSubclass, changedName);
                }

                virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode)
                {
                    this->xmlPortSubclass_ = true;
                    this->modeSubclass_ = mode;

                    SUPER(TestSubclass, XMLPort, xmlelement, mode);
                }

                bool changedNameSubclass_;
                bool xmlPortSubclass_;
                XMLPort::Mode modeSubclass_;
        };

        RegisterClass(TestClass);
        RegisterClass(TestSubclass);

        // Fixture
        class SuperTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances();
                    Identifier::initConfigValues_s = false; // TODO: hack!
                    IdentifierManager::getInstance().createClassHierarchy();

                    Context::setRootContext(new Context(NULL));
                }

                virtual void TearDown()
                {
                    Context::setRootContext(NULL);

                    IdentifierManager::getInstance().destroyClassHierarchy();
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances();
                }
        };
    }

    TEST_F(SuperTest, ClassHierarchyIsCorrect)
    {
        {
            Identifier* identifier = Class(TestSubclass);

            EXPECT_EQ(0u, identifier->getDirectChildren().size());

            EXPECT_EQ(1u, identifier->getDirectParents().size());
            EXPECT_TRUE(std::find(identifier->getDirectParents().begin(), identifier->getDirectParents().end(), Class(TestClass)) != identifier->getDirectParents().end());
        }
        {
            Identifier* identifier = Class(TestClass);

            EXPECT_EQ(1u, identifier->getDirectChildren().size());
            EXPECT_TRUE(identifier->getDirectChildren().find(Class(TestSubclass)) != identifier->getDirectChildren().end());

            EXPECT_EQ(1u, identifier->getDirectParents().size());
            EXPECT_TRUE(std::find(identifier->getDirectParents().begin(), identifier->getDirectParents().end(), Class(BaseObject)) != identifier->getDirectParents().end());
        }
    }

    TEST_F(SuperTest, SuperCallWithoutArguments)
    {
        TestSubclass test;

        EXPECT_FALSE(test.changedNameBase_);
        EXPECT_FALSE(test.changedNameSubclass_);

        test.changedName();

        EXPECT_TRUE(test.changedNameBase_);
        EXPECT_TRUE(test.changedNameSubclass_);
    }

    TEST_F(SuperTest, SuperCallWithArguments)
    {
        TestSubclass test;

        EXPECT_FALSE(test.xmlPortBase_);
        EXPECT_FALSE(test.xmlPortSubclass_);
        EXPECT_EQ(XMLPort::NOP, test.modeBase_);
        EXPECT_EQ(XMLPort::NOP, test.modeSubclass_);

        Element* element = NULL;
        test.XMLPort(*element, XMLPort::SaveObject);

        EXPECT_TRUE(test.xmlPortBase_);
        EXPECT_TRUE(test.xmlPortSubclass_);
        EXPECT_EQ(XMLPort::SaveObject, test.modeBase_);
        EXPECT_EQ(XMLPort::SaveObject, test.modeSubclass_);
    }
}
