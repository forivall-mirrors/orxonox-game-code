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

        It is implemented as singleton because we (currently) use only one
        log-file. The path of the file can be changed, in which case the file
        is rewritten by using the output stored by MemoryWriter. This adds the
        possibility to change the desired output levels before changing the
        path in order to get the complete output with the new output levels
        at the new path.
    */
    class _UtilExport LogWriter : public BaseWriter
    {
        public:
            static LogWriter& getInstance();

            void setLogPath(const std::string& path);

        protected:
            virtual void printLine(const std::string& line, OutputLevel level);

        private:
            LogWriter();
            LogWriter(const LogWriter&);
            virtual ~LogWriter();

            void openFile();
            void closeFile();

            std::string filename_;  ///< The name of the log-file (without directories)
            std::string path_;      ///< The path of the log-file (without file-name)
            bool bDefaultPath_;     ///< If true, the log-file resides at the default path (which is usually a temporary directory)

            std::ofstream file_;    ///< The output file stream.
    };
}

#endif /* _LogWriter_H__ */
