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
 *      Nicolas Perrenoud <nicolape_at_ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _String2Number_H__
#define _String2Number_H__

#include <string>
#include <sstream>
#include <iostream>

#include "core/Debug.h"
#include "UtilPrereqs.h"

/**
 * String to number conversion
 *
 * This class converts a number inside a std::string
 * into a numeric type number (int,float,double)
 * Number in string can be decimal, hexadecimal or octal
 *
 * @author Nicolas Perrenoud<nicolape_at_ee.ethz.ch>
 *
 * @example
 * float f;
 * String2Number<float>(f, std::string(" 123.45 "));
 */

template <class T>
class String2Number
{
  private:
  bool success_;

  public:
  /**
   * Constructor
   *
   * First value is the target variable, the second vector is the
   * string where the number is taken from, the third parameter
   * should be one of std::hex, std::dec or std::oct (dec is default value)
   */
  inline String2Number(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) =  std::dec, int haltOnError=1)
  {
    std::istringstream iss(s);
    success_ = !(iss >> f >> t).fail();

    if (!success_ && haltOnError==1)
    {
      COUT(1) << "Error: Conversion from string to number in \"" << s << "\" failed." << std::endl;
      exit(1);
    }
  }
};

#endif /* _String2Number_H__ */
