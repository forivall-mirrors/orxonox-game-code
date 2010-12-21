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
    @ingroup Util
    @brief Declaration of the SignalHandler class.
*/

#ifndef _SignalHandler_H__
#define _SignalHandler_H__

#include "UtilPrereqs.h"

#include <cassert>
#include <string>

#include "Singleton.h"
#include "SpecialConfig.h"

#if defined(ORXONOX_PLATFORM_LINUX)

#include <list>
#include <signal.h>

namespace orxonox
{
    typedef int (*SignalCallback)( void * someData );

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

    /// The SignalHandler is used to catch signals like SIGSEGV and write a backtrace to the logfile.
    class _UtilExport SignalHandler : public Singleton<SignalHandler>
    {
        friend class Singleton<SignalHandler>;

        public:
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

#elif defined(ORXONOX_PLATFORM_WINDOWS) && defined(DBGHELP_FOUND)

#include <windows.h>

namespace orxonox
{
    /// The SignalHandler is used to catch unhandled exceptions like access violations and write a backtrace to the logfile.
    class _UtilExport SignalHandler : public Singleton<SignalHandler>
    {
        friend class Singleton<SignalHandler>;

        public:
            SignalHandler();
            ~SignalHandler();

            void doCatch(const std::string& appName, const std::string& filename);

            static std::string getStackTrace(PEXCEPTION_POINTERS pExceptionInfo = NULL);
            static std::string getExceptionType(PEXCEPTION_POINTERS pExceptionInfo);

        private:
            static LONG WINAPI exceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

            static std::string getModuleName(const std::string& path);
            static DWORD getModuleBase(DWORD dwAddress);

            template <typename T>
            static std::string pointerToString(T pointer, bool bFillZeros = true);
            template <typename T>
            static std::string pointerToString(T* pointer);

            static SignalHandler* singletonPtr_s;

            std::string filename_;
            LPTOP_LEVEL_EXCEPTION_FILTER prevExceptionFilter_;
    };
}

#else

namespace orxonox
{
    /// The SignalHandler is used to catch signals like SIGSEGV and write a backtrace to the logfile. Not implemented on systems except Linux and Windows.
    class _UtilExport SignalHandler : public Singleton<SignalHandler>
    {
        friend class Singleton<SignalHandler>;

        public:
            void doCatch(const std::string& appName, const std::string& filename) {}

        private:
            static SignalHandler* singletonPtr_s;
    };
}

#endif

#endif /* _SignalHandler_H__ */
