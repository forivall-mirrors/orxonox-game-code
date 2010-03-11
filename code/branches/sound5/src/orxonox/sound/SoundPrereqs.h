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
    Enums, constants, functions and forward declarations for the sound files.
*/

#ifndef _SoundPrereqs_H__
#define _SoundPrereqs_H__

#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class AmbientSound;
    class BaseSound;
    class SoundBuffer;
    class SoundManager;
    class SoundStreamer;
    class WorldSound;
}

typedef struct ALCcontext_struct ALCcontext;
typedef struct ALCdevice_struct ALCdevice;
typedef unsigned int ALuint;
typedef int ALint;
typedef int ALenum;

//-----------------------------------------------------------------------
// Helper functions
//-----------------------------------------------------------------------

namespace orxonox
{
    //! Returns an error string for an AL error code
    _OrxonoxExport std::string getALErrorString(ALenum error);
}

#endif /* _SoundPrereqs_H__ */
