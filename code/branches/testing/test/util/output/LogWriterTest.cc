#include <gtest/gtest.h>
#include "util/Output.h"
#include "util/output/LogWriter.h"
#include "util/Convert.h"

namespace orxonox
{
    namespace
    {
        class MockLogWriter : public LogWriter
        {
            public:
                virtual void printLine(const std::string& line, OutputLevel level)
                    { this->LogWriter::printLine(line, level); }
        };
    }

    // test constructor opens file
    TEST(LogWriterTest, ConstructorOpensFile)
    {
        LogWriter logWriter;
        EXPECT_TRUE(logWriter.getFile().is_open());
    }

    // setLogDirectory changes file, opens correct file
    bool fileExists(const std::string& path)
    {
        std::ifstream stream(path.c_str());
        bool exists = stream.is_open() && stream.good();
        stream.close();
        return exists;
    }

    std::string getLineWhichContains(const std::string& path, const std::string& message)
    {
        std::ifstream file(path.c_str());
        EXPECT_TRUE(file.is_open());
        EXPECT_TRUE(file.good());
        EXPECT_FALSE(file.eof());

        while (file.is_open() && file.good() && !file.eof())
        {
            std::string line;
            std::getline(file, line);

            // see if we find the output and return
            if (line.find(message) == (line.size() - message.size()))
                return line;
        }

        return std::string();
    }

    bool fileContains(const std::string& path, const std::string& message)
    {
        return !getLineWhichContains(path, message).empty();
    }

    bool deleteFile(const std::string& path)
    {
        bool result = std::remove(path.c_str()) == 0;
        EXPECT_FALSE(fileExists(path));
        return result;
    }

    TEST(LogWriterTest, SetLogDirectoryOpensNewFile)
    {
        std::string path = "./orxonox.log";

        // cleanup before test
        deleteFile(path);

        {
            LogWriter logWriter;
            EXPECT_FALSE(fileExists(path));
            logWriter.setLogDirectory(".");
            EXPECT_TRUE(fileExists(path));
        }

        // cleanup after test
        EXPECT_TRUE(deleteFile(path));
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

        EXPECT_TRUE(fileContains(path, "mytestoutput"));
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

        std::string line = getLineWhichContains(path, "myothertestoutput");
        EXPECT_FALSE(line.empty());
        EXPECT_TRUE(std::isdigit(line[0]));
        EXPECT_TRUE(std::isdigit(line[1]));
        EXPECT_EQ(':', line[2]);
        EXPECT_TRUE(std::isdigit(line[3]));
        EXPECT_TRUE(std::isdigit(line[4]));
        EXPECT_EQ(':', line[5]);
        EXPECT_TRUE(std::isdigit(line[6]));
        EXPECT_TRUE(std::isdigit(line[7]));
        EXPECT_EQ(' ', line[8]);
    }

    void deleteAllLogFiles()
    {
        std::string path;
        {
            LogWriter writer;
            path = writer.getPath();
        }

        deleteFile(path);
        deleteFile(path + ".1");
        deleteFile(path + ".2");
        deleteFile(path + ".3");
        deleteFile(path + ".4");
        deleteFile(path + ".5");
        deleteFile(path + ".6");
        deleteFile(path + ".7");
        deleteFile(path + ".8");
        deleteFile(path + ".9");
    }

    TEST(LogWriterTest, ArchivesOldLogFile)
    {
        deleteAllLogFiles();

        std::string path;

        {
            MockLogWriter writer;
            path = writer.getPath();
            writer.printLine("test1", level::message);
        }

        EXPECT_TRUE(fileExists(path));
        EXPECT_TRUE(fileContains(path, "test1"));
        EXPECT_FALSE(fileExists(path + ".1"));
        EXPECT_FALSE(fileExists(path + ".2"));
        EXPECT_FALSE(fileExists(path + ".3"));

        {
            MockLogWriter writer;
            writer.printLine("test2", level::message);
        }

        EXPECT_TRUE(fileExists(path));
        EXPECT_TRUE(fileContains(path, "test2"));
        EXPECT_TRUE(fileExists(path + ".1"));
        EXPECT_TRUE(fileContains(path + ".1", "test1"));
        EXPECT_FALSE(fileExists(path + ".2"));
        EXPECT_FALSE(fileExists(path + ".3"));

        {
            MockLogWriter writer;
            writer.printLine("test3", level::message);
        }

        EXPECT_TRUE(fileExists(path));
        EXPECT_TRUE(fileContains(path, "test3"));
        EXPECT_TRUE(fileExists(path + ".1"));
        EXPECT_TRUE(fileContains(path + ".1", "test2"));
        EXPECT_TRUE(fileExists(path + ".2"));
        EXPECT_TRUE(fileContains(path + ".2", "test1"));
        EXPECT_FALSE(fileExists(path + ".3"));
    }

    TEST(LogWriterTest, ArchivesNineLogFiles)
    {
        deleteAllLogFiles();

        std::string path;
        for (int i = 0; i < 20; ++i)
        {
            MockLogWriter writer;
            path = writer.getPath();
            writer.printLine("test" + multi_cast<std::string>(i), level::message);
        }

        EXPECT_TRUE(fileContains(path, "test19"));
        EXPECT_TRUE(fileContains(path + ".1", "test18"));
        EXPECT_TRUE(fileContains(path + ".2", "test17"));
        EXPECT_TRUE(fileContains(path + ".3", "test16"));
        EXPECT_TRUE(fileContains(path + ".4", "test15"));
        EXPECT_TRUE(fileContains(path + ".5", "test14"));
        EXPECT_TRUE(fileContains(path + ".6", "test13"));
        EXPECT_TRUE(fileContains(path + ".7", "test12"));
        EXPECT_TRUE(fileContains(path + ".8", "test11"));
        EXPECT_TRUE(fileContains(path + ".9", "test10"));
        EXPECT_FALSE(fileExists(path + ".10"));
        EXPECT_FALSE(fileExists(path + ".11"));
    }
}
