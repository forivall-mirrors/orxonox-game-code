#include <gtest/gtest.h>
#include "util/Output.h"

namespace orxonox
{
    TEST(OutputDefinitions, Levels)
    {
        EXPECT_EQ(0x0000, level::none);
    }
}
