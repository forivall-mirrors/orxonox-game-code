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

#ifndef _LuaState_H__
#define _LuaState_H__

#include "CorePrereqs.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "util/ScopeGuard.h"
#include "core/Functor.h"
#include "ToluaInterface.h"

namespace orxonox // tolua_export
{ // tolua_export
    class Functor; // tolua_export

    //! Functor subclass that simply executes code with 0 arguments.
    class _CoreExport LuaFunctor : public Functor
    {
        public:
            LuaFunctor(const std::string& code, LuaState* luaState);
            void operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null);
            void evaluateParam(unsigned int index, MultiType& param) const {}

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

        void doFile(const std::string& filename); // tolua_export
        void doString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo = shared_ptr<ResourceInfo>());

        void includeFile(const std::string& filename); // tolua_export
        void includeString(const std::string& code, const shared_ptr<ResourceInfo>& sourceFileInfo = shared_ptr<ResourceInfo>());

        void luaPrint(const std::string& str); // tolua_export
        void luaLog(unsigned int level, const std::string& message); // tolua_export
        bool fileExists(const std::string& filename); // tolua_export

        const std::stringstream& getOutput() const { return output_; }
        void clearOutput() { output_.clear(); } // tolua_export

        void setIncludeParser(std::string (*function)(const std::string&)) { includeParseFunction_ = function; }
        lua_State* getInternalLuaState() { return luaState_; }

        void setDefaultResourceInfo(const shared_ptr<ResourceInfo>& sourceFileInfo) { this->sourceFileInfo_ = sourceFileInfo; }
        const shared_ptr<ResourceInfo>& getDefaultResourceInfo() { return this->sourceFileInfo_; }

        Functor* createLuaFunctor(const std::string& code) { return new LuaFunctor(code, this); } // tolua_export

        static bool addToluaInterface(int (*function)(lua_State*), const std::string& name);
        static bool removeToluaInterface(const std::string& name);
        static void openToluaInterfaces(lua_State* state);
        static void closeToluaInterfaces(lua_State* state);

    private:
        shared_ptr<ResourceInfo> getFileInfo(const std::string& filename);

#if LUA_VERSION_NUM != 501
        struct LoadS
        {
            const char* s;
            size_t size;
        };

        static const char * lua_Chunkreader(lua_State *L, void *data, size_t *size);
#endif

        std::stringstream output_;
        lua_State* luaState_;
        bool bIsRunning_;
        shared_ptr<ResourceInfo> sourceFileInfo_;
        std::string (*includeParseFunction_)(const std::string&);

        typedef std::map<std::string, int (*)(lua_State *L)> ToluaInterfaceMap;
        static ToluaInterfaceMap toluaInterfaces_s;
        static std::vector<LuaState*> instances_s;
    }; // tolua_export
} // tolua_export

#endif /* _LuaState_H__ */
