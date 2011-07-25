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
 *      Reto Grieder
 *
 */

/**
@file
@brief
    Declaration of classes related to output (logging).
*/

#ifndef _OutputHandler_H__
#define _OutputHandler_H__

#include "UtilPrereqs.h"

#include <list>
#include <ostream>
#include <string>
#include <vector>
#include <utility>

namespace orxonox
{
    /**
    @brief
        Denotes different levels of text output (log output)

        0, None   : Very important output
        1, Error  : Errors
        2, Warning: Warnings
        3, Info   : Information
        4, Debug  : Debug information
        5, Verbose: More debug information
        6, Ultra  : Crazy debug information
    */
    namespace OutputLevel
    {
        enum Value
        {
            None    = 0,
            Error   = 1,
            Warning = 2,
            Info    = 3,
            Debug   = 4,
            Verbose = 5,
            Ultra   = 6,
        };
    }

    // Forward declarations for classes in the source file
    class LogFileWriter;
    class ConsoleWriter;
    class MemoryLogWriter;

    /**
    @brief
        The OutputHandler acts like std::cout, but output isn't only shown in the console.

        You can register your own listener for output by inheriting from OutputListner.
        And if you need the output previously processed, iterate over it with
        OutputHandler::getOutputVector[Begin/End].
        The way to output text is to first set the desired output level with
        OutputHandler::getOutStream(level) and then use the "<<" operator like with std::cout.
    */
    class _UtilExport OutputHandler
    {
        public:
            //! Returns a reference to the only existing instance of the OutputHandler class.
            static OutputHandler& getInstance();

            //! Sets the output level and returns a stream to be used with "<<"
            static inline OutputHandler& getOutStream(int level)
                { return OutputHandler::getInstance().setOutputLevel(level); }

            typedef std::vector<std::pair<int, std::string> >::const_iterator OutputVectorIterator;
            //! Returns an iterator to the beginning of the all-output vector
            OutputVectorIterator getOutputVectorBegin() const;
            //! Returns an iterator to the end of the all-output vector
            OutputVectorIterator getOutputVectorEnd() const;

            //! Writes to all output devices
            static inline const std::string& log(const std::string& text)
                { OutputHandler::getOutStream(0).output(text) << std::endl; return text; }

            //! Writes an error message to the output
            static inline const std::string& error(const std::string& text)
                { OutputHandler::getOutStream(1).output(text) << std::endl; return text; }

            //! Writes a warning message to the output
            static inline const std::string& warning(const std::string& text)
                { OutputHandler::getOutStream(2).output(text) << std::endl; return text; }

            //! Writes an informational message to the output
            static inline const std::string& info(const std::string& text)
                { OutputHandler::getOutStream(3).output(text) << std::endl; return text; }

            //! Writes a debug message to the output
            static inline const std::string& debug(const std::string& text)
                { OutputHandler::getOutStream(4).output(text) << std::endl; return text; }

            //! Registers an object that receives output via a provided std::ostream
            void registerOutputListener(OutputListener* listener);
            //! Unregisters an object that receives output via a provided std::ostream
            void unregisterOutputListener(OutputListener* listener);

            //! Set the log path once the program has been properly initialised
            void setLogPath(const std::string& path);
            //! Disables the std::cout stream for output
            void disableCout();
            //! Enables the std::cout stream for output (startup behaviour)
            void enableCout();

            //! Sets the level of the incoming output and returns the OutputHandler
            inline OutputHandler& setOutputLevel(int level)
                { this->outputLevel_ = level; return *this; }

            //! Returns the level of the incoming output
            inline int getOutputLevel() const
                { return this->outputLevel_; }

            //! Returns the maximum debug level over all registered listeners (devices)
            static int getSoftDebugLevel() { return softDebugLevel_s; }
            //! Returns the soft debug level for a device by its name   @return The level or -1 if the listener was not found
            int  getSoftDebugLevel(const std::string& name) const;
            //! Sets the soft debug level for a listener by its name   @remarks Only works for registered listeners!
            void setSoftDebugLevel(const std::string& name, int level);

            /**
            @brief
                General template that copes with all output.
                Required because operator << might be ambiguous.
                @a output will be streamed into every listener with an appropriate debug level
            @return
                Returns a reference to the OutputHandler so you can use it again directly
            */
            template <class T>
            OutputHandler& output(const T& output);

            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned char val)      { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(short val)              { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned short val)     { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(int val)                { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned int val)       { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long val)               { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned long val)      { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long long val)          { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned long long val) { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(float val)              { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(double val)             { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long double val)        { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(const void* val)        { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(bool val)               { return this->output(val); }

            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(std::streambuf* sb)     { return this->output(sb); }

            //! Overloaded << operator, redirect the output of classes with self defined 'operator <<' to the listeners
            template <class T>
            inline OutputHandler& operator<<(const T& val)           { return this->output(val); }

            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ostream&  (*manip)(std::ostream&))  { return this->output(manip); }
            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ios&      (*manip)(std::ios&))      { return this->output(manip); }
            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ios_base& (*manip)(std::ios_base&)) { return this->output(manip); }

            //! Dummy operator required by Debug.h for the ternary operator
            inline operator int() const { return 0; }

            //! Name of the OutputListener that writes to the log file
            static const std::string logFileOutputListenerName_s;

        private:
            OutputHandler();
            ~OutputHandler();
            OutputHandler(const OutputHandler& rhs); //! Unused and undefined

            std::list<OutputListener*> listeners_;        //!< Array with all registered output listeners
            int                        outputLevel_;      //!< The level of the incoming output
            LogFileWriter*             logFile_;          //!< Listener that writes to the log file
            ConsoleWriter*             consoleWriter_;    //!< Listener for std::cout (just program beginning)
            MemoryLogWriter*           output_;           //!< Listener that Stores ALL output below the current soft debug level
            static int                 softDebugLevel_s;  //!< Maximum of all soft debug levels. @note This is only static for faster access
    };

    /**
    @brief
        Interface for listening to output.
    @remarks
        Remember to register the listener (not done automatically!)
    */
    class OutputListener
    {
        friend class OutputHandler;

    public:
        OutputListener(const std::string& name)
            : outputStream_(NULL)
            , name_(name)
            , softDebugLevel_(OutputLevel::Info)
        {}
        virtual ~OutputListener() {}

        //! Gets called whenever output is put into the stream
        virtual void outputChanged(int level) {}
        //! Returns the name of this output listener
        const std::string& getOutputListenerName() const { return this->name_; }
        //! Returns the soft debug level of the listener
        int getSoftDebugLevel() const { return this->softDebugLevel_; }
        //! Sets the soft debug level of the listener
        void setSoftDebugLevel(int level)
        {
            this->softDebugLevel_ = level;
            OutputHandler::getInstance().setSoftDebugLevel(this->name_, level);
        }

    protected:
        std::ostream*     outputStream_;   //!< Pointer to the associated output stream, can be NULL

    private:
        const std::string name_;           //!< Name of the listener, constant and unique!
        int               softDebugLevel_; //!< Current soft debug level that defines what kind of output is written to the stream
    };

    template<class T>
    inline OutputHandler& OutputHandler::output(const T& output)
    {
        for (std::list<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if (this->outputLevel_ <= (*it)->softDebugLevel_ && (*it)->outputStream_ != NULL)
            {
                std::ostream& stream = *((*it)->outputStream_);
                stream << output;
                stream.flush();
                (*it)->outputChanged(this->outputLevel_);
            }
        }

        return *this;
    }
}

#endif /* _OutputHandler_H__ */
