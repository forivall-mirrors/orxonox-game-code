#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/Output.h"
#include "util/output/SubcontextOutputListener.h"

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
        class MockSubcontextOutputListener : public SubcontextOutputListener
        {
            public:
                MockSubcontextOutputListener(bool bRegister = true) : SubcontextOutputListener(bRegister) {}
                MOCK_METHOD3(output, void(OutputLevel, const OutputContextContainer&, const std::vector<std::string>&));
        };
    }

    TEST(SubcontextOutputListenerTest, ContextsDefinedCorrectly)
    {
        EXPECT_TRUE(context::unittest1().sub_id == context::no_subcontext);
        EXPECT_TRUE(context::unittest2().sub_id == context::no_subcontext);
        EXPECT_TRUE(context::unittest3().sub_id == context::no_subcontext);
        EXPECT_FALSE(context::sub1().sub_id == context::no_subcontext);
        EXPECT_FALSE(context::sub2().sub_id == context::no_subcontext);
        EXPECT_FALSE(context::sub3().sub_id == context::no_subcontext);
    }

    // test setAdditionalContextsMask
    TEST(SubcontextOutputListenerTest, SetAdditionalContextsMask)
    {
        MockSubcontextOutputListener listener;

        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest2().mask);

        OutputContextMask mask = listener.getAdditionalContextsMask();

        EXPECT_TRUE(mask & context::unittest1().mask);
        EXPECT_TRUE(mask & context::unittest2().mask);
        EXPECT_FALSE(mask & context::unittest3().mask);
    }

    // test setAdditionalSubcontexts
    TEST(SubcontextOutputListenerTest, SetAdditionalSubcontexts)
    {
        MockSubcontextOutputListener listener;

        std::set<const OutputContextContainer*> subcontexts;
        subcontexts.insert(&context::sub1());
        subcontexts.insert(&context::sub3());

        listener.setAdditionalSubcontexts(subcontexts);
        OutputContextMask mask = listener.getAdditionalContextsMask();

        EXPECT_FALSE(mask & context::unittest1().mask);
        EXPECT_FALSE(mask & context::unittest2().mask);
        EXPECT_TRUE(mask & context::unittest3().mask);
    }

    // test setAdditionalContextsMask and setAdditionalSubcontexts
    TEST(SubcontextOutputListenerTest, SetAdditionalContextsAndSubcontexts)
    {
        MockSubcontextOutputListener listener;

        std::set<const OutputContextContainer*> subcontexts;
        subcontexts.insert(&context::sub1());
        subcontexts.insert(&context::sub3());

        listener.setAdditionalSubcontexts(subcontexts);
        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest2().mask);

        OutputContextMask mask = listener.getAdditionalContextsMask();

        EXPECT_TRUE(mask & context::unittest1().mask);
        EXPECT_TRUE(mask & context::unittest2().mask);
        EXPECT_TRUE(mask & context::unittest3().mask);
    }

    // test acceptsOutput
    namespace
    {
        void testContext(const OutputListener& listener, const OutputContextContainer& context, bool accepted)
        {
            EXPECT_TRUE(accepted == listener.acceptsOutput(level::message, context));
            EXPECT_TRUE(accepted == listener.acceptsOutput(level::user_error, context));
            EXPECT_TRUE(accepted == listener.acceptsOutput(level::internal_info, context));
        }
    }

    TEST(SubcontextOutputListenerTest, NoContext)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        testContext(listener, context::unittest1(), false);
        testContext(listener, context::unittest2(), false);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), false);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), false);
    }

    TEST(SubcontextOutputListenerTest, OneContext)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        listener.setAdditionalContextsMask(context::unittest1().mask);

        testContext(listener, context::unittest1(), true);
        testContext(listener, context::unittest2(), false);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), false);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), false);
    }

    TEST(SubcontextOutputListenerTest, TwoContexts)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest2().mask);

        testContext(listener, context::unittest1(), true);
        testContext(listener, context::unittest2(), true);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), false);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), false);
    }

    TEST(SubcontextOutputListenerTest, OneSubcontext)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        std::set<const OutputContextContainer*> subcontexts;
        subcontexts.insert(&context::sub1());

        listener.setAdditionalSubcontexts(subcontexts);

        testContext(listener, context::unittest1(), false);
        testContext(listener, context::unittest2(), false);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), true);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), false);
    }

    TEST(SubcontextOutputListenerTest, TwoSubcontexts)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        std::set<const OutputContextContainer*> subcontexts;
        subcontexts.insert(&context::sub1());
        subcontexts.insert(&context::sub3());

        listener.setAdditionalSubcontexts(subcontexts);

        testContext(listener, context::unittest1(), false);
        testContext(listener, context::unittest2(), false);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), true);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), true);
    }

    TEST(SubcontextOutputListenerTest, ContextsAndSubcontexts)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        std::set<const OutputContextContainer*> subcontexts;
        subcontexts.insert(&context::sub1());
        subcontexts.insert(&context::sub3());

        listener.setAdditionalContextsMask(context::unittest1().mask | context::unittest2().mask);
        listener.setAdditionalSubcontexts(subcontexts);

        testContext(listener, context::unittest1(), true);
        testContext(listener, context::unittest2(), true);
        testContext(listener, context::unittest3(), false);
        testContext(listener, context::sub1(), true);
        testContext(listener, context::sub2(), false);
        testContext(listener, context::sub3(), true);
    }

    TEST(SubcontextOutputListenerTest, BaseContextEnablesAllSubcontexts)
    {
        MockSubcontextOutputListener listener;
        listener.setAdditionalContextsLevelMax(level::verbose);

        listener.setAdditionalContextsMask(context::unittest3().mask);

        testContext(listener, context::unittest1(), false);
        testContext(listener, context::unittest2(), false);
        testContext(listener, context::unittest3(), true);
        testContext(listener, context::sub1(), true);
        testContext(listener, context::sub2(), true);
        testContext(listener, context::sub3(), true);
    }
}
