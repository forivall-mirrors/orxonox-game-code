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

#include <queue>
#include <map>
#include <list>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

#include "CorePrereqs.h"
#include "Tickable.h"
#include "cpptcl/CppTcl.h"

namespace orxonox
{
    class boost::thread;

    struct _CoreExport TclInterpreterBundle
    {
        unsigned int id_;

        std::list<std::string> queue_;
        boost::mutex queueMutex_;

        Tcl::interpreter* interpreter_;
        std::string interpreterName_;
        boost::try_mutex interpreterMutex_;

        std::list<unsigned int> queriers_;
        boost::mutex queriersMutex_;

        bool running_;
        boost::mutex runningMutex_;

        bool finished_;
        boost::mutex finishedMutex_;
        boost::condition finishedCondition_;
    };

    class _CoreExport TclThreadManager : public Tickable
    {
        public:
            static TclThreadManager& getInstance();

            static unsigned int create();
            static unsigned int createID(unsigned int threadID);
            static void destroy(unsigned int threadID);
            static void execute(unsigned int threadID, const std::string& command);
            static std::string query(unsigned int threadID, const std::string& command);
            static void status();
            static void dump(unsigned int threadID);
            static void flush(unsigned int threadID);

            static void tcl_execute(Tcl::object const &args);
            static std::string tcl_query(int querierID, Tcl::object const &args);
            static std::string tcl_crossquery(int querierID, int threadID, Tcl::object const &args);
            static bool tcl_running(int threadID);

            Tcl::interpreter* createNewTclInterpreter(const std::string& threadID);
            TclInterpreterBundle* getInterpreterBundle(unsigned int threadID);
            std::string dumpList(const std::list<unsigned int>& list);
            void error(const std::string& error);
            void debug(const std::string& error);

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

            virtual void tick(float dt);

            std::list<std::pair<std::string, std::string> > getThreadList() const;

        private:
            TclThreadManager();
            TclThreadManager(const TclThreadManager& other);
            ~TclThreadManager() {}

            unsigned int threadCounter_;
            TclInterpreterBundle orxonoxInterpreterBundle_;
            std::map<unsigned int, TclInterpreterBundle*> interpreterBundles_;
            boost::mutex bundlesMutex_;
            boost::condition fullQueueCondition_;
            boost::condition orxonoxEvalCondition_;
#if (BOOST_VERSION >= 103500)
            boost::thread::id threadID_;
#else
            boost::thread threadID_;
#endif
    };

    _CoreExport void tclThread(TclInterpreterBundle* interpreterBundle, std::string command);
}

#endif /* _TclThreadManager_H__ */
