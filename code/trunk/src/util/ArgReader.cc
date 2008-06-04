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
 *      Reto Grieder
 *   Co-authors:
 *      Benjamin Knecht <beni_at_orxonox.net>
 *
 */

/**
 @file  ArgReader.cc
 @brief Argument Reader
 */

#include "ArgReader.h"
#include "SubString.h"

ArgReader::ArgReader(int argc, char **argv)
{
  failure_ = false;
  errorString_ = "";
  CmdLineArg arg;

  int i = 1;
  while (i < argc)
  {
    if (argv[i][0] == '-' && argv[i][1] == '-') // name
    {
      if (argv[i][2] == '\0')
      {
        failure_ = true;
        errorString_ += "Expected word after \"--\".\n";
      }
      arg.bChecked_ = false;
      arg.name_ = argv[i] + 2;
      arg.value_ = "";
      arguments_.push_back(arg);
    }
    else // value
    {
      if (arguments_.size() == 0)
      {
        failure_ = true;
        errorString_ += "Expected \"--\" in command line arguments.\n";
        arg.bChecked_ = false;
        arg.name_ = "";
        arg.value_ = "";
        arguments_.push_back(arg);
      }

      if (arguments_.back().value_ != "")
        arguments_.back().value_ += " " + std::string(argv[i]);
      else
        arguments_.back().value_ = argv[i];
    }
    ++i;
  }
}

bool ArgReader::errorHandling()
{
  bool argumentsChecked = true;
  for (unsigned int i = 1; i < arguments_.size(); ++i)
    argumentsChecked &= arguments_[i].bChecked_;

  if (!argumentsChecked)
    errorString_ += "Not all arguments could be matched.\n";

  return !argumentsChecked || failure_;
}

const std::string& ArgReader::getErrorString()
{
  return errorString_;
}
