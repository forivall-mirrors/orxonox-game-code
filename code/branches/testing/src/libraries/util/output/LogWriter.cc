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
    @brief Implementation of the LogWriter singleton.
*/

#include "LogWriter.h"

#include <ctime>
#include <cstdlib>

#include "OutputManager.h"
#include "MemoryWriter.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes the desired output levels and the name and path of the log-file, and opens the log-file.

        By default, LogWriter receives all output up to level::internal_info.
        The log-file has a default name which usually doesn't change. The path
        is initialized with a temporary directory, depending on the system,
        and can be changed later.
    */
    LogWriter::LogWriter() : BaseWriter("Log")
    {
        this->setLevelMax(level::internal_info);

        this->filename_ = "orxonox.log";

        // get the path for a temporary file, depending on the system
#ifdef ORXONOX_PLATFORM_WINDOWS
        this->path_ = getenv("TEMP");
#else
        this->path_ = "/tmp";
#endif
        this->bDefaultPath_ = true;

        this->openFile();
    }

    /**
        @brief Destructor, closes the log-file.
    */
    LogWriter::~LogWriter()
    {
        this->closeFile();
    }

    /**
        @brief Opens the log-file in order to write output to it.
    */
    void LogWriter::openFile()
    {
        // get the full file-name
        std::string name = this->path_ + '/' + this->filename_;

        // if we open the log file in the default directory, send a message to the user so that he can find the file in the case of a crash.
        if (this->bDefaultPath_)
            OutputManager::getInstance().pushMessage(level::user_info, context::undefined(), "Opening log file " + name);

        // open the file
        this->file_.open(name.c_str(), std::fstream::out);

        // check if it worked and print some output
        if (this->file_.is_open())
            this->printLine("Log file opened", level::none);
        else
            OutputManager::getInstance().pushMessage(level::user_warning, context::undefined(), "Failed to open log file. File logging disabled.");
    }

    /**
        @brief Closes the log-file.
    */
    void LogWriter::closeFile()
    {
        if (this->file_.is_open())
        {
            this->printLine("Log file closed", level::none);
            this->file_.close();
        }
    }

    /**
        @brief Changes the path of the log-file. Re-writes the log-file by using MemoryWriter.
    */
    void LogWriter::setLogPath(const std::string& path)
    {
        // notify about the change of the log-file (because the old file will no longer be updated)
        OutputManager::getInstance().pushMessage(level::internal_info, context::undefined(), "Migrating log file from " + this->path_ + "\nto " + path);

        // close the old file, update the path and open the new file
        this->closeFile();
        this->path_ = path;
        this->bDefaultPath_ = false;
        this->openFile();

        // request old output from MemoryWriter
        OutputManager::getInstance().getMemoryWriter().resendOutput(this);
    }

    /**
        @brief Inherited function from BaseWriter, writers output together with a timestamp to the log-file.
    */
    void LogWriter::printLine(const std::string& line, OutputLevel)
    {
        if (!this->file_.is_open())
            return;

        // get the current time
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // print timestamp and output line to the log file
        this->file_ << (timeinfo->tm_hour < 10 ? "0" : "") << timeinfo->tm_hour << ':' <<
                       (timeinfo->tm_min  < 10 ? "0" : "") << timeinfo->tm_min  << ':' <<
                       (timeinfo->tm_sec  < 10 ? "0" : "") << timeinfo->tm_sec  << ' ' << line << std::endl;
    }
}
