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
 *      Oliver Scheuss
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _IOConsole_H__
#define _IOConsole_H__

#include "CorePrereqs.h"

#include <string>
#include <vector>
#include "util/Singleton.h"
#include "core/Shell.h"

#ifdef ORXONOX_PLATFORM_UNIX
struct termios;
#endif

namespace orxonox
{
    class _CoreExport IOConsole : public Singleton<IOConsole>, public ShellListener
    {
        friend class Singleton<IOConsole>;

    public:
        IOConsole();
        ~IOConsole();

        void update(const Clock& time);

    private:
        void setTerminalMode();
        void resetTerminalMode();
        void getTerminalSize();
        bool willPrintStatusLines();
        int extractLogLevel(std::string* text);

        void printLogText(const std::string& line);
        void printInputLine();
        void printStatusLines();

        // Methods from ShellListener
        void linesChanged();
        void onlyLastLineChanged();
        void lineAdded();
        void inputChanged();
        void cursorChanged();
        void executed();
        void exit();
        Shell*                  shell_;
        InputBuffer*            buffer_;
        unsigned int            terminalWidth_;
        unsigned int            terminalHeight_;
        unsigned int            lastTerminalWidth_;
        unsigned int            lastTerminalHeight_;
        bool                    bPrintStatusLine_;
        bool                    bStatusPrinted_;
        std::vector<unsigned>   statusLineWidths_;
        unsigned int            statusLineMaxWidth_;
        const std::string       promptString_;
        static const unsigned   minOutputLines_ = 3;

#ifdef ORXONOX_PLATFORM_UNIX
        termios*                originalTerminalSettings_;
#endif

        static IOConsole* singletonPtr_s;
    };
}

#endif /* _IOConsole_H__ */
