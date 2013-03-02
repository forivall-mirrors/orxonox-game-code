#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/output/OutputListener.h"
#include "util/output/OutputManager.h"
#include "util/SharedPtr.h"

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
                MockOutputListener(bool bRegister = true) : OutputListener(bRegister) {}

                MOCK_METHOD3(output, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));

                inline const std::vector<AdditionalContextListener*>& getListeners() const
                    { return OutputListener::getListeners(); }
        };

        class MockAdditionalContextListener : public AdditionalContextListener
        {
            public:
                MOCK_METHOD1(updatedLevelMask, void(const OutputListener*));
                MOCK_METHOD1(updatedAdditionalContextsLevelMask, void(const OutputListener*));
                MOCK_METHOD1(updatedAdditionalContextsMask, void(const OutputListener*));
        };

        class MockOutputManager : public OutputManager
        {
            public:
                MOCK_METHOD1(registerListener, void(OutputListener*));
                MOCK_METHOD1(unregisterListener, void(OutputListener*));
        };
    }

    // test default settings
    TEST(OutputListenerTest, DefaultConstructorAcceptsNothing)
    {
        MockOutputListener listener;

        EXPECT_EQ(level::none, listener.getLevelMask());
        EXPECT_EQ(level::none, listener.getAdditionalContextsLevelMask());
        EXPECT_EQ(context::none, listener.getAdditionalContextsMask());
    }

    // test setLevelMax
    TEST(OutputListenerTest, SetLevelMax)
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
    TEST(OutputListenerTest, SetLevelRange)
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
    TEST(OutputListenerTest, SetLevelMask)
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
    TEST(OutputListenerTest, SetAdditionalContextsLevelMax)
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
    TEST(OutputListenerTest, SetAdditionalContextsLevelRange)
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
    TEST(OutputListenerTest, SetAdditionalContextsLevelMask)
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
    TEST(OutputListenerTest, SetAdditionalContextsMask)
    {
        MockOutputListener listener;

        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest3().mask);

        OutputContextMask mask = listener.getAdditionalContextsMask();

        EXPECT_TRUE(mask & context::unittest1().mask);
        EXPECT_FALSE(mask & context::unittest2().mask);
        EXPECT_TRUE(mask & context::unittest3().mask);
    }

    // test registerListener
    TEST(OutputListenerTest, RegisterListener)
    {
        MockOutputListener outputListener(false);
        MockAdditionalContextListener additionalContextListener;

        EXPECT_EQ(0u, outputListener.getListeners().size());
        outputListener.registerListener(&additionalContextListener);
        EXPECT_EQ(1u, outputListener.getListeners().size());
        EXPECT_EQ(&additionalContextListener, outputListener.getListeners()[0]);
    }

    // test unregisterListener
    TEST(OutputListenerTest, UnregisterListener)
    {
        MockOutputListener outputListener(false);
        MockAdditionalContextListener additionalContextListener;

        outputListener.registerListener(&additionalContextListener);
        EXPECT_EQ(1u, outputListener.getListeners().size());
        EXPECT_EQ(&additionalContextListener, outputListener.getListeners()[0]);

        outputListener.unregisterListener(&additionalContextListener);
        EXPECT_EQ(0u, outputListener.getListeners().size());
    }

    // test setLevelMask calls OutputManager::updatedLevelMask
    TEST(OutputListenerTest, SetLevelMaskCallsListeners)
    {
        MockOutputListener listener;
        MockAdditionalContextListener additionalContextListener;
        listener.registerListener(&additionalContextListener);

        EXPECT_CALL(additionalContextListener, updatedLevelMask(&listener)).Times(1);

        listener.setLevelMask(level::debug_output);
    }

    // test setAdditionalContextsLevelMask calls OutputManager::updatedAdditionalContextsLevelMask
    TEST(OutputListenerTest, SetAdditionalContextsLevelMaskCallsListeners)
    {
        MockOutputListener listener;
        MockAdditionalContextListener additionalContextListener;
        listener.registerListener(&additionalContextListener);

        EXPECT_CALL(additionalContextListener, updatedAdditionalContextsLevelMask(&listener)).Times(1);

        listener.setAdditionalContextsLevelMask(level::debug_output);
    }

    // test setAdditionalContextsMask calls OutputManager::updatedAdditionalContextsMask
    TEST(OutputListenerTest, SetAdditionalContextsMaskCallsListeners)
    {
        MockOutputListener listener;
        MockAdditionalContextListener additionalContextListener;
        listener.registerListener(&additionalContextListener);

        EXPECT_CALL(additionalContextListener, updatedAdditionalContextsMask(&listener)).Times(1);

        listener.setAdditionalContextsMask(context::unittest1().mask);
    }

    // test acceptsOutput
    // test unfilteredOutput

    // Fixture
    class OutputListenerTestWithMockedOutputManager : public ::testing::Test
    {
        public:
            virtual void SetUp()
            {
                this->manager_ = new MockOutputManager();
                OutputManager::Testing::getInstancePointer() = this->manager_;
            }

            virtual void TearDown()
            {
                OutputManager::Testing::getInstancePointer() = new OutputManager();
            }

        protected:
            MockOutputManager* manager_;
    };

    // test default-constructor calls OutputManager::registerListener
    TEST_F(OutputListenerTestWithMockedOutputManager, ConstructorRegistersInOutputManager)
    {
        EXPECT_CALL(*this->manager_, registerListener(::testing::_)).Times(1);
        MockOutputListener listener;
    }

    // test prevent constructor from calling OutputManager::registerListener
    TEST_F(OutputListenerTestWithMockedOutputManager, PreventRegisteringInOutputManager)
    {
        EXPECT_CALL(*this->manager_, registerListener(::testing::_)).Times(0);
        MockOutputListener listener(false);
    }

    // test destructor calls OutputManager::unregisterListener
    TEST_F(OutputListenerTestWithMockedOutputManager, DestructorUnregistersFromOutputManager)
    {
        MockOutputListener listener;
        EXPECT_CALL(*this->manager_, unregisterListener(::testing::_)).Times(1);
    }
}
