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

#include "LogWriter.h"

#include <ctime>

#include "OutputManager.h"
#include "MemoryWriter.h"

namespace orxonox
{
    LogWriter::LogWriter() : BaseWriter("Log")
    {
        this->setLevelMax(level::internal_info);

        this->filename_ = "orxonox.log";

        // Get path for a temporary file
#ifdef ORXONOX_PLATFORM_WINDOWS
        this->path_ = getenv("TEMP");
#else
        this->path_ = "/tmp";
#endif
        this->bDefaultPath_ = true;

        this->openFile();
    }

    LogWriter::~LogWriter()
    {
        this->closeFile();
    }

    /*static*/ LogWriter& LogWriter::getInstance()
    {
        static LogWriter instance;
        return instance;
    }

    void LogWriter::openFile()
    {
        std::string name = this->path_ + '/' + this->filename_;

        if (this->bDefaultPath_)
            OutputManager::getInstance().pushMessage(level::user_info, context::output(), "Opening log file " + name);

        this->file_.open(name.c_str(), std::fstream::out);

        if (this->file_.is_open())
            this->printLine("Log file opened", level::none);
        else
            OutputManager::getInstance().pushMessage(level::user_warning, context::output(), "Failed to open log file. File logging disabled.");
    }

    void LogWriter::closeFile()
    {
        if (this->file_.is_open())
        {
            this->printLine("Log file closed", level::none);
            this->file_.close();
        }
    }

    void LogWriter::setLogPath(const std::string& path)
    {
        OutputManager::getInstance().pushMessage(level::internal_info, context::output(), "Migrating log file from " + this->path_ + "\nto " + path);

        this->closeFile();
        this->path_ = path;
        this->bDefaultPath_ = false;
        this->openFile();

        MemoryWriter::getInstance().resendOutput(this);
    }

    void LogWriter::printLine(const std::string& line, OutputLevel)
    {
        if (!this->file_.is_open())
            return;

        // Get current time
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // print timestamp and line to the log file
        this->file_ << (timeinfo->tm_hour < 10 ? "0" : "") << timeinfo->tm_hour << ':' <<
                       (timeinfo->tm_min  < 10 ? "0" : "") << timeinfo->tm_min  << ':' <<
                       (timeinfo->tm_sec  < 10 ? "0" : "") << timeinfo->tm_sec  << ' ' << line << std::endl;
    }
}
