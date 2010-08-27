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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _CommandExecutor_H__
#define _CommandExecutor_H__

#include "core/CorePrereqs.h"

#include <map>
#include <list>
#include <string>

#include "util/MultiType.h"
#include "CommandEvaluation.h"

// tolua_begin
namespace orxonox
{
    class _CoreExport CommandExecutor
    {
// tolua_end
        public:
            static int execute(const std::string& command, bool useTcl = true); // tolua_export

            static MultiType queryMT(const std::string& command, int* error = 0, bool useTcl = true);
            static std::string query(const std::string& command, int* error = 0, bool useTcl = true); // tolua_export

            static CommandEvaluation evaluate(const std::string& command);

            static const int Success = 0;
            static const int Error = 1;
            static const int Incomplete = 2;
            static const int Deactivated = 3;
            static const int Denied = 4;

            static MultiType unhide(const std::string& command);
            static void _autocomplete(const std::string& group, const std::string& name) {}

        private:
            CommandExecutor() {}
            CommandExecutor(const CommandExecutor& other);
            ~CommandExecutor() {}

            static CommandExecutor& getInstance();

            bool getCached(const std::string& command, CommandEvaluation& evaluation);
            void cache(const std::string& command, const CommandEvaluation& evaluation);

            struct CacheEntry
            {
                CommandEvaluation evaluation_;
                std::list<std::string>::iterator iterator_;
            };

            std::map<std::string, CacheEntry> cache_;
            std::list<std::string> cachelist_;
    }; // tolua_export
} // tolua_export

#endif /* _CommandExecutor_H__ */
