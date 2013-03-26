#include <gtest/gtest.h>
#include "util/SubString.h"

namespace orxonox
{
    TEST(SubString, Empty)
    {
        SubString test;
        EXPECT_TRUE(test.empty());
        EXPECT_EQ(0u, test.size());
    }

    TEST(SubString, CreateSimple)
    {
        SubString test("This is a test");
        EXPECT_FALSE(test.empty());
        ASSERT_EQ(4u, test.size());

        EXPECT_EQ("This", test[0]);
        EXPECT_EQ("is", test[1]);
        EXPECT_EQ("a", test[2]);
        EXPECT_EQ("test", test[3]);

        EXPECT_EQ("This", test.getString(0));
        EXPECT_EQ("is", test.getString(1));
        EXPECT_EQ("a", test.getString(2));
        EXPECT_EQ("test", test.getString(3));

        EXPECT_EQ("This", test.front());
        EXPECT_EQ("test", test.back());
    }

    TEST(SubString, CreateDelimiter)
    {
        SubString test1("This|is|a|test");
        EXPECT_EQ(1u, test1.size());

        SubString test2("This|is|a|test", "|");
        EXPECT_EQ(4u, test2.size());

        SubString test3("This|is|a|test", "|e");
        EXPECT_EQ(5u, test3.size());
    }

    TEST(SubString, CreateDelimiterNeighbours)
    {
        SubString test1("This | is | a | test", "|");
        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ(" a ", test1[2]);

        SubString test2("This | is | a | test", "|", " ");
        EXPECT_EQ(4u, test2.size());
        EXPECT_EQ("a", test2[2]);
    }

    TEST(SubString, CreateAllowEmptyEntries)
    {
        SubString test1("This |  | is |  | a ||| test", "|", " ");
        EXPECT_EQ(4u, test1.size());

        SubString test2("This |  | is |  | a ||| test", "|", " ", true);
        EXPECT_EQ(8u, test2.size());
    }

    TEST(SubString, CreateEscapeChar)
    {
        SubString test1("This is a test", SubString::WhiteSpaces, "", false, '\\', true);
        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ("is", test1[1]);

        SubString test2("This is\\ a test", SubString::WhiteSpaces, "", false, '\\', true);
        EXPECT_EQ(3u, test2.size());
        EXPECT_EQ("is a", test2[1]);

        SubString test3("This is\\ a test", SubString::WhiteSpaces, "", false, '\\', false);
        EXPECT_EQ(3u, test3.size());
        EXPECT_EQ("is\\ a", test3[1]);
    }

    TEST(SubString, CreateSafemodeChar)
    {
        SubString test1("This is a test", SubString::WhiteSpaces, "", false, '\\', true, '"', true);
        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ("is", test1[1]);

        SubString test2("This \"is a\" test", SubString::WhiteSpaces, "", false, '\\', true, '"', true);
        EXPECT_EQ(3u, test2.size());
        EXPECT_EQ("is a", test2[1]);

        SubString test3("This \"is a\" test", SubString::WhiteSpaces, "", false, '\\', true, '"', false);
        EXPECT_EQ(3u, test3.size());
        EXPECT_EQ("\"is a\"", test3[1]);
    }

    TEST(SubString, CreateParenthesisChars)
    {
        SubString test1("This is a test", SubString::WhiteSpaces, "", false, '\\', true, '"', true, '{', '}', true);
        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ("is", test1[1]);

        SubString test2("This {is a} test", SubString::WhiteSpaces, "", false, '\\', true, '"', true, '{', '}', true);
        EXPECT_EQ(3u, test2.size());
        EXPECT_EQ("is a", test2[1]);

        SubString test3("This {is a} test", SubString::WhiteSpaces, "", false, '\\', true, '"', true, '{', '}', false);
        EXPECT_EQ(3u, test3.size());
        EXPECT_EQ("{is a}", test3[1]);
    }

    TEST(SubString, Compare)
    {
        SubString test1("This is a test");
        SubString test2("This is a test");
        SubString test3("This is not a test");

        EXPECT_TRUE(test1 == test2);
        EXPECT_TRUE(test1.compare(test2));

        EXPECT_FALSE(test1 == test3);
        EXPECT_FALSE(test1.compare(test3));

        EXPECT_FALSE(test1.compare(test3, 100));
        EXPECT_FALSE(test1.compare(test3, 4));
        EXPECT_FALSE(test1.compare(test3, 3));
        EXPECT_TRUE(test1.compare(test3, 2));
        EXPECT_TRUE(test1.compare(test3, 1));
        EXPECT_TRUE(test1.compare(test3, 0));
    }

    TEST(SubString, Assign)
    {
        SubString test1("This is a test");
        SubString test2("Hello World");

        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ(2u, test2.size());
        EXPECT_FALSE(test1 == test2);

        test2 = test1;

        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ(4u, test2.size());
        EXPECT_TRUE(test1 == test2);
    }

    TEST(SubString, Append)
    {
        SubString test1("This is a test");
        SubString test2("Hello World");

        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ(2u, test2.size());

        test1 += test2;

        EXPECT_EQ(6u, test1.size());
        EXPECT_EQ(2u, test2.size());
    }

    TEST(SubString, Join)
    {
        SubString test1("This is a test");

        EXPECT_EQ("This is a test", test1.join());
        EXPECT_EQ("This|is|a|test", test1.join("|"));
        EXPECT_EQ("This123is123a123test", test1.join("123"));

        SubString test2("Hello World");

        EXPECT_EQ("Hello World", test2.join());

        test1 += test2;

        EXPECT_EQ("This is a test Hello World", test1.join());
    }

    TEST(SubString, Subset)
    {
        SubString test1("This is a test");
        EXPECT_EQ(4u, test1.size());
        EXPECT_EQ("This is a test", test1.join());

        SubString test2 = test1.subSet(1);
        ASSERT_EQ(3u, test2.size());
        EXPECT_EQ("is a test", test2.join());

        SubString test3 = test1.subSet(1, 2);
        ASSERT_EQ(2u, test3.size());
        EXPECT_EQ("is a", test3.join());
    }
}
