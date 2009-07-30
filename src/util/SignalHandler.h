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
 *      Christoph Renner
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Declaration of the SignalHandler class.
*/

#ifndef _SignalHandler_H__
#define _SignalHandler_H__

#include "UtilPrereqs.h"

#include <cassert>
#include <list>
#include <string>
#include "Singleton.h"

namespace orxonox
{
    typedef int (*SignalCallback)( void * someData );
}

#ifdef ORXONOX_PLATFORM_LINUX
#include <signal.h>

namespace orxonox
{
    struct SignalRec
    {
        int signal;
        sig_t handler;
    };

    struct SignalCallbackRec
    {
        SignalCallback cb;
        void * someData;
    };


    typedef std::list<SignalRec> SignalRecList;
    typedef std::list<SignalCallbackRec> SignalCallbackList;

    class SignalHandler : public Singleton<SignalHandler>
    {
        friend class Singleton<SignalHandler>;
    public:
        SignalHandler()  { }
        ~SignalHandler() { }

        void registerCallback( SignalCallback cb, void * someData );

        void doCatch( const std::string & appName, const std::string & filename );
        void dontCatch();

    private:
        static void sigHandler( int sig );

        void catchSignal( int sig );
        SignalRecList sigRecList;

        SignalCallbackList callbackList;

        static SignalHandler* singletonPtr_s;

        std::string appName;
        std::string filename;
    };
}

#else /* ORXONOX_PLATFORM_LINUX */

namespace orxonox
{
    class _UtilExport SignalHandler : public Singleton<SignalHandler>
    {
        friend class Singleton<SignalHandler>;
    public:
        SignalHandler()  { }
        ~SignalHandler() { }
        void doCatch( const std::string & appName, const std::string & filename ) {}
        void dontCatch() {}
        void registerCallback( SignalCallback cb, void * someData ) {}

    private:
        static SignalHandler* singletonPtr_s;
    };
}

#endif /* ORXONOX_PLATFORM_LINUX */

#endif /* _SignalHandler_H__ */
