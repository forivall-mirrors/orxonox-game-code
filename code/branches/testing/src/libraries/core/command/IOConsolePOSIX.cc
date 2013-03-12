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
#include <termios.h>
#include <sys/ioctl.h>

#include "util/Clock.h"
#include "util/Math.h"
#include "util/output/ConsoleWriter.h"
#include "util/output/OutputManager.h"
#include "core/Game.h"
#include "core/input/InputBuffer.h"

namespace orxonox
{
    IOConsole* IOConsole::singletonPtr_s = NULL;

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
        : shell_(new Shell("Console", false))
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
        OutputManager::getInstance().getConsoleWriter()->disable();
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
            this->shell_->addOutput(this->origCout_.str(), Shell::Cout);
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
        OutputManager::getInstance().getConsoleWriter()->enable();
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
            this->shell_->addOutput(this->origCout_.str(), Shell::Cout);
            this->origCout_.str("");
        }
    }

    void IOConsole::printOutputLine(const std::string& text, Shell::LineType type)
    {
        // Colour line
        switch (type)
        {
            case Shell::Message:
            case Shell::DebugOutput:     this->cout_ << "\033[0m"; break;

            case Shell::UserError:       this->cout_ << "\033[91m"; break;
            case Shell::UserWarning:     this->cout_ << "\033[93m"; break;
            case Shell::UserStatus:      this->cout_ << "\033[92m"; break;
            case Shell::UserInfo:        this->cout_ << "\033[96m"; break;

            case Shell::InternalError:   this->cout_ << "\033[31m"; break;
            case Shell::InternalWarning: this->cout_ << "\033[33m"; break;
            case Shell::InternalStatus:  this->cout_ << "\033[32m"; break;
            case Shell::InternalInfo:    this->cout_ << "\033[36m"; break;

            case Shell::Verbose:         this->cout_ << "\033[94m"; break;
            case Shell::VerboseMore:     this->cout_ << "\033[34m"; break;
            case Shell::VerboseUltra:    this->cout_ << "\033[34m"; break;

            case Shell::Command:         this->cout_ << "\033[95m"; break;
            case Shell::Hint:            this->cout_ << "\033[35m"; break;

            default:                     this->cout_ << "\033[37m"; break;
        }

        // Print output line
        this->cout_ << text;

        // Reset colour atributes
        this->cout_ << "\033[0m";
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
        this->terminalWidth_  = 0;
        this->terminalHeight_ = 0;

#ifdef TIOCGSIZE
        struct ttysize win;
        if (!ioctl(STDIN_FILENO, TIOCGSIZE, &win))
        {
            this->terminalWidth_  = win.ts_cols;
            this->terminalHeight_ = win.ts_lines;
        }
#elif defined TIOCGWINSZ
        struct winsize win;
        if (!ioctl(STDIN_FILENO, TIOCGWINSZ, &win))
        {
            this->terminalWidth_  = win.ws_col;
            this->terminalHeight_ = win.ws_row;
        }
#endif

        const char* s;
        if (!this->terminalWidth_ && (s = getenv("COLUMNS")))
            this->terminalWidth_  = strtol(s, NULL, 10);
        if (!this->terminalWidth_)
            this->terminalWidth_ = 80;
        if (!this->terminalHeight_ && (s = getenv("LINES")))
            this->terminalHeight_ = strtol(s, NULL, 10);
        if (!this->terminalHeight_)
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
