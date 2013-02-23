#include <gtest/gtest.h>
#include "util/Output.h"

namespace orxonox
{
    namespace context
    {
        namespace
        {
            REGISTER_OUTPUT_CONTEXT(unittest);
        }
    }

    TEST(Output, CanUseOrxout)
    {
        orxout() << "test" << endl;
    }

    TEST(Output, OrxoutUsesCorrectOutputLevel)
    {
        {
            const OutputStream& stream = orxout(internal_warning);
            EXPECT_EQ(internal_warning, stream.getOutputLevel());
        }

        {
            const OutputStream& stream = orxout(debug_output);
            EXPECT_EQ(debug_output, stream.getOutputLevel());
        }
    }

    TEST(Output, OrxoutUsesCorrectOutputContext)
    {
        const OutputStream& stream = orxout(verbose, context::unittest());
        EXPECT_EQ(verbose, stream.getOutputLevel());
        EXPECT_EQ(context::unittest().mask, stream.getOutputContext()->mask);
        EXPECT_EQ(context::unittest().name, stream.getOutputContext()->name);
        EXPECT_EQ(context::unittest().sub_id, stream.getOutputContext()->sub_id);
    }

    TEST(Output, OrxoutAcceptsFunctionPointer)
    {
        const OutputStream& stream = orxout(verbose, context::unittest);
        EXPECT_EQ(verbose, stream.getOutputLevel());
        EXPECT_EQ(context::unittest().mask, stream.getOutputContext()->mask);
        EXPECT_EQ(context::unittest().name, stream.getOutputContext()->name);
        EXPECT_EQ(context::unittest().sub_id, stream.getOutputContext()->sub_id);
    }
}
