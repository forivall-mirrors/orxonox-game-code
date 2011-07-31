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

#include <tolua++.h>
extern "C" {
#include <lua.h>
#include <lualib.h>
}
#include <loki/ScopeGuard.h>
#include <boost/preprocessor/stringize.hpp>

#include "util/Output.h"
#include "util/Exception.h"
#include "Resource.h"
#include "command/IOConsole.h"

namespace orxonox
{
    const std::string LuaState::ERROR_HANDLER_NAME = "errorHandler";

    LuaState::LuaState()
        : bIsRunning_(false)
        , includeParseFunction_(NULL)
    {
        // Create new lua state and configure it
        luaState_ = lua_open();
        Loki::ScopeGuard luaStateGuard = Loki::MakeGuard(&lua_close, luaState_);
        luaL_openlibs(luaState_);

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
        if (!this->doFile("LuaStateInit.lua"))
            ThrowException(InitialisationFailed, "Running LuaStateInit.lua failed");

        luaStateGuard.Dismiss();
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

    bool LuaState::includeFile(const std::string& filename)
    {
        shared_ptr<ResourceInfo> sourceInfo = this->getFileInfo(filename);
        if (sourceInfo != NULL)
            return this->includeString(Resource::open(sourceInfo)->getAsString(), sourceInfo);
        else
        {
            orxout(internal_warning, context::lua) << "LuaState: Cannot include file '" << filename << "' (not found)." << endl;
            return false;
        }
    }

    bool LuaState::includeString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo)
    {
        // Parse string with provided include parser (otherwise don't preparse at all)
        std::string luaInput;
        if (includeParseFunction_ != NULL)
            luaInput = (*includeParseFunction_)(code);
        else
            luaInput = code;

        if (sourceFileInfo != NULL)
        {
            // Also fill a map with the actual source code. This is just for the include* commands
            // where the content of sourceFileInfo->filename doesn't match 'code'
            this->sourceCodeMap_[sourceFileInfo->filename] = code;
        }

        bool returnValue = this->doString(luaInput, sourceFileInfo);

        if (sourceFileInfo != NULL)
        {
            // Delete source code entry
            if (sourceFileInfo != NULL)
                this->sourceCodeMap_.erase(sourceFileInfo->filename);
        }

        return returnValue;
    }

    bool LuaState::doFile(const std::string& filename)
    {
        shared_ptr<ResourceInfo> sourceInfo = this->getFileInfo(filename);
        if (sourceInfo != NULL)
            return this->doString(Resource::open(sourceInfo)->getAsString(), sourceInfo);
        else
        {
            orxout(internal_warning, context::lua) << "LuaState: Cannot do file '" << filename << "' (not found)." << endl;
            return false;
        }
    }

    bool LuaState::doString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo)
    {
        // Save the old source file info
        shared_ptr<ResourceInfo> oldSourceFileInfo = sourceFileInfo_;
        // Only override if sourceFileInfo provides useful information
        if (sourceFileInfo != NULL)
            sourceFileInfo_ = sourceFileInfo;

        std::string chunkname;
        if (sourceFileInfo != NULL)
        {
            // Provide lua_load with the filename for debug purposes
            // The '@' is a Lua convention to identify the chunk name as filename
            chunkname = '@' + sourceFileInfo->filename;
        }
        else
        {
            // Use the code string to identify the chunk
            chunkname = code;
        }

        // Push custom error handler that uses the debugger
        lua_getglobal(this->luaState_, ERROR_HANDLER_NAME.c_str());
        int errorHandler = lua_gettop(luaState_);
        if (lua_isnil(this->luaState_, -1))
        {
            lua_pop(this->luaState_, 1);
            errorHandler = 0;
        }

        int error = luaL_loadbuffer(luaState_, code.c_str(), code.size(), chunkname.c_str());

        switch (error)
        {
        case LUA_ERRSYNTAX: // Syntax error
            orxout(internal_error, context::lua) << "Lua syntax error: " << lua_tostring(luaState_, -1) << endl;
            break;
        case LUA_ERRMEM:    // Memory allocation error
            orxout(internal_error, context::lua) << "Lua memory allocation error: Consult your dentist immediately!" << endl;
            break;
        }

        if (error == 0)
        {
            // Execute the chunk in protected mode with an error handler function (stack index)
            error = lua_pcall(luaState_, 0, 1, errorHandler);

            switch (error)
            {
            case LUA_ERRRUN: // Runtime error
                if (errorHandler)
                {
                    // Do nothing (we already display the error in the
                    // 'errorHandler' Lua function in LuaStateInit.lua)
                }
                else
                {
                    std::string errorString = lua_tostring(this->luaState_, -1);
                    if (errorString.find("Error propagation") == std::string::npos)
                        orxout(internal_error, context::lua) << "Lua runtime error: " << errorString << endl;
                }
                break;
            case LUA_ERRERR: // Error in the error handler
                orxout(internal_error, context::lua) << "Lua error in error handler. No message available." << endl;
                break;
            case LUA_ERRMEM: // Memory allocation error
                orxout(internal_error, context::lua) << "Lua memory allocation error: Consult your dentist immediately!" << endl;
                break;
            }
        }

        if (error != 0)
        {
            lua_pop(luaState_, 1);  // Remove error message
            lua_pushnil(luaState_); // Push a nil return value
        }

        if (errorHandler != 0)
            lua_remove(luaState_, errorHandler); // Remove error handler from stack

        // Set return value to a global variable because we cannot return a table in this function
        // here. It would work for numbers, pointers and strings, but certainly not for Lua tables.
        lua_setglobal(luaState_, "LuaStateReturnValue");

        // Load the old info again
        sourceFileInfo_ = oldSourceFileInfo;

        return (error == 0);
    }

    void LuaState::luaPrint(const std::string& str)
    {
        output_ << str;
    }

    void LuaState::luaLog(unsigned int level, const std::string& message)
    {
#pragma message(__FILE__ "("BOOST_PP_STRINGIZE(__LINE__)") : Warning: TODO: use correct level, inspect lua support (and remove boost include)")
        orxout(debug_output, context::lua) << "luaLog (level: " << level << "): " << message << endl;
    }

    bool LuaState::fileExists(const std::string& filename)
    {
        shared_ptr<ResourceInfo> info = this->getFileInfo(filename);
        if (info == NULL)
            return false;
        else
            return true;
    }

    //! Returns the content of a file
    std::string LuaState::getSourceCode(const std::string& filename)
    {
        // Try the internal map first to get the actual Lua code
        // and not just some pseudo Lua-XML code when using include* commands
        std::map<std::string, std::string>::const_iterator it = this->sourceCodeMap_.find(filename);
        if (it != this->sourceCodeMap_.end())
            return it->second;
        shared_ptr<ResourceInfo> info = Resource::getInfo(filename);
        if (info == NULL)
            return "";
        else
            return Resource::open(info)->getAsString();
    }

    bool LuaState::usingIOConsole() const
    {
        return IOConsole::exists();
    }

    /*static*/ LuaState::ToluaInterfaceMap& LuaState::getToluaInterfaces()
    {
        static ToluaInterfaceMap p;
        return p;
    }

    /*static*/ std::vector<LuaState*>& LuaState::getInstances()
    {
        static std::vector<LuaState*> p;
        return p;
    }

    /*static*/ bool LuaState::addToluaInterface(int (*function)(lua_State*), const std::string& name)
    {
        for (ToluaInterfaceMap::const_iterator it = getToluaInterfaces().begin(); it != getToluaInterfaces().end(); ++it)
        {
            if (it->first == name || it->second == function)
            {
                orxout(internal_warning, context::lua) << "Trying to add a Tolua interface with the same name or function." << endl;
                return true;
            }
        }
        getToluaInterfaces()[name] = function;

        // Open interface in all LuaStates
        for (std::vector<LuaState*>::const_iterator it = getInstances().begin(); it != getInstances().end(); ++it)
            (*function)((*it)->luaState_);

        // Return dummy bool
        return true;
    }

    /*static*/ bool LuaState::removeToluaInterface(const std::string& name)
    {
        ToluaInterfaceMap::iterator it = getToluaInterfaces().find(name);
        if (it == getToluaInterfaces().end())
        {
            orxout(internal_warning, context::lua) << "Cannot remove Tolua interface '" << name << "': Not found" << endl;
            return true;
        }

        // Close interface in all LuaStates
        for (std::vector<LuaState*>::const_iterator itState = getInstances().begin(); itState != getInstances().end(); ++itState)
        {
            lua_pushnil((*itState)->luaState_);
            lua_setglobal((*itState)->luaState_, it->first.c_str());
        }

        // Remove entry
        getToluaInterfaces().erase(it);

        // Return dummy bool
        return true;
    }

    /*static*/ void LuaState::openToluaInterfaces(lua_State* state)
    {
        for (ToluaInterfaceMap::const_iterator it = getToluaInterfaces().begin(); it != getToluaInterfaces().end(); ++it)
            (*it->second)(state);
    }

    /*static*/ void LuaState::closeToluaInterfaces(lua_State* state)
    {
        for (ToluaInterfaceMap::const_iterator it = getToluaInterfaces().begin(); it != getToluaInterfaces().end(); ++it)
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

    void LuaFunctor::operator()()
    {
        lua_->doString(this->code_);
    }
}
