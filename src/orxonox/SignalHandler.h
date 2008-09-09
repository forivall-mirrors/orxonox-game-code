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
    @file SignalHandler.h
    @brief Definition of the SignalHandler class.
*/

#ifndef _SignalHandler_H__
#define _SignalHandler_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <string>

typedef int (*SignalCallback)( void * someData );

#if ORXONOX_PLATFORM != ORXONOX_PLATFORM_WIN32
#include <signal.h>

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

class SignalHandler
{
  private:
    SignalHandler();
  public:
    inline static SignalHandler* getInstance() { if (!SignalHandler::singletonRef) SignalHandler::singletonRef = new SignalHandler(); return SignalHandler::singletonRef; }
    ~SignalHandler(){ SignalHandler::singletonRef = NULL; }

    void registerCallback( SignalCallback cb, void * someData );

    void doCatch( const std::string & appName, const std::string & fileName );
    void dontCatch();

  private:
    static void sigHandler( int sig );

    void catchSignal( int sig );
    SignalRecList sigRecList;

    SignalCallbackList callbackList;

    static SignalHandler * singletonRef;

    std::string appName;
    std::string fileName;

    // used to turn on KeyAutoRepeat if OIS crashes
    static bool bXAutoKeyRepeatOn_;
};

#else /* ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32 */

class _OrxonoxExport SignalHandler
{
  public:
    inline static SignalHandler* getInstance() { if (!SignalHandler::singletonRef) SignalHandler::singletonRef = new SignalHandler(); return SignalHandler::singletonRef; };
    void doCatch( const std::string & appName, const std::string & fileName ) {};
    void dontCatch() {};
    void registerCallback( SignalCallback cb, void * someData ) {};

  private:
    static SignalHandler * singletonRef;
};

#endif /* ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32 */

#endif /* _SignalHandler_H__ */
