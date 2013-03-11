#include <gtest/gtest.h>
#include "util/Output.h"

namespace orxonox
{
    TEST(OutputDefinitionsTest, Levels)
    {
        EXPECT_EQ(0x0000, level::none);
    }

    TEST(OutputDefinitionsTest, OutputContextContainer)
    {
        OutputContextContainer container1;
        container1.mask = 1;
        container1.sub_id = 1;
        container1.name = "1";

        OutputContextContainer container2;
        container2.mask = 1;
        container2.sub_id = 1;
        container2.name = "1";

        EXPECT_TRUE(container1 == container2);

        {
            OutputContextContainer container3(container2);
            EXPECT_TRUE(container1 == container3);
            container3.mask = 2;
            EXPECT_FALSE(container1 == container3);
        }

        {
            OutputContextContainer container3(container2);
            EXPECT_TRUE(container1 == container3);
            container3.sub_id = 2;
            EXPECT_FALSE(container1 == container3);
        }

        {
            OutputContextContainer container3(container2);
            EXPECT_TRUE(container1 == container3);
            container3.name = "2";
            EXPECT_FALSE(container1 == container3);
        }
    }
}
