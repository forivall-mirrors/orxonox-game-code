/*!
 * @file SignalHandler.h
 */

#ifndef _SignalHandler_H__
#define _SignalHandler_H__

#include <list>
#include <string>

typedef int (*SignalCallback)( void * someData );

#ifndef __WIN32__
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
};

#else /* if defined __WIN32__ */
class SignalHandler
{
 public:
   inline static SignalHandler* getInstance() { if (!SignalHandler::singletonRef) SignalHandler::singletonRef = new SignalHandler(); return SignalHandler::singletonRef; };
   void doCatch( const std::string & appName, const std::string & fileName ) {};
  void dontCatch() {};
  void registerCallback( SignalCallback cb, void * someData ) {};

 private:
    static SignalHandler * singletonRef;
};
#endif

#endif /* _SignalHandler_H__ */

