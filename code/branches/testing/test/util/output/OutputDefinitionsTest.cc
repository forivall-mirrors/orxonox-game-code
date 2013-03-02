#include <gtest/gtest.h>
#include "util/Output.h"

namespace orxonox
{
    TEST(OutputDefinitionsTest, Levels)
    {
        EXPECT_EQ(0x0000, level::none);
    }
}
