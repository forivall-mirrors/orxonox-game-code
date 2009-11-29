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

#include <sstream>
#include <string>
#include <vector>
#include "util/Singleton.h"
#include "core/Shell.h"

#ifdef ORXONOX_PLATFORM_UNIX
struct termios;
#elif defined(ORXONOX_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
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
        void getTerminalSize();
        int extractLogLevel(std::string* text);

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
        std::ostream            cout_;
        std::ostringstream      origCout_;
        unsigned int            terminalWidth_;
        unsigned int            terminalHeight_;
        unsigned int            lastTerminalWidth_;
        unsigned int            lastTerminalHeight_;
        const std::string       promptString_;

#ifdef ORXONOX_PLATFORM_UNIX
        bool willPrintStatusLines();
        void printOutputLine(const std::string& line);
        void printInputLine();
        static void resetTerminalMode();

        bool                    bPrintStatusLine_;
        bool                    bStatusPrinted_;
        std::vector<unsigned>   statusLineWidths_;
        unsigned int            statusLineMaxWidth_;
        static const unsigned   minOutputLines_ = 3;
        termios*                originalTerminalSettings_;

#elif defined(ORXONOX_PLATFORM_WINDOWS)
        void resetTerminalMode();
        void moveCursor(int dx, int dy);
        void writeText(const std::string& text, const COORD& pos, WORD attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        void createNewOutputLines(unsigned int lines);
        void printOutputLine(const std::string& line, const COORD& pos);

        DWORD                   originalTerminalSettings_;
        HANDLE                  stdInHandle_;
        HANDLE                  stdOutHandle_;
        int                     inputLineRow_;
        unsigned int            inputLineHeight_;
        const unsigned int      statusLines_;
        unsigned int            lastOutputLineHeight_;
        uint64_t                lastRefreshTime_;
#endif

        static IOConsole* singletonPtr_s;
    };
}

#endif /* _IOConsole_H__ */
