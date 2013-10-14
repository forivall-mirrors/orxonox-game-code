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

/**
    @file
    @ingroup Command Tcl
    @brief Declaration of TclThreadManager, used to create multithreaded Tcl interpreters.
*/

#ifndef _TclThreadManager_H__
#define _TclThreadManager_H__

#include "core/CorePrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>

#include "util/Singleton.h"

struct Tcl_Interp;

namespace orxonox
{
    class _CoreExport TclThreadManager : public Singleton<TclThreadManager>
    {
        friend class Singleton<TclThreadManager>;
        friend class TclBind;
        friend _CoreExport void tclThread(TclInterpreterBundle* bundle, const std::string& command);
        friend _CoreExport void sourceThread(const std::string& file);
        friend _CoreExport int Tcl_OrxonoxAppInit(Tcl_Interp* interp);

        public:
            TclThreadManager(Tcl::interpreter* interpreter);
            virtual ~TclThreadManager();

            static unsigned int      create();
            static Tcl::interpreter* createWithId(unsigned int id);
            static void              destroy(unsigned int id);
            static void              execute(unsigned int target_id, const std::string& command);
            static std::string       query(unsigned int target_id, const std::string& command);
            static void              source(const std::string& file);

            static void error(const std::string& error);
            static void debug(const std::string& error);

            void preUpdate(const Clock& time);

            std::list<unsigned int> getThreadList() const;

        private:
            static void tcl_execute(const Tcl::object& args);
            static void tcl_crossexecute(int target_id, const Tcl::object& args);
            static std::string tcl_query(int source_id, const Tcl::object& args);
            static std::string tcl_crossquery(int source_id, int target_id, const Tcl::object& args);
            static bool tcl_running(int id);

            void _execute(unsigned int target_id, const std::string& command);
            std::string _query(unsigned int source_id, unsigned int target_id, const std::string& command, bool bUseCommandExecutor = false);

            TclInterpreterBundle* getInterpreterBundle(unsigned int id);
            std::string dumpList(const std::list<unsigned int>& list);

            static void initialize(TclInterpreterBundle* bundle);
            static std::string eval(TclInterpreterBundle* bundle, const std::string& command, const std::string& action);

            static TclThreadManager* singletonPtr_s;                            ///< Singleton pointer

            unsigned int numInterpreterBundles_;                                ///< Number of created Tcl-interpreters (only used for auto-numbered interpreters, not affected by @ref createWithId)
            std::map<unsigned int, TclInterpreterBundle*> interpreterBundles_;  ///< A map containing all Tcl-interpreters
            boost::shared_mutex* interpreterBundlesMutex_;                      ///< A mutex used to synchronize threads when accessing @ref interpreterBundles_
            TclThreadList<std::string>* messageQueue_;                          ///< A queue to pass messages from Tcl-threads to the main thread
            boost::mutex* mainInterpreterMutex_;                                ///< A mutex to synchronize queries to the main interpreter
    };

    _CoreExport void tclThread(TclInterpreterBundle* bundle, const std::string& command);
    _CoreExport void sourceThread(const std::string& file);
    _CoreExport int Tcl_OrxonoxAppInit(Tcl_Interp* interp);
}

#endif /* _TclThreadManager_H__ */
