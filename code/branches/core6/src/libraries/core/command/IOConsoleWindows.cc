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

#include "IOConsole.h"

#include <iomanip>
#include <iostream>

#include "util/Clock.h"
#include "util/Math.h"
#include "util/output/ConsoleWriter.h"
#include "util/output/OutputManager.h"
#include "core/Game.h"
#include "core/input/InputBuffer.h"

namespace orxonox
{
    IOConsole* IOConsole::singletonPtr_s = NULL;

    //! Redirects std::cout, creates the corresponding Shell and changes the terminal mode
    IOConsole::IOConsole()
        : shell_(new Shell("Console", false))
        , buffer_(shell_->getInputBuffer())
        , cout_(std::cout.rdbuf())
        , promptString_("orxonox # ")
        , inputLineHeight_(1)
        , statusLines_(1)
        , lastOutputLineHeight_(0)
    {
        // Disable standard this->cout_ logging
        OutputManager::getInstance().getConsoleWriter()->disable();
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
            this->shell_->addOutput(this->origCout_.str(), Shell::Cout);

        this->shell_->unregisterListener(this);

        // Erase input and status lines
        COORD pos = {0, this->inputLineRow_};
        this->writeText(std::string((this->inputLineHeight_ + this->statusLines_) * this->terminalWidth_, ' '), pos);
        // Move cursor to the beginning of the line
        SetConsoleCursorPosition(stdOutHandle_, pos);

        // Restore this->cout_ redirection
        std::cout.rdbuf(this->cout_.rdbuf());
        // Enable standard this->cout_ logging again
        OutputManager::getInstance().getConsoleWriter()->enable();

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
            this->shell_->addOutput(this->origCout_.str(), Shell::Cout);
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
            case Shell::Message:
            case Shell::DebugOutput:     colour = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;

            case Shell::UserError:       colour = FOREGROUND_INTENSITY | FOREGROUND_RED | 0                | 0              ; break;
            case Shell::UserWarning:     colour = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | 0              ; break;
            case Shell::UserStatus:      colour = FOREGROUND_INTENSITY | 0              | FOREGROUND_GREEN | 0              ; break;
            case Shell::UserInfo:        colour = FOREGROUND_INTENSITY | 0              | FOREGROUND_GREEN | FOREGROUND_BLUE; break;

            case Shell::InternalError:   colour = 0                    | FOREGROUND_RED | 0                | 0              ; break;
            case Shell::InternalWarning: colour = 0                    | FOREGROUND_RED | FOREGROUND_GREEN | 0              ; break;
            case Shell::InternalStatus:  colour = 0                    | 0              | FOREGROUND_GREEN | 0              ; break;
            case Shell::InternalInfo:    colour = 0                    | 0              | FOREGROUND_GREEN | FOREGROUND_BLUE; break;

            case Shell::Verbose:         colour = FOREGROUND_INTENSITY | 0              | 0                | FOREGROUND_BLUE; break;
            case Shell::VerboseMore:     colour = FOREGROUND_INTENSITY | 0              | 0                | FOREGROUND_BLUE; break;
            case Shell::VerboseUltra:    colour = FOREGROUND_INTENSITY | 0              | 0                | FOREGROUND_BLUE; break;

            case Shell::Command:         colour = FOREGROUND_INTENSITY | FOREGROUND_RED | 0                | FOREGROUND_BLUE; break;
            case Shell::Hint:            colour = 0                    | FOREGROUND_RED | 0                | FOREGROUND_BLUE; break;

            default:                     colour = 0                    | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
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
            orxout(user_error) << "Could not set Windows console settings" << endl;
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
        this->shell_->addOutput(this->promptString_ + this->shell_->getInput(), Shell::Command);
    }

    //! Called if the console gets closed
    void IOConsole::exit()
    {
        // Exit is not an option, just do nothing (Shell doesn't really exit too)
    }

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
