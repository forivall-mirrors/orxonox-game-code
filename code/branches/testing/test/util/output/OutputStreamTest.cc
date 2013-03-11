#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define DISABLE_OUTPUT_CONTEXT_STATIC_CACHE

#include "util/Output.h"
#include "util/output/OutputStream.h"
#include "util/output/OutputManager.h"
#include "util/output/MemoryWriter.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    namespace context
    {
        namespace
        {
            REGISTER_OUTPUT_CONTEXT(unittest1);
        }
    }

    namespace
    {
        class MockOutputManager : public OutputManager
        {
            public:
                MOCK_METHOD3(pushMessage, void(OutputLevel, const OutputContextContainer&, const std::string&));
        };
    }

    // test level and context in constructor
    TEST(OutputStreamTest, Constructor)
    {
        OutputStream stream(level::user_warning, context::unittest1());

        EXPECT_EQ(level::user_warning, stream.getOutputLevel());
        EXPECT_EQ(&context::unittest1(), stream.getOutputContext());
    }

    // test setOutputAttributes
    TEST(OutputStreamTest, SetOutputAttributes)
    {
        OutputStream stream;
        stream.setOutputAttributes(level::user_warning, context::unittest1());

        EXPECT_EQ(level::user_warning, stream.getOutputLevel());
        EXPECT_EQ(&context::unittest1(), stream.getOutputContext());
    }

    // test manipulator applied
    TEST(OutputStreamTest, ManipulatorApplied)
    {
        OutputStream stream;
        stream << "test";
        EXPECT_EQ(4u, stream.str().size());

        // apply manipulator
        stream << std::ends;
        EXPECT_EQ(5u, stream.str().size());
    }

    // test manipulator only applied if output accepted
    TEST(OutputStreamTest, ManipulatorOnlyAppliedIfOutputAccepted)
    {
        // disable MemoryWriter because otherwise we would always accept all output
        OutputManager::getInstance().getMemoryWriter().disable();

        {
            OutputStream stream;
            EXPECT_TRUE(stream.acceptsOutput());
            stream << std::ends;
            EXPECT_EQ(1u, stream.str().size());
        }

        {
            OutputStream stream(level::verbose, context::undefined());
            EXPECT_FALSE(stream.acceptsOutput());
            stream << std::ends;
            EXPECT_EQ(0u, stream.str().size());
        }
    }

    // Fixture
    class OutputStreamTestWithMockedOutputManager : public ::testing::Test
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

    // test endl-manipulator sends output to OutputManager
    TEST_F(OutputStreamTestWithMockedOutputManager, EndlSendsToOutputManager)
    {
        OutputStream stream;

        const OutputContextContainer& context = context::unittest1();

        stream.setOutputAttributes(level::user_warning, context);
        EXPECT_TRUE(stream.acceptsOutput());
        EXPECT_EQ(context, *stream.getOutputContext());

        EXPECT_CALL(*this->manager_, pushMessage(level::user_warning, context, "some output"));

        stream << "some output" << std::endl;
    }

    // test multiple lines sent to OutputManager
    TEST_F(OutputStreamTestWithMockedOutputManager, MultipleLinesSentToOutputManager)
    {
        OutputStream stream;

        const OutputContextContainer& context = context::unittest1();

        stream.setOutputAttributes(level::user_warning, context);
        EXPECT_TRUE(stream.acceptsOutput());
        EXPECT_EQ(context, *stream.getOutputContext());

        EXPECT_CALL(*this->manager_, pushMessage(level::user_warning, context, "some output"));
        EXPECT_CALL(*this->manager_, pushMessage(level::user_warning, context, "more output"));
        EXPECT_CALL(*this->manager_, pushMessage(level::user_warning, context, "yet another line of output"));

        stream << "some output" << std::endl;
        stream << "more output" << std::endl;
        stream << "yet another line of output" << std::endl;
    }

    // test output only sent if accepted
    TEST_F(OutputStreamTestWithMockedOutputManager, OutputOnlySentToOutputManagerIfAccepted)
    {
        // disable MemoryWriter because otherwise we would always accept all output
        OutputManager::getInstanceAndCreateListeners().getMemoryWriter().disable();

        OutputStream stream;

        const OutputContextContainer& context = context::unittest1();

        stream.setOutputAttributes(level::verbose, context);
        EXPECT_FALSE(stream.acceptsOutput());

        EXPECT_CALL(*this->manager_, pushMessage(::testing::_, ::testing::_, ::testing::_)).Times(0);

        stream << "some output" << std::endl;
    }

    // test desctructor sends remaining output to OutputManager
    TEST_F(OutputStreamTestWithMockedOutputManager, DestructorSendsRemainingOutputToOutputManager)
    {
        OutputStream stream;

        const OutputContextContainer& context = context::undefined();

        stream.setOutputAttributes(level::verbose, context);
        EXPECT_TRUE(stream.acceptsOutput());

        EXPECT_CALL(*this->manager_, pushMessage(::testing::_, ::testing::_, "some output [missing endl]"));

        stream << "some output";
    }
}
