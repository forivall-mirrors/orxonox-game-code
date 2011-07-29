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

#ifndef _LogWriter_H__
#define _LogWriter_H__

#include "util/UtilPrereqs.h"

#include <fstream>

#include "BaseWriter.h"

namespace orxonox
{
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

            std::string filename_;
            std::string path_;
            bool bDefaultPath_;

            std::ofstream file_;
    };
}

#endif /* _LogWriter_H__ */
