#include <gtest/gtest.h>
#include <gmock/gmock.h>
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
        }
    }

    namespace
    {
        class MockOutputListener : public OutputListener
        {
            public:
                MOCK_METHOD3(output, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));
        };
    }

    // test default settings
    TEST(OutputListener, DefaultConstructorAcceptsNothing)
    {
        MockOutputListener listener;

        EXPECT_EQ(level::none, listener.getLevelMask());
        EXPECT_EQ(level::none, listener.getAdditionalContextsLevelMask());
        EXPECT_EQ(context::none, listener.getAdditionalContextsMask());
    }

    // test setLevelMax
    TEST(OutputListener, TestSetLevelMax)
    {
        MockOutputListener listener;

        listener.setLevelMax(level::user_warning);

        OutputLevel mask = listener.getLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_TRUE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_TRUE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setLevelRange
    TEST(OutputListener, TestSetLevelRange)
    {
        MockOutputListener listener;

        listener.setLevelRange(level::debug_output, level::user_warning);

        OutputLevel mask = listener.getLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_FALSE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_TRUE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setLevelMask
    TEST(OutputListener, TestSetLevelMask)
    {
        MockOutputListener listener;

        listener.setLevelMask(static_cast<OutputLevel>(level::debug_output | level::user_warning));

        OutputLevel mask = listener.getLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_FALSE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_FALSE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setAdditionalContextsLevelMax
    TEST(OutputListener, TestSetAdditionalContextsLevelMax)
    {
        MockOutputListener listener;

        listener.setAdditionalContextsLevelMax(level::user_warning);

        OutputLevel mask = listener.getAdditionalContextsLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_TRUE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_TRUE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setAdditionalContextsLevelRange
    TEST(OutputListener, TestSetAdditionalContextsLevelRange)
    {
        MockOutputListener listener;

        listener.setAdditionalContextsLevelRange(level::debug_output, level::user_warning);

        OutputLevel mask = listener.getAdditionalContextsLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_FALSE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_TRUE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setAdditionalContextsLevelMask
    TEST(OutputListener, TestSetAdditionalContextsLevelMask)
    {
        MockOutputListener listener;

        listener.setAdditionalContextsLevelMask(static_cast<OutputLevel>(level::debug_output | level::user_warning));

        OutputLevel mask = listener.getAdditionalContextsLevelMask();

        EXPECT_FALSE(mask & level::none);
        EXPECT_FALSE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_FALSE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
    }

    // test setAdditionalContextsMask
    TEST(OutputListener, TestSetAdditionalContextsMask)
    {
        MockOutputListener listener;

        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest3().mask);

        OutputContextMask mask = listener.getAdditionalContextsMask();

        EXPECT_TRUE(mask & context::unittest1().mask);
        EXPECT_FALSE(mask & context::unittest2().mask);
        EXPECT_TRUE(mask & context::unittest3().mask);
    }

    // test setAdditionalContextsLevelMask calls OutputManager::updateCombinedAdditionalContextsLevelMask
    // test setAdditionalContextsMask calls OutputManager::updateCombinedAdditionalContextsMask

    // test acceptsOutput
    // test unfilteredOutput
}
