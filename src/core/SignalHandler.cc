/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
    @file SignalHandler.cc
    @brief Implementation of the SignalHandler class.
*/

#include <assert.h>

#include "Debug.h"
#include "SignalHandler.h"

SignalHandler * SignalHandler::singletonRef = NULL;

#ifndef __WIN32__

#include <wait.h>

SignalHandler::SignalHandler()
{
}

/**
 * register signal handlers for SIGSEGV and SIGABRT
 * @param appName path to executable eg argv[0]
 * @param fileName filename to append backtrace to
 */
void SignalHandler::doCatch( const std::string & appName, const std::string & fileName )
{
  this->appName = appName;
  this->fileName = fileName;

  // make sure doCatch is only called once without calling dontCatch
  assert( sigRecList.size() == 0 );

  catchSignal( SIGSEGV );
  catchSignal( SIGABRT );
  catchSignal( SIGILL );
}

/**
 * restore previous signal handlers
 */
void SignalHandler::dontCatch()
{
  for ( SignalRecList::iterator it = sigRecList.begin(); it != sigRecList.end(); it++ )
  {
    signal( it->signal, it->handler );
  }

  sigRecList.clear();
}

/**
 * catch signal sig
 * @param sig signal to catch
 */
void SignalHandler::catchSignal( int sig )
{
  sig_t handler = signal( sig, SignalHandler::sigHandler );

  assert( handler != SIG_ERR );

  SignalRec rec;
  rec.signal = sig;
  rec.handler = handler;

  sigRecList.push_front( rec );
}

/**
 * sigHandler is called when receiving signals
 * @param sig
 */
void SignalHandler::sigHandler( int sig )
{
  for ( SignalCallbackList::iterator it = SignalHandler::getInstance()->callbackList.begin(); it != SignalHandler::getInstance()->callbackList.end(); it++  )
  {
    (*(it->cb))( it->someData );
  }

  std::string sigName = "UNKNOWN";

  switch ( sig )
  {
    case SIGSEGV:
      sigName = "SIGSEGV";
      break;
    case SIGABRT:
      sigName = "SIGABRT";
      break;
    case SIGILL:
      sigName = "SIGILL";
      break;
  }

  PRINTF(0)( "recieved signal %s\ntry to write backtrace to file orxonox.log\n", sigName.c_str() );

  int sigPipe[2];
  if ( pipe(sigPipe) == -1 )
  {
    perror("pipe failed!\n");
    exit(EXIT_FAILURE);
  }

  int sigPid = fork();

  if ( sigPid == -1 )
  {
    perror("fork failed!\n");
    exit(EXIT_FAILURE);
  }

  // gdb will be attached to this process
  if ( sigPid == 0 )
  {
    getInstance()->dontCatch();
    // wait for message from parent when it has attached gdb
    int someData;

    read( sigPipe[0], &someData, sizeof(someData) );

    if ( someData != 0x12345678 )
    {
      PRINTF(0)("something went wrong :(\n");
    }

    return;
  }

  int gdbIn[2];
  int gdbOut[2];
  int gdbErr[2];

  if ( pipe(gdbIn) == -1 || pipe(gdbOut) == -1 || pipe(gdbErr) == -1 )
  {
    perror("pipe failed!\n");
    kill( sigPid, SIGTERM );
    waitpid( sigPid, NULL, 0 );
    exit(EXIT_FAILURE);
  }

  int gdbPid = fork();
  // this process will run gdb

  if ( gdbPid == -1 )
  {
    perror("fork failed\n");
    kill( sigPid, SIGTERM );
    waitpid( sigPid, NULL, 0 );
    exit(EXIT_FAILURE);
  }

  if ( gdbPid == 0 )
  {
    // start gdb

    close(gdbIn[1]);
    close(gdbOut[0]);
    close(gdbErr[0]);

    dup2( gdbIn[0], STDIN_FILENO );
    dup2( gdbOut[1], STDOUT_FILENO );
    dup2( gdbErr[1], STDERR_FILENO );

    execlp( "sh", "sh", "-c", "gdb", (void*)NULL);
  }

  char cmd[256];
  snprintf( cmd, 256, "file %s\nattach %d\nc\n", getInstance()->appName.c_str(), sigPid );
  write( gdbIn[1], cmd, strlen(cmd) );

  int charsFound = 0;
  int promptFound = 0;
  char byte;
  while ( read( gdbOut[0], &byte, 1 ) == 1 )
  {
    if (
      charsFound == 0 && byte == '(' ||
      charsFound == 1 && byte == 'g' ||
      charsFound == 2 && byte == 'd' ||
      charsFound == 3 && byte == 'b' ||
      charsFound == 4 && byte == ')' ||
      charsFound == 5 && byte == ' '
        )
          charsFound++;
    else
      charsFound = 0;

    if ( charsFound == 6 )
    {
      promptFound++;
      charsFound = 0;
    }

    if ( promptFound == 3 )
    {
      break;
    }
  }

  int someData = 0x12345678;
  write( sigPipe[1], &someData, sizeof(someData) );

  write( gdbIn[1], "bt\nk\nq\n", 7 );


  charsFound = 0;
  promptFound = 0;
  std::string bt;
  while ( read( gdbOut[0], &byte, 1 ) == 1 )
  {
    bt += std::string( &byte, 1 );

    if (
      charsFound == 0 && byte == '(' ||
      charsFound == 1 && byte == 'g' ||
      charsFound == 2 && byte == 'd' ||
      charsFound == 3 && byte == 'b' ||
      charsFound == 4 && byte == ')' ||
      charsFound == 5 && byte == ' '
        )
          charsFound++;
    else
      charsFound = 0;

    if ( charsFound == 6 )
    {
      promptFound++;
      charsFound = 0;
      bt += "\n";
    }

    if ( promptFound == 3 )
    {
      break;
    }
  }


  waitpid( sigPid, NULL, 0 );
  waitpid( gdbPid, NULL, 0 );

  int wsRemoved = 0;

  while ( wsRemoved < 2 && bt.length() > 0 )
  {
    if ( bt[1] == '\n' )
      wsRemoved++;
    bt.erase(0, 1);
  }

  if ( bt.length() > 0 )
    bt.erase(0, 1);

  time_t now = time(NULL);

  std::string timeString = "\n\n\n\n"
	                   "=======================================================\n"
	                   "= time: " + std::string(ctime(&now)) +
			   "=======================================================\n";
  bt.insert(0, timeString);

  FILE * f = fopen( getInstance()->fileName.c_str(), "a" );

  if ( !f )
  {
    perror( ( std::string( "could not append to " ) + getInstance()->fileName ).c_str() );
    exit(EXIT_FAILURE);
  }

  if ( fwrite( bt.c_str(), 1, bt.length(), f ) != bt.length() )
  {
    PRINTF(0)( ( std::string("could not write %d byte to ") + getInstance()->fileName ).c_str(), bt.length());
    exit(EXIT_FAILURE);
  }

  exit(EXIT_FAILURE);
}

void SignalHandler::registerCallback( SignalCallback cb, void * someData )
{
  SignalCallbackRec rec;
  rec.cb = cb;
  rec.someData = someData;

  callbackList.push_back(rec);
}

#endif /* __WIN32__ */
