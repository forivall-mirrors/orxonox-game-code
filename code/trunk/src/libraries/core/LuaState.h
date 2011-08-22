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

/**
    @defgroup Lua Lua
    @ingroup Core
*/

/**
    @file
    @ingroup Lua
*/

#ifndef _LuaState_H__
#define _LuaState_H__

#include "CorePrereqs.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "util/Output.h"

namespace orxonox // tolua_export
{ // tolua_export
    class LuaFunctor; // tolua_export

    //! callback class that executes lua code
    class _CoreExport LuaFunctor
    {
        public:
            LuaFunctor(const std::string& code, LuaState* luaState);
            void operator()();

        private:
            std::string code_;
            LuaState*   lua_;
    };


    /**
    @brief
        Representation of an interface to lua
    */
    class _CoreExport LuaState // tolua_export
    { // tolua_export
    public:
        LuaState();
        ~LuaState();

        bool doFile(const std::string& filename); // tolua_export
        bool doString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo = shared_ptr<ResourceInfo>());

        bool includeFile(const std::string& filename); // tolua_export
        bool includeString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo = shared_ptr<ResourceInfo>());

        void luaPrint(const std::string& str); // tolua_export
        void luaOutput(orxonox::level::OutputLevel level, const std::string& context, const std::string& message); // tolua_export
        void luaOutput(orxonox::level::OutputLevel level, const std::string& message); // tolua_export
        void luaOutput(const std::string& message); // tolua_export
        bool fileExists(const std::string& filename); // tolua_export
        std::string getSourceCode(const std::string& filename); // tolua_export

        const std::stringstream& getOutput() const { return output_; }
        void clearOutput() { output_.clear(); } // tolua_export

        void setIncludeParser(std::string (*function)(const std::string&)) { includeParseFunction_ = function; }
        lua_State* getInternalLuaState() { return luaState_; }

        void setDefaultResourceInfo(const shared_ptr<ResourceInfo>& sourceFileInfo) { this->sourceFileInfo_ = sourceFileInfo; }
        const shared_ptr<ResourceInfo>& getDefaultResourceInfo() { return this->sourceFileInfo_; }

        LuaFunctor* createLuaFunctor(const std::string& code) { return new LuaFunctor(code, this); } // tolua_export
        //! Tells about whether IOConsole was activated. The Lua debugger only works with a normal console.
        bool usingIOConsole() const; // tolua_export

        static bool addToluaInterface(int (*function)(lua_State*), const std::string& name);
        static bool removeToluaInterface(const std::string& name);
        static void openToluaInterfaces(lua_State* state);
        static void closeToluaInterfaces(lua_State* state);

        static const std::string ERROR_HANDLER_NAME;

    private:
        shared_ptr<ResourceInfo> getFileInfo(const std::string& filename);

        std::stringstream output_;
        lua_State* luaState_;
        bool bIsRunning_;
        shared_ptr<ResourceInfo> sourceFileInfo_;
        std::map<std::string, std::string> sourceCodeMap_;
        std::string (*includeParseFunction_)(const std::string&);

        typedef std::map<std::string, int (*)(lua_State *L)> ToluaInterfaceMap;
        static ToluaInterfaceMap& getToluaInterfaces();
        static std::vector<LuaState*>& getInstances();
    }; // tolua_export


    //! Helper class that registers/unregisters tolua bindings
    class ToluaBindingsHelper
    {
    public:
        ToluaBindingsHelper(int (*function)(lua_State*), const std::string& libraryName)
            : libraryName_(libraryName)
        {
            LuaState::addToluaInterface(function, libraryName_);
        }
        ~ToluaBindingsHelper()
        {
            LuaState::removeToluaInterface(libraryName_);
        }
    private:
        std::string libraryName_;
    };
} // tolua_export

#endif /* _LuaState_H__ */
