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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "Script.h"

#include <fstream>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}


namespace orxonox
{
  Script::Script()
  {
    fileLines_ = std::list<std::string>();

    state_ = lua_open();
#if LUA_VERSION_NUM == 501
    luaL_openlibs(state_);
#else
    luaopen_base(state_);
    luaopen_table(state_);
    luaopen_io(state_);
    luaopen_string(state_);
    luaopen_math(state_);
    luaopen_debug(state_);
#endif
  }

  Script::~Script()
  {

  }

  /**
      @brief Loads the specified file line by line
      @param filename The filename of the file
  */
  void Script::loadFile(std::string filename)
  {
    std::ifstream file;
    file.open(filename.c_str(), std::fstream::in);

    if (!file.is_open())
    {
      // some error msg
    }

    char line[1024];

    while (file.good() && !file.eof())
    {
      file.getline(line, 1024);
      this->fileLines_.push_back(line);
    }

    // The last line is useless
    this->fileLines_.pop_back();

    file.close();
  }

  /**
      @brief Parses the level file to correct Lua code
  */
  void Script::xmlToLua()
  {
    // We will iterate through all the lines and replace things.
    std::list<std::string>::iterator it;
    for(it = this->fileLines_.begin(); it != this->fileLines_.end(); ++it)
    {
      int pos = (*it).find("<?lua");
      while (pos < (int)(*it).length())
      {
        // We found a lua tag
        std::string front = (*it).substr(0,pos);
        std::string back = (*it).substr(pos + 5);
        (*it) = front + "]])" + back;
        pos = (*it).find("<?lua");
      }

      pos = (*it).find("?>");
      while (pos < (int)(*it).length())
      {
        // We found a lua tag
        std::string front = (*it).substr(0,pos);
        std::string back = (*it).substr(pos + 2);
        (*it) = front + "print([[" + back;
        pos = (*it).find("?>");
      }
    }
    this->fileLines_.push_front("print([[");
    this->fileLines_.push_back("]])");
  }

}
