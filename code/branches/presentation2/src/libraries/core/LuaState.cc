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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "LuaState.h"

#include <tolua/tolua++.h>
extern "C" {
#include <lua.h>
#include <lualib.h>
}

#include "util/Debug.h"
#include "Resource.h"
#include "ToluaBindCore.h"

namespace orxonox
{
    LuaState::ToluaInterfaceMap LuaState::toluaInterfaces_s;
    std::vector<LuaState*> LuaState::instances_s;

    // Do this after declaring toluaInterfaces_s and instances_s to avoid larger problems
    DeclareToluaInterface(Core);

    LuaState::LuaState()
        : bIsRunning_(false)
        , includeParseFunction_(NULL)
    {
        // Create new lua state and configure it
        luaState_ = lua_open();
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

        // Open all available tolua interfaces
        this->openToluaInterfaces(luaState_);

        // Create dummy file info
        sourceFileInfo_.reset(new ResourceInfo());
        sourceFileInfo_->group = "General";
        sourceFileInfo_->size = 0;

        // Push 'this' pointer
        tolua_pushusertype(luaState_, static_cast<void*>(this), "orxonox::LuaState");
        lua_setglobal(luaState_, "luaState");

        // Parse init script
        this->doFile("LuaStateInit.lua");
    }

    LuaState::~LuaState()
    {
        lua_close(luaState_);
    }

    shared_ptr<ResourceInfo> LuaState::getFileInfo(const std::string& filename)
    {
        // Look in the current directory first
        shared_ptr<ResourceInfo> sourceInfo = Resource::getInfo(sourceFileInfo_->path + filename);
        // Continue search in root directories
        if (sourceInfo == NULL && !sourceFileInfo_->path.empty())
            sourceInfo = Resource::getInfo(filename);
        return sourceInfo;
    }

    void LuaState::includeFile(const std::string& filename)
    {
        shared_ptr<ResourceInfo> sourceInfo = this->getFileInfo(filename);
        if (sourceInfo != NULL)
            this->includeString(Resource::open(sourceInfo)->getAsString(), sourceInfo);
        else
            COUT(2) << "LuaState: Cannot include file '" << filename << "'." << std::endl;
    }

    void LuaState::includeString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo)
    {
        // Parse string with provided include parser (otherwise don't preparse at all)
        std::string luaInput;
        if (includeParseFunction_ != NULL)
            luaInput = (*includeParseFunction_)(code);
        else
            luaInput = code;

        this->doString(luaInput, sourceFileInfo);
    }

    void LuaState::doFile(const std::string& filename)
    {
        shared_ptr<ResourceInfo> sourceInfo = this->getFileInfo(filename);
        if (sourceInfo != NULL)
            this->doString(Resource::open(sourceInfo)->getAsString(), sourceInfo);
        else
            COUT(2) << "LuaState: Cannot do file '" << filename << "'." << std::endl;
    }

    void LuaState::doString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo)
    {
        // Save the old source file info
        shared_ptr<ResourceInfo> oldSourceFileInfo = sourceFileInfo_;
        // Only override if sourceFileInfo provides useful information
        if (sourceFileInfo != NULL)
            sourceFileInfo_ = sourceFileInfo;

        int error = 0;
#if LUA_VERSION_NUM != 501
        LoadS ls;
        ls.s = code.c_str();
        ls.size = code.size();
        error = lua_load(luaState_, &orxonox::LuaState::lua_Chunkreader, &ls, code.c_str());
#else
        error = luaL_loadstring(luaState_, code.c_str());
#endif

        // execute the chunk
        if (error == 0)
            error = lua_pcall(luaState_, 0, 1, 0);
        if (error != 0)
        {
            std::string origin;
            if (sourceFileInfo != NULL)
                origin = " originating from " + sourceFileInfo_->filename;
            COUT(1) << "Error in Lua-script" << origin << ": " << lua_tostring(luaState_, -1) << std::endl;
            // return value is nil
            lua_pushnil(luaState_);
        }
        // push return value because it will get lost since the return value of this function is void
        lua_setglobal(luaState_, "LuaStateReturnValue");

        // Load the old info again
        sourceFileInfo_ = oldSourceFileInfo;
    }

    void LuaState::luaPrint(const std::string& str)
    {
        output_ << str;
    }

    void LuaState::luaLog(unsigned int level, const std::string& message)
    {
        OutputHandler::getOutStream(level) << message << std::endl;
    }

    bool LuaState::fileExists(const std::string& filename)
    {
        shared_ptr<ResourceInfo> info = this->getFileInfo(filename);
        if (info == NULL)
            return false;
        else
            return true;
    }

#if LUA_VERSION_NUM != 501
    const char * LuaState::lua_Chunkreader(lua_State *L, void *data, size_t *size)
    {
        LoadS* ls = static_cast<LoadS*>(data);
        if (ls->size == 0)
            return NULL;
        *size = ls->size;
        ls->size = 0;
        return ls->s;
    }
#endif

    /*static*/ bool LuaState::addToluaInterface(int (*function)(lua_State*), const std::string& name)
    {
        for (ToluaInterfaceMap::const_iterator it = toluaInterfaces_s.begin(); it != toluaInterfaces_s.end(); ++it)
        {
            if (it->first == name || it->second == function)
            {
                COUT(2) << "Warning: Trying to add a Tolua interface with the same name or function." << std::endl;
                return true;
            }
        }
        toluaInterfaces_s[name] = function;

        // Open interface in all LuaStates
        for (std::vector<LuaState*>::const_iterator it = instances_s.begin(); it != instances_s.end(); ++it)
            (*function)((*it)->luaState_);

        // Return dummy bool
        return true;
    }

    /*static*/ bool LuaState::removeToluaInterface(const std::string& name)
    {
        ToluaInterfaceMap::iterator it = toluaInterfaces_s.find(name);
        if (it == toluaInterfaces_s.end())
        {
            COUT(2) << "Warning: Cannot remove Tolua interface '" << name << "': Not found" << std::endl;
            return true;
        }

        // Close interface in all LuaStates
        for (std::vector<LuaState*>::const_iterator itState = instances_s.begin(); itState != instances_s.end(); ++itState)
        {
            lua_pushnil((*itState)->luaState_);
            lua_setglobal((*itState)->luaState_, it->first.c_str());
        }

        // Remove entry
        toluaInterfaces_s.erase(it);

        // Return dummy bool
        return true;
    }

    /*static*/ void LuaState::openToluaInterfaces(lua_State* state)
    {
        for (ToluaInterfaceMap::const_iterator it = toluaInterfaces_s.begin(); it != toluaInterfaces_s.end(); ++it)
            (*it->second)(state);
    }

    /*static*/ void LuaState::closeToluaInterfaces(lua_State* state)
    {
        for (ToluaInterfaceMap::const_iterator it = toluaInterfaces_s.begin(); it != toluaInterfaces_s.end(); ++it)
        {
            lua_pushnil(state);
            lua_setglobal(state, it->first.c_str());
        }
    }


    LuaFunctor::LuaFunctor(const std::string& code, LuaState* luaState)
    {
        this->code_ = code;
        this->lua_ = luaState;
    }

    void LuaFunctor::operator()(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
    {
        lua_->doString(this->code_);
    }
}
