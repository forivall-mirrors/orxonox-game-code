#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/Output.h"
#include "util/output/MemoryWriter.h"
#include "util/output/OutputManager.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    namespace
    {
        class MockOutputListener : public OutputListener
        {
            public:
                MOCK_METHOD3(output, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));
        };

        // Fixture
        class MemoryWriterTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    // reset output manager
                    OutputManager::Testing::getInstancePointer() = new OutputManager();
                }

                virtual void TearDown()
                {
                }
        };
    }

    TEST_F(MemoryWriterTest, Disable)
    {
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
        MemoryWriter writer;
        EXPECT_EQ(1U, OutputManager::getInstance().getListeners().size());
        writer.disable();
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
    }

    TEST_F(MemoryWriterTest, ResendOutput)
    {
        MemoryWriter writer;

        std::vector<std::string> lines;
        lines.push_back("random line of output");
        lines.push_back("another line of output");

        writer.unfilteredOutput(level::user_info, context::undefined(), lines);
        writer.unfilteredOutput(level::verbose, context::xml(), lines);

        MockOutputListener other;
        other.setLevelMask(level::all);

        EXPECT_CALL(other, output(level::user_info, context::undefined(), lines));
        EXPECT_CALL(other, output(level::verbose, context::xml(), lines));

        writer.resendOutput(&other);
    }
}
