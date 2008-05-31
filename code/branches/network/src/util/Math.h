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

#ifndef _Util_Math_H__
#define _Util_Math_H__

#include "UtilPrereqs.h"

#include <ostream>

#include <OgreMath.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreMatrix3.h>
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
  typedef Ogre::Quaternion Quaternion;
  typedef Ogre::ColourValue ColourValue;
}

_UtilExport std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian);
_UtilExport std::istream& operator>>(std::istream& in, orxonox::Radian& radian);
_UtilExport std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree);
_UtilExport std::istream& operator>>(std::istream& in, orxonox::Degree& degree);

template <typename T>
inline T sgn(T x)
{
    return (x >= 0) ? 1 : -1;
}

template <typename T>
inline T min(T a, T b)
{
    return (a <= b) ? a : b;
}

template <typename T>
inline T max(T a, T b)
{
    return (a >= b) ? a : b;
}

template <typename T>
inline T clamp(T x, T min, T max)
{
    if (x < min)
        return min;

    if (x > max)
        return max;

    return x;
}

template <typename T>
inline T square(T x)
{
    return x*x;
}

template <typename T>
inline T cube(T x)
{
    return x*x*x;
}

template <typename T>
inline int floor(T x)
{
    return (int)(x);
}

template <typename T>
inline int ceil(T x)
{
    int temp = floor(x);
    return (temp != x) ? (temp + 1) : temp;
}

template <typename T>
inline int round(T x)
{
    return (int)(x + 0.5);
}

template <typename T>
inline int mod(T x, int max)
{
    if (x >= 0)
        return (x % max);
    else
        return ((x % max) + max);
}

template <typename T>
T interpolate(float time, const T& start, const T& end)
{
    return time * (end - start) + start;
}

template <typename T>
T interpolateSmooth(float time, const T& start, const T& end)
{
    return (-2 * (end - start) * cube(time)) + (3 * (end - start) * square(time)) + start;
}

inline _UtilExport float rnd()
{
    return ((float)rand() / RAND_MAX);
}

inline _UtilExport float rnd(float max)
{
    return rnd() * max;
}

inline _UtilExport float rnd(float min, float max)
{
    return rnd(max - min) + min;
}

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
