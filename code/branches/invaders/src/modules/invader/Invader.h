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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Invader.h
    @brief Declaration of the Invader class.
    @ingroup Invader
*/

#ifndef _Invader_H__
#define _Invader_H__

#include "invader/InvaderPrereqs.h"

#include "gametypes/Deathmatch.h"

namespace orxonox
{

    class _InvaderExport Invader : public Deathmatch
    {
        public:
            Invader(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~Invader(); //!< Destructor. Cleans up, if initialized.

            virtual void start(); //!< Starts the Invader minigame.
            virtual void end(); ///!< Ends the Invader minigame.
    };
}

#endif /* _Invader_H__ */
