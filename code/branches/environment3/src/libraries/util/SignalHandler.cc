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
    @brief Implementation of the SignalHandler class.
*/

#include "SignalHandler.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "Output.h"

namespace orxonox
{
    SignalHandler* SignalHandler::singletonPtr_s = NULL;
}

#if defined(ORXONOX_PLATFORM_LINUX)

#include <wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <sys/prctl.h>

namespace orxonox
{
    /**
     * register signal handlers for SIGSEGV and SIGABRT
     * @param appName path to executable eg argv[0]
     * @param filename filename to append backtrace to
     */
    void SignalHandler::doCatch( const std::string & appName, const std::string & filename )
    {
      this->appName = appName;
      this->filename = filename;

      // make sure doCatch is only called once without calling dontCatch
      assert( sigRecList.size() == 0 );

      catchSignal( SIGSEGV );
//      catchSignal( SIGABRT );
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
      // if the signalhandler has already been destroyed then don't do anything
      if( SignalHandler::singletonPtr_s == 0 )
      {
        orxout(user_error) << "Received signal(sigHandler already destroyed) " << sigName.c_str() << endl << "Can't write backtrace because SignalHandler is already destroyed" << endl;
        exit(EXIT_FAILURE);
      }

      for ( SignalCallbackList::iterator it = SignalHandler::getInstance().callbackList.begin(); it != SignalHandler::getInstance().callbackList.end(); it++  )
      {
        (*(it->cb))( it->someData );
      }


      orxout(user_error) << "Received signal(second error) " << sigName.c_str() << endl << "Try to write backtrace to file orxonox_crash.log" << endl;

      
      // First start GDB which will be attached to this process later on
      
      int gdbIn[2];
      int gdbOut[2];
      int gdbErr[2];
      
      if ( pipe(gdbIn) == -1 || pipe(gdbOut) == -1 || pipe(gdbErr) == -1 )
      {
        perror("pipe failed!\n");
        exit(EXIT_FAILURE);
      }
      
      int gdbPid = fork();
      // this process will run gdb
      
      if ( gdbPid == -1 )
      {
        perror("fork failed\n");
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
        
        execlp( "sh", "sh", "-c", "gdb", static_cast<void*>(NULL));
      }
      
      
      // Now start a fork of this process on which GDB will be attached on
      
      int sigPipe[2];
      if ( pipe(sigPipe) == -1 )
      {
        perror("pipe failed!\n");
        kill( gdbPid, SIGTERM );
        waitpid( gdbPid, NULL, 0 );
        exit(EXIT_FAILURE);
      }

      int sigPid = fork();

      if ( sigPid == -1 )
      {
        perror("fork failed!\n");
        kill( gdbPid, SIGTERM );
        waitpid( gdbPid, NULL, 0 );
        exit(EXIT_FAILURE);
      }

      // gdb will be attached to this process
      if ( sigPid == 0 )
      {
        getInstance().dontCatch();
        
        // make sure gdb is allowed to attach to our PID even if there are some system restrictions
#ifdef PR_SET_PTRACER
        if( prctl(PR_SET_PTRACER, gdbPid, 0, 0, 0) == -1 )
          orxout(user_error) << "could not set proper permissions for GDB to attach to process..." << endl;
#endif
        
        // wait for message from parent when it has attached gdb
        int someData;

        if( read( sigPipe[0], &someData, sizeof(someData) ) != sizeof(someData) )
          orxout(user_error) << "something went wrong :(" << endl;

        if ( someData != 0x12345678 )
        {
          orxout(user_error) << "something went wrong :(" << endl;
        }

        return;
      }

      char cmd[256];
      snprintf( cmd, 256, "file %s\nattach %d\nc\n", getInstance().appName.c_str(), sigPid );
      write( gdbIn[1], cmd, strlen(cmd) );

      int charsFound = 0;
      int promptFound = 0;
      char byte;
      while ( read( gdbOut[0], &byte, 1 ) == 1 )
      {
        if (
          (charsFound == 0 && byte == '(') ||
          (charsFound == 1 && byte == 'g') ||
          (charsFound == 2 && byte == 'd') ||
          (charsFound == 3 && byte == 'b') ||
          (charsFound == 4 && byte == ')') ||
          (charsFound == 5 && byte == ' ')
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
          (charsFound == 0 && byte == '(') ||
          (charsFound == 1 && byte == 'g') ||
          (charsFound == 2 && byte == 'd') ||
          (charsFound == 3 && byte == 'b') ||
          (charsFound == 4 && byte == ')') ||
          (charsFound == 5 && byte == ' ')
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

      std::string timeString =
                         "=======================================================\n"
                         "= time: " + std::string(ctime(&now)) +
                         "=======================================================\n";
      bt.insert(0, timeString);

      FILE * f = fopen( getInstance().filename.c_str(), "w" );

      if ( !f )
      {
        perror( ( std::string( "could not append to " ) + getInstance().filename ).c_str() );
        exit(EXIT_FAILURE);
      }

      if ( fwrite( bt.c_str(), 1, bt.length(), f ) != bt.length() )
      {
        orxout(user_error) << "could not write " << bt.length() << " byte to " << getInstance().filename << endl;
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
}

#elif defined(ORXONOX_PLATFORM_WINDOWS) && defined(DBGHELP_FOUND)

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <dbghelp.h>
#include <tlhelp32.h>

#include "Convert.h"

#ifdef ORXONOX_COMPILER_GCC
#   include <cxxabi.h>
#endif

#ifdef ORXONOX_COMPILER_GCC
/// Overwrite the original abort() function in MinGW to enable a break point.
_UtilExport void __cdecl abort()
{
    using namespace orxonox;
    orxout(user_error) << "This application has requested the Runtime to terminate it in an unusual way." << endl;
    orxout(user_error) << "Please contact the application's support team for more information." << endl;
    DebugBreak();
    exit(0x3);
}

/// Overwrite the original _abort() function in MinGW to enable a break point.
_UtilExport void __cdecl _assert(const char* expression, const char* file, int line)
{
    using namespace orxonox;
    orxout(user_error) << "Assertion failed: " << expression << ", file " << file << ", line " << line << endl;
    orxout(user_error) << endl;
    abort();
}
#endif

namespace orxonox
{
    /// Constructor: Initializes the values, but doesn't register the exception handler.
    SignalHandler::SignalHandler()
    {
        this->prevExceptionFilter_ = NULL;
    }

    /// Destructor: Removes the exception handler.
    SignalHandler::~SignalHandler()
    {
        if (this->prevExceptionFilter_ != NULL)
        {
            // Remove the unhandled exception filter function
            SetUnhandledExceptionFilter(this->prevExceptionFilter_);
            this->prevExceptionFilter_ = NULL;
        }
    }

    /// Registers an exception handler and initializes the filename of the crash log.
    void SignalHandler::doCatch(const std::string&, const std::string& filename)
    {
        this->filename_ = filename;

        // don't register twice
        assert(this->prevExceptionFilter_ == NULL);

        if (this->prevExceptionFilter_ == NULL)
        {
            // Install the unhandled exception filter function
            this->prevExceptionFilter_ = SetUnhandledExceptionFilter(&SignalHandler::exceptionFilter);
        }
    }

    /// Exception handler: Will be called by Windows if an unhandled exceptions occurs.
    /* static */ LONG WINAPI SignalHandler::exceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
    {
        // avoid loops
        static bool bExecuting = false;

        if (!bExecuting)
        {
            bExecuting = true;

            orxout(user_error) << endl;

            // if the signalhandler has already been destroyed then don't do anything
            if (SignalHandler::singletonPtr_s == 0)
            {
                orxout(user_error) << "Caught an unhandled exception" << endl << "Can't write backtrace because SignalHandler is already destroyed" << endl;
                exit(EXIT_FAILURE);
            }

            orxout(user_error) << "Caught an unhandled exception" << endl << "Try to write backtrace to orxonox_crash.log..." << endl;

            // write the crash log
            std::ofstream crashlog(SignalHandler::getInstance().filename_.c_str());

            time_t now = time(NULL);

            crashlog << "=======================================================" << endl;
            crashlog << "= Time: " << std::string(ctime(&now));
            crashlog << "=======================================================" << endl;
            crashlog << endl;

            const std::string& error = SignalHandler::getExceptionType(pExceptionInfo);

            crashlog << error << endl;
            crashlog << endl;

            const std::string& callstack = SignalHandler::getStackTrace(pExceptionInfo);

            crashlog << "Call stack:" << endl;
            crashlog << callstack << endl;

            crashlog.close();

            // print the same information also to the console
            orxout(user_error) << endl;
            orxout(user_error) << error << endl;
            orxout(user_error) << endl;
            orxout(user_error) << "Call stack:" << endl;
            orxout(user_error) << callstack << endl;

            bExecuting = false;
        }
        else
        {
            orxout(user_error) << "An error occurred while writing the backtrace" << endl;
        }

        if (SignalHandler::getInstance().prevExceptionFilter_)
            return SignalHandler::getInstance().prevExceptionFilter_(pExceptionInfo);
        else
            return EXCEPTION_CONTINUE_SEARCH;
    }

    /// Returns the stack trace for either the current function, or, if @a pExceptionInfo is not NULL, for the given exception context.
    /* static */ std::string SignalHandler::getStackTrace(PEXCEPTION_POINTERS pExceptionInfo)
    {
        // Initialise the symbol table to get function names:
        SymSetOptions
        (
            SYMOPT_DEFERRED_LOADS
#ifndef ORXONOX_COMPILER_GCC
            | SYMOPT_UNDNAME
#endif
        );
        SymInitialize(GetCurrentProcess(), 0, true);

        // Store the current stack frame here:
        STACKFRAME64 frame;
        memset(&frame, 0, sizeof(STACKFRAME64));

        // Get processor information for the current thread:
        CONTEXT context;
        memset(&context, 0, sizeof(CONTEXT));

        if (pExceptionInfo)
        {
            // get the context of the exception
            context = *pExceptionInfo->ContextRecord;
        }
        else
        {
            context.ContextFlags = CONTEXT_FULL;

            // Load the RTLCapture context function:
            HMODULE kernel32 = LoadLibrary("Kernel32.dll");
            typedef void (*RtlCaptureContextFunc) (CONTEXT* ContextRecord);
            RtlCaptureContextFunc rtlCaptureContext = (RtlCaptureContextFunc) GetProcAddress(kernel32, "RtlCaptureContext");

            // Capture the thread context
            rtlCaptureContext(&context);
        }

        DWORD type;

        // set the flags and initialize the stackframe struct
#ifdef _M_IX86
        type = IMAGE_FILE_MACHINE_I386;

        frame.AddrPC.Offset         = context.Eip;              // program counter
        frame.AddrFrame.Offset      = context.Ebp;              // frame pointer (for function arguments)
        frame.AddrStack.Offset      = context.Esp;              // stack pointer
#elif _M_X64
        type = IMAGE_FILE_MACHINE_AMD64;

        frame.AddrPC.Offset         = context.Rip;              // program counter
        frame.AddrFrame.Offset      = context.Rbp; // (or Rdi)  // frame pointer (for function arguments)
        frame.AddrStack.Offset      = context.Rsp;              // stack pointer
#elif _M_IA64
        type = IMAGE_FILE_MACHINE_IA64;

        frame.AddrPC.Offset         = context.StIIP;            // program counter
        frame.AddrFrame.Offset      = context.RsBSP;            // frame pointer (for function arguments) // <-- unneeded on Intel IPF, may be removed
        frame.AddrStack.Offset      = context.IntSp;            // stack pointer
        frame.AddrBStore.Offset     = context.RsBSP;            // backing store
        frame.AddrBStore.Mode       = AddrModeFlat;
#else
        return "";
#endif

        frame.AddrPC.Mode           = AddrModeFlat;
        frame.AddrFrame.Mode        = AddrModeFlat;
        frame.AddrStack.Mode        = AddrModeFlat;

        std::string output;

        // Keep getting stack frames from windows till there are no more left:
        for (int i = 0;
            StackWalk64
            (
                type                      ,      // MachineType
                GetCurrentProcess()       ,      // Process to get stack trace for
                GetCurrentThread()        ,      // Thread to get stack trace for
                &frame                    ,      // Where to store next stack frame
                &context                  ,      // Pointer to processor context record
                0                         ,      // Routine to read process memory: use the default ReadProcessMemory
                &SymFunctionTableAccess64 ,      // Routine to access the modules symbol table
                &SymGetModuleBase64       ,      // Routine to access the modules base address
                0                                // Something to do with 16-bit code. Not needed.
            );
            ++i
        )
        {
            //------------------------------------------------------------------
            // Declare an image help symbol structure to hold symbol info and
            // name up to 256 chars This struct is of variable lenght though so
            // it must be declared as a raw byte buffer.
            //------------------------------------------------------------------
            static char symbolBuffer[sizeof(SYMBOL_INFO) + 255];
            memset(symbolBuffer, 0, sizeof(symbolBuffer));

            // Cast it to a symbol struct:
            SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;

            // Need to set two fields of this symbol before obtaining name info:
            symbol->SizeOfStruct    = sizeof(SYMBOL_INFO);
            symbol->MaxNameLen      = 255;

            // The displacement from the beginning of the symbol is stored here: pretty useless
            long long unsigned int displacement = 0;

            if (i < 10)
                output += " ";
            output += multi_cast<std::string>(i) + ": ";

            // Print the function's address:
            output += SignalHandler::pointerToString(frame.AddrPC.Offset);

            // Get the symbol information from the address of the instruction pointer register:
            bool bCorrected = false;
            BOOL result = SymFromAddr
            (
                GetCurrentProcess() ,   // Process to get symbol information for
                frame.AddrPC.Offset ,   // Address to get symbol for: instruction pointer register
                &displacement       ,   // Displacement from the beginning of the symbol
                symbol                  // Where to save the symbol
            );

            // If the symbol was found, but the displacement is 0, we likely got the wrong symbol - decrease the program counter and try again
            if (result && displacement == 0)
            {
                bCorrected = true;
                result = SymFromAddr
                (
                    GetCurrentProcess()     ,
                    frame.AddrPC.Offset - 1 ,
                    &displacement           ,
                    symbol
                );
            }

            // Display the function name + offset
            if (result)
            {
                // Add the name of the function to the function list:
                output += " ";

#ifdef ORXONOX_COMPILER_GCC
                int status;
                char* demangled = __cxxabiv1::__cxa_demangle(symbol->Name, NULL, NULL, &status);
                if (demangled)
                {
                    output += demangled;
                    free(demangled);
                }
                else
#endif
                {
                    output += symbol->Name;
                }

                output += " +" + SignalHandler::pointerToString(displacement, false);
                if (bCorrected)
                    output += " (?)";
            }

            output += "\n";

            // Get the file name and line number
            IMAGEHLP_LINE64 line;
            memset(&line, 0, sizeof(IMAGEHLP_LINE64));
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            DWORD displacement2 = 0;

            if
            (
                SymGetLineFromAddr64
                (
                    GetCurrentProcess(),
                    frame.AddrPC.Offset - bCorrected ? 1 : 0,
                    &displacement2,
                    &line
                )
            )
            {
                output += "               ";
                output += line.FileName;
                output += ":";
                output += multi_cast<std::string>(line.LineNumber);
                output += "\n";
            }
        }

        // Cleanup the symbol table:
        SymCleanup(GetCurrentProcess());

        return output;
    }

    /// Returns a description of the given exception.
    // Based on code from Dr. Mingw by José Fonseca
    /* static */ std::string SignalHandler::getExceptionType(PEXCEPTION_POINTERS pExceptionInfo)
    {
        PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;
        TCHAR szModule[MAX_PATH];
        HMODULE hModule;

        std::string output = (GetModuleFileName(NULL, szModule, MAX_PATH) ? SignalHandler::getModuleName(szModule) : "Application");
        output += " caused ";

        switch(pExceptionRecord->ExceptionCode)
        {
            case EXCEPTION_ACCESS_VIOLATION:            output += "an Access Violation";        break;
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:       output += "an Array Bound Exceeded";    break;
            case EXCEPTION_BREAKPOINT:                  output += "a Breakpoint";               break;
            case EXCEPTION_DATATYPE_MISALIGNMENT:       output += "a Datatype Misalignment";    break;
            case EXCEPTION_FLT_DENORMAL_OPERAND:        output += "a Float Denormal Operand";   break;
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:          output += "a Float Divide By Zero";     break;
            case EXCEPTION_FLT_INEXACT_RESULT:          output += "a Float Inexact Result";     break;
            case EXCEPTION_FLT_INVALID_OPERATION:       output += "a Float Invalid Operation";  break;
            case EXCEPTION_FLT_OVERFLOW:                output += "a Float Overflow";           break;
            case EXCEPTION_FLT_STACK_CHECK:             output += "a Float Stack Check";        break;
            case EXCEPTION_FLT_UNDERFLOW:               output += "a Float Underflow";          break;
            case EXCEPTION_GUARD_PAGE:                  output += "a Guard Page";               break;
            case EXCEPTION_ILLEGAL_INSTRUCTION:         output += "an Illegal Instruction";     break;
            case EXCEPTION_IN_PAGE_ERROR:               output += "an In Page Error";           break;
            case EXCEPTION_INT_DIVIDE_BY_ZERO:          output += "an Integer Divide By Zero";  break;
            case EXCEPTION_INT_OVERFLOW:                output += "an Integer Overflow";        break;
            case EXCEPTION_INVALID_DISPOSITION:         output += "an Invalid Disposition";     break;
            case EXCEPTION_INVALID_HANDLE:              output += "an Invalid Handle";          break;
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:    output += "a Noncontinuable Exception"; break;
            case EXCEPTION_PRIV_INSTRUCTION:            output += "a Privileged Instruction";   break;
            case EXCEPTION_SINGLE_STEP:                 output += "a Single Step";              break;
            case EXCEPTION_STACK_OVERFLOW:              output += "a Stack Overflow";           break;
            case DBG_CONTROL_C:                         output += "a Control+C";                break;
            case DBG_CONTROL_BREAK:                     output += "a Control+Break";            break;
            case DBG_TERMINATE_THREAD:                  output += "a Terminate Thread";         break;
            case DBG_TERMINATE_PROCESS:                 output += "a Terminate Process";        break;
            case RPC_S_UNKNOWN_IF:                      output += "an Unknown Interface";       break;
            case RPC_S_SERVER_UNAVAILABLE:              output += "a Server Unavailable";       break;
            default:                                    output += "an Unknown Exception (" + SignalHandler::pointerToString(pExceptionRecord->ExceptionCode) + ")"; break;
        }

        // Now print information about where the fault occured
        output += " at location " + SignalHandler::pointerToString(pExceptionRecord->ExceptionAddress);
        if ((hModule = (HMODULE) SignalHandler::getModuleBase((DWORD) pExceptionRecord->ExceptionAddress)) && GetModuleFileName(hModule, szModule, MAX_PATH))
        {
            output += " in module ";
            output += SignalHandler::getModuleName(szModule);
        }

        // If the exception was an access violation, print out some additional information, to the error log and the debugger.
        if(pExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && pExceptionRecord->NumberParameters >= 2)
        {
            output += " ";
            output += pExceptionRecord->ExceptionInformation[0] ? "writing to" : "reading from";
            output += " location ";
            output += SignalHandler::pointerToString(pExceptionRecord->ExceptionInformation[1]);
        }

        return output;
    }

    /// Strips the directories from the path to the module and returns the module's name only.
    /* static */ std::string SignalHandler::getModuleName(const std::string& path)
    {
        return path.substr(path.find_last_of('\\') + 1);
    }

    /// Retrieves the base address of the module that contains the specified address.
    // Code from Dr. Mingw by José Fonseca
    /* static */ DWORD SignalHandler::getModuleBase(DWORD dwAddress)
    {
        MEMORY_BASIC_INFORMATION Buffer;

        return VirtualQuery((LPCVOID) dwAddress, &Buffer, sizeof(Buffer)) ? (DWORD) Buffer.AllocationBase : 0;
    }

    /// Converts a value to string, formatted as pointer.
    template <typename T>
    /* static */ std::string SignalHandler::pointerToString(T pointer, bool bFillZeros)
    {
        std::ostringstream oss;

        if (bFillZeros)
            oss << std::setw(8) << std::setfill('0');

        oss << std::hex << pointer;

        return std::string("0x") + oss.str();
    }

    /// Converts a pointer to string.
    template <typename T>
    /* static */ std::string SignalHandler::pointerToString(T* pointer)
    {
        std::ostringstream oss;

        oss << pointer;

        return oss.str();
    }
}

#endif
