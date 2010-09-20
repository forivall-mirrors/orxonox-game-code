/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @defgroup Math Mathematical functions
    @ingroup Util
*/

/**
    @file
    @ingroup Math
    @brief Declaration and implementation of several math-functions.
*/

#ifndef _Util_Math_H__
#define _Util_Math_H__

#include "UtilPrereqs.h"

#include <string>
#include <cmath>
#include <cstdlib>

// Certain headers might define unwanted macros...
#undef max
#undef min
#undef sgn
#undef clamp
#undef sqrt
#undef square
#undef mod
#undef rnd

namespace orxonox
{
    // C++ doesn't define any constants for pi, e, etc.
    namespace math
    {
        const float pi      = 3.14159265f;      ///< PI
        const float pi_2    = 1.57079633f;      ///< PI / 2
        const float pi_4    = 7.85398163e-1f;   ///< PI / 4
        const float e       = 2.71828183f;      ///< e
        const float sqrt2   = 1.41421356f;      ///< sqrt(2)
        const float sqrt2_2 = 7.07106781e-1f;   ///< sqrt(2) / 2

        const double pi_d      = 3.14159265358979324;       ///< PI (double)
        const double pi_2_d    = 1.57079632679489662;       ///< PI / 2 (double)
        const double pi_4_d    = 7.85398163397448310e-1;    ///< PI / 4 (double)
        const double e_d       = 2.71828182845904524;       ///< e (double)
        const double sqrt2_d   = 1.41421356237309505;       ///< sqrt(2) (double)
        const double sqrt2_2_d = 7.07106781186547524e-1;    ///< sqrt(2) / 2 (double)
    }

    /**
        @brief Returns the sign of the given value.
        @param x The value
        @return 1 if the value is positive or zero, -1 if the value is negative
    */
    template <typename T>
    inline T sgn(T x)
    {
        return (x >= 0) ? (T)1 : (T)-1;
    }

    /**
        @brief Keeps a value between a lower and an upper limit. Values beyond these limits are limited to either @a min or @a max.
        @param x The value
        @param min The lower limit
        @param max The upper limit
    */
    template <typename T>
    inline T clamp(T x, T min, T max)
    {
        if (x < min)
            return min;

        if (x > max)
            return max;

        return x;
    }

    /**
        @brief Returns the squared value (x^2).
    */
    template <typename T>
    inline T square(T x)
    {
        return x*x;
    }

    /**
        @brief Returns the cubed value (x^3).
    */
    template <typename T>
    inline T cube(T x)
    {
        return x*x*x;
    }

    /**
        @brief Rounds the value to the nearest integer.
    */
    template <typename T>
    inline int round(T x)
    {
        return static_cast<int>(x + 0.5);
    }

    /**
        @brief The modulo operation, enhanced to work properly with negative values.
        @param x The value
        @param max The operand

        The built in modulo operator % yields a strange behavior with negative values.
        This function corrects this - the result is guaranteed to lie always between
        zero and (max-1).

        Example:
        @code
        int var = 11 % 10;      //  1
        int var = -1 % 10;      // -1

        int var = mod(11, 10);  //  1
        int var = mod(-1, 10);  //  9
        @endcode
    */
    template <typename T>
    inline int mod(T x, int max)
    {
        if (x >= 0)
            return (x % max);
        else
            return ((x % max) + max);
    }

    /**
        @brief Interpolates between two values for a time between 0 and 1.
        @param time The time is a value between 0 and 1 - the function returns @a start if @a time is 0, @a end if @a time is 1, and interpolates if @a time is between 0 and 1.
        @param start The value at @a time = 0
        @param end The value at @a time = 1
        @return The interpolated value at a given time
    */
    template <typename T>
    inline T interpolate(float time, const T& start, const T& end)
    {
        return time * (end - start) + start;
    }

    /**
        @brief Interpolates smoothly between two values for a time between 0 and 1. The function starts slowly, increases faster and stops slowly again.
        @param time The time is a value between 0 and 1 - the function returns @a start if @a time is 0, @a end if @a time is 1, and interpolates if @a time is between 0 and 1.
        @param start The value at @a time = 0
        @param end The value at @a time = 1
        @return The interpolated value at a given time
    */
    template <typename T>
    inline T interpolateSmooth(float time, const T& start, const T& end)
    {
        return (-2 * (end - start) * cube(time)) + (3 * (end - start) * square(time)) + start;
    }

    /**
        @brief Returns a random number between 0 and almost 1: <tt>0 <= rnd < 1</tt>.
    */
    inline float rnd()
    {
        return rand() / (RAND_MAX + 1.0f);
    }

    /**
        @brief Returns a random number between 0 and almost @a max: <tt>0 <= rnd < max</tt>.
        @param max The maximum
    */
    inline float rnd(float max)
    {
        return rnd() * max;
    }

    /**
        @brief Returns a random number between @a min and almost @a max: <tt>min <= rnd < max</tt>.
        @param min The minimum
        @param max The maximum
    */
    inline float rnd(float min, float max)
    {
        return rnd(max - min) + min;
    }

    /**
        @brief Returns randomly 1 or -1 with equal probability.
    */
    inline float rndsgn()
    {
        return static_cast<float>((rand() & 0x2) - 1); // rand() & 0x2 is either 2 or 0
    }

    _UtilExport unsigned long getUniqueNumber();
}

#endif /* _Util_Math_H__ */
