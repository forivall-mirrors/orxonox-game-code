#include <gtest/gtest.h>
#include "util/VA_NARGS.h"

TEST(VA_NARGS, DirectCall)
{
    //EXPECT_EQ(0, ORXONOX_VA_NARGS()); // documented issue of the macro - min number of elements is 1

    EXPECT_EQ(1, ORXONOX_VA_NARGS(some));
    EXPECT_EQ(2, ORXONOX_VA_NARGS(some, argument));
    EXPECT_EQ(3, ORXONOX_VA_NARGS(some, argument, to));
    EXPECT_EQ(4, ORXONOX_VA_NARGS(some, argument, to, test));
    EXPECT_EQ(5, ORXONOX_VA_NARGS(some, argument, to, test, the));
    EXPECT_EQ(6, ORXONOX_VA_NARGS(some, argument, to, test, the, macro));
}


#define TEST_MACRO_CONSTANT(...) \
    BOOST_PP_EXPAND(BOOST_PP_CAT(TEST_MACRO_CONSTANT, ORXONOX_VA_NARGS(__VA_ARGS__)))

#define TEST_MACRO_CONSTANT0 000
#define TEST_MACRO_CONSTANT1 100
#define TEST_MACRO_CONSTANT2 200
#define TEST_MACRO_CONSTANT3 300
#define TEST_MACRO_CONSTANT4 400
#define TEST_MACRO_CONSTANT5 500
#define TEST_MACRO_CONSTANT6 600

TEST(VA_NARGS, MacroConstant)
{
    //EXPECT_EQ(000, TEST_MACRO_CONSTANT()); // documented issue of the macro - min number of elements is 1

    EXPECT_EQ(100, TEST_MACRO_CONSTANT(some));
    EXPECT_EQ(200, TEST_MACRO_CONSTANT(some, argument));
    EXPECT_EQ(300, TEST_MACRO_CONSTANT(some, argument, to));
    EXPECT_EQ(400, TEST_MACRO_CONSTANT(some, argument, to, test));
    EXPECT_EQ(500, TEST_MACRO_CONSTANT(some, argument, to, test, the));
    EXPECT_EQ(600, TEST_MACRO_CONSTANT(some, argument, to, test, the, macro));
}


#define TEST_MACRO_FUNCTION(...) \
    BOOST_PP_EXPAND(BOOST_PP_CAT(TEST_MACRO_FUNCTION, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__))

#define TEST_MACRO_FUNCTION0(...) 000
#define TEST_MACRO_FUNCTION1(...) 100
#define TEST_MACRO_FUNCTION2(...) 200
#define TEST_MACRO_FUNCTION3(...) 300
#define TEST_MACRO_FUNCTION4(...) 400
#define TEST_MACRO_FUNCTION5(...) 500
#define TEST_MACRO_FUNCTION6(...) 600

TEST(VA_NARGS, MacroFunction)
{
    //EXPECT_EQ(000, TEST_MACRO_FUNCTION()); // documented issue of the macro - min number of elements is 1

    EXPECT_EQ(100, TEST_MACRO_FUNCTION(some));
    EXPECT_EQ(200, TEST_MACRO_FUNCTION(some, argument));
    EXPECT_EQ(300, TEST_MACRO_FUNCTION(some, argument, to));
    EXPECT_EQ(400, TEST_MACRO_FUNCTION(some, argument, to, test));
    EXPECT_EQ(500, TEST_MACRO_FUNCTION(some, argument, to, test, the));
    EXPECT_EQ(600, TEST_MACRO_FUNCTION(some, argument, to, test, the, macro));
}
