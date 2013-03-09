#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/Output.h"
#include "util/output/BaseWriter.h"
#include "util/output/OutputManager.h"

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
        class MockBaseWriter : public BaseWriter
        {
            public:
                MockBaseWriter(const std::string& name, bool bRegister = true) : BaseWriter(name, bRegister) {}

                MOCK_METHOD2(printLine, void(const std::string&, OutputLevel));

                virtual void output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
                    { this->BaseWriter::output(level, context, lines); }
        };
    }

    // test getName
    TEST(BaseWriterTest, GetName)
    {
        MockBaseWriter writer("mytestname");
        EXPECT_EQ("mytestname", writer.getName());
    }

    // test configurable names contain name
    TEST(BaseWriterTest, ConfigurableNamesContainName)
    {
        MockBaseWriter writer("mytestname");
        EXPECT_NE(writer.getConfigurableMaxLevelName().find(writer.getName()), std::string::npos);
        EXPECT_NE(writer.getConfigurableAdditionalContextsMaxLevelName().find(writer.getName()), std::string::npos);
        EXPECT_NE(writer.getConfigurableAdditionalContextsName().find(writer.getName()), std::string::npos);

        // but the section name is 'static' (does not contain the name)
        EXPECT_EQ(writer.getConfigurableSectionName().find(writer.getName()), std::string::npos);
    }

    // output calls printLine for each line
    TEST(BaseWriterTest, OutputCallsPrintLine)
    {
        MockBaseWriter writer("mytestname");

        std::vector<std::string> lines;
        lines.push_back("line1");
        lines.push_back("line2");

        EXPECT_CALL(writer, printLine(::testing::_, level::debug_output)).Times(2);

        writer.output(level::debug_output, context::undefined(), lines);
    }

    // output adds prefix to first line
    TEST(BaseWriterTest, OutputAddsPrefixToFirstLine)
    {
        MockBaseWriter writer("mytestname");

        std::vector<std::string> lines;
        lines.push_back("line1");

        std::string prefix = OutputManager::getInstance().getDefaultPrefix(level::debug_output, context::undefined());
        EXPECT_CALL(writer, printLine(prefix + "line1", level::debug_output));

        writer.output(level::debug_output, context::undefined(), lines);
    }

    // setLevelMax sets level max
    void testLevelMaxUserWarning(OutputLevel mask)
    {
        EXPECT_FALSE(mask & level::none);
        EXPECT_TRUE(mask & level::message);
        EXPECT_TRUE(mask & level::debug_output);
        EXPECT_TRUE(mask & level::user_error);
        EXPECT_TRUE(mask & level::user_warning);
        EXPECT_FALSE(mask & level::user_status);
        EXPECT_FALSE(mask & level::verbose);
    }

    TEST(BaseWriterTest, SetLevelMax)
    {
        MockBaseWriter writer("mytestname");

        writer.setLevelMax(level::user_warning);
        testLevelMaxUserWarning(writer.getLevelMask());
    }

    // setAdditionalContextsLevelMax sets additional contexts level max
    TEST(BaseWriterTest, SetAdditionalContextsLevelMax)
    {
        MockBaseWriter writer("mytestname");

        writer.setAdditionalContextsLevelMax(level::user_warning);
        testLevelMaxUserWarning(writer.getAdditionalContextsLevelMask());
    }

    // changedConfigurableLevel sets max level to configurableMaxLevel_
    TEST(BaseWriterTest, ChangedConfigurableLevel)
    {
        MockBaseWriter writer("mytestname");

        writer.configurableMaxLevel_ = level::user_warning;
        writer.changedConfigurableLevel();

        testLevelMaxUserWarning(writer.getLevelMask());
    }

    // changedConfigurableAdditionalContextsLevel sets additional contexts level max to configurableAdditionalContextsMaxLevel_
    TEST(BaseWriterTest, ChangedConfigurableAdditionalContextsLevel)
    {
        MockBaseWriter writer("mytestname");

        writer.configurableAdditionalContextsMaxLevel_ = level::user_warning;
        writer.changedConfigurableAdditionalContextsLevel();

        testLevelMaxUserWarning(writer.getAdditionalContextsLevelMask());
    }

    // changedConfigurableAdditionalContexts sets additional contexts to configurableAdditionalContexts_
    TEST(BaseWriterTest, ChangedConfigurableAdditionalContexts)
    {
        MockBaseWriter writer("mytestname");

        writer.configurableAdditionalContexts_.push_back("unittest1");
        writer.changedConfigurableAdditionalContexts();

        EXPECT_TRUE(writer.getAdditionalContextsMask() & context::unittest1().mask);
    }

    // contexts added by name
    TEST(BaseWriterTest, DefineContextsByName)
    {
        MockBaseWriter writer("mytestname");

        writer.configurableAdditionalContexts_.push_back("unittest1");
        writer.configurableAdditionalContexts_.push_back("unittest2");
        writer.changedConfigurableAdditionalContexts();

        EXPECT_TRUE(writer.acceptsOutput(level::message, context::unittest1()));
        EXPECT_TRUE(writer.acceptsOutput(level::message, context::unittest2()));
        EXPECT_FALSE(writer.acceptsOutput(level::message, context::unittest3()));
    }

    // subcontexts added by name, separated from base context with "::"
    TEST(BaseWriterTest, DefineSubcontextsByName)
    {
        MockBaseWriter writer("mytestname");

        writer.configurableAdditionalContexts_.push_back("unittest3::sub1");
        writer.configurableAdditionalContexts_.push_back("unittest3::sub3");
        writer.changedConfigurableAdditionalContexts();

        EXPECT_FALSE(writer.acceptsOutput(level::message, context::unittest3()));
        EXPECT_FALSE(writer.acceptsOutput(level::message, context::unittest3()));
        EXPECT_FALSE(writer.acceptsOutput(level::message, context::unittest3()));
        EXPECT_TRUE(writer.acceptsOutput(level::message, context::sub1()));
        EXPECT_FALSE(writer.acceptsOutput(level::message, context::sub2()));
        EXPECT_TRUE(writer.acceptsOutput(level::message, context::sub3()));
    }
}
