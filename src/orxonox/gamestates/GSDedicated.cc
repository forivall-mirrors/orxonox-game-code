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
 *      Fabian 'x3n' Landau
 *
 */

#include "GSDedicated.h"

#include "util/Debug.h"
#include "util/Sleep.h"
#include "core/Clock.h"
#include "core/CommandLine.h"
#include "core/CommandExecutor.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Server.h"

#include <iostream>
#include <iomanip>
#include <boost/bind.hpp>

#ifdef ORXONOX_PLATFORM_UNIX
#include <termios.h>
#endif


namespace orxonox
{
    const unsigned int MAX_COMMAND_LENGTH = 255;
    
    DeclareGameState(GSDedicated, "dedicated", false, false);
    
    termios* GSDedicated::originalTerminalSettings_;

    GSDedicated::GSDedicated(const GameStateConstrParams& params)
        : GameState(params)
        , server_(0)
        , closeThread_(false)
        , cleanLine_(true)
        , inputIterator_(0)
        , cursorX_(0)
        , cursorY_(0)
    {
    }

    GSDedicated::~GSDedicated()
    {
    }

    void GSDedicated::activate()
    {
        GameMode::setHasServer(true);
        
        this->inputThread_ = new boost::thread(boost::bind(&GSDedicated::inputThread, this));
        
#ifndef ORXONOX_PLATFORM_WINDOWS
        this->originalTerminalSettings_ = new termios;
        this->setTerminalMode();
#endif

        this->server_ = new Server(CommandLine::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        server_->open();
    }

    void GSDedicated::deactivate()
    {
        this->server_->close();
        delete this->server_;
        
        closeThread_ = true;
#ifdef ORXONOX_PLATFORM_UNIX
        std::cout << "\033[0G\033[K";
        std::cout.flush();
        resetTerminalMode();
        delete this->originalTerminalSettings_;
#else
        COUT(0) << "Press enter to end the game..." << std::endl;
#endif
        inputThread_->join();
        delete this->inputThread_;

        GameMode::setHasServer(false);
    }

    void GSDedicated::update(const Clock& time)
    {
        server_->update(time);
        processQueue();
        printLine();
    }
    
    void GSDedicated::inputThread()
    {
        this->commandLine_ = new unsigned char[MAX_COMMAND_LENGTH];
//         memset( this->commandLine_, 0, MAX_COMMAND_LENGTH );
        unsigned char c;
        unsigned int  escapeChar=0;
        while(!closeThread_)
        {
            size_t count = read(STDIN_FILENO, &c, 1);
            if (count == 1)
            {
//                 boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
                if ( inputIterator_>=MAX_COMMAND_LENGTH-1 && c!='\n' )
                    continue;
                if( escapeChar > 0 )
                {
                    if( c == '[' )
                    {
                        escapeChar = 2;
                        continue;
                    }
                    else if ( escapeChar == 2 )
                    {
                        switch (c)
                        {
                            case 'A': //keyup
                                
                                break;
                            case 'B': //keydown
                                
                                break;
                            case 'C': //keyright
                                if(cursorX_<inputIterator_)
                                    ++cursorX_;
                                break;
                            case 'D': //keyleft
                                if(cursorX_>0)
                                    --cursorX_;
                                break;
                            default: //not supported...
//                                 std::cout << endl << c << endl;
                                break;
                        }
                        escapeChar = 0;
                    }
                }
                else // not in escape sequence mode
                {
                    switch (c)
                    {
                        case '\n':
                            this->cleanLine_ = true;
                            {
                                boost::recursive_mutex::scoped_lock(this->inputQueueMutex_);
                                boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
                                this->commandQueue_.push( std::string((const char*)this->commandLine_,inputIterator_) );
                            }
                            memset( this->commandLine_, 0, inputIterator_ );
                            inputIterator_ = 0;
                            this->cursorX_ = 0;
                            this->cursorY_ = 0;
                            std::cout << endl;
                            break;
                        case 127: // backspace
                        case '\b':
                            deleteCharacter( this->cursorX_ );
                            break;
                        case '\t':
                        {
//                             boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
                            std::cout << endl << CommandExecutor::hint( std::string((const char*)this->commandLine_,inputIterator_) ) << endl;
                            strncpy(reinterpret_cast<char*>(this->commandLine_), CommandExecutor::complete( std::string(reinterpret_cast<char*>(this->commandLine_),inputIterator_) ).c_str(), MAX_COMMAND_LENGTH);
                            this->inputIterator_ = strlen((const char*)this->commandLine_);
                            this->cursorX_ = this->inputIterator_;
                            break;
                        }
                        case '\033': // 1. escape character
                            escapeChar = 1;
                            break;
                        default:
                            insertCharacter( this->cursorX_, c );
                            break;
                    }
                }
            }
        }

        delete[] this->commandLine_;
    }
    
    void GSDedicated::printLine()
    {
#ifdef ORXONOX_PLATFORM_UNIX
        // set cursor to the begining of the line and erase the line
        std::cout << "\033[0G\033[K";
//         boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
        // print status line
        std::cout << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, " << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms avg ticktime # ";
        //save cursor position
        std::cout << "\033[s";
        //print commandLine buffer
        std::cout << std::string((const char*)this->commandLine_, inputIterator_);
        //restore cursor position and move it cursorX_ to the right
        std::cout << "\033[u";
        if( this->cursorX_ > 0 )
            std::cout << "\033[" << this->cursorX_ << "C";
        std::cout.flush();
#endif
    }
    
    void GSDedicated::processQueue()
    {
        std::string tempstr;
        {
            boost::recursive_mutex::scoped_lock lock1(this->inputQueueMutex_);
            while(true)
            {
                if ( !this->commandQueue_.empty() )
                {
                    tempstr = this->commandQueue_.front();
                    this->commandQueue_.pop();
                    lock1.unlock();
                }
                else
                    break;
                CommandExecutor::execute(tempstr, true);
            }
        }
    }
    
    void GSDedicated::setTerminalMode()
    {
#ifdef ORXONOX_PLATFORM_UNIX
        termios new_settings;
     
        tcgetattr(0,this->originalTerminalSettings_);
        new_settings = *this->originalTerminalSettings_;
        new_settings.c_lflag &= ~( ICANON | ECHO );
//         new_settings.c_lflag |= ( ISIG | IEXTEN );
        new_settings.c_cc[VTIME] = 1;
        new_settings.c_cc[VMIN] = 0;
        tcsetattr(0,TCSANOW,&new_settings);
        COUT(0) << endl;
//       atexit(&GSDedicated::resetTerminalMode);
#endif
    }
    
    void GSDedicated::resetTerminalMode()
    {
#ifdef ORXONOX_PLATFORM_UNIX
        tcsetattr(0, TCSANOW, GSDedicated::originalTerminalSettings_);
#endif
    }
    
    void GSDedicated::insertCharacter( unsigned int position, char c )
    {
//         std::cout << endl << static_cast<unsigned int>(c) << endl;
        // check that we do not exceed MAX_COMMAND_LENGTH
        if( inputIterator_+1 < MAX_COMMAND_LENGTH )
        {
            // if cursor not at end of line then move the rest of the line
            if( position != this->inputIterator_ )
                    memmove( this->commandLine_+position+1, this->commandLine_+position, this->inputIterator_-position);
//             boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
            this->commandLine_[position] = c;
            ++this->cursorX_;
            ++this->inputIterator_;
        }
    }
    void GSDedicated::deleteCharacter( unsigned int position )
    {
//         boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
        if ( this->inputIterator_>0 && position>0 )
        {
            if ( position != this->inputIterator_ )
                memmove( this->commandLine_+position-1, this->commandLine_+position, this->inputIterator_-position);
            --this->cursorX_;
            --this->inputIterator_;
        }
    }
    
}
