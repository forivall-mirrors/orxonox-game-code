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

/**
@file
@brief
    Implementation of three sleep functions. Avoids including windows.h
*/

#include "Sleep.h"
#include "Debug.h"

#ifdef ORXONOX_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef min
#undef max

namespace orxonox
{
    void usleep(unsigned long microseconds)
    {
        //if (microseconds < 1000)
        //    COUT(2) << "Warning: Windows cannot sleep less than 1ms, ignoring" << std::endl;
        Sleep(microseconds / 1000);
    }

    void msleep(unsigned long milliseconds)
    {
        Sleep(milliseconds);
    }

    void sleep(unsigned long seconds)
    {
        Sleep(seconds * 1000);
    }
}

#else /* Linux/Apple */
#include <unistd.h>

namespace orxonox
{
    void usleep(unsigned long usec)
    {
        ::usleep(usec);
    }
    void msleep(unsigned long msec)
    {
        ::usleep(msec * 1000);
    }
    void sleep(unsigned long sec)
    {
        ::usleep(sec * 1000000);
    }
}

#endif
