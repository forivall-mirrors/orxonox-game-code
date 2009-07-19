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

#ifndef _TclThreadManager_H__
#define _TclThreadManager_H__

#include "CorePrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>
#include "core/OrxonoxClass.h"

namespace orxonox
{
    // Internal struct
    struct TclInterpreterBundle;

    class _CoreExport TclThreadManager : public OrxonoxClass
    {
        friend class IRC;
        friend class TclBind;

        public:
            TclThreadManager(Tcl::interpreter* interpreter);
            ~TclThreadManager();

            static TclThreadManager& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

            static unsigned int create();
            static unsigned int createID(unsigned int threadID);
            static void destroy(unsigned int threadID);
            static void execute(unsigned int threadID, const std::string& command);
            static std::string query(unsigned int threadID, const std::string& command);
            static void status();
            static void dump(unsigned int threadID);
            static void flush(unsigned int threadID);

            void error(const std::string& error);
            void debug(const std::string& error);

            void update(const Clock& time);

            std::list<unsigned int> getThreadList() const;

        private:
            TclThreadManager(const TclThreadManager& other);

            static void tcl_execute(Tcl::object const &args);
            static std::string tcl_query(int querierID, Tcl::object const &args);
            static std::string tcl_crossquery(int querierID, int threadID, Tcl::object const &args);
            static bool tcl_running(int threadID);

            Tcl::interpreter* createNewTclInterpreter(const std::string& threadID);
            Tcl::interpreter* getTclInterpreter(unsigned int threadID);
            TclInterpreterBundle* getInterpreterBundle(unsigned int threadID);
            std::string dumpList(const std::list<unsigned int>& list);

            void pushCommandToQueue(const std::string& command);
            void forceCommandToFrontOfQueue(const std::string& command);
            std::string popCommandFromQueue();
            bool queueIsEmpty();

            void pushCommandToQueue(unsigned int threadID, const std::string& command);
            std::string popCommandFromQueue(unsigned int threadID);
            bool queueIsEmpty(unsigned int threadID);

            bool updateQueriersList(TclInterpreterBundle* querier, TclInterpreterBundle* target);

            std::string evalQuery(unsigned int querierID, const std::string& command);
            std::string evalQuery(unsigned int querierID, unsigned int threadID, const std::string& command);

            unsigned int threadCounter_;
            TclInterpreterBundle* orxonoxInterpreterBundle_;
            std::map<unsigned int, TclInterpreterBundle*> interpreterBundles_;

            static TclThreadManager* singletonRef_s;
    };

    _CoreExport void tclThread(TclInterpreterBundle* interpreterBundle, std::string command);
}

#endif /* _TclThreadManager_H__ */
