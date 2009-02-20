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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "LuaBind.h"

#include <fstream>
#include <map>

extern "C" {
#include <lua.h>
#include <lualib.h>
}
#include <tolua/tolua++.h>
#include <boost/filesystem.hpp>

#include "util/String.h"
#include "util/Debug.h"
#include "ToluaBindCore.h"
#include "Core.h"

namespace orxonox
{
  LuaBind* LuaBind::singletonRef_s = NULL;

  LuaBind::LuaBind()
  {
    assert(LuaBind::singletonRef_s == 0);
    LuaBind::singletonRef_s = this;

    this->includePath_ = Core::getMediaPath();

    luaState_ = lua_open();
    luaSource_ = "";
#if LUA_VERSION_NUM == 501
    luaL_openlibs(luaState_);
#else
    luaopen_base(luaState_);
    luaopen_string(luaState_);
    luaopen_table(luaState_);
    luaopen_math(luaState_);
    luaopen_io(luaState_);
    luaopen_debug(luaState_);
#endif
    tolua_Core_open(luaState_);
    output_ = "";
    isRunning_ = false;
  }

  void LuaBind::luaPrint(std::string str)
  {
    output_ += str;
//    COUT(4) << "Lua_output!:" << std::endl << str << std::endl << "***" << std::endl;
    COUT(5) << str;
  }

  /**
      @brief Loads the specified file line by line
      @param filename The filename of the file
      @param luaTags if true, the loaded file gets stripped off luaTags
  */
  void LuaBind::loadFile(std::string filename, bool luaTags)
  {
    boost::filesystem::path filepath(filename);

    output_ = "";
    std::ifstream file;
    file.open(filepath.native_file_string().c_str(), std::fstream::in);

    if (!file.is_open())
    {
      // some error msg
    }

    char line[1024*32];
    std::string levelString = "";

    while (file.good() && !file.eof())
    {
      file.getline(line, 1024*32);
      levelString += line;
      levelString += "\n";
    }

    file.close();
    //std::string output;

    if (luaTags)
      luaSource_ = replaceLuaTags(levelString);
    else
      luaSource_ = levelString;
    COUT(5) << "ParsedSourceCode: " << luaSource_ << std::endl;
  }

  void LuaBind::loadString(std::string code)
  {
    luaSource_ = code;
    output_ = "";
  }

#if LUA_VERSION_NUM != 501
  const char * LuaBind::lua_Chunkreader(lua_State *L, void *data, size_t *size)
  {
    LoadS* ls = ((LoadS*)data);
    if (ls->size == 0) return NULL;
    *size = ls->size;
    ls->size = 0;
    return ls->s;
  }
#endif
  void LuaBind::run()
  {
    if (!isRunning_)
    {
      isRunning_ = true;
      int error = 0;
      std::string init =
         "local scr = orxonox.LuaBind:getInstance()\n \
          local debug = print\n \
          print = function(s)\n \
              scr:luaPrint(s)\n \
          end\n \
          include = function(f)\n \
              file = assert(io.open(\"" + this->includePath_ + "\"..\"/\"..f))\n \
              content = file:read(\"*a\")\n \
              file:close()\n \
              source = scr:replaceLuaTags(content)\n \
              assert(loadstring(source))()\n \
          end\n";
      init += luaSource_;
  #if LUA_VERSION_NUM == 501
      error = luaL_loadstring(luaState_, init.c_str());
  #else
      LoadS ls;
      ls.s = init.c_str();
      ls.size = init.size();
      error = lua_load(luaState_, &orxonox::LuaBind::lua_Chunkreader, &ls, init.c_str());
  #endif
      if (error == 0)
      {
        error = lua_pcall(luaState_, 0, 0, 0);
      }
      if (error != 0)
      {
        COUT(2) << "Error in Lua-script: " << lua_tostring(luaState_, -1) << std::endl;
      }
      isRunning_ = false;
    }
    else
    {
      COUT(2) << "Warning: Lua's run is called while running!" << std::endl;
    }
  }

  std::string LuaBind::replaceLuaTags(const std::string& text)
  {
    // chreate map with all Lua tags
    std::map<size_t, bool> luaTags;
    {
      size_t pos = 0;
      while ((pos = text.find("<?lua", pos)) != std::string::npos)
        luaTags[pos++] = true;
    }
    {
      size_t pos = 0;
      while ((pos = text.find("?>", pos)) != std::string::npos)
        luaTags[pos++] = false;
    }

    // erase all tags from the map that are between two quotes
    {
      std::map<size_t, bool>::iterator it = luaTags.begin();
      std::map<size_t, bool>::iterator it2 = it;
      bool bBetweenQuotes = false;
      size_t pos = 0;
      while ((pos = getNextQuote(text, pos)) != std::string::npos)
      {
        while ((it != luaTags.end()) && (it->first < pos))
        {
          if (bBetweenQuotes) {
            it2++;
            if(it->second && !(it2->second) && it2->first < pos)
              it = ++it2;
            else
              luaTags.erase(it++);
          }
          else
            ++it;
        }
        bBetweenQuotes = !bBetweenQuotes;
        pos++;
      }
    }

    // check whether on every opening <?lua tag a closing ?> tag follows
    {
      bool expectedValue = true;
      for (std::map<size_t, bool>::iterator it = luaTags.begin(); it != luaTags.end(); ++it)
      {
        if (it->second == expectedValue)
          expectedValue = !expectedValue;
        else
        {
          expectedValue = false;
          break;
        }
      }
      if (!expectedValue) {
        COUT(2) << "Warning: Error in level file" << std::endl;
        // todo: errorhandling
        return "";
      }
    }

    // cut the original string into pieces and put them together with print() instead of lua tags
    std::string output;
    {
      std::map<size_t, bool>::iterator it = luaTags.begin();
      bool bInPrintFunction = true;
      size_t start = 0;
      size_t end = 0;

      do
      {
        if (it != luaTags.end())
          end = (*(it++)).first;
        else
          end = std::string::npos;

        unsigned int equalSignCounter = 0;

        if (bInPrintFunction)
        {
          // count ['='[ and ]'='] and replace tags with print([[ and ]])
          std::string temp = text.substr(start, end - start);
          {
            size_t pos = 0;
            while ((pos = temp.find('[', pos)) != std::string::npos)
            {
              unsigned int tempCounter = 1;
              size_t tempPos = pos++;
              while(temp[++tempPos] == '=') {
                tempCounter++;
              }
              if(temp[tempPos] != '[') {
                tempCounter = 0;
              }
              else if(tempCounter == 0) {
                tempCounter = 1;
              }
              if (tempCounter > equalSignCounter)
                equalSignCounter = tempCounter;
            }
          }
          {
            size_t pos = 0;
            while ((pos = temp.find(']', pos)) != std::string::npos)
            {
              unsigned int tempCounter = 1;
              size_t tempPos = pos++;
              while(temp[++tempPos] == '=') {
                tempCounter++;
              }
              if(temp[tempPos] != ']') {
                tempCounter = 0;
              }
              else if(tempCounter == 0) {
                tempCounter = 1;
              }
              if (tempCounter > equalSignCounter)
                equalSignCounter = tempCounter;
            }
          }
          std::string equalSigns = "";
          for(unsigned int i = 0; i < equalSignCounter; i++) {
            equalSigns += "=";
          }
          output += "print([" + equalSigns + "[" + temp + "]" + equalSigns +"])";
          start = end + 5;
        }
        else
        {
          output += text.substr(start, end - start);
          start = end + 2;
        }

        bInPrintFunction = !bInPrintFunction;
      }
      while (end != std::string::npos);
    }

    return output;
  }

}
