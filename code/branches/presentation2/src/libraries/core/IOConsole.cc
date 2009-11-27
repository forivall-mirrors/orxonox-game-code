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

#include <iomanip>
#include <iostream>

#include "util/Math.h"
#include "core/Game.h"
#include "core/input/InputBuffer.h"

// ##########################
// ###   Mutual methods   ###
// ##########################
namespace orxonox
{
    IOConsole* IOConsole::singletonPtr_s = NULL;

    int IOConsole::extractLogLevel(std::string* text)
    {
        // Handle line colouring by inspecting the first letter
        char level = 0;
        if (!text->empty())
        {
            level = (*text)[0];
            if (level == -1 || (level >= 1 && level <= 6))
            {
                *text = text->substr(1);
                if (level != -1)
                    return level;
            }
        }
        return 0;
    }

    inline bool IOConsole::willPrintStatusLines()
    {
        return !this->statusLineWidths_.empty()
             && this->terminalWidth_  >= this->statusLineMaxWidth_
             && this->terminalHeight_ >= (this->minOutputLines_ + this->statusLineWidths_.size());
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    //! Called if all output-lines have to be reprinted
    void IOConsole::linesChanged()
    {
        // Method only gets called upon start to draw all the lines
        // or when scrolling. But scrolling is disabled and the output
        // is already in std::cout when we start the IOConsole
    }

    //! Called if the text in the input-line has changed
    void IOConsole::inputChanged()
    {
        this->printInputLine();
        this->cout_.flush();
    }

    //! Called if the position of the cursor in the input-line has changed
    void IOConsole::cursorChanged()
    {
        this->printInputLine();
        this->cout_.flush();
    }

    //! Called if a command is about to be executed
    void IOConsole::executed()
    {
        this->shell_->addOutputLine(this->promptString_ + this->shell_->getInput());
    }

    //! Called if the console gets closed
    void IOConsole::exit()
    {
        // Exit is not an option, just do nothing (Shell doesn't really exit too)
    }
}

#ifdef ORXONOX_PLATFORM_UNIX
// ###############################
// ###   Unix Implementation   ###
// ###############################

#include <termios.h>
#include <sys/ioctl.h>

namespace orxonox
{
    termios* IOConsole::originalTerminalSettings_s = 0;
    
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
        : shell_(new Shell("IOConsole", false, true))
        , buffer_(shell_->getInputBuffer())
        , cout_(std::cout.rdbuf())
        , bStatusPrinted_(false)
        , promptString_("orxonox # ")
    {
        this->setTerminalMode();
        this->shell_->registerListener(this);

        // Manually set the widths of the individual status lines
        this->statusLineWidths_.push_back(29);
        this->statusLineMaxWidth_ = 29;

        this->getTerminalSize();
        this->lastTerminalWidth_ = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;

        // Disable standard std::cout logging
        OutputHandler::getInstance().disableCout();
        // Redirect std::cout to an ostringstream
        // (Other part is in the initialiser list)
        std::cout.rdbuf(this->origCout_.rdbuf());

        // Make sure we make way for the status lines
        this->update(Game::getInstance().getGameClock());
    }

    IOConsole::~IOConsole()
    {
        // Empty all buffers
        this->update(Game::getInstance().getGameClock());
        // Erase input and status lines
        this->cout_ << "\033[1G\033[J";
        // Move cursor to the bottom
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'B';
        // Scroll terminal to compensate for erased lines
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'T';

        resetTerminalMode();
        this->shell_->destroy();

        // Restore this->cout_ redirection
        std::cout.rdbuf(this->cout_.rdbuf());
        // Enable standard std::cout logging again
        OutputHandler::getInstance().enableCout();
    }

    void IOConsole::update(const Clock& time)
    {
        unsigned char c;
        std::string escapeSequence;
        EscapeMode::Value escapeMode = EscapeMode::None;
        while (std::cin.good())
        {
            c = std::cin.get();
            if (!std::cin.good())
                break;

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
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::PageUp,   0, 0));
                else if (escapeSequence == "6~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::PageDown, 0, 0));
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
        // Reset error flags in std::cin
        std::cin.clear();

        // If there is still an escape key pending (escape key ONLY), then
        // it sure isn't an escape sequence anymore
        if (escapeMode == EscapeMode::First)
            this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape, '\033', 0));

        // Determine terminal width and height
        this->lastTerminalWidth_ = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;
        this->getTerminalSize();

        int heightDiff = this->terminalHeight_ - this->lastTerminalHeight_;
        if (this->bStatusPrinted_ && heightDiff < 0)
        {
            // Terminal width has shrunk. The cursor will still be on the input line,
            // but that line might very well be the last
            int newLines = std::min((int)this->statusLineWidths_.size(), -heightDiff);
            // Scroll terminal to create new lines
            this->cout_ << "\033[" << newLines << 'S';
        }

        if (!this->bStatusPrinted_ && this->willPrintStatusLines())
        {
            // Scroll console to make way for status lines
            this->cout_ << "\033[" << this->statusLineWidths_.size() << 'S';
            this->bStatusPrinted_ = true;
        }

        // We always assume that the cursor is on the inputline.
        // But we cannot always be sure about that, esp. if we scroll the console
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'B';
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'A';

        // Erase status and input lines
        this->cout_ << "\033[1G\033[J";
        this->printInputLine();
        this->printStatusLines();
        this->cout_.flush();

        // Process output written to std::cout
        if (!this->origCout_.str().empty())
        {
            this->shell_->addOutputLine(this->origCout_.str());
            this->origCout_.str("");
        }
    }

    void IOConsole::printLogText(const std::string& text)
    {
        std::string output = text;
        /*int level =*/ this->extractLogLevel(&output);

/*
        // Colour line
        switch (level)
        {
        case -1: this->cout_ << "\033[37m"; break;
        case  1: this->cout_ << "\033[91m"; break;
        case  2: this->cout_ << "\033[31m"; break;
        case  3: this->cout_ << "\033[34m"; break;
        case  4: this->cout_ << "\033[36m"; break;
        case  5: this->cout_ << "\033[35m"; break;
        case  6: this->cout_ << "\033[37m"; break;
        default: break;
        }
*/

        // Print output line
        this->cout_ << output;

        // Reset colour to white
//        this->cout_ << "\033[37m";
    }

    void IOConsole::printInputLine()
    {
        // Set cursor to the beginning of the line and erase the line
        this->cout_ << "\033[1G\033[K";
        // Indicate a command prompt
        this->cout_ << this->promptString_;
        // Save cursor position
        this->cout_ << "\033[s";
        // Print command line buffer
        this->cout_ << this->shell_->getInput();
        // Restore cursor position and move it to the right
        this->cout_ << "\033[u";
        if (this->buffer_->getCursorPosition() > 0)
            this->cout_ << "\033[" << this->buffer_->getCursorPosition() << "C";
    }

    void IOConsole::printStatusLines()
    {
        if (this->willPrintStatusLines())
        {
            // Save cursor position
            this->cout_ << "\033[s";
            // Move cursor down (don't create a new line here because the buffer might flush then!)
            this->cout_ << "\033[1B\033[1G";
            this->cout_ << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, ";
            this->cout_ <<               std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms tick time";
            // Restore cursor position
            this->cout_ << "\033[u";
            this->bStatusPrinted_ = true;
        }
        else
            this->bStatusPrinted_ = false;
    }

    void IOConsole::setTerminalMode()
    {
        termios new_settings;
        IOConsole::originalTerminalSettings_s = new termios();

        tcgetattr(0, this->originalTerminalSettings_s);
        new_settings = *this->originalTerminalSettings_s;
        new_settings.c_lflag &= ~(ICANON | ECHO);
        //new_settings.c_lflag |= (ISIG | IEXTEN);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN]  = 0;
        tcsetattr(0, TCSANOW, &new_settings);
        atexit(&IOConsole::resetTerminalMode);
    }

    /*static*/ void IOConsole::resetTerminalMode()
    {
        if(IOConsole::originalTerminalSettings_s)
        {
            tcsetattr(0, TCSANOW, IOConsole::originalTerminalSettings_s);
            delete IOConsole::originalTerminalSettings_s;
            IOConsole::originalTerminalSettings_s = 0;
        }
    }

    void IOConsole::getTerminalSize()
    {
#ifdef TIOCGSIZE
        struct ttysize win;
        if (!ioctl(STDIN_FILENO, TIOCGSIZE, &win))
        {
            this->terminalWidth_  = win.ts_cols;
            this->terminalHeight_ = win.ts_lines;
            return;
        }
#elif defined TIOCGWINSZ
        struct winsize win;
        if (!ioctl(STDIN_FILENO, TIOCGWINSZ, &win))
        {
            this->terminalWidth_  = win.ws_col;
            this->terminalHeight_ = win.ws_row;
            return;
        }
#else
        const char* s = getenv("COLUMNS");
        this->terminalWidth_  = s ? strtol(s, NULL, 10) : 80;
        s = getenv("LINES");
        this->terminalHeight_ = s ? strtol(s, NULL, 10) : 24;
        return;
#endif
        this->terminalWidth_  = 80;
        this->terminalHeight_ = 24;
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    //! Called if only the last output-line has changed
    void IOConsole::onlyLastLineChanged()
    {
        // Save cursor position and move it to the beginning of the first output line
        this->cout_ << "\033[s\033[1A\033[1G";
        // Erase the line
        this->cout_ << "\033[K";
        // Reprint the last output line
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        // Restore cursor
        this->cout_ << "\033[u";
        this->cout_.flush();
    }

    //! Called if a new output-line was added
    void IOConsole::lineAdded()
    {
        int newLines = this->shell_->getNewestLineIterator()->size() / this->terminalWidth_ + 1;
        // Create new lines by scrolling the screen
        this->cout_ << "\033[" << newLines << 'S';
        // Move cursor to the beginning of the new (last) output line
        this->cout_ << "\033[" << newLines << "A\033[1G";
        // Erase screen from here
        this->cout_ << "\033[J";
        // Print the new output lines
        for (int i = 0; i < newLines; ++i)
            this->printLogText(this->shell_->getNewestLineIterator()->substr(i*this->terminalWidth_, this->terminalWidth_));
        // Move cursor down
        this->cout_ << "\033[1B\033[1G";
        // Print status and input lines
        this->printInputLine();
        this->printStatusLines();
        this->cout_.flush();
    }
}

#elif defined(ORXONOX_PLATFORM_WINDOWS)
// ##################################
// ###   Windows Implementation   ###
// ##################################

#include <windows.h>

namespace orxonox
{
    IOConsole::IOConsole()
        : shell_(new Shell("IOConsole", false, true))
        , buffer_(shell_->getInputBuffer())
        , cout_(std::cout.rdbuf())
        , bStatusPrinted_(false)
        , promptString_("orxonox # ")
    {
        this->setTerminalMode();
        this->shell_->registerListener(this);

        // Manually set the widths of the individual status lines
        this->statusLineWidths_.push_back(29);
        this->statusLineMaxWidth_ = 29;

        this->getTerminalSize();
        this->lastTerminalWidth_ = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;

        // Disable standard this->cout_ logging
        OutputHandler::getInstance().disableCout();
        // Redirect std::cout to an ostringstream
        // (Other part is in the initialiser list)
        std::cout.rdbuf(this->origCout_.rdbuf());

        // Make sure we make way for the status lines
        this->update(Game::getInstance().getGameClock());
    }

    IOConsole::~IOConsole()
    {
        // Empty all buffers
        this->update(Game::getInstance().getGameClock());

        resetTerminalMode();
        this->shell_->destroy();

        // Restore this->cout_ redirection
        std::cout.rdbuf(this->cout_.rdbuf());
        // Enable standard this->cout_ logging again
        OutputHandler::getInstance().enableCout();
    }

    void IOConsole::update(const Clock& time)
    {
        while (true)
        {
            DWORD count;
            INPUT_RECORD inrec;
            PeekConsoleInput(this->stdInHandle_, &inrec, 1, &count);
            if (count == 0)
                break;
            ReadConsoleInput(this->stdInHandle_, &inrec, 1, &count);
            if (inrec.EventType == KEY_EVENT && inrec.Event.KeyEvent.bKeyDown)
            {
                // Process keyboard modifiers (Ctrl, Alt and Shift)
                DWORD modifiersIn = inrec.Event.KeyEvent.dwControlKeyState;
                int modifiersOut = 0;
                if ((modifiersIn & (LEFT_ALT_PRESSED  | RIGHT_ALT_PRESSED))  != 0)
                    modifiersOut |= KeyboardModifier::Alt;
                if ((modifiersIn & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0)
                    modifiersOut |= KeyboardModifier::Ctrl;
                if ((modifiersIn & SHIFT_PRESSED) != 0)
                    modifiersOut |= KeyboardModifier::Shift;

                // ASCII character (0 for special keys)
                char asciiChar = inrec.Event.KeyEvent.uChar.AsciiChar;

                // Process special keys and if not found, use Key::A as dummy (InputBuffer uses the ASCII text anyway)
                switch (inrec.Event.KeyEvent.wVirtualKeyCode)
                {
                case VK_BACK:   this->buffer_->buttonPressed(KeyEvent(KeyCode::Back,     asciiChar, modifiersOut)); break;
                case VK_TAB:    this->buffer_->buttonPressed(KeyEvent(KeyCode::Back,     asciiChar, modifiersOut)); break;
                case VK_RETURN: this->buffer_->buttonPressed(KeyEvent(KeyCode::Back,     asciiChar, modifiersOut)); break;
                case VK_PAUSE:  this->buffer_->buttonPressed(KeyEvent(KeyCode::Pause,    asciiChar, modifiersOut)); break;
                case VK_ESCAPE: this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape,   asciiChar, modifiersOut)); break;
                case VK_SPACE:  this->buffer_->buttonPressed(KeyEvent(KeyCode::Space,    asciiChar, modifiersOut)); break;
                case VK_PRIOR:  this->buffer_->buttonPressed(KeyEvent(KeyCode::PageUp,   asciiChar, modifiersOut)); break;
                case VK_NEXT:   this->buffer_->buttonPressed(KeyEvent(KeyCode::PageDown, asciiChar, modifiersOut)); break;
                case VK_END:    this->buffer_->buttonPressed(KeyEvent(KeyCode::End,      asciiChar, modifiersOut)); break;
                case VK_HOME:   this->buffer_->buttonPressed(KeyEvent(KeyCode::Home,     asciiChar, modifiersOut)); break;
                case VK_LEFT:   this->buffer_->buttonPressed(KeyEvent(KeyCode::Left,     asciiChar, modifiersOut)); break;
                case VK_UP:     this->buffer_->buttonPressed(KeyEvent(KeyCode::Up,       asciiChar, modifiersOut)); break;
                case VK_RIGHT:  this->buffer_->buttonPressed(KeyEvent(KeyCode::Right,    asciiChar, modifiersOut)); break;
                case VK_DOWN:   this->buffer_->buttonPressed(KeyEvent(KeyCode::Down,     asciiChar, modifiersOut)); break;
                case VK_INSERT: this->buffer_->buttonPressed(KeyEvent(KeyCode::Insert,   asciiChar, modifiersOut)); break;
                case VK_DELETE: this->buffer_->buttonPressed(KeyEvent(KeyCode::Delete,   asciiChar, modifiersOut)); break;
                default:        this->buffer_->buttonPressed(KeyEvent(KeyCode::A,        asciiChar, modifiersOut));
                }
            }
        }

        // Get info about cursor and terminal size
        this->getTerminalSize();

        // Refresh status line
        this->printStatusLines();

        // Process output written to std::cout
        if (!this->origCout_.str().empty())
        {
            this->shell_->addOutputLine(this->origCout_.str());
            this->origCout_.str("");
        }
        this->cout_.flush();
    }

    void IOConsole::printLogText(const std::string& text)
    {
        std::string output = text;
        int level = this->extractLogLevel(&output);

        // Colour line
        switch (level)
        {
        case  1: SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
        case  2: SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); break;
        case  3: SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_INTENSITY); break;
        case  4: SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_INTENSITY); break;
        default: break;
        }

        // Print output line
        this->cout_ << output;

        // Reset colour to white
        SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }

    void IOConsole::printInputLine()
    {
        SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        this->moveCursorYAndHome(0);
        this->clearCurrentLine();
        this->cout_ << this->promptString_ << this->shell_->getInput();
        this->moveCursorYAndHome(0);
        this->moveCursor(this->promptString_.size() + this->buffer_->getCursorPosition(), 0);
        SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }

    void IOConsole::printStatusLines()
    {
        if (this->willPrintStatusLines())
        {
            SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_GREEN);
            this->bStatusPrinted_ = true;
            // Put cursor on home position, one line down the input line
            this->moveCursorYAndHome(1);
            this->cout_ << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, ";
            this->cout_ <<               std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms tick time";
            // Clear rest of the line
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo(this->stdOutHandle_, &info);
            this->cout_ << std::string(info.dwSize.X - info.dwCursorPosition.X - 1, ' ');
            // Restore cursor position
            this->moveCursorYAndHome(-1);
            this->moveCursor(this->promptString_.size() + this->buffer_->getCursorPosition(), 0);
            SetConsoleTextAttribute(stdOutHandle_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        }
        else
            this->bStatusPrinted_ = false;
    }

    void IOConsole::setTerminalMode()
    {
        // Set the console mode to no-echo, raw input, and no window or mouse events
        this->stdOutHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
        this->stdInHandle_  = GetStdHandle(STD_INPUT_HANDLE);
        if (this->stdInHandle_ == INVALID_HANDLE_VALUE
            || !GetConsoleMode(this->stdInHandle_, &this->originalTerminalSettings_)
            || !SetConsoleMode(this->stdInHandle_, 0))
        {
            COUT(1) << "Error: Could not set Windows console settings" << std::endl;
            return;
        }
        FlushConsoleInputBuffer(this->stdInHandle_);
    }

    void IOConsole::resetTerminalMode()
    {
        SetConsoleMode(this->stdInHandle_, this->originalTerminalSettings_);
    }

    //! Moves the console cursor around and inserts new lines when reaching the end.
    //! Moving out on the right is just clamped though.
    void IOConsole::moveCursor(int dx, int dy)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &info);
        SHORT& x = info.dwCursorPosition.X;
        x = clamp(x + dx, 0, info.dwSize.X - 1);
        SHORT& y = info.dwCursorPosition.Y;
        if (y + dy >= info.dwSize.Y)
        {
            // Insert new lines
            this->cout_ << std::string(y + dy - info.dwSize.Y + 1, 'n');
            y = info.dwSize.Y - 1;
        }
        else if (y < 0)
            y = 0;
        else
            y += dy;
        SetConsoleCursorPosition(this->stdOutHandle_, info.dwCursorPosition);
    }

    void IOConsole::moveCursorYAndHome(int dy)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &info);
        this->moveCursor(-info.dwCursorPosition.X, dy);
    }

    void IOConsole::clearCurrentLine()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &info);
        info.dwCursorPosition.X = 0;
        DWORD count;
        FillConsoleOutputCharacter(this->stdOutHandle_, ' ', info.dwSize.X, info.dwCursorPosition, &count);;
    }

    void IOConsole::getTerminalSize()
    {
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &screenBufferInfo);
        // dwSize is the maximum size. If you resize you will simply see scroll bars.
        // And if you want to write outside these boundaries, you can't.
        this->terminalWidth_  = screenBufferInfo.dwSize.X;
        this->terminalHeight_ = screenBufferInfo.dwSize.Y;
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    //! Called if only the last output-line has changed
    void IOConsole::onlyLastLineChanged()
    {
        this->moveCursorYAndHome(-1);
        this->clearCurrentLine();
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        this->moveCursorYAndHome(1);
        this->moveCursor(this->promptString_.size() + this->shell_->getInput().size(), 0);
        this->cout_.flush();
    }

    //! Called if a new output-line was added
    void IOConsole::lineAdded()
    {
        // Move cursor to the beginning of the new (last) output line
        this->moveCursorYAndHome(0);
        // Print the new output lines
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        // Move cursor down
        this->moveCursorYAndHome(1);
        // Print status and input lines
        this->printInputLine();
        this->printStatusLines();
        this->cout_.flush();
    }
}

#endif /* ORXONOX_PLATFORM_UNIX */
