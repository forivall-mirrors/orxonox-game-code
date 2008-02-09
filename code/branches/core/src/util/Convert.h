/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Benjamin Grauer
 *   Co-authors:
 *      Fabian 'x3n' Landau
 */

/*!
    @file Convert.h
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Convert_H__
#define _Convert_H__

#include <string>
#include <sstream>

#include "UtilPrereqs.h"

// DEFAULT CLASS
template <typename FromType, typename ToType>
class _UtilExport Converter
{
 public:
  bool operator()(ToType* output, const FromType& input) const
  {
    return false;
  }
};

// PARTIAL SPECIALIZATION TO CONVERT TO STRINGS
template<typename FromType>
class _UtilExport Converter<FromType, std::string>
{
 public:
  bool operator()(std::string* output, const FromType& input) const
  {
    std::ostringstream oss;
    if (oss << input)
    {
      (*output) = oss.str();
      return true;
    }
    else
      return false;
  }
};

// PARTIAL SPECIALIZATION TO CONVERT FROM STRING
template<typename ToType>
class _UtilExport Converter<std::string, ToType>
{
 public:
  bool operator()(ToType* output, const std::string& input) const
  {
    std::istringstream iss(input);
    if (iss >> (*output))
      return true;
    else
      return false;
  }
};

// FUNCTION SO WE DO NOT HAVE TO TELL THE COMPILER ABOUT THE TYPE
template<typename FromType, typename ToType>
static _UtilExport bool ConvertValue(ToType* output, const FromType& input)
{
  Converter<FromType, ToType> converter;
  return converter(output, input);
}

// THE SAME, BUT WITH DEFAULT VALUE
template<typename FromType, typename ToType>
static _UtilExport bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback)
{
  Converter<FromType, ToType> converter;
  if (converter(output, input))
    return true;

  (*output) = fallback;
  return false;
}

#endif /* _Convert_H__ */
