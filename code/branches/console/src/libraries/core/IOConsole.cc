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

#ifdef ORXONOX_PLATFORM_UNIX

    termios* IOConsole::originalTerminalSettings_;

    IOConsole::IOConsole()
        : shell_(Shell::getInstance())
        , escapeMode_(None)
        , buffer_(Shell::getInstance().getInputBuffer())
    {
        this->originalTerminalSettings_ = new termios;
        this->setTerminalMode();
        this->shell_.registerListener(this);
    }

    IOConsole::~IOConsole()
    {
        std::cout << "\033[0G\033[K";
        std::cout.flush();
        resetTerminalMode();
        delete this->originalTerminalSettings_;
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
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (this->escapeMode_ == First && (c == '[' || c=='O') )
                this->escapeMode_ = Second;
            // Get Alt+Tab combination when switching applications
            else if (this->escapeMode_ == First && c == '\t')
            {
                this->buffer_->buttonPressed(KeyEvent(KeyCode::Tab, '\t', KeyboardModifier::Alt));
                this->escapeMode_ = None;
            }
            else if (this->escapeMode_ == Second)
            {
                this->escapeSequence_ += c;
                this->escapeMode_ = None;
                if      (this->escapeSequence_ == "A")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Up,       0, 0));
                else if (this->escapeSequence_ == "B")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Down,     0, 0));
                else if (this->escapeSequence_ == "C")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Right,    0, 0));
                else if (this->escapeSequence_ == "D")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Left,     0, 0));
                else if (this->escapeSequence_ == "1~" || this->escapeSequence_ == "H")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Home,     0, 0));
                else if (this->escapeSequence_ == "2~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Insert,   0, 0));
                else if (this->escapeSequence_ == "3~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Delete,   0, 0));
                else if (this->escapeSequence_ == "4~" || this->escapeSequence_ == "F")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::End,      0, 0));
                else if (this->escapeSequence_ == "5~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::AltPageUp,   0, 0));
                else if (this->escapeSequence_ == "6~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::AltPageDown, 0, 0));
                else if (this->escapeSequence_.size() > 4)
                    // User probably very quickly pressed ESC and [
                    this->escapeMode_ = None;
                else
                    // Waiting for sequence to complete
                    this->escapeMode_ = Second;
            }
            else // not in an escape sequence OR user might have pressed just ESC
            {
                if (this->escapeMode_ == First)
                {
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape, c, 0));
                    this->escapeMode_ = None;
                }
                if (c == '\033')
                {
                    this->escapeMode_ = First;
                    this->escapeSequence_.clear();
                }
                else
                {
                    KeyCode::ByEnum code;
                    switch (c)
                    {
                    case '\n': code = KeyCode::Return; break;
                    case '\r': code = KeyCode::Return; break;
                    case  127: code = KeyCode::Back;   break; 
                    case '\b': code = KeyCode::Back;   break;
                    case '\t': code = KeyCode::Tab;    break;
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
        // it sure isn't an escape sequence here
        if (this->escapeMode_ == First)
            this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape, '\033', 0));
        // Reset in any case because escape sequences always come in one piece
        this->escapeMode_ = None;

        // Print input line
        this->printInputLine();
    }

    void IOConsole::print(const std::string& text)
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

        // Print output line
        std::cout << output;

        // Reset colour to white
        std::cout << "\033[37m";
        std::cout.flush();
    }

    void IOConsole::printInputLine()
    {
        // set cursor to the beginning of the line and erase the line
        std::cout << "\033[1G\033[K";
        // print status line
        //std::cout << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, " << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms avg ticktime # ";
        // Show an arrow to indicate a command prompt
        std::cout << "orxonox>";
        // save cursor position
        std::cout << "\033[s";
        // print commandLine buffer
        std::cout << this->shell_.getInput();
        // restore cursor position and move it to the right
        std::cout << "\033[u";
        if (this->buffer_->getCursorPosition() > 0)
            std::cout << "\033[" << this->buffer_->getCursorPosition() << "C";
        std::cout.flush();
    }

#elif defined(ORXONOX_PLATFORM_WINDOWS)

    IOConsole::IOConsole()
        : shell_(Shell::getInstance())
        , buffer_(Shell::getInstance().getInputBuffer())
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

    void IOConsole::print(const std::string& text)
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
        // Save cursor position and move it the beginning of the second to last line
        std::cout << "\033[s\033[1F";
        // Erase the second to last line
        std::cout << "\033[K";
        this->print(*(this->shell_.getNewestLineIterator()));
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
        // Move curosr the beginning of the line and erase it
        std::cout << "\033[1G\033[K";
        this->print(*(this->shell_.getNewestLineIterator()));
        std::cout << std::endl;
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
        std::cout << "orxonox>" << this->shell_.getInput() << std::endl;
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
