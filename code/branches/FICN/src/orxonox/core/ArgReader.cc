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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  ArgReader.cc
 @brief Argument Reader
 */

#include <iostream>

#include "ArgReader.h"

namespace orxonox {

  ArgReader::ArgReader(int argc, char** argv)
  {
    counter_ = argc;
    arguments_ = argv;
    fail_ = false;
    errorStr_ = "";
  }

  void ArgReader::checkArgument(String option, String &string, bool must)
  {
    int argpos = checkOption(option) + 1;
    if(argpos != 0)
    {
      string = arguments_[argpos];
    }
    else
    {
      if(must) {
        errorStr_ = errorStr_ + "Cannot find mandatory argument \"" + option + "\"\n";
        fail_ = true;
      }
    }

  }

  void ArgReader::checkArgument(String option, int &integer, bool must)
  {
    int argpos = checkOption(option) + 1;
    if(argpos != 0)
    {
      integer = atoi(arguments_[argpos]);
    }
    else
    {
      if(must) {
        errorStr_ = errorStr_ + "Cannot find mandatory argument \"" + option + "\"\n";
        fail_ = true;
      }
    }
  }

  void ArgReader::checkArgument(String option, float &floating, bool must)
  {
    int argpos = checkOption(option) + 1;
    if(argpos != 0)
    {
      floating = (float)atof(arguments_[argpos]);
    }
    else
    {
      if(must) {
        errorStr_ = errorStr_ + "Cannot find mandatory argument \"" + option + "\"\n";
        fail_ = true;
      }
    }
  }

  int ArgReader::checkOption(String option)
  {
    for(int i = 1; i < counter_; i++)
    {
      if(arguments_[i] == "--" + option)
          return i;
    }
    return -1;
  }

  bool ArgReader::errorHandling()
  {
    if(fail_)
      std::cout << errorStr_;
    return fail_;
  }
}

