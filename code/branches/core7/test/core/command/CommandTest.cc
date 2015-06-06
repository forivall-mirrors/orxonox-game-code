#include <gtest/gtest.h>
#include "core/class/Identifier.h"
#include "core/class/IdentifierManager.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "core/command/CommandExecutor.h"
#include "core/object/Destroyable.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        int functionNr = 0;
        int valueA = 0;
        int valueB = 0;
        int valueC = 0;

        void testfunction1(int a);
        void testfunction2(int a, int b);
        void testfunction3(int a, int b);
        void testfunction4(int a, int b, int c);

        class Testclass : public Destroyable
        {
            public:
                void testfunction5(int a);
                void testfunction6(int a);

                Testclass* other_;
                int b_;
        };

        SetConsoleCommand("test", &testfunction1);

        Testclass* class1 = new Testclass();
        Testclass* class2 = new Testclass();

        void testfunction1(int a)
        {
            functionNr = 1;
            valueA = a;
            valueB = 0;
            valueC = 0;

            if (a == 1)
                ModifyConsoleCommand("test").pushFunction().defaultValue(1, 10).setFunction(&testfunction2);
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        void testfunction2(int a, int b)
        {
            functionNr = 2;
            valueA = a;
            valueB = b;
            valueC = 0;

            if (a == 1)
                ModifyConsoleCommand("test").pushFunction().defaultValue(1, 20).setFunction(&testfunction3);
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        void testfunction3(int a, int b)
        {
            functionNr = 3;
            valueA = a;
            valueB = b;
            valueC = 0;

            if (a == 1)
                ModifyConsoleCommand("test").pushFunction().defaultValue(1, 30).defaultValue(2, 40).setFunction(&testfunction4);
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        void testfunction4(int a, int b, int c)
        {
            functionNr = 4;
            valueA = a;
            valueB = b;
            valueC = c;

            if (a == 1)
                ModifyConsoleCommand("test").pushFunction().setFunction(&Testclass::testfunction5).setObject(class1);
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        void Testclass::testfunction5(int a)
        {
            class1->b_ = 11;
            class2->b_ = 22;

            class1->other_ = class2;
            class2->other_ = class1;

            functionNr = 5;
            valueA = a;
            valueB = b_;
            valueC = 0;

            if (a == 2)
                ModifyConsoleCommand("test").pushObject(this->other_);
            else if (a == -2)
                ModifyConsoleCommand("test").popObject();
            else if (a == 1)
                ModifyConsoleCommand("test").pushFunction(&Testclass::testfunction6, this);
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        void Testclass::testfunction6(int a)
        {
            class1->b_ = 11;
            class2->b_ = 22;

            class1->other_ = class2;
            class2->other_ = class1;

            functionNr = 6;
            valueA = a;
            valueB = b_;
            valueC = 0;

            if (a == 2)
                ModifyConsoleCommand("test").pushObject(this->other_);
            else if (a == -2)
                ModifyConsoleCommand("test").popObject();
            else if (a == 1)
                ModifyConsoleCommand("test").setFunction(FunctorPtr(0));
            else if (a == -1)
                ModifyConsoleCommand("test").popFunction();
        }

        // Fixture
        class CommandTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(0);
                    Identifier::initConfigValues_s = false; // TODO: hack!
                    IdentifierManager::getInstance().createClassHierarchy();
                }

                virtual void TearDown()
                {
                    IdentifierManager::getInstance().destroyClassHierarchy();
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(0);
                }
        };
    }

    void test(int function, int b, int c)
    {
        EXPECT_EQ(function, functionNr);
        EXPECT_EQ(b, valueB);
        EXPECT_EQ(c, valueC);
    }

    TEST_F(CommandTest, ModuleTest)
    {
        test(0, 0, 0);
        CommandExecutor::execute("test 0", false);
        test(1, 0, 0);
        CommandExecutor::execute("test 1", false);
        test(1, 0, 0);
        CommandExecutor::execute("test 0", false);
        test(2, 10, 0);
        CommandExecutor::execute("test 0 123", false);
        test(2, 123, 0);
        CommandExecutor::execute("test 1 1234", false);
        test(2, 1234, 0);
        CommandExecutor::execute("test 1", false);
        test(3, 20, 0);
        CommandExecutor::execute("test 0", false);
        test(4, 30, 40);
        CommandExecutor::execute("test 1 100 200", false);
        test(4, 100, 200);
        CommandExecutor::execute("test 0", false);
        test(5, 11, 0);
        CommandExecutor::execute("test 1", false);
        test(5, 11, 0);
        CommandExecutor::execute("test 2", false);
        test(6, 11, 0);
        CommandExecutor::execute("test 0", false);
        test(6, 22, 0);
        CommandExecutor::execute("test -1", false);
        test(6, 22, 0);
        CommandExecutor::execute("test -1", false);
        test(5, 11, 0);
        CommandExecutor::execute("test 0", false);
        test(4, 30, 40);
    }
}
