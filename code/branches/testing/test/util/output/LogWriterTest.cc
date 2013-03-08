#include <gtest/gtest.h>
#include "util/Output.h"
#include "util/output/LogWriter.h"

namespace orxonox
{
    // test constructor opens file
    TEST(LogWriterTest, ConstructorOpensFile)
    {
        LogWriter logWriter;
        EXPECT_TRUE(logWriter.getFile().is_open());
    }

    // setLogDirectory changes file, opens correct file
    bool fileExists(const std::string& path)
    {
        std::ifstream stream(path.c_str(), std::fstream::in);
        bool exists = stream.is_open();
        stream.close();
        return exists;
    }

    bool fileSuccessfullyDeleted(const std::string& path)
    {
        return std::remove(path.c_str()) == 0;
    }

    TEST(LogWriterTest, SetLogDirectoryOpensNewFile)
    {
        std::string path = "./orxonox.log";

        // cleanup before test
        fileSuccessfullyDeleted(path);

        {
            LogWriter logWriter;
            EXPECT_FALSE(fileExists(path));
            logWriter.setLogDirectory(".");
            EXPECT_TRUE(fileExists(path));
        }

        // cleanup after test
        EXPECT_TRUE(fileSuccessfullyDeleted(path));
    }

    // prints output to logfile
    TEST(LogWriterTest, PrintsOutputToLogfile)
    {
        std::string path;

        {
            // write lines to log file
            std::vector<std::string> lines;
            lines.push_back("mytestoutput");

            LogWriter logWriter;
            logWriter.setLogDirectory(".");
            logWriter.unfilteredOutput(level::debug_output, context::undefined(), lines);

            path = logWriter.getPath();
        }

        {
            std::ifstream file(path.c_str(), std::fstream::in);
            ASSERT_TRUE(file.is_open());
            ASSERT_TRUE(file.good());
            ASSERT_FALSE(file.eof());

            while (!file.eof())
            {
                std::string line;
                std::getline(file, line);

                // see if we find the output and return
                if (line.find("mytestoutput") != std::string::npos)
                    return;
            }

            // output not found - failure!
            FAIL();
        }
    }

    // prints time to logfile
    TEST(LogWriterTest, PrintsTimestampToLogfile)
    {
        std::string path;

        {
            // write lines to log file
            std::vector<std::string> lines;
            lines.push_back("myothertestoutput");

            LogWriter logWriter;
            logWriter.setLogDirectory(".");
            logWriter.unfilteredOutput(level::debug_output, context::undefined(), lines);

            path = logWriter.getPath();
        }

        {
            std::ifstream file(path.c_str(), std::fstream::in);
            ASSERT_TRUE(file.is_open());
            ASSERT_TRUE(file.good());
            ASSERT_FALSE(file.eof());

            while (!file.eof())
            {
                std::string line;
                std::getline(file, line);

                // see if we find the output and return
                if (line.find("myothertestoutput") != std::string::npos)
                {
                    EXPECT_TRUE(std::isdigit(line[0]));
                    EXPECT_TRUE(std::isdigit(line[1]));
                    EXPECT_EQ(':', line[2]);
                    EXPECT_TRUE(std::isdigit(line[3]));
                    EXPECT_TRUE(std::isdigit(line[4]));
                    EXPECT_EQ(':', line[5]);
                    EXPECT_TRUE(std::isdigit(line[6]));
                    EXPECT_TRUE(std::isdigit(line[7]));
                    EXPECT_EQ(' ', line[8]);
                    return;
                }
            }

            // output not found - failure!
            FAIL();
        }
    }
}
