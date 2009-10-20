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
        , cleanLine_(true)
        , bEscapeMode_(false)
        , buffer_(Shell::getInstance().getInputBuffer())
        , inputIterator_(0)
        , cursorX_(0)
        , cursorY_(0)
    {
        this->originalTerminalSettings_ = new termios;
        this->setTerminalMode();
    }

    IOConsole::~IOConsole()
    {
        std::cout << "\033[0G\033[K";
        std::cout.flush();
        resetTerminalMode();
        delete this->originalTerminalSettings_;
        COUT(0) << "Press enter to end the game..." << std::endl;
    }

    void IOConsole::setTerminalMode()
    {
        termios new_settings;

        tcgetattr(0,this->originalTerminalSettings_);
        new_settings = *this->originalTerminalSettings_;
        new_settings.c_lflag &= ~( ICANON | ECHO );
        //         new_settings.c_lflag |= ( ISIG | IEXTEN );
        new_settings.c_cc[VTIME] = 1;
        new_settings.c_cc[VMIN] = 0;
        tcsetattr(0,TCSANOW,&new_settings);
        COUT(0) << endl;
        //       atexit(&IOConsole::resetTerminalMode);
    }

    void IOConsole::resetTerminalMode()
    {
        tcsetattr(0, TCSANOW, IOConsole::originalTerminalSettings_);
    }

    void IOConsole::update(const Clock& time)
    {
        unsigned c;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (this->bEscapeMode_)
            {
                this->escapeSequence_ += c;
                bool clear = true;
                if      (this->escapeSequence_ == "\033")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Escape,   0, 0));
                else if (this->escapeSequence_ == "[A")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Up,       0, 0));
                else if (this->escapeSequence_ == "[B")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Down,     0, 0));
                else if (this->escapeSequence_ == "[C")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Right,    0, 0));
                else if (this->escapeSequence_ == "[D")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Left,     0, 0));
                else if (this->escapeSequence_ == "[1~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Home,     0, 0));
                else if (this->escapeSequence_ == "[2~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Insert,   0, 0));
                else if (this->escapeSequence_ == "[4~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::End,      0, 0));
                else if (this->escapeSequence_ == "[5~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::PageUp,   0, 0));
                else if (this->escapeSequence_ == "[6~")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::PageDown, 0, 0));
                // Get Alt+Tab combination when switching applications
                else if (this->escapeSequence_ == "\t")
                    this->buffer_->buttonPressed(KeyEvent(KeyCode::Tab, '\t', KeyboardModifier::Alt));
                else if (this->escapeSequence_.size() > 4)
                    clear = true; // Something went wrong, start over
                else
                    clear = false;

                if (clear)
                    this->escapeSequence_.clear();
            }
            else // not in an escape sequence
            {
                if (c == '\033')
                    this->bEscapeMode_ = true;
                else
                {
                    KeyCode::ByEnum code;
                    switch (c)
                    {
                    case '\n': code = KeyCode::Return; break;
                    case  127: code = KeyCode::Delete; break; 
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
    }

    void IOConsole::printOutputLine(const std::string& line)
    {
        // Save cursor position
        std::cout << "\033[s";

        std::string output;

        // Handle line colouring by inspecting the first letter
        char level = 0;
        if (!line.empty())
            level = line[0];
        if (level >= -1 && level <= 6)
            output = line.substr(1);
        else
            output = line;

        // Colour line
        switch (level)
        {
        case -1: std::cout << "\033[30m"; break;
        case  1: std::cout << "\033[91m"; break;
        case  2: std::cout << "\033[31m"; break;
        case  3: std::cout << "\033[34m"; break;
        case  4: std::cout << "\033[36m"; break;
        case  5: std::cout << "\033[35m"; break;
        case  6: std::cout << "\033[37m"; break;
        default:
        }

        // Print output line
        std::cout << output << std::endl;

        // Reset colour to black
        std::cout << "\033[30m";
        // Restore cursor position
        std::cout << "\033[u";
        std::cout.flush();
    }

    void IOConsole::printInputLine()
    {
        // set cursor to the beginning of the line and erase the line
        std::cout << "\033[0G\033[K";
        // print status line
        std::cout << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, " << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms avg ticktime # ";
        // save cursor position
        std::cout << "\033[s";
        // print commandLine buffer
        std::cout << this->shell_.getInput();
        // restore cursor position and move it cursorX_ to the right
        std::cout << "\033[u";
        if (this->buffer_->getCursorPosition() > 0)
            std::cout << "\033[" << this->buffer_->getCursorPosition() << "C";
        std::cout.flush();
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
        // We cannot do anything here because printed lines are fixed
        COUT(2) << "Warning: Trying to edit last console lines, which is impossible!" << std::endl;
    }

    /**
    @brief
        Called if a new output-line was added.
    */
    void IOConsole::lineAdded()
    {
        this->printOutputLine(*(this->shell_.getNewestLineIterator()));
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
        Called if the console gets closed.
    */
    void InGameConsole::exit()
    {
        // Exit is not an option, IOConsole always exists
    }

}
