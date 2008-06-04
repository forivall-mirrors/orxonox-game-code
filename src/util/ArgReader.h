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
 @file  Argreader.h
 @brief reads arguments
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _ArgReader_H__
#define _ArgReader_H__

#include "UtilPrereqs.h"

#include <string>
#include <vector>
#include "Convert.h"

struct _UtilExport CmdLineArg
{
  std::string name_;
  std::string value_;
  bool bChecked_;
};

class _UtilExport ArgReader
{
  public:
    ArgReader(int argc, char **argv);
    template <class T>
    void checkArgument(std::string option, T* value, bool must = false);
    bool errorHandling();
    const std::string& getErrorString();

  private:
    std::vector<CmdLineArg> arguments_;
    bool failure_;
    std::string errorString_;
};

template <class T>
void ArgReader::checkArgument(std::string option, T* value, bool must)
{
  unsigned int iArg = 0;
  while (iArg < arguments_.size())
  {
    if (arguments_[iArg].name_ == option)
      break;
    ++iArg;
  }
  if (iArg == arguments_.size())
  {
    if (must)
    {
      failure_ = true;
      errorString_ += "Cannot find mandatory argument \"" + option + "\"\n";
      return;
    }
    else
      return;
  }

  arguments_[iArg].bChecked_ = true;

  if (!convertValue(value, arguments_[iArg].value_))
  {
    failure_ = true;
    errorString_ += "Cannot convert argument value for option \"" + option + "\"\n";
  }
}

template <>
void ArgReader::checkArgument(std::string option, bool* value, bool must)
{
  // for type bool, only check whether the option was set or not
  unsigned int iArg = 0;
  while (iArg < arguments_.size())
  {
    if (arguments_[iArg].name_ == option)
    {
      arguments_[iArg].bChecked_ = true;
      *value = true;
      break;
    }
    ++iArg;
  }
  if (iArg == arguments_.size())
    *value = false;
}

#endif /* _ArgReader_H__ */
