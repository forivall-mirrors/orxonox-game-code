/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 @file  Sleep.h
 @brief Macros for using sleep() and usleep() under windows
 */

#include "OrxonoxPlatform.h"

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
#  include <windows.h>
#  define usleep(x) Sleep((x) / 1000)
#  define msleep(x) Sleep(x)
#  define  sleep(x) Sleep((x) * 1000)
#else
#  include <unistd.h>
#  define msleep(x) uleep((x) * 1000)
#endif
