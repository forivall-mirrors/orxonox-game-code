#include <gtest/gtest.h>
#include "util/mbool.h"

namespace orxonox
{
    TEST(mbool, Create)
    {
        // default constructor
        mbool test1;
        EXPECT_FALSE((bool)test1);

        // initialize with false
        mbool test2(false);
        EXPECT_FALSE((bool)test2);

        // initialize with true
        mbool test3(true);
        EXPECT_TRUE((bool)test3);

        // copy constructor
        mbool test4(test2);
        EXPECT_FALSE((bool)test4);

        // copy constructor
        mbool test5(test3);
        EXPECT_TRUE((bool)test5);

        // default constructor & assignment
        mbool test6;
        test6 = test2;
        EXPECT_FALSE((bool)test6);

        // default constructor & assignment
        mbool test7;
        test7 = test3;
        EXPECT_TRUE((bool)test7);
    }

    TEST(mbool, Compare)
    {
        mbool test1(false);
        mbool test2(true);
        mbool test3(false);
        mbool test4(true);

        EXPECT_NE(test1, test2);
        EXPECT_EQ(test1, test3);
        EXPECT_EQ(test2, test4);

        EXPECT_FALSE(test1 == true);
        EXPECT_TRUE (test1 != true);
        EXPECT_FALSE(test1 == test2);
        EXPECT_TRUE (test1 != test2);
        EXPECT_TRUE (test1 == false);
        EXPECT_FALSE(test1 != false);
        EXPECT_TRUE (test1 == test3);
        EXPECT_FALSE(test1 != test3);


        EXPECT_FALSE(test2 == false);
        EXPECT_TRUE (test2 != false);
        EXPECT_FALSE(test2 == test1);
        EXPECT_TRUE (test2 != test1);
        EXPECT_TRUE (test2 == true);
        EXPECT_FALSE(test2 != true);
        EXPECT_TRUE (test2 == test4);
        EXPECT_FALSE(test2 != test4);

        EXPECT_EQ(!test1, true);
        EXPECT_EQ(!test2, false);
    }

    TEST(mbool, Assign)
    {
        mbool test(false);
        mbool test_false = false;
        mbool test_true = true;

        EXPECT_EQ(test, false);        // eq because initialized with false
        EXPECT_NE(test, true);         // "
        EXPECT_EQ(test, test_false);   // eq because both initialized with false
        EXPECT_NE(test, test_true);    // ne because initialized differently

        test = false; // expect no change after assigning false again

        EXPECT_EQ(test, false);        // no change, so still the same
        EXPECT_NE(test, true);         // "
        EXPECT_EQ(test, test_false);   // "
        EXPECT_NE(test, test_true);    // "

        test = true; // expect change after assigning true

        EXPECT_NE(test, false);        // ne because assigned true
        EXPECT_EQ(test, true);         // eq "
        EXPECT_NE(test, test_false);   // ne "
        EXPECT_EQ(test, test_true);    // eq "

        test = false; // expect change after assigning false again

        EXPECT_EQ(test, false);        // eq because assigned false again
        EXPECT_NE(test, true);         // ne "
        EXPECT_NE(test, test_false);   // ne because changed state in between, so internal memory differs !!!!!
        EXPECT_NE(test, test_true);    // ne because assigned false again
    }
}
