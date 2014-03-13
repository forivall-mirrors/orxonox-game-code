#include <gtest/gtest.h>
#include "util/StringUtils.h"

namespace orxonox
{
    TEST(StringUtils, BLANKSTRING)
    {
        EXPECT_EQ("", BLANKSTRING);
    }

    TEST(StringUtils, getUniqueNumberString)
    {
        std::set<std::string> numberStrings;

        const size_t iterations = 1000;

        for (size_t i = 0; i < iterations; ++i)
            numberStrings.insert(getUniqueNumberString());

        // must have exactly <iterations> entries in the set, since all strings should be unique
        EXPECT_EQ(iterations, numberStrings.size());
    }

//    _UtilExport std::string  getStripped(const std::string& str);
    TEST(StringUtils, getStripped)
    {
        EXPECT_EQ("", getStripped(""));
        EXPECT_EQ("test", getStripped("test"));
        EXPECT_EQ("test", getStripped("te st"));
        EXPECT_EQ("test", getStripped("   test"));
        EXPECT_EQ("test", getStripped("test  "));
        EXPECT_EQ("test", getStripped("   test   "));
        EXPECT_EQ("test", getStripped("   t  e  s  t   "));
        EXPECT_EQ("test", getStripped("  \t te \n\n\n  st\n"));
    }

//    _UtilExport std::string  removeTrailingWhitespaces(const std::string& str);
    TEST(StringUtils, removeTrailingWhitespaces)
    {
        EXPECT_EQ("", removeTrailingWhitespaces(""));
        EXPECT_EQ("test", removeTrailingWhitespaces("test"));
        EXPECT_EQ("test", removeTrailingWhitespaces("   test"));
        EXPECT_EQ("test", removeTrailingWhitespaces("test"));
        EXPECT_EQ("test", removeTrailingWhitespaces("   test   "));
        EXPECT_EQ("test", removeTrailingWhitespaces(" \t  test  \n "));
        EXPECT_EQ("te  st", removeTrailingWhitespaces("te  st"));
        EXPECT_EQ("te  st", removeTrailingWhitespaces("   te  st   "));
        EXPECT_EQ("Hel lo\nWo rld", removeTrailingWhitespaces(" \t Hel lo\nWo rld \n"));
    }

//    _UtilExport void         vectorize(const std::string& str, char delimiter, std::vector<std::string>* output);
    TEST(StringUtils, vectorize)
    {
        std::vector<std::string> output;
        vectorize("this is a test", ' ', &output);
        ASSERT_EQ(4u, output.size());
        EXPECT_EQ("this", output[0]);
        EXPECT_EQ("is", output[1]);
        EXPECT_EQ("a", output[2]);
        EXPECT_EQ("test", output[3]);

        vectorize("this is a test", 't', &output);
        ASSERT_EQ(4u, output.size());
        EXPECT_EQ("", output[0]);
        EXPECT_EQ("his is a ", output[1]);
        EXPECT_EQ("es", output[2]);
        EXPECT_EQ("", output[3]);
    }

//    _UtilExport size_t       getNextQuote(const std::string& str, size_t start = 0);
    TEST(StringUtils, getNextQuote)
    {
        EXPECT_EQ(std::string::npos, getNextQuote(""));
        EXPECT_EQ(std::string::npos, getNextQuote("text"));
        EXPECT_EQ(0u, getNextQuote("\"text"));
        EXPECT_EQ(4u, getNextQuote("text\""));
        EXPECT_EQ(std::string::npos, getNextQuote("\\\"text"));
        EXPECT_EQ(std::string::npos, getNextQuote("text\\\""));
        EXPECT_EQ(3u, getNextQuote("012\"456\"89", 0));
        EXPECT_EQ(7u, getNextQuote("012\"456\"89", 5));
    }

//    _UtilExport bool         isBetweenQuotes(const std::string& str, size_t pos);
    TEST(StringUtils, isBetweenQuotes)
    {
        EXPECT_FALSE(isBetweenQuotes("", 0));

        EXPECT_FALSE(isBetweenQuotes("abc", 0));
        EXPECT_FALSE(isBetweenQuotes("abc", 1));
        EXPECT_FALSE(isBetweenQuotes("abc", 2));
        EXPECT_FALSE(isBetweenQuotes("abc", 3));

        EXPECT_FALSE(isBetweenQuotes("\"abc", 0));
        EXPECT_FALSE(isBetweenQuotes("\"abc", 1));
        EXPECT_FALSE(isBetweenQuotes("\"abc", 2));
        EXPECT_FALSE(isBetweenQuotes("\"abc", 3));

        EXPECT_FALSE(isBetweenQuotes("abc\"", 0));
        EXPECT_FALSE(isBetweenQuotes("abc\"", 1));
        EXPECT_FALSE(isBetweenQuotes("abc\"", 2));
        EXPECT_FALSE(isBetweenQuotes("abc\"", 3));

        EXPECT_FALSE(isBetweenQuotes("\"abc\"", 0));
        EXPECT_TRUE(isBetweenQuotes("\"abc\"", 1));
        EXPECT_TRUE(isBetweenQuotes("\"abc\"", 2));
        EXPECT_TRUE(isBetweenQuotes("\"abc\"", 3));
        EXPECT_FALSE(isBetweenQuotes("\"abc\"", 4));

        //                            012 3456 7890
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 0));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 1));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 2));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 3));
        EXPECT_TRUE(isBetweenQuotes("abc\"abc\"abc", 4));
        EXPECT_TRUE(isBetweenQuotes("abc\"abc\"abc", 5));
        EXPECT_TRUE(isBetweenQuotes("abc\"abc\"abc", 6));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 7));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 8));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 9));
        EXPECT_FALSE(isBetweenQuotes("abc\"abc\"abc", 10));
    }

//    _UtilExport bool         hasStringBetweenQuotes(const std::string& str);
    TEST(StringUtils, hasStringBetweenQuotes)
    {
        EXPECT_FALSE(hasStringBetweenQuotes(""));
        EXPECT_FALSE(hasStringBetweenQuotes("test"));
        EXPECT_FALSE(hasStringBetweenQuotes("\"test"));
        EXPECT_FALSE(hasStringBetweenQuotes("te\"st"));
        EXPECT_FALSE(hasStringBetweenQuotes("test\""));
        EXPECT_TRUE(hasStringBetweenQuotes("\"test\""));
        EXPECT_TRUE(hasStringBetweenQuotes("te\"st\""));
        EXPECT_TRUE(hasStringBetweenQuotes("\"te\"st"));
        EXPECT_FALSE(hasStringBetweenQuotes("te\"\"st"));
        EXPECT_FALSE(hasStringBetweenQuotes("\"\"test"));
        EXPECT_FALSE(hasStringBetweenQuotes("test\"\""));
    }

//    _UtilExport std::string  getStringBetweenQuotes(const std::string& str);
    TEST(StringUtils, getStringBetweenQuotes)
    {
        EXPECT_EQ("", getStringBetweenQuotes(""));
        EXPECT_EQ("", getStringBetweenQuotes("test"));
        EXPECT_EQ("", getStringBetweenQuotes("\"test"));
        EXPECT_EQ("", getStringBetweenQuotes("te\"st"));
        EXPECT_EQ("", getStringBetweenQuotes("test\""));
        EXPECT_EQ("test", getStringBetweenQuotes("\"test\""));
        EXPECT_EQ("st", getStringBetweenQuotes("te\"st\""));
        EXPECT_EQ("te", getStringBetweenQuotes("\"te\"st"));
        EXPECT_EQ("", getStringBetweenQuotes("te\"\"st"));
        EXPECT_EQ("", getStringBetweenQuotes("\"\"test"));
        EXPECT_EQ("", getStringBetweenQuotes("test\"\""));
    }

//    _UtilExport std::string  stripEnclosingQuotes(const std::string& str);
    TEST(StringUtils, stripEnclosingQuotes)
    {
        EXPECT_EQ("", stripEnclosingQuotes(""));
        EXPECT_EQ("test", stripEnclosingQuotes("test"));
        EXPECT_EQ(" test", stripEnclosingQuotes(" test"));
        EXPECT_EQ("   test   ", stripEnclosingQuotes("   test   "));
        EXPECT_EQ("\"test", stripEnclosingQuotes("\"test"));
        EXPECT_EQ("test\"", stripEnclosingQuotes("test\""));
        EXPECT_EQ(" \"test", stripEnclosingQuotes(" \"test"));
        EXPECT_EQ("test\" ", stripEnclosingQuotes("test\" "));
        EXPECT_EQ("test", stripEnclosingQuotes("\"test\""));
        EXPECT_EQ("test", stripEnclosingQuotes("  \"test\"  "));
        EXPECT_EQ("a  \"test\"  ", stripEnclosingQuotes("a  \"test\"  "));
    }

//    _UtilExport std::string  stripEnclosingBraces(const std::string& str);
    TEST(StringUtils, stripEnclosingBraces)
    {
        EXPECT_EQ("", stripEnclosingBraces(""));
        EXPECT_EQ("test", stripEnclosingBraces("test"));
        EXPECT_EQ("{test", stripEnclosingBraces("{test"));
        EXPECT_EQ("test}", stripEnclosingBraces("test}"));
        EXPECT_EQ("t{es}t", stripEnclosingBraces("t{es}t"));
        EXPECT_EQ(" {test}", stripEnclosingBraces(" {test}"));
        EXPECT_EQ("}test}", stripEnclosingBraces("}test}"));
        EXPECT_EQ("{test{", stripEnclosingBraces("{test{"));
        EXPECT_EQ("test", stripEnclosingBraces("{test}"));
    }

//    _UtilExport bool         isEmpty(const std::string& str);
    TEST(StringUtils, isEmpty)
    {
        EXPECT_TRUE(isEmpty(""));
        EXPECT_TRUE(isEmpty(" "));
        EXPECT_TRUE(isEmpty("       "));
        EXPECT_TRUE(isEmpty("\t"));
        EXPECT_TRUE(isEmpty("\n"));
        EXPECT_TRUE(isEmpty(" \t\n\t\n  \t"));
        EXPECT_FALSE(isEmpty("a"));
        EXPECT_FALSE(isEmpty("test"));
        EXPECT_FALSE(isEmpty("    a   "));
    }

//    _UtilExport bool         isComment(const std::string& str);
    TEST(StringUtils, isComment)
    {
        EXPECT_FALSE(isComment(""));
        EXPECT_FALSE(isComment("test"));
        EXPECT_TRUE(isComment("#test"));
        EXPECT_TRUE(isComment("%test"));
        EXPECT_TRUE(isComment(";test"));
        EXPECT_TRUE(isComment("//test"));
        EXPECT_FALSE(isComment("/test"));

        EXPECT_TRUE(isComment("        #test"));
        EXPECT_TRUE(isComment("      %test"));
        EXPECT_TRUE(isComment("    ;test"));
        EXPECT_TRUE(isComment("  //test"));
    }

//    _UtilExport std::string  addSlashes(const std::string& str);
    TEST(StringUtils, addSlashes)
    {
        EXPECT_EQ("", addSlashes(""));
        EXPECT_EQ("test", addSlashes("test"));
        EXPECT_EQ("test\\ntest", addSlashes("test\ntest"));
        EXPECT_EQ("\\n\\t\\r\\f\\a\\b\\v\\\"\\\\", addSlashes("\n\t\r\f\a\b\v\"\\"));
    }

//    _UtilExport std::string  removeSlashes(const std::string& str);
    TEST(StringUtils, removeSlashes)
    {
        EXPECT_EQ("", removeSlashes(""));
        EXPECT_EQ("test", removeSlashes("test"));
        EXPECT_EQ("test\ntest", removeSlashes("test\\ntest"));
        EXPECT_EQ("\n\t\r\f\a\b\v\"\\", removeSlashes("\\n\\t\\r\\f\\a\\b\\v\\\"\\\\"));
    }

//    _UtilExport std::string  getLowercase(const std::string& str);
    TEST(StringUtils, getLowercase)
    {
        EXPECT_EQ("", getLowercase(""));
        EXPECT_EQ("test", getLowercase("test"));
        EXPECT_EQ("test", getLowercase("Test"));
        EXPECT_EQ("test", getLowercase("TeSt"));
        EXPECT_EQ("test", getLowercase("TEST"));
        EXPECT_EQ("a test b", getLowercase("a TEST b"));
    }

//    _UtilExport std::string  getUppercase(const std::string& str);
    TEST(StringUtils, getUppercase)
    {
        EXPECT_EQ("", getUppercase(""));
        EXPECT_EQ("TEST", getUppercase("test"));
        EXPECT_EQ("TEST", getUppercase("Test"));
        EXPECT_EQ("TEST", getUppercase("TeSt"));
        EXPECT_EQ("TEST", getUppercase("TEST"));
        EXPECT_EQ("A TEST B", getUppercase("A test B"));
    }

//    _UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2);
    TEST(StringUtils, nocaseCmp)
    {
        EXPECT_EQ(0, nocaseCmp("", ""));
        EXPECT_EQ(0, nocaseCmp("a", "a"));
        EXPECT_EQ(0, nocaseCmp("a", "A"));
        EXPECT_EQ(0, nocaseCmp("A", "a"));
        EXPECT_EQ(0, nocaseCmp("A", "A"));
        EXPECT_EQ(0, nocaseCmp("test", "test"));
        EXPECT_EQ(0, nocaseCmp("test", "TEST"));
        EXPECT_EQ(0, nocaseCmp("tESt", "test"));
        EXPECT_EQ(0, nocaseCmp("TesT", "teST"));

        EXPECT_EQ(strcmp("a", "b"), nocaseCmp("a", "b"));
        EXPECT_EQ(strcmp("b", "a"), nocaseCmp("b", "a"));
        EXPECT_EQ(strcmp("hello", "world"), nocaseCmp("hello", "world"));
    }

//    _UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2, size_t len);
    TEST(StringUtils, nocaseCmpWithLen)
    {
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 0));
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 1));
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 2));
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 3));
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 4));
        EXPECT_EQ(0, nocaseCmp("test", "TEST", 5));
    }

//    _UtilExport bool         hasComment(const std::string& str);
    TEST(StringUtils, hasComment)
    {
        EXPECT_FALSE(hasComment(""));
        EXPECT_FALSE(hasComment("test"));
        EXPECT_FALSE(hasComment(" asdf / asdf "));
        EXPECT_TRUE(hasComment("#test"));
        EXPECT_TRUE(hasComment(" %test"));
        EXPECT_TRUE(hasComment("test ;test"));
        EXPECT_TRUE(hasComment("test // test // test"));
    }

//    _UtilExport std::string  getComment(const std::string& str);
    TEST(StringUtils, getComment)
    {
        EXPECT_EQ("", getComment(""));
        EXPECT_EQ("", getComment("test"));
        EXPECT_EQ("", getComment(" asdf / asdf "));
        EXPECT_EQ("#test", getComment("#test"));
        EXPECT_EQ(" %test", getComment(" %test"));
        EXPECT_EQ(" ;test", getComment("test ;test"));
        EXPECT_EQ(" // test // test", getComment("test // test // test"));
    }

//    _UtilExport size_t       getNextCommentPosition(const std::string& str, size_t start = 0);
    TEST(StringUtils, getNextCommentPosition)
    {
        EXPECT_EQ(std::string::npos, getNextCommentPosition(""));
        EXPECT_EQ(std::string::npos, getNextCommentPosition("test"));
        EXPECT_EQ(std::string::npos, getNextCommentPosition(" asdf / asdf "));
        EXPECT_EQ(0u, getNextCommentPosition("#test"));
        EXPECT_EQ(0u, getNextCommentPosition(" %test"));
        EXPECT_EQ(4u, getNextCommentPosition("test ;test"));
        EXPECT_EQ(4u, getNextCommentPosition("test // test // test"));

        EXPECT_EQ(std::string::npos, getNextCommentPosition("", 10));
        EXPECT_EQ(std::string::npos, getNextCommentPosition("test", 10));
        EXPECT_EQ(std::string::npos, getNextCommentPosition(" asdf / asdf ", 3));
        EXPECT_EQ(std::string::npos, getNextCommentPosition("#test", 1));
        EXPECT_EQ(1u, getNextCommentPosition(" %test", 1));
        EXPECT_EQ(5u, getNextCommentPosition("test ;test", 5));
        EXPECT_EQ(12u, getNextCommentPosition("test // test // test", 6));
    }

//    _UtilExport size_t       replaceCharacters(std::string& str, char target, char replacement);
    TEST(StringUtils, replaceCharacters)
    {
        std::string test1 = "test";
        EXPECT_EQ(1u, replaceCharacters(test1, 'e', '3'));
        EXPECT_EQ("t3st", test1);

        std::string test2 = "test";
        EXPECT_EQ(2u, replaceCharacters(test2, 't', '1'));
        EXPECT_EQ("1es1", test2);
    }

//    _UtilExport unsigned int getLevenshteinDistance(const std::string& str1, const std::string& str2);
    TEST(StringUtils, getLevenshteinDistance)
    {
        EXPECT_EQ(0u, getLevenshteinDistance("test", "test"));
        EXPECT_EQ(1u, getLevenshteinDistance("test", "tes"));
        EXPECT_EQ(1u, getLevenshteinDistance("test", "testt"));
        EXPECT_EQ(1u, getLevenshteinDistance("test", "t3st"));
        EXPECT_EQ(1u, getLevenshteinDistance("test", "te1st"));
        EXPECT_EQ(2u, getLevenshteinDistance("test", "1es1"));
        EXPECT_EQ(3u, getLevenshteinDistance("test", "1es11"));
        EXPECT_EQ(4u, getLevenshteinDistance("test", "blub"));
        EXPECT_EQ(5u, getLevenshteinDistance("test", "abcde"));
        EXPECT_EQ(6u, getLevenshteinDistance("test", "abcdef"));
    }
}
