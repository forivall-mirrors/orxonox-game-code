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

#include "Script.h"

#include <fstream>
#include <map>

#include "CoreIncludes.h"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include "tolua/tolua++.h"
#include "tolua/tolua_bind.h"

namespace orxonox
{
  Script* Script::singletonRef = NULL;

  Script::Script()
  {
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
    tolua_core_open(luaState_);
    output_ = "";
  }

  void Script::luaPrint(std::string str)
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
  void Script::loadFile(std::string filename, bool luaTags)
  {
    std::ifstream file;
    file.open(filename.c_str(), std::fstream::in);

    if (!file.is_open())
    {
      // some error msg
    }

    char line[1024];
    std::string levelString = "";

    while (file.good() && !file.eof())
    {
      file.getline(line, 1024);
      levelString += line;
      levelString += "\n";
    }

    file.close();
    //std::string output;

    if (luaTags) luaSource_ = replaceLuaTags(levelString);
    COUT(5) << "ParsedSourceCode: " << luaSource_ << std::endl;
  }

#if LUA_VERSION_NUM != 501
  const char * Script::lua_Chunkreader(lua_State *L, void *data, size_t *size)
  {
    LoadS* ls = ((LoadS*)data);
    if (ls->size == 0) return NULL;
    *size = ls->size;
    ls->size = 0;
    return ls->s;
  }
#endif
  void Script::run()
  {
    int error = 0;
    std::string init = "local scr = orxonox.Script:getInstance()\nprint = function(s)\nscr:luaPrint(s)\nend\n";
    init += luaSource_;
#if LUA_VERSION_NUM == 501
    error = luaL_loadstring(luaState_, init.c_str());
#else
    LoadS ls;
    ls.s = init.c_str();
    ls.size = init.size();
    error = lua_load(luaState_, &orxonox::Script::lua_Chunkreader, &ls, init.c_str());
#endif
    if (error == 0)
      error = lua_pcall(luaState_, 0, 0, 0);
    if (error != 0)
    {
      COUT(2) << "Error in Lua-script: " << lua_tostring(luaState_, -1) << std::endl;
    }
  }

  unsigned int Script::getNextQuote(const std::string& text, unsigned int start)
  {
    unsigned int quote = start - 1;

    while ((quote = text.find('\"', quote + 1)) != std::string::npos)
    {
      unsigned int backslash = quote;
      unsigned int numbackslashes = 0;
      for (; backslash > 0; backslash--, numbackslashes++)
        if (text[backslash - 1] != '\\')
          break;

      if (numbackslashes % 2 == 0)
        break;
    }

    return quote;
  }

  std::string Script::replaceLuaTags(const std::string& text)
  {
    // chreate map with all Lua tags
    std::map<unsigned int, bool> luaTags;
    {
      unsigned int pos = 0;
      while ((pos = text.find("<?lua", pos)) != std::string::npos)
        luaTags[pos++] = true;
    }
    {
      unsigned int pos = 0;
      while ((pos = text.find("?>", pos)) != std::string::npos)
        luaTags[pos++] = false;
    }

    // erase all tags from the map that are between two quotes
    {
      std::map<unsigned int, bool>::iterator it = luaTags.begin();
      std::map<unsigned int, bool>::iterator it2 = it;
      bool bBetweenQuotes = false;
      unsigned int pos = 0;
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
      for (std::map<unsigned int, bool>::iterator it = luaTags.begin(); it != luaTags.end(); ++it)
      {
        if ((*it).second == expectedValue)
          expectedValue = !expectedValue;
        else
        {
          expectedValue = false;
          break;
        }
      }
      if (!expectedValue) {
        // todo: errorhandling
        return "";
      }
    }

    // cut the original string into pieces and put them together with print() instead of lua tags
    std::string output;
    {
      std::map<unsigned int, bool>::iterator it = luaTags.begin();
      bool bInPrintFunction = true;
      unsigned int start = 0;
      unsigned int end = 0;

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
            unsigned int pos = 0;
            while ((pos = temp.find('[', pos)) != std::string::npos)
            {
              unsigned int tempCounter = 1;
              unsigned int tempPos = pos++;
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
            unsigned int pos = 0;
            while ((pos = temp.find(']', pos)) != std::string::npos)
            {
              unsigned int tempCounter = 1;
              unsigned int tempPos = pos++;
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
