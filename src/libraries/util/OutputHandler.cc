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
    Definition of classes related to output (logging).
*/

#include "OutputHandler.h"

#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Debug.h"

namespace orxonox
{
    //! How the log file shall be named on the filesystem
    const std::string logFileBaseName_g = "orxonox.log";

    /////////////////////////
    ///// LogFileWriter /////
    /////////////////////////
    /**
    @brief
        Writes the output to the log file.
    @note
        As long as the correct log path is not yet known (for pre main code), the
        LogFileWriter will write to a temporary file in /temp (Unix) or %TEMP% (Windows).
        As soon as you set the correct path setLogPath the content of the temporary file
        is read and put into the new file as well.
    */
    class LogFileWriter : public OutputListener
    {
    public:
        /**
        @brief
            Gets temporary log path and starts the log file
        */
        LogFileWriter()
            : OutputListener(OutputHandler::logFileOutputListenerName_s)
        {
            // Get path for a temporary file
#ifdef ORXONOX_PLATFORM_WINDOWS
            char* pTempDir = getenv("TEMP");
            this->logFilename_ = std::string(pTempDir) + '/' + logFileBaseName_g;
#else
            this->logFilename_ = std::string("/tmp/") + logFileBaseName_g;
#endif

            // Get current time
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);

            this->logFile_.open(this->logFilename_.c_str(), std::fstream::out);
            this->logFile_ << "Started log on " << asctime(timeinfo) << std::endl;
            this->logFile_.flush();

            this->outputStream_ = &this->logFile_;
        }

        //! Closes the log file
        ~LogFileWriter()
        {
            this->logFile_ << "Closed log" << std::endl;
            this->logFile_.close();
        }

        //! Changes the log path
        void setLogPath(const std::string& path)
        {
            this->logFile_.close();
            // Read old file into a buffer
            std::ifstream old(this->logFilename_.c_str());
            this->logFilename_ = path + logFileBaseName_g;
            // Open the new file and feed it the content of the old one
            this->logFile_.open(this->logFilename_.c_str(), std::fstream::out);
            this->logFile_ << old.rdbuf();
            this->logFile_.flush();
            old.close();
        }

    private:
        std::ofstream logFile_;     //!< File handle for the log file
        std::string   logFilename_; //!< Filename of the log file
    };


    /////////////////////////
    ///// ConsoleWriter /////
    /////////////////////////
    /**
    @brief
        Writes the output to std::cout.
    @note
        This listener will usually be disable once an actual shell with console is instantiated.
    */
    class ConsoleWriter : public OutputListener
    {
    public:
        //! Only assigns the output stream with std::cout
        ConsoleWriter()
            : OutputListener("consoleLog")
        {
            this->outputStream_ = &std::cout;
        }
    };


    ///////////////////////////
    ///// MemoryLogWriter /////
    ///////////////////////////
    /**
    @brief
        OutputListener that writes all the output piece by piece to an array
        associated with the corresponding output level.
    @note
        Only output below or equal to the current soft debug level is written
        to minimise huge arrays for the normal run.
    */
    class MemoryLogWriter : public OutputListener
    {
    public:
        friend class OutputHandler;

        /**
        @brief
            Sets the right soft debug level and registers itself
        */
        MemoryLogWriter()
            : OutputListener("memoryLog")
        {
            this->outputStream_ = &this->buffer_;
        }

        //! Pushed the just written output to the internal array
        void outputChanged(int level)
        {
            if (!this->buffer_.str().empty())
            {
                // Read ostringstream and store it
                this->output_.push_back(std::make_pair(level, this->buffer_.str()));
                // Clear content and flags
                this->buffer_.str(std::string());
            }
            this->buffer_.clear();
        }

    private:
        std::ostringstream                        buffer_; //!< Stream object used to process the output
        std::vector<std::pair<int, std::string> > output_; //!< Vector containing ALL output
    };


    /////////////////////////
    ///// OutputHandler /////
    /////////////////////////
    const std::string OutputHandler::logFileOutputListenerName_s = "logFile";
          int         OutputHandler::softDebugLevel_s = hardDebugLevel;

    //! Creates the LogFileWriter and the MemoryLogWriter
    OutputHandler::OutputHandler()
        : outputLevel_(OutputLevel::Verbose)
    {
#ifdef ORXONOX_RELEASE
        const OutputLevel::Value defaultLevelConsole = OutputLevel::Error;
        const OutputLevel::Value defaultLevelLogFile = OutputLevel::Info;
#else
        const OutputLevel::Value defaultLevelConsole = OutputLevel::Info;
        const OutputLevel::Value defaultLevelLogFile = OutputLevel::Debug;
#endif

        this->logFile_ = new LogFileWriter();
        // Use default level until we get the configValue from the Core
        this->logFile_->softDebugLevel_ = defaultLevelLogFile;
        this->registerOutputListener(this->logFile_);

        this->consoleWriter_ = new ConsoleWriter();
        this->consoleWriter_->softDebugLevel_ = defaultLevelConsole;
        this->registerOutputListener(this->consoleWriter_);

        this->output_ = new MemoryLogWriter();
        // We capture as much input as the listener with the highest level
        this->output_->softDebugLevel_ = getSoftDebugLevel();
        this->registerOutputListener(this->output_);
    }

    //! Destroys the LogFileWriter and the MemoryLogWriter
    OutputHandler::~OutputHandler()
    {
        delete this->logFile_;
        delete this->consoleWriter_;
        delete this->output_;
    }

    OutputHandler& OutputHandler::getInstance()
    {
        static OutputHandler orxout;
        return orxout;
    }

    void OutputHandler::registerOutputListener(OutputListener* listener)
    {
        for (std::list<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == listener->name_)
            {
                COUT(2) << "OutputHandler, Warning: Trying to register two listeners with the same name!" << std::endl;
                return;
            }
        }
        this->listeners_.push_back(listener);
        // Update global soft debug level
        this->setSoftDebugLevel(listener->getOutputListenerName(), listener->getSoftDebugLevel());
    }

    void OutputHandler::unregisterOutputListener(OutputListener* listener)
    {
        this->listeners_.remove(listener);
    }

    void OutputHandler::setLogPath(const std::string& path)
    {
        this->logFile_->setLogPath(path);
    }

    void OutputHandler::disableCout()
    {
        this->unregisterOutputListener(this->consoleWriter_);
    }

    void OutputHandler::enableCout()
    {
        this->registerOutputListener(this->consoleWriter_);
    }

    OutputHandler::OutputVectorIterator OutputHandler::getOutputVectorBegin() const
    {
        return this->output_->output_.begin();
    }

    OutputHandler::OutputVectorIterator OutputHandler::getOutputVectorEnd() const
    {
        return this->output_->output_.end();
    }

    int OutputHandler::getSoftDebugLevel(const std::string& name) const
    {
        for (std::list<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == name)
                return (*it)->softDebugLevel_;
        }
        return -1;
    }

    void OutputHandler::setSoftDebugLevel(const std::string& name, int level)
    {
        int globalSoftDebugLevel = -1;
        for (std::list<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == name)
                (*it)->softDebugLevel_ = level;
            if ((*it)->softDebugLevel_ > globalSoftDebugLevel)
                globalSoftDebugLevel = (*it)->softDebugLevel_;
        }
        // Update global soft debug level
        OutputHandler::softDebugLevel_s = globalSoftDebugLevel;
    }
}
