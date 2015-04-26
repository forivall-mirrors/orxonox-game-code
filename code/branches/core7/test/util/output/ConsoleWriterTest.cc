#include <gtest/gtest.h>
#include "util/Output.h"
#include "util/output/ConsoleWriter.h"
#include "util/output/OutputManager.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    namespace
    {
        // Fixture
        class ConsoleWriterTest : public ::testing::Test
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

    TEST_F(ConsoleWriterTest, Disable)
    {
        std::ostream stream(NULL);
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
        ConsoleWriter writer(stream);
        EXPECT_EQ(1U, OutputManager::getInstance().getListeners().size());
        writer.disable();
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
    }

    TEST_F(ConsoleWriterTest, Enable)
    {
        std::ostream stream(NULL);
        ConsoleWriter writer(stream);
        writer.disable();
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
        writer.enable();
        EXPECT_EQ(1U, OutputManager::getInstance().getListeners().size());
    }

    TEST_F(ConsoleWriterTest, WritesNoOutputToOutputStream)
    {
        std::stringbuf buffer;
        std::ostream stream(&buffer);
        ConsoleWriter writer(stream);
        writer.setLevelMax(level::verbose);

        EXPECT_EQ(0, buffer.in_avail());
        EXPECT_EQ("", buffer.str());
    }

    TEST_F(ConsoleWriterTest, WritesOutputToOutputStream)
    {
        std::stringbuf buffer;
        std::ostream stream(&buffer);
        ConsoleWriter writer(stream);
        writer.setLevelMax(level::verbose);

        std::vector<std::string> lines;
        lines.push_back("test");

        EXPECT_TRUE(writer.acceptsOutput(level::debug_output, context::undefined()));

        writer.unfilteredOutput(level::debug_output, context::undefined(), lines);

        EXPECT_LT(0, buffer.in_avail());
        EXPECT_NE("", buffer.str());
        EXPECT_NE(std::string::npos, buffer.str().find("test"));
    }

    TEST_F(ConsoleWriterTest, DefaultConsoleWriterUsesCout)
    {
        OutputManager::getInstanceAndCreateListeners();
        EXPECT_EQ(std::cout, OutputManager::getInstance().getConsoleWriter()->getOutputStream());
    }
}
