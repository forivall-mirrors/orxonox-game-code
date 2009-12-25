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
    @brief Declaration of the GameMode class.
*/

#ifndef _GameMode_H__
#define _GameMode_H__

#include "CorePrereqs.h"

// tolua_begin
namespace orxonox
{
    class _CoreExport GameMode
    {
// tolua_end
        friend class Core;

        public:
// tolua_begin
            static bool showsGraphics() { return bShowsGraphics_s; }
            static bool playsSound()    { return bPlaysSound_s; }
            static bool hasServer()     { return bHasServer_s; }
            static bool isClient()      { return bIsClient_s; }
            static bool isStandalone()  { return bIsStandalone_s; }
            static bool isMaster()      { return bIsMaster_s; }
// tolua_end

            static void setPlaysSound   (bool val) { bPlaysSound_s    = val; }
            static void setHasServer    (bool val) { bHasServer_s     = val; updateIsMaster(); }
            static void setIsClient     (bool val) { bIsClient_s      = val; updateIsMaster(); }
            static void setIsStandalone (bool val) { bIsStandalone_s  = val; updateIsMaster(); }

        private:
            GameMode();
            GameMode(const GameMode& inst);
            ~GameMode();

            static void updateIsMaster()
            {
                bIsMaster_s = (bHasServer_s || bIsStandalone_s);
            }

            static bool bShowsGraphics_s;                   //!< global variable that tells whether to show graphics
            static bool bPlaysSound_s;                      //!< global variable that tells whether to sound works
            static bool bHasServer_s;                       //!< global variable that tells whether this is a server
            static bool bIsClient_s;
            static bool bIsStandalone_s;
            static bool bIsMaster_s;
    }; // tolua_export
} // tolua_export

#endif /* _GameMode_H__ */
