#include <gtest/gtest.h>
#include "util/Output.h"
#include "util/output/ConsoleWriter.h"
#include "util/output/OutputManager.h"

namespace orxonox
{
    TEST(ConsoleWriterTest, Disable)
    {
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
        ConsoleWriter writer;
        EXPECT_EQ(1U, OutputManager::getInstance().getListeners().size());
        writer.disable();
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
    }

    TEST(ConsoleWriterTest, Enable)
    {
        ConsoleWriter writer;
        writer.disable();
        EXPECT_EQ(0U, OutputManager::getInstance().getListeners().size());
        writer.enable();
        EXPECT_EQ(1U, OutputManager::getInstance().getListeners().size());
    }
}
