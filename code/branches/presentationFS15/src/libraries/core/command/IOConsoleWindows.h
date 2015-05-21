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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @ingroup Command ShellConsole
*/

#ifndef _IOConsole_H__
#define _IOConsole_H__

#include "core/CorePrereqs.h"

#include <sstream>
#include <string>
#include "util/Singleton.h"
#include "Shell.h"

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace orxonox
{
    class _CoreExport IOConsole : public Singleton<IOConsole>, public ShellListener
    {
        friend class Singleton<IOConsole>;

    public:
        IOConsole();
        ~IOConsole();

        void preUpdate(const Clock& time);

    private:
        void setTerminalMode();
        void getTerminalSize();
        void printStatusLines();
        static int extractLogLevel(std::string* text);

        // Methods from ShellListener
        void linesChanged();
        void lineAdded();
        void inputChanged();
        void cursorChanged();
        void executed();
        void exit();

        void resetTerminalMode();
        void moveCursor(int dx, int dy);
        void writeText(const std::string& text, const COORD& pos, WORD attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        void createNewOutputLines(int lines);
        void printOutputLine(const std::string& line, Shell::LineType type, const COORD& pos);

        static inline COORD makeCOORD(SHORT x, SHORT y)
        {
            COORD val = {x, y};
            return val;
        }

        static inline SMALL_RECT makeSMALL_RECT(SHORT left, SHORT top, SHORT right, SHORT bottom)
        {
            SMALL_RECT val = {left, top, right, bottom};
            return val;
        }

        Shell*                  shell_;
        InputBuffer*            buffer_;
        std::ostream            cout_;
        std::ostringstream      origCout_;
        int                     terminalWidth_;
        int                     terminalHeight_;
        int                     lastTerminalWidth_;
        int                     lastTerminalHeight_;
        const std::string       promptString_;

        DWORD                   originalTerminalSettings_;
        HANDLE                  stdInHandle_;
        HANDLE                  stdOutHandle_;
        int                     inputLineRow_;
        int                     inputLineHeight_;
        const int               statusLines_;
        int                     lastOutputLineHeight_;
        uint64_t                lastRefreshTime_;

        static IOConsole* singletonPtr_s;
    };
}

#endif /* _IOConsole_H__ */
