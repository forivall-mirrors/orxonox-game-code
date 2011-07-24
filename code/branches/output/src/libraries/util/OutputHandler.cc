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
            : OutputListener("LogFile")
        {
            this->logFileBaseName_ = "orxonox.log";

            // Get path for a temporary file
#ifdef ORXONOX_PLATFORM_WINDOWS
            char* pTempDir = getenv("TEMP");
            this->logFilename_ = std::string(pTempDir) + '/' + this->logFileBaseName_;
#else
            this->logFilename_ = std::string("/tmp/") + this->logFileBaseName_;
#endif

            // Get current time
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);

            this->openFile();
            if (this->logFile_.is_open())
            {
                this->logFile_ << "Started log on " << asctime(timeinfo) << std::endl;
                this->logFile_.flush();
            }
        }

        //! Closes the log file
        ~LogFileWriter()
        {
            if (this->logFile_.is_open())
            {
                this->logFile_ << "Closed log" << std::endl;
                this->logFile_.close();
            }
        }

        //! Changes the log path
        void setLogPath(const std::string& path)
        {
            if (this->logFile_.is_open())
                this->logFile_.close();

            // Open the new file
            this->logFilename_ = path + this->logFileBaseName_;
            this->openFile();
        }

        //! Erases the log file
        void clearFile()
        {
            if (this->logFile_.is_open())
            {
                this->logFile_.close();
                this->openFile();
            }
        }

    private:
        void openFile()
        {
            this->logFile_.open(this->logFilename_.c_str(), std::fstream::out);

            if (this->logFile_.is_open())
                this->outputStream_ = &this->logFile_;
            else
            {
                COUT(2) << "Warning: Failed to open log file. File logging disabled." << std::endl;
                this->outputStream_ = NULL;
            }
        }

        std::ofstream logFile_;         //!< File handle for the log file
        std::string   logFilename_;     //!< Filename of the log file
        std::string   logFileBaseName_; //!< How the log file shall be named on the filesystem
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
            : OutputListener("Console")
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
        Used as buffer until all output devices have been initialised.
    @note
        At some point, OutputHandler::disableMemoryLog() has to be called in
        order to avoid large memory footprints of this class.
    */
    class MemoryLogWriter : public OutputListener
    {
    public:
        friend class OutputHandler;

        MemoryLogWriter()
            : OutputListener("memoryLog")
        {
            this->outputStream_ = &this->buffer_;
        }

        //! Push the just written output to the internal array
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
        std::ostringstream          buffer_; //!< Stream object used to process the output
        OutputHandler::OutputVector output_; //!< Vector containing ALL output
    };


    /////////////////////////
    ///// OutputHandler /////
    /////////////////////////
    int OutputHandler::softDebugLevel_s = hardDebugLevel;

    //! Creates the LogFileWriter and the MemoryLogWriter
    OutputHandler::OutputHandler()
        : outputLevel_(OutputLevel::Verbose)
    {
        // Note: These levels only concern startup before orxonox.ini is read.
#ifdef ORXONOX_RELEASE
        const OutputLevel::Value initialLevelConsole = OutputLevel::Error;
#else
        const OutputLevel::Value initialLevelConsole = OutputLevel::Info;
#endif
        // Use high log level because we rewrite the log file anyway with the
        // correct level. But if Orxonox were to crash before that, we might be
        // grateful to have a high debug level, esp. for releases.
        const OutputLevel::Value intialLevelLogFile = OutputLevel::Debug;

        this->logFile_ = new LogFileWriter();
        // Use default level until we get the configValue from the Core
        this->logFile_->softDebugLevel_ = intialLevelLogFile;
        this->registerOutputListener(this->logFile_);

        this->consoleWriter_ = new ConsoleWriter();
        this->consoleWriter_->softDebugLevel_ = initialLevelConsole;
        this->registerOutputListener(this->consoleWriter_);

        this->memoryBuffer_ = new MemoryLogWriter();
        // Write everything, e.g. use hardDebugLevel
        this->memoryBuffer_->softDebugLevel_ = hardDebugLevel;
        this->registerOutputListener(this->memoryBuffer_);
    }

    //! Destroys the LogFileWriter and the MemoryLogWriter
    OutputHandler::~OutputHandler()
    {
        delete this->logFile_;
        delete this->consoleWriter_;
        delete this->memoryBuffer_; // Might already be NULL
    }

    OutputHandler& OutputHandler::getInstance()
    {
        static OutputHandler orxout;
        return orxout;
    }

    void OutputHandler::registerOutputListener(OutputListener* listener)
    {
        for (std::vector<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == listener->name_)
            {
                COUT(2) << "OutputHandler, Warning: Trying to register two listeners with the same name!" << std::endl;
                return;
            }
        }
        this->listeners_.push_back(listener);
        this->updateGlobalDebugLevel();
    }

    void OutputHandler::unregisterOutputListener(OutputListener* listener)
    {
        for (std::vector<OutputListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == listener->name_)
            {
                this->listeners_.erase(it);
                break;
            }
        }
        this->updateGlobalDebugLevel();
    }

    void OutputHandler::setLogPath(const std::string& path)
    {
        this->logFile_->setLogPath(path);
        this->rewriteLogFile();
    }

    void OutputHandler::rewriteLogFile()
    {
        logFile_->clearFile();

        if (logFile_->outputStream_ == NULL)
            return;

        for (OutputVector::const_iterator it = this->getOutput().begin(); it != this->getOutput().end(); ++it)
        {
            if (it->first <= logFile_->softDebugLevel_)
                (*logFile_->outputStream_) << it->second;
        }
        logFile_->outputStream_->flush();
    }

    void OutputHandler::disableCout()
    {
        this->unregisterOutputListener(this->consoleWriter_);
    }

    void OutputHandler::enableCout()
    {
        this->registerOutputListener(this->consoleWriter_);
    }

    void OutputHandler::disableMemoryLog()
    {
        this->unregisterOutputListener(this->memoryBuffer_);
        // Only clear the buffer so we can still reference the vector
        this->memoryBuffer_->output_.clear();
    }

    const OutputHandler::OutputVector& OutputHandler::getOutput() const
    {
        return this->memoryBuffer_->output_;
    }

    int OutputHandler::getSoftDebugLevel(const std::string& name) const
    {
        for (std::vector<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == name)
                return (*it)->softDebugLevel_;
        }
        return -1;
    }

    void OutputHandler::setSoftDebugLevel(const std::string& name, int level)
    {
        for (std::vector<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->name_ == name)
                (*it)->softDebugLevel_ = level;
        }
        this->updateGlobalDebugLevel();
    }

    void OutputHandler::updateGlobalDebugLevel()
    {
        int globalSoftDebugLevel = -1;
        std::vector<OutputListener*>::const_iterator it = this->listeners_.begin();
        for (; it != this->listeners_.end(); ++it)
            globalSoftDebugLevel = std::max(globalSoftDebugLevel, (*it)->softDebugLevel_);

        OutputHandler::softDebugLevel_s = globalSoftDebugLevel;
    }
}
