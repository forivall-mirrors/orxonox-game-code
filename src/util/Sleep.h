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
    Functions declarations to make the current thread sleep.
 */

#ifndef _Sleep_H__
#define _Sleep_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    //! Makes the thread sleep for a few @a microseconds
    _UtilExport void usleep(unsigned long microseconds);
    //! Makes the thread sleep for a few @a milliseconds
    _UtilExport void msleep(unsigned long milliseconds);
    //! Makes the thread sleep for a few @a seconds
    _UtilExport void sleep (unsigned long seconds);
}

#endif /* _Sleep_H__ */
