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
            if (level == -1 || level >= 1 && level <= 6)
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
        // or when scrolling. But scrolling is disable and the output
        // is already in std::cout when we start the IOConsole
    }

    //! Called if the text in the input-line has changed
    void IOConsole::inputChanged()
    {
        this->printInputLine();
        std::cout.flush();
    }

    //! Called if the position of the cursor in the input-line has changed
    void IOConsole::cursorChanged()
    {
        this->printInputLine();
        std::cout.flush();
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
#include <sys/stat.h>

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
        : shell_(new Shell("IOConsole", false, true))
        , buffer_(shell_->getInputBuffer())
        , originalTerminalSettings_(new termios())
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
    }

    IOConsole::~IOConsole()
    {
        // Goto last line and create a new one
        if (this->bStatusPrinted_)
            std::cout << "\033[" << this->statusLineWidths_.size() << 'E';
        std::cout << std::endl;

        resetTerminalMode();
        delete this->originalTerminalSettings_;
        this->shell_->destroy();

        // Enable standard std::cout logging again
        OutputHandler::getInstance().enableCout();
    }

    void IOConsole::update(const Clock& time)
    {
        unsigned char c = 0;
        std::string escapeSequence;
        EscapeMode::Value escapeMode = EscapeMode::None;
        while (std::cin.good())
        {
            c = std::cin.get();
            if (std::cin.bad())
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
            std::cout << std::string(newLines, '\n');
            // Move cursor up again
            std::cout << "\033[" << newLines << 'F';
        }

        if (!this->bStatusPrinted_ && this->willPrintStatusLines())
        {
            // Print new lines to make way for status lines
            std::cout << std::string(this->statusLineWidths_.size(), '\n');
            // Move cursor up again
            std::cout << "\033[" << this->statusLineWidths_.size() << 'F';
            this->bStatusPrinted_ = true;
        }
        // Erase status and input lines
        std::cout << "\033[1G\033[J";
        this->printInputLine();
        this->printStatusLines();
        std::cout.flush();
    }

    void IOConsole::printLogText(const std::string& text)
    {
        std::string output = text;
        int level = this->extractLogLevel(&output);

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
    }

    void IOConsole::printInputLine()
    {
        // Set cursor to the beginning of the line and erase the line
        std::cout << "\033[1G\033[K";
        // Indicate a command prompt
        std::cout << this->promptString_;
        // Save cursor position
        std::cout << "\033[s";
        // Print command line buffer
        std::cout << this->shell_->getInput();
        // Restore cursor position and move it to the right
        std::cout << "\033[u";
        if (this->buffer_->getCursorPosition() > 0)
            std::cout << "\033[" << this->buffer_->getCursorPosition() << "C";
    }

    void IOConsole::printStatusLines()
    {
        if (this->willPrintStatusLines())
        {
            // Save cursor position
            std::cout << "\033[s";
            // Move cursor down (don't create a new line here because the buffer might flush then!)
            std::cout << "\033[1E";
            std::cout << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, ";
            std::cout <<               std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms tick time";
            // Restore cursor position
            std::cout << "\033[u";
            this->bStatusPrinted_ = true;
        }
        else
            this->bStatusPrinted_ = false;
    }

    void IOConsole::setTerminalMode()
    {
        termios new_settings;

        tcgetattr(0, this->originalTerminalSettings_);
        new_settings = *this->originalTerminalSettings_;
        new_settings.c_lflag &= ~(ICANON | ECHO);
        //new_settings.c_lflag |= (ISIG | IEXTEN);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN]  = 0;
        tcsetattr(0, TCSANOW, &new_settings);
    }

    void IOConsole::resetTerminalMode()
    {
        tcsetattr(0, TCSANOW, IOConsole::originalTerminalSettings_);
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
        std::cout << "\033[s\033[1F";
        // Erase the line
        std::cout << "\033[K";
        // Reprint the last output line
        this->printLogText(*(this->shell_->getNewestLineIterator()));
        // Restore cursor
        std::cout << "\033[u";
        std::cout.flush();
    }

    //! Called if a new output-line was added
    void IOConsole::lineAdded()
    {
        // Move cursor to the bottom line
        if (this->bStatusPrinted_)
            std::cout << "\033[" << this->statusLineWidths_.size() << 'E';
        // Create new lines on the screen
        int newLines = this->shell_->getNewestLineIterator()->size() / this->terminalWidth_ + 1;
        std::cout << std::string(newLines, '\n');
        // Move cursor to the beginning of the new (last) output line
        std::cout << "\033[" << (newLines + this->statusLineWidths_.size()) << 'F';
        // Erase screen from here
        std::cout << "\033[J";
        // Print the new output line
        for (int i = 0; i < newLines; ++i)
            this->printLogText(this->shell_->getNewestLineIterator()->substr(i*this->terminalWidth_, this->terminalWidth_));
        // Move cursor down
        std::cout << "\033[1E";
        // Print status and input lines
        this->printInputLine();
        this->printStatusLines();
        std::cout.flush();
    }
}

#elif defined(ORXONOX_PLATFORM_WINDOWS)
// ##################################
// ###   Windows Implementation   ###
// ##################################

namespace orxonox
{
    IOConsole::IOConsole()
        : shell_(new Shell("IOConsole", false, true))
        , buffer_(shell_->getInputBuffer())
        , bStatusPrinted_(false)
        , promptString_("orxonox # ")
    {
/*
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
*/
    }

    IOConsole::~IOConsole()
    {
/*
        resetTerminalMode();
        this->shell_->destroy();

        // Enable standard std::cout logging again
        OutputHandler::getInstance().enableCout();
*/
    }

    void IOConsole::update(const Clock& time)
    {
/*
        unsigned char c = 0;
        while (std::cin.good())
        {
            c = std::cin.get();
            if (std::cin.bad())
                break;
        }
        // Reset error flags in std::cin
        std::cin.clear();

        // Determine terminal width and height
        this->lastTerminalWidth_ = this->terminalWidth_;
        this->lastTerminalHeight_ = this->terminalHeight_;
        this->getTerminalSize();
*/
    }

    void IOConsole::printLogText(const std::string& text)
    {
    }

    void IOConsole::printInputLine()
    {
    }

    void IOConsole::printStatusLines()
    {
/*
        if (this->willPrintStatusLines())
        {
            this->bStatusPrinted_ = true;
        }
        else
            this->bStatusPrinted_ = false;
*/
    }

    void IOConsole::setTerminalMode()
    {
    }

    void IOConsole::resetTerminalMode()
    {
    }

    void IOConsole::getTerminalSize()
    {
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    //! Called if only the last output-line has changed
    void IOConsole::onlyLastLineChanged()
    {
    }

    //! Called if a new output-line was added
    void IOConsole::lineAdded()
    {
    }
}

#endif /* ORXONOX_PLATFORM_UNIX */
