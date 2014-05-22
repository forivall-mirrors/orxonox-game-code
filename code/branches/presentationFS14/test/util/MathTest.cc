#include <gtest/gtest.h>
#include "util/Math.h"
#include <cmath>

namespace orxonox
{
    ///////////
    // sgn() //
    ///////////
    TEST(sgn, PositiveInt)
    {
        EXPECT_EQ(1, sgn( 1));
        EXPECT_EQ(1, sgn(10));
    }

    TEST(sgn, PositiveDouble)
    {
        EXPECT_DOUBLE_EQ(1.0, sgn( 0.1));
        EXPECT_DOUBLE_EQ(1.0, sgn( 1.0));
        EXPECT_DOUBLE_EQ(1.0, sgn(10.0));
    }

    TEST(sgn, NegativeInt)
    {
        EXPECT_EQ(-1, sgn( -1));
        EXPECT_EQ(-1, sgn(-10));
    }

    TEST(sgn, NegativeDouble)
    {
        EXPECT_DOUBLE_EQ(-1.0, sgn( -0.1));
        EXPECT_DOUBLE_EQ(-1.0, sgn( -1.0));
        EXPECT_DOUBLE_EQ(-1.0, sgn(-10.0));
    }

    TEST(sgn, ZeroInt)
    {
        EXPECT_EQ(1, sgn(0));
    }

    TEST(sgn, ZeroDouble)
    {
        EXPECT_DOUBLE_EQ(1.0, sgn(0.0));
    }

    /////////////
    // clamp() //
    /////////////
    TEST(clamp, InRange)
    {
        // positive
        EXPECT_EQ(10, clamp(10, 10, 20));
        EXPECT_EQ(15, clamp(15, 10, 20));
        EXPECT_EQ(20, clamp(20, 10, 20));

        // netagive
        EXPECT_EQ(-10, clamp(-10, -20, -10));
        EXPECT_EQ(-15, clamp(-15, -20, -10));
        EXPECT_EQ(-20, clamp(-20, -20, -10));

        // mixed
        EXPECT_EQ(-10, clamp(-10, -10, 10));
        EXPECT_EQ(  0, clamp(  0, -10, 10));
        EXPECT_EQ( 10, clamp( 10, -10, 10));
    }

    TEST(clamp, OutOfRange)
    {
        // positive
        EXPECT_EQ(10, clamp( 5, 10, 20));
        EXPECT_EQ(20, clamp(25, 10, 20));

        // netagive
        EXPECT_EQ(-10, clamp( -5, -20, -10));
        EXPECT_EQ(-20, clamp(-25, -20, -10));

        // mixed
        EXPECT_EQ(-10, clamp(-15, -10, 10));
        EXPECT_EQ( 10, clamp( 15, -10, 10));
    }

    //////////////
    // square() //
    //////////////
    TEST(square, Int)
    {
        // zero
        EXPECT_EQ( 0, square(0));

        // positive
        EXPECT_EQ(1, square(1));
        EXPECT_EQ(4, square(2));
        EXPECT_EQ(9, square(3));

        // negative
        EXPECT_EQ(1, square(-1));
        EXPECT_EQ(4, square(-2));
        EXPECT_EQ(9, square(-3));
    }

    TEST(square, Double)
    {
        // zero
        EXPECT_DOUBLE_EQ( 0.0, square(0.0));

        // positive
        EXPECT_DOUBLE_EQ( 1.00, square(1.0));
        EXPECT_DOUBLE_EQ( 2.25, square(1.5));
        EXPECT_DOUBLE_EQ( 4.00, square(2.0));

        // negative
        EXPECT_DOUBLE_EQ( 1.00, square(-1.0));
        EXPECT_DOUBLE_EQ( 2.25, square(-1.5));
        EXPECT_DOUBLE_EQ( 4.00, square(-2.0));
    }

    ////////////
    // cube() //
    ////////////
    TEST(cube, Int)
    {
        // zero
        EXPECT_EQ( 0, cube(0));

        // positive
        EXPECT_EQ( 1, cube(1));
        EXPECT_EQ( 8, cube(2));
        EXPECT_EQ(27, cube(3));

        // negative
        EXPECT_EQ( -1, cube(-1));
        EXPECT_EQ( -8, cube(-2));
        EXPECT_EQ(-27, cube(-3));
    }

    TEST(cube, Double)
    {
        // zero
        EXPECT_DOUBLE_EQ( 0.0, cube(0.0));

        // positive
        EXPECT_DOUBLE_EQ( 1.000, cube(1.0));
        EXPECT_DOUBLE_EQ( 3.375, cube(1.5));
        EXPECT_DOUBLE_EQ( 8.000, cube(2.0));

        // negative
        EXPECT_DOUBLE_EQ( -1.000, cube(-1.0));
        EXPECT_DOUBLE_EQ( -3.375, cube(-1.5));
        EXPECT_DOUBLE_EQ( -8.000, cube(-2.0));
    }

    ///////////
    // mod() //
    ///////////
    TEST(mod, ModOperator)
    {
        // zero
        EXPECT_EQ(0,  0 % 10);

        // positive
        EXPECT_EQ(1,  1 % 10);
        EXPECT_EQ(5,  5 % 10);
        EXPECT_EQ(9,  9 % 10);
        EXPECT_EQ(0, 10 % 10);
        EXPECT_EQ(5, 15 % 10);

        // negative
        EXPECT_EQ(-1,  -1 % 10);
        EXPECT_EQ(-5,  -5 % 10);
        EXPECT_EQ(-9,  -9 % 10);
        EXPECT_EQ( 0, -10 % 10);
        EXPECT_EQ(-5, -15 % 10);
    }

    TEST(mod, ModFunction)
    {
        // zero
        EXPECT_EQ(0,  mod(0, 10));

        // positive
        EXPECT_EQ(1, mod( 1, 10));
        EXPECT_EQ(5, mod( 5, 10));
        EXPECT_EQ(9, mod( 9, 10));
        EXPECT_EQ(0, mod(10, 10));
        EXPECT_EQ(5, mod(15, 10));

        // negative
        EXPECT_EQ(9, mod( -1, 10));
        EXPECT_EQ(5, mod( -5, 10));
        EXPECT_EQ(1, mod( -9, 10));
        EXPECT_EQ(0, mod(-10, 10));
        EXPECT_EQ(5, mod(-15, 10));
    }

    ///////////////
    // zeroise() //
    ///////////////
    TEST(zeroise, Test)
    {
        EXPECT_EQ(0, zeroise<int>());
        EXPECT_EQ((unsigned int)0, zeroise<unsigned int>());
        EXPECT_DOUBLE_EQ(0.0, zeroise<double>());
        EXPECT_EQ("", zeroise<std::string>());
        EXPECT_EQ(Vector3::ZERO, zeroise<Vector3>());
    }

    ///////////////////
    // interpolate() //
    ///////////////////
    TEST(interpolate, Linear)
    {
        EXPECT_EQ( 0, interpolate(0.00f, 0, 20));
        EXPECT_EQ( 5, interpolate(0.25f, 0, 20));
        EXPECT_EQ(10, interpolate(0.50f, 0, 20));
        EXPECT_EQ(15, interpolate(0.75f, 0, 20));
        EXPECT_EQ(20, interpolate(1.00f, 0, 20));
    }

    TEST(interpolate, Smooth)
    {
        EXPECT_EQ( 0, interpolateSmooth(0.00f, 0, 20));
        EXPECT_GT( 5, interpolateSmooth(0.25f, 0, 20));
        EXPECT_EQ(10, interpolateSmooth(0.50f, 0, 20));
        EXPECT_LT(15, interpolateSmooth(0.75f, 0, 20));
        EXPECT_EQ(20, interpolateSmooth(1.00f, 0, 20));
    }

    ///////////
    // rnd() //
    ///////////
    TEST(rnd, Default)
    {
        // how to test randomness?
        std::set<float> numbers;

        const float max = 1.0;
        const float min = 0.0;
        const size_t iterations = 1000;

        for (size_t i = 0; i < iterations; ++i)
            numbers.insert(rnd());

        // must have at most <iterations> numbers in the set
        EXPECT_GE(iterations, numbers.size());

        // all numbers must satisfy 0 <= <number> < 1
        for (std::set<float>::iterator it = numbers.begin(); it != numbers.end(); ++it)
        {
            EXPECT_LE(min, *it);
            EXPECT_GT(max, *it);
        }
    }

    TEST(rnd, Max)
    {
        // how to test randomness?
        std::set<float> numbers;

        const float max = 10.0;
        const float min = 0.0;
        const size_t iterations = 1000;

        for (size_t i = 0; i < iterations; ++i)
            numbers.insert(rnd(max));

        // must have at most <iterations> numbers in the set
        EXPECT_GE(iterations, numbers.size());

        // all numbers must satisfy 0 <= <number> < <max>
        for (std::set<float>::iterator it = numbers.begin(); it != numbers.end(); ++it)
        {
            EXPECT_LE(min, *it);
            EXPECT_GT(max, *it);
        }
    }

    TEST(rnd, MinMax)
    {
        // how to test randomness?
        std::set<float> numbers;

        const float max = 10.0;
        const float min = 5.0;
        const size_t iterations = 1000;

        for (size_t i = 0; i < iterations; ++i)
            numbers.insert(rnd(min, max));

        // must have at most <iterations> numbers in the set
        EXPECT_GE(iterations, numbers.size());

        // all numbers must satisfy <min> <= <number> < <max>
        for (std::set<float>::iterator it = numbers.begin(); it != numbers.end(); ++it)
        {
            EXPECT_LE(min, *it);
            EXPECT_GT(max, *it);
        }
    }

    //////////////
    // rndsgn() //
    //////////////
    TEST(rndsgn, Test)
    {
        // how to test randomness?
        std::set<float> numbers;

        const size_t iterations = 100;

        for (size_t i = 0; i < iterations; ++i)
            numbers.insert(rndsgn());

        // must have at most 2 numbers in the set
        EXPECT_GE((size_t)2, numbers.size());

        // all numbers must be either 1 oder -1
        for (std::set<float>::iterator it = numbers.begin(); it != numbers.end(); ++it)
            EXPECT_TRUE(*it == 1 || *it == -1);
    }

    ///////////////////////
    // getUniqueNumber() //
    ///////////////////////
    TEST(getUniqueNumber, Test)
    {
        std::set<unsigned long> numbers;

        const size_t iterations = 1000;

        for (size_t i = 0; i < iterations; ++i)
            numbers.insert(getUniqueNumber());

        // must have exactly <iterations> numbers in the set, since all numbers should be unique
        EXPECT_EQ(iterations, numbers.size());
    }
/*
    getAngle
    get2DViewdirection
    get2DViewcoordinates
    getPredictedPosition
*/
}
