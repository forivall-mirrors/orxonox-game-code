#include <gtest/gtest.h>
#include <tinyxml/tinyxml.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "core/class/Super.h"

namespace orxonox
{
    namespace
    {
        class TestClass : public BaseObject
        {
            public:
                TestClass(Context* context = NULL) : BaseObject(context), changedNameBase_(false), xmlPortBase_(false), modeBase_(XMLPort::NOP)
                {
                    RegisterRootObject(TestClass);
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

        // Fixture
        class SuperTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    registerClass("OrxonoxClass", new ClassFactoryNoArgs<OrxonoxClass>());
                    registerClass("BaseObject", new ClassFactoryWithContext<BaseObject>());
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
