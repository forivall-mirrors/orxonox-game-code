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
    @file Math.h
    @brief Declaration and implementation of several math-functions, typedefs of some Ogre::Math classes to the orxonox namespace.
*/

#ifndef _Util_Math_H__
#define _Util_Math_H__

#include "UtilPrereqs.h"

#include <ostream>
#include <string>

#include <OgreMath.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>

namespace orxonox
{
  typedef Ogre::Radian Radian;
  typedef Ogre::Degree Degree;
  typedef Ogre::Vector2 Vector2;
  typedef Ogre::Vector3 Vector3;
  typedef Ogre::Vector4 Vector4;
  typedef Ogre::Matrix3 Matrix3;
  typedef Ogre::Matrix4 Matrix4;
  typedef Ogre::Quaternion Quaternion;
  typedef Ogre::ColourValue ColourValue;
}

_UtilExport std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian);
_UtilExport std::istream& operator>>(std::istream& in, orxonox::Radian& radian);
_UtilExport std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree);
_UtilExport std::istream& operator>>(std::istream& in, orxonox::Degree& degree);

_UtilExport float getAngle(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& otherposition);
_UtilExport orxonox::Vector2 get2DViewdirection(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition);
_UtilExport orxonox::Vector2 get2DViewcoordinates(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition);
_UtilExport orxonox::Vector3 getPredictedPosition(const orxonox::Vector3& myposition, float projectilespeed, const orxonox::Vector3& targetposition, const orxonox::Vector3& targetvelocity);

//Get around Windows hackery
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#endif

/**
    @brief Returns the sign of the given value.
    @param x The value
    @return 1 if the value is positive or zero, -1 if the value is negative
*/
template <typename T>
inline T sgn(T x)
{
    return (x >= 0) ? 1 : -1;
}

/**
    @brief Returns the smaller of two values.
*/
template <typename T>
inline T min(T a, T b)
{
    return (a <= b) ? a : b;
}

/**
    @brief Returns the greater of two values.
*/
template <typename T>
inline T max(T a, T b)
{
    return (a >= b) ? a : b;
}

/**
    @brief Keeps a value between a lower and an upper limit.
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
    @brief Returns the square value (x^2).
*/
template <typename T>
inline T square(T x)
{
    return x*x;
}

/**
    @brief Returns the cube value (x^3).
*/
template <typename T>
inline T cube(T x)
{
    return x*x*x;
}

/**
    @brief Rounds the value down.
*/
template <typename T>
inline int floor(T x)
{
    return (int)(x);
}

/**
    @brief Rounds the value up.
*/
template <typename T>
inline int ceil(T x)
{
    int temp = floor(x);
    return (temp != x) ? (temp + 1) : temp;
}

/**
    @brief Rounds the value.
*/
template <typename T>
inline int round(T x)
{
    return (int)(x + 0.5);
}

/**
    @brief The modulo operation, enhanced to work properly with negative values.
    @param x The value
    @param max The operand
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
    @param time The time is a value between 0 and 1 - the function returns start if time is 0 and end if time is 1 and interpolates if time is between 0 and 1.
    @param start The value at time = 0
    @param end The value at time = 1
    @return The interpolation at a given time
*/
template <typename T>
T interpolate(float time, const T& start, const T& end)
{
    return time * (end - start) + start;
}

/**
    @brief Interpolates smoothly between two values for a time between 0 and 1. The function starts slowly, increases faster and stops slowly again.
    @param time The time is a value between 0 and 1 - the function returns start if time is 0 and end if time is 1 and interpolates if time is between 0 and 1.
    @param start The value at time = 0
    @param end The value at time = 1
    @return The smoothed interpolation at a given time
*/
template <typename T>
T interpolateSmooth(float time, const T& start, const T& end)
{
    return (-2 * (end - start) * cube(time)) + (3 * (end - start) * square(time)) + start;
}

/**
    @brief Returns a random number between 0 and almost 1: 0 <= rnd < 1.
*/
inline _UtilExport float rnd()
{
    return ((float)rand() / (RAND_MAX + 1));
}

/**
    @brief Returns a random number between 0 and almost max: 0 <= rnd < max.
    @param max The maximum
*/
inline _UtilExport float rnd(float max)
{
    return rnd() * max;
}

/**
    @brief Returns a random number between min and almost max: min <= rnd < max.
    @param min The minimum
    @param max The maximum
*/
inline _UtilExport float rnd(float min, float max)
{
    return rnd(max - min) + min;
}

_UtilExport unsigned long getUniqueNumber();
_UtilExport std::string getUniqueNumberStr();

class _UtilExport IntVector2
{
public:
  IntVector2() : x(0), y(0) { }
  IntVector2(int _x, int _y) : x(_x), y(_y) { }
  int x;
  int y;
};

class _UtilExport IntVector3
{
public:
  IntVector3() : x(0), y(0), z(0) { }
  IntVector3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }
  int x;
  int y;
  int z;
};

#endif /* _Util_Math_H__ */
