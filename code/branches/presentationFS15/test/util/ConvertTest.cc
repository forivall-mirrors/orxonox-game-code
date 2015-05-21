#include <gtest/gtest.h>
#include "util/Convert.h"
#include "util/Math.h"
#include "util/MathConvert.h"

namespace orxonox
{
    TEST(multi_cast, ToString)
    {
        // int
        EXPECT_EQ("5", multi_cast<std::string>(static_cast<int>(5)));
        EXPECT_EQ("-13", multi_cast<std::string>(static_cast<int>(-13)));

        // unsigned int
        EXPECT_EQ("0", multi_cast<std::string>(static_cast<unsigned int>(0)));
        EXPECT_EQ("4000000000", multi_cast<std::string>(static_cast<unsigned int>(4000000000LL)));

        // float
        EXPECT_EQ("0", multi_cast<std::string>(0.0f));
        EXPECT_EQ("10", multi_cast<std::string>(10.0f));
        EXPECT_EQ("123.456", multi_cast<std::string>(123.456f));
        {
            // expect 1.234e+012 or 1.234e+12
            std::string value = multi_cast<std::string>(1234000000000.0f);
            EXPECT_TRUE(value == "1.234e+012" || value == "1.234e+12");
        }

        // double
        EXPECT_EQ("0", multi_cast<std::string>(0.0));
        EXPECT_EQ("10", multi_cast<std::string>(10.0));
        EXPECT_EQ("123.456", multi_cast<std::string>(123.456));
        {
            // expect 1.234e+012 or 1.234e+12
            std::string value = multi_cast<std::string>(1234000000000.0);
            EXPECT_TRUE(value == "1.234e+012" || value == "1.234e+12");
        }

        // char
        EXPECT_EQ("a", multi_cast<std::string>('a'));
        EXPECT_EQ("\n", multi_cast<std::string>('\n'));

        // bool
        EXPECT_EQ("true", multi_cast<std::string>(true));
        EXPECT_EQ("false", multi_cast<std::string>(false));

        // string
        EXPECT_EQ("testTEST123", multi_cast<std::string>("testTEST123"));
        EXPECT_EQ("", multi_cast<std::string>(""));

        // TODO: Check consistency of conversion depending on MathConvert.h and reversibility
/*
        // Vector2
        EXPECT_EQ("Vector2(10.1, 20)", multi_cast<std::string>(Vector2(10.1, 20)));

        // Vector3
        EXPECT_EQ("Vector3(10.1, 20, 300000)", multi_cast<std::string>(Vector3(10.1, 20, 300000)));

        // Vector4
        EXPECT_EQ("Vector4(10.1, 20, 300000, -321)", multi_cast<std::string>(Vector4(10.1, 20, 300000, -321)));

        // Quaternion
        EXPECT_EQ("Quaternion(10.1, 20, 300000, -321)", multi_cast<std::string>(Quaternion(10.1, 20, 300000, -321)));

        // ColourValue
        EXPECT_EQ("ColourValue(10.1, 20, 300000, -321)", multi_cast<std::string>(ColourValue(10.1, 20, 300000, -321)));

        // Radian
        EXPECT_EQ("Radian(1.234)", multi_cast<std::string>(Radian(1.234)));

        // Degree
        EXPECT_EQ("Degree(123)", multi_cast<std::string>(Degree(123)));
*/
    }

    TEST(multi_cast, FromString)
    {
        // int
        EXPECT_EQ(5, multi_cast<int>(std::string("5")));
        EXPECT_EQ(-13, multi_cast<int>(std::string("-13")));

        // unsigned int
        EXPECT_EQ(0U, multi_cast<unsigned int>(std::string("0")));
        EXPECT_EQ(4000000000LL, multi_cast<unsigned int>(std::string("4000000000")));

        // float
        EXPECT_EQ(0.0f, multi_cast<float>(std::string("0")));
        EXPECT_EQ(10.0f, multi_cast<float>(std::string("10")));
        EXPECT_EQ(123.456f, multi_cast<float>(std::string("123.456")));
        EXPECT_EQ(1234000000000.0f, multi_cast<float>(std::string("1.234e+012")));

        // double
        EXPECT_EQ(0.0, multi_cast<double>(std::string("0")));
        EXPECT_EQ(10.0, multi_cast<double>(std::string("10")));
        EXPECT_EQ(123.456, multi_cast<double>(std::string("123.456")));
        EXPECT_EQ(1234000000000.0, multi_cast<double>(std::string("1.234e+012")));

        // char
        EXPECT_EQ('a', multi_cast<char>(std::string("a")));
        EXPECT_EQ('\n', multi_cast<char>(std::string("\n")));
        EXPECT_EQ('\0', multi_cast<char>(std::string("")));
        EXPECT_EQ('1', multi_cast<char>(std::string("123")));

        // bool
        EXPECT_TRUE(multi_cast<bool>(std::string("true")));
        EXPECT_FALSE(multi_cast<bool>(std::string("false")));
        EXPECT_FALSE(multi_cast<bool>(std::string("0")));
        EXPECT_FALSE(multi_cast<bool>(std::string("0.123")));
        EXPECT_TRUE(multi_cast<bool>(std::string("1")));
        EXPECT_TRUE(multi_cast<bool>(std::string("123.456")));

        // TODO: Check consistency of conversion depending on MathConvert.h and reversibility
/*
        // Vector2
        EXPECT_EQ(Vector2(10.1, 20), multi_cast<Vector2>(std::string("Vector2(10.1, 20)")));

        // Vector3
        EXPECT_EQ(Vector3(10.1, 20, 300000), multi_cast<Vector3>(std::string("Vector3(10.1, 20, 300000)")));

        // Vector4
        EXPECT_EQ(Vector4(10.1, 20, 300000, -321), multi_cast<Vector4>(std::string("Vector4(10.1, 20, 300000, -321)")));

        // Quaternion
        EXPECT_EQ(Quaternion(10.1, 20, 300000, -321), multi_cast<Quaternion>(std::string("Quaternion(10.1, 20, 300000, -321)")));

        // ColourValue
        EXPECT_EQ(ColourValue(10.1, 20, 300000, -321), multi_cast<ColourValue>(std::string("ColourValue(10.1, 20, 300000, -321)")));

        // Radian
        EXPECT_EQ(Radian(1.234), multi_cast<Radian>(std::string("Radian(1.234)")));

        // Degree
        EXPECT_EQ(Degree(123), multi_cast<Degree>(std::string("Degree(123)")));
*/
    }
}
