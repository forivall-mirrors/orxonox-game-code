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

#include "util/Clock.h"
#include "util/Math.h"
#include "core/Game.h"
#include "core/input/InputBuffer.h"

// ##########################
// ###   Mutual methods   ###
// ##########################
namespace orxonox
{
    IOConsole* IOConsole::singletonPtr_s = NULL;

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

    //! Called if a command is about to be executed
    void IOConsole::executed()
    {
        this->shell_->addOutput(this->promptString_ + this->shell_->getInput() + '\n', Shell::Command);
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
        , cout_(std::cout.rdbuf())
        , promptString_("orxonox # ")
        , bStatusPrinted_(false)
        , originalTerminalSettings_(0)
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
        this->preUpdate(Game::getInstance().getGameClock());
    }

    IOConsole::~IOConsole()
    {
        // Process output written to std::cout in the meantime
        std::cout.flush();
        if (!this->origCout_.str().empty())
            this->shell_->addOutput(this->origCout_.str(), Shell::None);
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

    void IOConsole::preUpdate(const Clock& time)
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

        // We always assume that the cursor is on the input line.
        // But we cannot always be sure about that, esp. if we scroll the console
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'B';
        this->cout_ << "\033[" << this->statusLineWidths_.size() << 'A';

        // Erase status and input lines
        this->cout_ << "\033[1G\033[J";
        this->printInputLine();
        this->printStatusLines();
        this->cout_.flush();

        // Process output written to std::cout
        std::cout.flush();
        if (!this->origCout_.str().empty())
        {
            this->shell_->addOutput(this->origCout_.str(), Shell::None);
            this->origCout_.str("");
        }
    }

    void IOConsole::printOutputLine(const std::string& text, Shell::LineType type)
    {
/*
        // Colour line
        switch (type)
        {
        case Shell::None:    this->cout_ << "\033[37m"; break;
        case Shell::Error:   this->cout_ << "\033[91m"; break;
        case Shell::Warning: this->cout_ << "\033[31m"; break;
        case Shell::Info:    this->cout_ << "\033[34m"; break;
        case Shell::Debug:   this->cout_ << "\033[36m"; break;
        case Shell::Verbose: this->cout_ << "\033[35m"; break;
        case Shell::Ultra:   this->cout_ << "\033[37m"; break;
        default: break;
        }
*/

        // Print output line
        this->cout_ << text;

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
            this->cout_ << "\033[" << this->buffer_->getCursorPosition() << 'C';
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
        this->originalTerminalSettings_ = new termios();

        tcgetattr(0, this->originalTerminalSettings_);
        new_settings = *this->originalTerminalSettings_;
        new_settings.c_lflag &= ~(ICANON | ECHO);
        //new_settings.c_lflag |= (ISIG | IEXTEN);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN]  = 0;
        tcsetattr(0, TCSANOW, &new_settings);
        atexit(&IOConsole::resetTerminalMode);
    }

    /*static*/ void IOConsole::resetTerminalMode()
    {
        if (IOConsole::singletonPtr_s && IOConsole::singletonPtr_s->originalTerminalSettings_)
        {
            tcsetattr(0, TCSANOW, IOConsole::singletonPtr_s->originalTerminalSettings_);
            delete IOConsole::singletonPtr_s->originalTerminalSettings_;
            IOConsole::singletonPtr_s->originalTerminalSettings_ = 0;
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

    inline bool IOConsole::willPrintStatusLines()
    {
        return !this->statusLineWidths_.empty()
             && this->terminalWidth_  >= this->statusLineMaxWidth_
             && this->terminalHeight_ >= this->minOutputLines_ + (int)this->statusLineWidths_.size();
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
        this->printOutputLine(this->shell_->getNewestLineIterator()->first, this->shell_->getNewestLineIterator()->second);
        // Restore cursor
        this->cout_ << "\033[u";
        this->cout_.flush();
    }

    //! Called if a new output-line was added
    void IOConsole::lineAdded()
    {
        int newLines = this->shell_->getNewestLineIterator()->first.size() / this->terminalWidth_ + 1;
        // Create new lines by scrolling the screen
        this->cout_ << "\033[" << newLines << 'S';
        // Move cursor to the beginning of the new (last) output line
        this->cout_ << "\033[" << newLines << "A\033[1G";
        // Erase screen from here
        this->cout_ << "\033[J";
        // Print the new output lines
        for (int i = 0; i < newLines; ++i)
        {
            Shell::LineList::const_iterator it = this->shell_->getNewestLineIterator();
            this->printOutputLine(it->first.substr(i*this->terminalWidth_, this->terminalWidth_), it->second);
        }
        // Move cursor down
        this->cout_ << "\033[1B\033[1G";
        // Print status and input lines
        this->printInputLine();
        this->printStatusLines();
        this->cout_.flush();
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
}

#elif defined(ORXONOX_PLATFORM_WINDOWS)
// ##################################
// ###   Windows Implementation   ###
// ##################################

#include <windows.h>

namespace orxonox
{
    //! Redirects std::cout, creates the corresponding Shell and changes the terminal mode
    IOConsole::IOConsole()
        : shell_(new Shell("IOConsole", false))
        , buffer_(shell_->getInputBuffer())
        , cout_(std::cout.rdbuf())
        , promptString_("orxonox # ")
        , inputLineHeight_(1)
        , statusLines_(1)
        , lastOutputLineHeight_(0)
    {
        // Disable standard this->cout_ logging
        OutputHandler::getInstance().disableCout();
        // Redirect std::cout to an ostringstream
        // (Other part is in the initialiser list)
        std::cout.rdbuf(this->origCout_.rdbuf());

        this->setTerminalMode();
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &screenBufferInfo);
        this->terminalWidth_  = screenBufferInfo.dwSize.X;
        this->terminalHeight_ = screenBufferInfo.dwSize.Y;
        // Determines where we are in respect to output already written with std::cout
        this->inputLineRow_ = screenBufferInfo.dwCursorPosition.Y;
/*
        this->lastTerminalWidth_  = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;
*/

        // Cursor already at the end of the screen buffer?
        // (assuming the current input line height is 1)
        if (this->inputLineRow_ >= this->terminalHeight_ - this->statusLines_)
            SetConsoleCursorPosition(this->stdOutHandle_, makeCOORD(0, this->terminalHeight_ - this->statusLines_));

        // Prevent input line from overflowing
        int maxInputLength = (this->terminalHeight_ - this->statusLines_) * this->terminalWidth_ - 1 - this->promptString_.size();
        // Consider that the echo of a command might include the command plus some other characters (assumed max 80)
        // Also put a minimum so the config file parser is not overwhelmed with the command history
        this->buffer_->setMaxLength(std::min(8192, (maxInputLength - 80) / 2));

        // Print input and status line and position cursor
        this->inputChanged();
        this->cursorChanged();
        this->lastRefreshTime_ = Game::getInstance().getGameClock().getRealMicroseconds();
        this->preUpdate(Game::getInstance().getGameClock());

        this->shell_->registerListener(this);
    }

    //! Resets std::cout redirection and restores the terminal mode
    IOConsole::~IOConsole()
    {
        // Process output written to std::cout in the meantime
        std::cout.flush();
        if (!this->origCout_.str().empty())
            this->shell_->addOutput(this->origCout_.str(), Shell::None);

        this->shell_->unregisterListener(this);

        // Erase input and status lines
        COORD pos = {0, this->inputLineRow_};
        this->writeText(std::string((this->inputLineHeight_ + this->statusLines_) * this->terminalWidth_, ' '), pos);
        // Move cursor to the beginning of the line
        SetConsoleCursorPosition(stdOutHandle_, pos);

        // Restore this->cout_ redirection
        std::cout.rdbuf(this->cout_.rdbuf());
        // Enable standard this->cout_ logging again
        OutputHandler::getInstance().enableCout();

        resetTerminalMode();
        this->shell_->destroy();
    }

    //! Processes the pending input key strokes, refreshes the status lines and handles std::cout (redirected)
    void IOConsole::preUpdate(const Clock& time)
    {
        // Process input
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
                case VK_TAB:    this->buffer_->buttonPressed(KeyEvent(KeyCode::Tab,      asciiChar, modifiersOut)); break;
                case VK_RETURN: this->buffer_->buttonPressed(KeyEvent(KeyCode::Return,   asciiChar, modifiersOut)); break;
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

        // TODO: Respect screen buffer size changes
/*
        // The user can manually adjust the screen buffer size on Windows
        // And we don't want to screw the console because of that
        this->lastTerminalWidth_ = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;
        this->getTerminalSize(); // Also sets this->inputLineRow_ according to the cursor position
        // Is there still enough space below the cursor for the status line(s)?
        if (this->inputLineRow_ >= this->terminalHeight_ - this->statusLines_)
            this->moveCursor(0, -this->inputLineRow_ + this->terminalHeight_ - this->statusLines_ - 1);
*/

        // Refresh status line 5 times per second
        if (time.getMicroseconds() > this->lastRefreshTime_ + 1000000)
        {
            this->printStatusLines();
            this->lastRefreshTime_ = time.getMicroseconds();
        }

        // Process output written to std::cout
        std::cout.flush();
        if (!this->origCout_.str().empty())
        {
            this->shell_->addOutput(this->origCout_.str(), Shell::None);
            this->origCout_.str("");
        }
    }

    //! Prints output text. Similar to writeText, but sets the colour according to the output level
    void IOConsole::printOutputLine(const std::string& text, Shell::LineType type, const COORD& pos)
    {
        // Colour line
        WORD colour = 0;
        switch (type)
        {
        case Shell::Error:   colour = FOREGROUND_INTENSITY                    | FOREGROUND_RED; break;
        case Shell::Warning: colour = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED; break;
        case Shell::Info:
        case Shell::Debug:
        case Shell::Verbose:
        case Shell::Ultra:   colour = FOREGROUND_INTENSITY                                     ; break;
        case Shell::Command: colour =                        FOREGROUND_GREEN                  | FOREGROUND_BLUE; break;
        case Shell::Hint:    colour =                        FOREGROUND_GREEN | FOREGROUND_RED                  ; break;
        default:             colour =                        FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE; break;
        }

        // Print output line
        this->writeText(text, pos, colour);
    }

    //! Prints all status lines with current content
    void IOConsole::printStatusLines()
    {
        // Prepare text to be written
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, ";
        oss <<               std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms tick time";
        // Clear rest of the line by inserting spaces
        oss << std::string(this->terminalWidth_ - oss.str().size(), ' ');
        this->writeText(oss.str(), makeCOORD(0, this->inputLineRow_ + this->inputLineHeight_), FOREGROUND_GREEN);
    }

    //! Changes the console parameters for unbuffered input
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

    //! Restores the console parameters
    void IOConsole::resetTerminalMode()
    {
        SetConsoleMode(this->stdInHandle_, this->originalTerminalSettings_);
    }

    //! Sets this->terminalWidth_ and this->terminalHeight_
    void IOConsole::getTerminalSize()
    {
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        GetConsoleScreenBufferInfo(this->stdOutHandle_, &screenBufferInfo);
        this->terminalWidth_  = screenBufferInfo.dwSize.X;
        this->terminalHeight_ = screenBufferInfo.dwSize.Y;
    }

    //! Writes arbitrary text to the console with a certain colour and screen buffer position
    void IOConsole::writeText(const std::string& text, const COORD& coord, WORD attributes)
    {
        DWORD count;
        WriteConsoleOutputCharacter(stdOutHandle_, text.c_str(), text.size(), coord, &count);
        FillConsoleOutputAttribute(stdOutHandle_, attributes, text.size(), coord, &count);
    }

    /** Scrolls the console screen buffer to create empty lines above the input line.
    @details
        If the input and status lines are already at the bottom of the screen buffer
        the whole output gets scrolled up. In the other case the input and status
        lines get scrolled down.
        In any case the status and input lines get scrolled down as far as possible.
    @param lines
        Number of lines to be inserted. Behavior for negative values is undefined.
    */
    void IOConsole::createNewOutputLines(int lines)
    {
        CHAR_INFO fillChar = {{' '}, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED};
        // Lines to scroll input/status down (if possible)
        int linesDown = clamp(terminalHeight_ - inputLineRow_ - inputLineHeight_ - statusLines_, 0, lines);
        if (linesDown > 0)
        {
            // Scroll input and status lines down
            SMALL_RECT oldRect = {0, this->inputLineRow_,
                this->terminalWidth_ - 1, this->inputLineRow_ + this->inputLineHeight_ + this->statusLines_ - 1};
            this->inputLineRow_ += linesDown;
            ScrollConsoleScreenBuffer(stdOutHandle_, &oldRect, NULL, makeCOORD(0, this->inputLineRow_), &fillChar);
            // Move cursor down to the new bottom so the user can see the status lines
            COORD pos = {0, this->inputLineRow_ + this->inputLineHeight_ - 1 + this->statusLines_};
            SetConsoleCursorPosition(stdOutHandle_, pos);
            // Get cursor back to the right position
            this->cursorChanged();
        }
        // Check how many lines we still have to scroll up the output
        if (lines - linesDown > 0)
        {
            // Scroll output up
            SMALL_RECT oldRect = {0, lines - linesDown, this->terminalWidth_ - 1, this->inputLineRow_ - 1};
            ScrollConsoleScreenBuffer(stdOutHandle_, &oldRect, NULL, makeCOORD(0, 0), &fillChar);
        }
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    //! Called if the text in the input line has changed
    void IOConsole::inputChanged()
    {
        int newInputLineLength = this->promptString_.size() + this->shell_->getInput().size();
        int newInputLineHeight = 1 + newInputLineLength / this->terminalWidth_;
        int newLines = newInputLineHeight - this->inputLineHeight_;
        if (newLines > 0)
        {
            // Abuse this function to scroll the console
            this->createNewOutputLines(newLines);
            // Either Compensate for side effects (input/status lines scrolled down)
            // or we have to do this anyway (output scrolled up)
            this->inputLineRow_ -= newLines;
        }
        else if (newLines < 0)
        {
            // Scroll status lines up
            int statusLineRow = this->inputLineRow_ + this->inputLineHeight_;
            SMALL_RECT oldRect = {0, statusLineRow, this->terminalWidth_ - 1, statusLineRow + this->statusLines_};
            CHAR_INFO fillChar = {{' '}, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED};
            ScrollConsoleScreenBuffer(stdOutHandle_, &oldRect, NULL, makeCOORD(0, statusLineRow + newLines), &fillChar);
            // Clear potential leftovers
            if (-newLines - this->statusLines_ > 0)
            {
                COORD pos = {0, this->inputLineRow_ + newInputLineHeight + this->statusLines_};
                this->writeText(std::string((-newLines - this->statusLines_) * this->terminalWidth_, ' '), pos);
            }
        }
        this->inputLineHeight_ = newInputLineHeight;

        // Print the whole line, including spaces that erase leftovers
        std::string inputLine = this->promptString_ + this->shell_->getInput();
        inputLine += std::string(this->terminalWidth_ - newInputLineLength % this->terminalWidth_, ' ');
        this->writeText(inputLine, makeCOORD(0, this->inputLineRow_), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        // If necessary, move cursor
        if (newLines != 0)
            this->cursorChanged();
    }

    //! Called if the position of the cursor in the input-line has changed
    void IOConsole::cursorChanged()
    {
        int rawCursorPos = this->promptString_.size() + this->buffer_->getCursorPosition();
        // Compensate for cursor further to the right than the terminal width
        COORD pos;
        pos.X = rawCursorPos % this->terminalWidth_;
        pos.Y = this->inputLineRow_ + rawCursorPos / this->terminalWidth_;
        SetConsoleCursorPosition(stdOutHandle_, pos);
    }

    //! Called if only the last output-line has changed
    void IOConsole::onlyLastLineChanged()
    {
        int newLineHeight = 1 + this->shell_->getNewestLineIterator()->first.size() / this->terminalWidth_;
        // Compute the number of new lines needed
        int newLines = newLineHeight - this->lastOutputLineHeight_;
        this->lastOutputLineHeight_ = newLineHeight;
        // Scroll console if necessary
        if (newLines > 0) // newLines < 0 is assumed impossible
            this->createNewOutputLines(newLines);
        Shell::LineList::const_iterator it = this->shell_->getNewestLineIterator();
        this->printOutputLine(it->first, it->second, makeCOORD(0, this->inputLineRow_ - newLineHeight));
    }

    //! Called if a new output line was added
    void IOConsole::lineAdded()
    {
        Shell::LineList::const_iterator it = this->shell_->getNewestLineIterator();
        // Scroll console
        this->lastOutputLineHeight_ = 1 + it->first.size() / this->terminalWidth_;
        this->createNewOutputLines(this->lastOutputLineHeight_);
        // Write the text
        COORD pos = {0, this->inputLineRow_ - this->lastOutputLineHeight_};
        this->printOutputLine(it->first, it->second, pos);
    }
}

#endif /* ORXONOX_PLATFORM_UNIX */
