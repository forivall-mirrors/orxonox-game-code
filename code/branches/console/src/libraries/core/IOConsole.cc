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

#include "IOConsole.h"

#include <cstring>
#include <iomanip>
#include <iostream>

#include "util/Clock.h"
#include "util/Debug.h"
#include "util/Sleep.h"
#include "core/CommandExecutor.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/Shell.h"
#include "core/input/InputBuffer.h"

#ifdef ORXONOX_PLATFORM_UNIX
#include <termios.h>
#endif

namespace orxonox
{
    IOConsole* IOConsole::singletonPtr_s = NULL;
    const std::string promptString_g = "orxonox>";

#ifdef ORXONOX_PLATFORM_UNIX

    termios* IOConsole::originalTerminalSettings_ = new termios();

    namespace EscapeMode
    {
        enum Value
        {
            None,
            First,
            Second
        };
    }

    IOConsole::IOConsole()
        : shell_(new Shell("IOConsole", false))
        , buffer_(shell_->getInputBuffer())
        , bStatusPrinted_(false)
    {
        this->setTerminalMode();
        this->shell_->registerListener(this);

        // Manually set the widths of the individual status lines
        this->statusLineWidths_.push_back(6);
        this->statusLineMaxWidth_ = 6;
    }

    IOConsole::~IOConsole()
    {
        std::cout << "\033[0G\033[K";
        std::cout.flush();
        resetTerminalMode();
        delete this->originalTerminalSettings_;
        this->shell_->destroy();
    }

    void IOConsole::setTerminalMode()
    {
        termios new_settings;

        tcgetattr(0, this->originalTerminalSettings_);
        new_settings = *this->originalTerminalSettings_;
        new_settings.c_lflag &= ~(ICANON | ECHO);
        //         new_settings.c_lflag |= ( ISIG | IEXTEN );
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN]  = 0;
        tcsetattr(0, TCSANOW, &new_settings);
        COUT(0) << endl;
        //       atexit(&IOConsole::resetTerminalMode);
    }

    void IOConsole::resetTerminalMode()
    {
        tcsetattr(0, TCSANOW, IOConsole::originalTerminalSettings_);
    }

    void IOConsole::update(const Clock& time)
    {
        unsigned char c = 0;
        std::string escapeSequence;
        EscapeMode::Value escapeMode = EscapeMode::None;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (escapeMode == EscapeMode::First && (c == '[' || c=='O') )
                escapeMode = EscapeMode::Second;
            // Get Alt+Tab combination when switching applications
            else if (escapeMode == EscapeMode::First && c == '\t')
            {
                this->buffer_->buttonPressed(KeyEvent(KeyCode::Tab, '\t', KeyboardModifier::Alt));
                escapeMode = EscapeMode::None;
            }
            else if (escapeMode == EscapeMode::Second)
            {
                escapeSequence += c;
                escapeMode = EscapeMode::None;
                if      (escapeSequence == "A")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Up,       0, 0));
                else if (escapeSequence == "B")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Down,     0, 0));
                else if (escapeSequence == "C")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Right,    0, 0));
                else if (escapeSequence == "D")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Left,     0, 0));
                else if (escapeSequence == "1~" || escapeSequence == "H")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Home,     0, 0));
                else if (escapeSequence == "2~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Insert,   0, 0));
                else if (escapeSequence == "3~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Delete,   0, 0));
                else if (escapeSequence == "4~" || escapeSequence == "F")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::End,      0, 0));
                else if (escapeSequence == "5~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::AltPageUp,   0, 0));
                else if (escapeSequence == "6~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::AltPageDown, 0, 0));
                else
                    // Waiting for sequence to complete
                    // If the user presses ESC and then '[' or 'O' while the loop is not
                    // running (for instance while loading), the whole sequence gets dropped
                    escapeMode = EscapeMode::Second;
            }
            else // not in an escape sequence OR user might have pressed just ESC
            {
                if (escapeMode == EscapeMode::First)
                {
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape, c, 0));
                    escapeMode = EscapeMode::None;
                }
                if (c == '\033')
                {
                    escapeMode = EscapeMode::First;
                    escapeSequence.clear();
                }
                else
                {
                    KeyCode::ByEnum code;
                    switch (c)
                    {
                    case '\n'  : case '\r': code = KeyCode::Return; break;
                    case '\177': case '\b': code = KeyCode::Back;   break;
                    case '\t'             : code = KeyCode::Tab;    break;
                    default:
                        // We don't encode the key code (would be a very large switch)
                        // because the InputBuffer will only insert the text anyway
                        // Replacement character is simply KeyCode::A
                        code = KeyCode::A;
                    }
                    this->buffer_->buttonPressed(KeyEvent(code, c, 0));
                }
            }
        }

        // If there is still an escape key pending (escape key ONLY), then
        // it sure isn't an escape sequence anymore
        if (escapeMode == EscapeMode::First)
            this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape, '\033', 0));

        // Clear screen below the last output line by first moving the cursor to the beginning of the first status line
        std::cout << "\033[" << (this->bStatusPrinted_ ? this->statusLineWidths_.size() : 0) << "F\033[J";
        this->printStatusLines();
        this->printInputLine();
    }

    void IOConsole::printLogText(const std::string& text)
    {
        std::string output;

        // Handle line colouring by inspecting the first letter
        char level = 0;
        if (!text.empty())
            level = text[0];
        if (level >= -1 && level <= 6)
            output = text.substr(1);
        else
            output = text;

        // Colour line
/*
        switch (level)
        {
        case -1: std::cout << "\033[37m"; break;
        case  1: std::cout << "\033[91m"; break;
        case  2: std::cout << "\033[31m"; break;
        case  3: std::cout << "\033[34m"; break;
        case  4: std::cout << "\033[36m"; break;
        case  5: std::cout << "\033[35m"; break;
        case  6: std::cout << "\033[37m"; break;
        default: break;
        }
*/

        // Print output line
        std::cout << output;

        // Reset colour to white
//        std::cout << "\033[37m";
        std::cout.flush();
    }

    void IOConsole::printInputLine()
    {
        // Set cursor to the beginning of the line and erase the line
        std::cout << "\033[1G\033[K";
        // Indicate a command prompt
        std::cout << promptString_g;
        // Save cursor position
        std::cout << "\033[s";
        // Print command line buffer
        std::cout << this->shell_->getInput();
        // Restore cursor position and move it to the right
        std::cout << "\033[u";
        if (this->buffer_->getCursorPosition() > 0)
            std::cout << "\033[" << this->buffer_->getCursorPosition() << "C";
        std::cout.flush();
    }

    void IOConsole::printStatusLines()
    {
        if (!this->statusLineWidths_.empty())
        {
            // Check terminal size
            int x, y;
            if (this->getTerminalSize(&x, &y) && (x < (int)this->statusLineMaxWidth_ || y < (int)(this->minOutputLines_ + this->statusLineWidths_.size())))
            {
                this->bStatusPrinted_ = false;
                return;
            }
            std::cout << "Status" << std::endl;
            std::cout.flush();
            this->bStatusPrinted_ = true;
        }
    }

    int IOConsole::getTerminalSize(int* x, int* y)
    {
#ifdef TIOCGSIZE
        struct ttysize win;
#elif defined(TIOCGWINSZ)
        struct winsize win;
#endif

#ifdef TIOCGSIZE
        if (ioctl(STDIN_FILENO, TIOCGSIZE, &win))
            return 0;
        *y = win.ts_lines;
        *x = win.ts_cols;
#elif defined TIOCGWINSZ
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win))
            return 0;
        *y = win.ws_row;
        *x = win.ws_col;
#else
        {
            const char* s = getenv("LINES");
            if (s)
                *y = strtol(s, NULL, 10);
            else
                *y = 25;
            s = getenv("COLUMNS");
            if (s)
                *x = strtol(s, NULL, 10);
            else
                *x = 80;
        }
#endif
        return 1;
    }

#elif defined(ORXONOX_PLATFORM_WINDOWS)

    IOConsole::IOConsole()
        : shell_(new Shell("IOConsole", false))
        , buffer_(shell_->getInputBuffer())
    {
        this->setTerminalMode();
    }

    IOConsole::~IOConsole()
    {
    }

    void IOConsole::setTerminalMode()
    {
    }

    void IOConsole::resetTerminalMode()
    {
    }

    void IOConsole::update(const Clock& time)
    {
    }

    void IOConsole::printLogText(const std::string& text)
    {
    }

    void IOConsole::printInputLine()
    {
    }

#endif /* ORXONOX_PLATFORM_UNIX */

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    /**
    @brief
        Called if all output-lines have to be redrawn.
    */
    void IOConsole::linesChanged()
    {
        // Method only gets called upon start to draw all the lines
        // But we are using std::cout anyway, so do nothing here
    }

    /**
    @brief
        Called if only the last output-line has changed.
    */
    void IOConsole::onlyLastLineChanged()
    {
        // Save cursor position and move it to the beginning of the first output line
        std::cout << "\033[s\033[" << (1 + this->statusLineWidths_.size()) << "F";
        // Erase the line
        std::cout << "\033[K";
        // Reprint the last output line
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        // Restore cursor
        std::cout << "\033[u";
        std::cout.flush();
    }

    /**
    @brief
        Called if a new output-line was added.
    */
    void IOConsole::lineAdded()
    {
        // Move cursor to the beginning of the first status line and erase screen from there
        std::cout << "\033[" << this->statusLineWidths_.size() << "F\033[J";
        // Print the new output line
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        std::cout << std::endl;
        this->printStatusLines();
        this->printInputLine();
    }

    /**
    @brief
        Called if the text in the input-line has changed.
    */
    void IOConsole::inputChanged()
    {
        this->printInputLine();
    }

    /**
    @brief
        Called if the position of the cursor in the input-line has changed.
    */
    void IOConsole::cursorChanged()
    {
        this->printInputLine();
    }

    /**
    @brief
        Called if a command is about to be executed
    */
    void IOConsole::executed()
    {
        // Move cursor the beginning of the line
        std::cout << "\033[1G";
        // Print command so the user knows what he has typed
        std::cout << promptString_g << this->shell_->getInput() << std::endl;
        this->printInputLine();
    }


    /**
    @brief
        Called if the console gets closed.
    */
    void IOConsole::exit()
    {
        // Exit is not an option, IOConsole always exists
    }

}
