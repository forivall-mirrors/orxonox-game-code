#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define DISABLE_OUTPUT_CONTEXT_STATIC_CACHE

#include "util/Output.h"
#include "util/output/OutputManager.h"
#include "util/output/OutputListener.h"

namespace orxonox
{
    namespace context
    {
        namespace
        {
            REGISTER_OUTPUT_CONTEXT(unittest1);
            REGISTER_OUTPUT_CONTEXT(unittest2);
            REGISTER_OUTPUT_CONTEXT(unittest3);

            REGISTER_OUTPUT_SUBCONTEXT(unittest3, sub1);
            REGISTER_OUTPUT_SUBCONTEXT(unittest3, sub2);
            REGISTER_OUTPUT_SUBCONTEXT(unittest3, sub3);
        }
    }

    namespace
    {
        class MockOutputManager : public OutputManager
        {
            public:
                MOCK_METHOD1(updatedLevelMask, void(const OutputListener*));
                MOCK_METHOD1(updatedAdditionalContextsLevelMask, void(const OutputListener*));
                MOCK_METHOD1(updatedAdditionalContextsMask, void(const OutputListener*));

                void originalUpdatedLevelMask(const OutputListener* listener) { this->OutputManager::updatedLevelMask(listener); }
                void originalUpdatedAdditionalContextsLevelMask(const OutputListener* listener) { this->OutputManager::updatedAdditionalContextsLevelMask(listener); }
                void originalUpdatedAdditionalContextsMask(const OutputListener* listener) { this->OutputManager::updatedAdditionalContextsMask(listener); }
        };

        class MockOutputListener : public OutputListener
        {
            public:
                MockOutputListener(bool bRegister = true) : OutputListener(bRegister) {}

                MOCK_METHOD3(output, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));
                MOCK_METHOD3(unfilteredOutput, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));

                inline const std::vector<AdditionalContextListener*>& getListeners() const
                    { return OutputListener::getListeners(); }
        };
    }

    // test getInstance does not create listeners
    TEST(OutputManagerTest, GetInstanceDoesNotCreateDefaultListeners)
    {
        EXPECT_TRUE(NULL == OutputManager::getInstance().getMemoryWriter());
        EXPECT_TRUE(NULL == OutputManager::getInstance().getConsoleWriter());
        EXPECT_TRUE(NULL == OutputManager::getInstance().getLogWriter());
    }

    // test getInstanceAndCreateListeners creates listeners
    TEST(OutputManagerTest, GetInstanceAndCreateListenersCreatesDefaultListeners)
    {
        EXPECT_TRUE(NULL != OutputManager::getInstanceAndCreateListeners().getMemoryWriter());
        EXPECT_TRUE(NULL != OutputManager::getInstanceAndCreateListeners().getConsoleWriter());
        EXPECT_TRUE(NULL != OutputManager::getInstanceAndCreateListeners().getLogWriter());
    }

    // test getInstanceAndCreateListeners called again does not re-create listeners
    TEST(OutputManagerTest, GetInstanceAndCreateListenersDoesNotCreateDefaultListenersTwice)
    {
        MemoryWriter* memoryWriter = OutputManager::getInstanceAndCreateListeners().getMemoryWriter();
        ConsoleWriter* consoleWriter = OutputManager::getInstanceAndCreateListeners().getConsoleWriter();
        LogWriter* logWriter = OutputManager::getInstanceAndCreateListeners().getLogWriter();

        EXPECT_TRUE(memoryWriter == OutputManager::getInstanceAndCreateListeners().getMemoryWriter());
        EXPECT_TRUE(consoleWriter == OutputManager::getInstanceAndCreateListeners().getConsoleWriter());
        EXPECT_TRUE(logWriter == OutputManager::getInstanceAndCreateListeners().getLogWriter());
    }

    // test constructor
    TEST(OutputManagerTest, MasksAreZeroAfterConstruction)
    {
        OutputManager manager;
        EXPECT_EQ(0u, manager.getCombinedLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsMask());
    }

    // test registerListener adds listener to list
    TEST(OutputManagerTest, RegisterListenerAddsListenerToList)
    {
        OutputManager manager;
        MockOutputListener listener;
        EXPECT_TRUE(manager.getListeners().empty());

        manager.registerListener(&listener);

        EXPECT_FALSE(manager.getListeners().empty());
    }

    // test unregisterListener removes listener from list
    TEST(OutputManagerTest, UnregisterListenerRemovesListenerFromList)
    {
        OutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);
        EXPECT_FALSE(manager.getListeners().empty());

        manager.unregisterListener(&listener);

        EXPECT_TRUE(manager.getListeners().empty());
    }

    // test registerListener registers manager at listener
    TEST(OutputManagerTest, RegisterListenerRegistersManagerAtListener)
    {
        OutputManager manager;
        MockOutputListener listener(false);
        EXPECT_TRUE(listener.getListeners().empty());

        manager.registerListener(&listener);

        EXPECT_FALSE(listener.getListeners().empty());
    }

    // test unregisterListener unregisters manager from listener
    TEST(OutputManagerTest, UnregisterListenerRemovesManagerFromListener)
    {
        OutputManager manager;
        MockOutputListener listener(false);
        manager.registerListener(&listener);
        EXPECT_FALSE(listener.getListeners().empty());

        manager.unregisterListener(&listener);

        EXPECT_TRUE(listener.getListeners().empty());
    }

    // test registerListener adjusts masks
    TEST(OutputManagerTest, RegisterListenerAdjustsMasks)
    {
        OutputManager manager;
        EXPECT_EQ(0u, manager.getCombinedLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsMask());

        MockOutputListener listener;
        listener.setLevelMask(level::internal_warning);
        listener.setAdditionalContextsLevelMask(level::verbose_more);
        listener.setAdditionalContextsMask(context::unittest2().mask);

        manager.registerListener(&listener);

        EXPECT_EQ(level::internal_warning, manager.getCombinedLevelMask());
        EXPECT_EQ(level::verbose_more, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(context::unittest2().mask, manager.getCombinedAdditionalContextsMask());
    }

    // test unregisterListener adjusts masks
    TEST(OutputManagerTest, UnregisterListenerAdjustsMasks)
    {
        OutputManager manager;
        MockOutputListener listener;
        listener.setLevelMask(level::internal_warning);
        listener.setAdditionalContextsLevelMask(level::verbose_more);
        listener.setAdditionalContextsMask(context::unittest2().mask);

        manager.registerListener(&listener);

        EXPECT_EQ(level::internal_warning, manager.getCombinedLevelMask());
        EXPECT_EQ(level::verbose_more, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(context::unittest2().mask, manager.getCombinedAdditionalContextsMask());

        manager.unregisterListener(&listener);

        EXPECT_EQ(0u, manager.getCombinedLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsMask());
    }

    // test updatedLevelMask updates mask
    TEST(OutputManagerTest, UpdatedLevelMask)
    {
        MockOutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);

        EXPECT_EQ(0u, manager.getCombinedLevelMask());
        EXPECT_CALL(manager, updatedLevelMask(&listener)).WillRepeatedly(::testing::Invoke(&manager, &MockOutputManager::originalUpdatedLevelMask));

        listener.setLevelMask(level::internal_warning);

        EXPECT_EQ(level::internal_warning, manager.getCombinedLevelMask());
    }

    // test updatedAdditionalContextsLevelMask updates mask
    TEST(OutputManagerTest, UpdatedAdditionalContextsLevelMask)
    {
        MockOutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);

        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_CALL(manager, updatedAdditionalContextsLevelMask(&listener)).WillRepeatedly(::testing::Invoke(&manager, &MockOutputManager::originalUpdatedAdditionalContextsLevelMask));

        listener.setAdditionalContextsLevelMask(level::internal_warning);

        EXPECT_EQ(level::internal_warning, manager.getCombinedAdditionalContextsLevelMask());
    }

    // test updatedAdditionalContextsMask updates mask
    TEST(OutputManagerTest, UpdatedAdditionalContextsMask)
    {
        MockOutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);

        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsMask());
        EXPECT_CALL(manager, updatedAdditionalContextsMask(&listener)).WillRepeatedly(::testing::Invoke(&manager, &MockOutputManager::originalUpdatedAdditionalContextsMask));

        listener.setAdditionalContextsMask(context::unittest2().mask);

        EXPECT_EQ(context::unittest2().mask, manager.getCombinedAdditionalContextsMask());
    }

    // test combines masks of all listeners
    TEST(OutputManagerTest, CombinesMasksOfAllListeners)
    {
        OutputManager manager;
        EXPECT_EQ(0u, manager.getCombinedLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(0u, manager.getCombinedAdditionalContextsMask());

        MockOutputListener listener1;
        listener1.setLevelMask(level::user_error);
        listener1.setAdditionalContextsLevelMask(level::verbose);
        listener1.setAdditionalContextsMask(context::unittest1().mask);
        manager.registerListener(&listener1);

        MockOutputListener listener2;
        listener2.setLevelMask(level::user_warning);
        listener2.setAdditionalContextsLevelMask(level::verbose_more);
        listener2.setAdditionalContextsMask(context::unittest2().mask);
        manager.registerListener(&listener2);

        MockOutputListener listener3;
        listener3.setLevelMask(level::internal_warning);
        listener3.setAdditionalContextsLevelMask(level::verbose_ultra);
        listener3.setAdditionalContextsMask(context::unittest3().mask);
        manager.registerListener(&listener3);

        EXPECT_EQ(level::user_error | level::user_warning | level::internal_warning, manager.getCombinedLevelMask());
        EXPECT_EQ(level::verbose | level::verbose_more | level::verbose_ultra, manager.getCombinedAdditionalContextsLevelMask());
        EXPECT_EQ(context::unittest1().mask | context::unittest2().mask | context::unittest3().mask, manager.getCombinedAdditionalContextsMask());
    }

    // test acceptsOutput
    TEST(OutputManagerTest, AcceptsOutput)
    {
        OutputManager manager;
        MockOutputListener listener;
        listener.setLevelMask(level::internal_warning);
        listener.setAdditionalContextsLevelMask(level::verbose_more);
        listener.setAdditionalContextsMask(context::unittest2().mask);

        manager.registerListener(&listener);

        EXPECT_FALSE(manager.acceptsOutput(level::user_error, context::undefined()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_error, context::undefined()));
        EXPECT_TRUE(manager.acceptsOutput(level::internal_warning, context::undefined()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_status, context::undefined()));
        EXPECT_FALSE(manager.acceptsOutput(level::verbose_more, context::undefined()));
        EXPECT_FALSE(manager.acceptsOutput(level::verbose_ultra, context::undefined()));

        EXPECT_FALSE(manager.acceptsOutput(level::user_error, context::unittest1()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_error, context::unittest1()));
        EXPECT_TRUE(manager.acceptsOutput(level::internal_warning, context::unittest1()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_status, context::unittest1()));
        EXPECT_FALSE(manager.acceptsOutput(level::verbose_more, context::unittest1()));
        EXPECT_FALSE(manager.acceptsOutput(level::verbose_ultra, context::unittest1()));

        EXPECT_FALSE(manager.acceptsOutput(level::user_error, context::unittest2()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_error, context::unittest2()));
        EXPECT_TRUE(manager.acceptsOutput(level::internal_warning, context::unittest2()));
        EXPECT_FALSE(manager.acceptsOutput(level::internal_status, context::unittest2()));
        EXPECT_TRUE(manager.acceptsOutput(level::verbose_more, context::unittest2()));
        EXPECT_FALSE(manager.acceptsOutput(level::verbose_ultra, context::unittest2()));
    }

    // test pushMessage sends lines to listeners
    TEST(OutputManagerTest, PushMessageSendsLinesToListeners)
    {
        OutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);

        std::vector<std::string> lines;
        lines.push_back("some output");

        EXPECT_CALL(listener, unfilteredOutput(level::user_status, context::unittest1(), lines));

        manager.pushMessage(level::user_status, context::unittest1(), "some output");
    }

    // test pushMessage splits lines by \n
    TEST(OutputManagerTest, PushMessageSplitsTextAtNewlineChar)
    {
        OutputManager manager;
        MockOutputListener listener;
        manager.registerListener(&listener);

        std::vector<std::string> lines;
        lines.push_back("some output");
        lines.push_back("and some more output");
        lines.push_back("!!!");

        EXPECT_CALL(listener, unfilteredOutput(level::user_status, context::unittest1(), lines));

        manager.pushMessage(level::user_status, context::unittest1(), "some output\nand some more output\n!!!");
    }

    // test registerContext for new context
    TEST(OutputManagerTest, RegisterNewContext)
    {
        OutputManager manager;
        OutputContextContainer container = manager.registerContext("whatever");
        EXPECT_EQ("whatever", container.name);
        EXPECT_EQ(0, container.sub_id);
    }

    // test registerContext for existing context
    TEST(OutputManagerTest, RegisterContextTwice)
    {
        OutputManager manager;
        OutputContextContainer container1 = manager.registerContext("whatever");
        OutputContextContainer container2 = manager.registerContext("whatever");
        EXPECT_EQ("whatever", container1.name);
        EXPECT_EQ(0, container1.sub_id);
        EXPECT_EQ(container1, container2);
    }

    // test registerContext for multiple contexts
    TEST(OutputManagerTest, RegisterMultipleContexts)
    {
        OutputManager manager;
        OutputContextContainer container1 = manager.registerContext("whatever");
        OutputContextContainer container2 = manager.registerContext("asdf");
        EXPECT_EQ("whatever", container1.name);
        EXPECT_EQ(0, container1.sub_id);
        EXPECT_EQ("asdf", container2.name);
        EXPECT_EQ(0, container2.sub_id);
        EXPECT_NE(container1.mask, container2.mask);
    }

    // test registerContext for new subcontext
    TEST(OutputManagerTest, RegisterNewSubcontext)
    {
        OutputManager manager;
        OutputContextContainer container = manager.registerContext("base", "subcontext");
        EXPECT_EQ("base::subcontext", container.name);
        EXPECT_NE(0, container.sub_id);
    }

    // test registerContext for existing subcontext
    TEST(OutputManagerTest, RegisterSubcontextTwice)
    {
        OutputManager manager;
        OutputContextContainer container1 = manager.registerContext("base", "subcontext");
        OutputContextContainer container2 = manager.registerContext("base", "subcontext");
        EXPECT_EQ("base::subcontext", container1.name);
        EXPECT_NE(0, container1.sub_id);
        EXPECT_EQ(container1, container2);
    }

    // test registerContext for multiple subcontexts
    TEST(OutputManagerTest, RegisterMultipleSubcontexts)
    {
        OutputManager manager;
        OutputContextContainer container1 = manager.registerContext("base", "subcontext");
        OutputContextContainer container2 = manager.registerContext("base", "meh");
        EXPECT_EQ("base::subcontext", container1.name);
        EXPECT_NE(0, container1.sub_id);
        EXPECT_EQ("base::meh", container2.name);
        EXPECT_NE(0, container2.sub_id);
        EXPECT_EQ(container1.mask, container2.mask);
        EXPECT_NE(container1.sub_id, container2.sub_id);
    }

    // test subcontext has mask of base context
    TEST(OutputManagerTest, SubcontextHasMaskOfBaseContext)
    {
        OutputManager manager;
        OutputContextContainer container1 = manager.registerContext("base", "subcontext");
        EXPECT_EQ("base::subcontext", container1.name);
        EXPECT_NE(0, container1.sub_id);

        OutputContextContainer container2 = manager.registerContext("base");
        EXPECT_EQ("base", container2.name);
        EXPECT_EQ(0, container2.sub_id);

        EXPECT_EQ(container1.mask, container2.mask);
    }

    // test getLevelName
    TEST(OutputManagerTest, GetLevelName)
    {
        OutputManager manager;
        EXPECT_EQ("Debug", manager.getLevelName(level::debug_output));
        EXPECT_EQ("Error", manager.getLevelName(level::user_error));
        EXPECT_EQ("Status", manager.getLevelName(level::user_status));
        EXPECT_EQ("Warning (internal)", manager.getLevelName(level::internal_warning));
        EXPECT_EQ("Info (internal)", manager.getLevelName(level::internal_info));
        EXPECT_EQ("Verbose", manager.getLevelName(level::verbose));
    }

    // test getDefaultPrefix for undefined context
    TEST(OutputManagerTest, GetDefaultPrefixWithoutContext)
    {
        OutputManager manager;
        std::string prefix = manager.getDefaultPrefix(level::debug_output, context::undefined());
        EXPECT_EQ(manager.getLevelName(level::debug_output) + ": ", prefix);
    }

    // test getDefaultPrefix for regular context
    TEST(OutputManagerTest, GetDefaultPrefixWithContext)
    {
        OutputManager manager;
        std::string prefix = manager.getDefaultPrefix(level::debug_output, context::unittest1());
        EXPECT_EQ(manager.getLevelName(level::debug_output) + ": [unittest1] ", prefix);
    }
}
