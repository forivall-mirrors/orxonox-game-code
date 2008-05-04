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

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "CorePrereqs.h"
#include "Tickable.h"
#include "cpptcl/CppTcl.h"

namespace orxonox
{
    class boost::thread;

    struct TclThread
    {
        Tcl::interpreter* interpreter_;
        boost::thread* thread_;
        boost::try_mutex* mutex_;
    };

    class _CoreExport TclThreadManager : public Tickable
    {
        public:
            static TclThreadManager& getInstance();

            static void tclthread(unsigned int threadID, const std::string& command);
            static unsigned int create();
            static void destroy(unsigned int threadID);
            static void execute(unsigned int threadID, const std::string& command);
            static std::string query(unsigned int threadID, const std::string& command);

            static void tcl_execute(Tcl::object const &args);
            static std::string tcl_orxonox(Tcl::object const &args);

            Tcl::interpreter* createTclInterpreter(unsigned int threadID) const;
            bool createTclThread();
            bool destroyTclThread(unsigned int threadID);

            void pushCommandBack(const std::string& command);
            std::string popCommandFront();
            bool isEmpty();

            void pushCommandBack(unsigned int threadID, const std::string& command);
            std::string popCommandFront(unsigned int threadID);
            bool isEmpty(unsigned int threadID);

            std::string eval(const std::string& command);
            std::string eval(unsigned int threadID, const std::string& command);

            virtual void tick(float dt);

        private:
            TclThreadManager();

            bool isReady_;

            unsigned int IDcount_;
            std::map<unsigned int, TclThread> threads_;
            std::map<unsigned int, std::queue<std::string> > threadQueues_;
            std::queue<std::string> orxonoxQueue_;

            boost::mutex orxonoxQueueMutex_;
            boost::mutex threadQueuesMutex_;
            boost::mutex threadsMutex_;
            boost::mutex orxonoxEvalMutex_;

            boost::condition orxonoxQueueCondition_;
            boost::condition orxonoxEvalCondition_;
    };

    _CoreExport void tclThreadLoop(Tcl::interpreter* interpreter, boost::try_mutex* mutex, unsigned int threadID);
}

#endif /* _TclThreadManager_H__ */
