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
    @ingroup Output
    @brief Declaration of the LogWriter singleton which writes output to a log-file.
*/

#ifndef _LogWriter_H__
#define _LogWriter_H__

#include "util/UtilPrereqs.h"

#include <fstream>

#include "BaseWriter.h"

namespace orxonox
{
    /**
        @brief The LogWriter class inherits from BaseWriter and writes output to a log-file.

        The path of the file can be changed, in which case the file
        is rewritten by using the output stored by MemoryWriter. This adds the
        possibility to change the desired output levels before changing the
        path in order to get the complete output with the new output levels
        at the new path.
    */
    class _UtilExport LogWriter : public BaseWriter
    {
        public:
            LogWriter();
            LogWriter(const LogWriter&);
            virtual ~LogWriter();

            void setLogDirectory(const std::string& directory);

            /** @brief Returns the path to the logfile. */
            inline std::string getPath() const
                { return this->directory_ + '/' + this->filename_; }
            /** @brief Returns the open file stream. */
            inline const std::ofstream& getFile() const
                { return this->file_; }

        protected:
            virtual void printLine(const std::string& line, OutputLevel level);

        private:
            void openFile();
            void closeFile();

            std::string filename_;  ///< The name of the log-file (without directory)
            std::string directory_; ///< The directory where the log-file resided (without file-name)
            std::ofstream file_;    ///< The output file stream.
    };
}

#endif /* _LogWriter_H__ */
