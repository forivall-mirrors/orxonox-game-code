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

/**
    @file
    @brief Implementation of the OutputHandler class.
*/

#include "OutputHandler.h"
#include <time.h>

namespace orxonox
{
    /**
        @brief Constructor: Opens the logfile and writes the first line.
        @param logfilename The name of the logfile
    */
    OutputHandler::OutputHandler(const std::string& logfilename)
    {
        this->outputBuffer_ = &this->fallbackBuffer_;
        this->softDebugLevel_[0] = this->softDebugLevel_[1] = this->softDebugLevel_[2] = this->softDebugLevel_[3] = 2;
        this->logfilename_ = logfilename;
        this->logfile_.open(this->logfilename_.c_str(), std::fstream::out);

        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        this->logfile_ << "Started log at " << asctime(timeinfo) << std::endl;
        this->logfile_.flush();
    }

    /**
        @brief Destructor: Writes the last line to the logfile and closes it.
    */
    OutputHandler::~OutputHandler()
    {
        this->logfile_ << "Closed log" << std::endl;
        this->logfile_.close();
    }

    /**
        @brief Returns a reference to the only existing instance of the OutputHandler class.
        @return The instance
    */
    OutputHandler& OutputHandler::getOutStream()
    {
        static OutputHandler orxout("orxonox.log");
        return orxout;
    }

    /**
        @brief Sets the soft debug level for a given output device.
        @param device The output device
        @param level The debug level
    */
    void OutputHandler::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
    {
        OutputHandler::getOutStream().softDebugLevel_[(unsigned int)device] = level;
    }

    /**
        @brief Returns the soft debug level for a given output device.
        @param device The output device
        @return The debug level
    */
    int OutputHandler::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        return OutputHandler::getOutStream().softDebugLevel_[(unsigned int)device];
    }

    /**
        @brief Sets the OutputBuffer, representing the third output stream.
        @param buffer The OutputBuffer
    */
    void OutputHandler::setOutputBuffer(OutputBuffer& buffer)
    {
        buffer.getStream() >> this->outputBuffer_->getStream().rdbuf();
        this->outputBuffer_ = &buffer;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console and the logfile.
        @param sb The streambuffer that should be shown in the console
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::streambuf* sb)
    {
        if (getSoftDebugLevel(OutputHandler::LD_Console) >= this->outputLevel_)
            std::cout << sb;

        if (getSoftDebugLevel(OutputHandler::LD_Logfile) >= this->outputLevel_)
        {
            this->logfile_ << sb;
            this->logfile_.flush();
        }

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= this->outputLevel_)
            (*this->outputBuffer_) << sb;

        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console, the logfile and the ingame shell.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ostream& (*manipulator)(std::ostream&))
    {
        if (getSoftDebugLevel(OutputHandler::LD_Console) >= this->outputLevel_)
            manipulator(std::cout);

        if (getSoftDebugLevel(OutputHandler::LD_Logfile) >= this->outputLevel_)
        {
            manipulator(this->logfile_);
            this->logfile_.flush();
        }

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= this->outputLevel_)
            (*this->outputBuffer_) << manipulator;

        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console, the logfile and the ingame shell.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ios& (*manipulator)(std::ios&))
    {
        if (getSoftDebugLevel(OutputHandler::LD_Console) >= this->outputLevel_)
            manipulator(std::cout);

        if (getSoftDebugLevel(OutputHandler::LD_Logfile) >= this->outputLevel_)
        {
            manipulator(this->logfile_);
            this->logfile_.flush();
        }

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= this->outputLevel_)
            (*this->outputBuffer_) << manipulator;

        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console, the logfile and the ingame shell.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ios_base& (*manipulator)(std::ios_base&))
    {
        if (getSoftDebugLevel(OutputHandler::LD_Console) >= this->outputLevel_)
            manipulator(std::cout);

        if (getSoftDebugLevel(OutputHandler::LD_Logfile) >= this->outputLevel_)
        {
            manipulator(this->logfile_);
            this->logfile_.flush();
        }

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= this->outputLevel_)
            (*this->outputBuffer_) << manipulator;

        return *this;
    }
}
