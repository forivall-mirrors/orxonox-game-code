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

#include "core/Clock.h"
#include "core/CommandLine.h"
#include "core/CommandExecutor.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/Iterator.h"
#include "network/Server.h"
#include "objects/Tickable.h"
#include "util/Sleep.h"

#include <iostream>
#include <iomanip>
#include <boost/bind.hpp>


namespace orxonox
{
    const unsigned int MAX_COMMAND_LENGTH = 255;
    
    AddGameState(GSDedicated, "dedicated");

    GSDedicated::GSDedicated(const std::string& name)
        : GameState(name)
        , server_(0)
        , timeSinceLastUpdate_(0)
        , closeThread_(false)
        , inputIterator_(0)
        , cleanLine_(true)
    {
        this->inputThread_ = new boost::thread(boost::bind(&GSDedicated::inputThread, this));
        this->commandLine_ = new unsigned char[MAX_COMMAND_LENGTH];
//         memset( this->commandLine_, 0, MAX_COMMAND_LENGTH );
    }

    GSDedicated::~GSDedicated()
    {
        closeThread_ = true;
#ifndef ORXONOX_PLATFORM_WINDOWS
        std::cout << "\033[0G\033[K";
        std::cout.flush();
#endif
        //inputThread_->join();
    }

    void GSDedicated::activate()
    {
        GameMode::setHasServer(true);

        this->server_ = new Server(CommandLine::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        server_->open();
    }

    void GSDedicated::deactivate()
    {
        this->server_->close();
        delete this->server_;

        GameMode::setHasServer(false);
    }

    void GSDedicated::update(const Clock& time)
    {
        timeSinceLastUpdate_ += time.getDeltaTime();
        if (timeSinceLastUpdate_ >= NETWORK_PERIOD)
        {
            timeSinceLastUpdate_ -= static_cast<unsigned int>(timeSinceLastUpdate_ / NETWORK_PERIOD) * NETWORK_PERIOD;
            server_->update(time);
        }
        else
        {
            usleep((unsigned int)((NETWORK_PERIOD - timeSinceLastUpdate_)*1000*1000 ));
            usleep(NETWORK_PERIOD*1000*1000); // NOTE: this is to throttle the non-network framerate
//            COUT(0) << "sleeping for " << (int)((NETWORK_PERIOD - timeSinceLastUpdate_) * 1000 * 1000) << " usec" << endl;
        }
        processQueue();
        printLine();
    }
    
    void GSDedicated::inputThread()
    {
        unsigned char c;
        while(!closeThread_)
        {
            c = getchar();
            {
//                 boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
                if ( inputIterator_>=MAX_COMMAND_LENGTH-1 && c!='\n' )
                    continue;
                this->commandLine_[this->inputIterator_++] = c;
                if( c == '\n' )
                {
                    this->cleanLine_ = true;
                    boost::recursive_mutex::scoped_lock(this->inputQueueMutex_);
                    this->commandQueue_.push( std::string((const char*)this->commandLine_,inputIterator_) );
                    inputIterator_ = 0;
                }
            }
        }
    }
    
    void GSDedicated::printLine()
    {
#ifndef ORXONOX_PLATFORM_WINDOWS
        std::cout << "\033[s\033[0G";
//         boost::recursive_mutex::scoped_lock(this->inputLineMutex_);
        std::cout << std::fixed << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgFPS() << " fps, " << std::setprecision(2) << std::setw(5) << Game::getInstance().getAvgTickTime() << " ms avg ticktime # ";
        if ( this->cleanLine_ )
            this->cleanLine_ = false;
        else
            std::cout <<"\033[u";
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
}
