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

#ifndef _GSDedicated_H__
#define _GSDedicated_H__

#include "OrxonoxPrereqs.h"

#include <cstring>
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "core/GameState.h"
#include "network/NetworkPrereqs.h"

struct termios;

namespace orxonox
{

    class _OrxonoxExport GSDedicated : public GameState
    {
    public:
        GSDedicated(const GameStateInfo& info);
        ~GSDedicated();

        void activate();
        void deactivate();
        void update(const Clock& time);

    private:
        void inputThread();
        void printLine();
        void processQueue();
        void setTerminalMode();
        static void resetTerminalMode();

        void insertCharacter( unsigned int position, char c );
        void deleteCharacter( unsigned int position );

        Server*                 server_;

        boost::thread           *inputThread_;
        boost::recursive_mutex  inputLineMutex_;
        boost::recursive_mutex  inputQueueMutex_;
        bool                    closeThread_;
        bool                    cleanLine_;
        unsigned char*          commandLine_;
        unsigned int            inputIterator_;
        std::queue<std::string> commandQueue_;
        static termios*         originalTerminalSettings_;

        unsigned int            cursorX_;
        unsigned int            cursorY_;
    };
}

#endif /* _GSDedicated_H__ */
