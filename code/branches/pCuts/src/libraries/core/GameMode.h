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
    @ingroup Management CoreGame
    @brief Declaration of the GameMode class which stores and returns the current mode of the game.
*/

#ifndef _GameMode_H__
#define _GameMode_H__

#include "CorePrereqs.h"

// tolua_begin
namespace orxonox
{
    /// Helper class, stores and returns the current mode of the game.
    class _CoreExport GameMode
    {
// tolua_end
        friend class Core;

        public:
// tolua_begin
            static bool showsGraphics() { return bShowsGraphics_s; }    ///< Returns true if the game shows graphics, false if it is in text-console mode
            static bool playsSound()    { return bPlaysSound_s; }       ///< Returns true if the game is able to play sounds
            static bool isServer()      { return bIsServer_s; }         ///< Returns true if we're currently a server (online)
            static bool isClient()      { return bIsClient_s; }         ///< Returns true if we're currently a client (online)
            static bool isStandalone()  { return bIsStandalone_s; }     ///< Returns true if we're in standalone mode (offline)
            static bool isMaster()      { return bIsMaster_s; }         ///< Returns true if we're in control of the game (either standalone or server)
// tolua_end

            static void setPlaysSound   (bool val) { bPlaysSound_s    = val; }                      ///< Defines if the game can play sounds
            static void setIsServer     (bool val) { bIsServer_s      = val; updateIsMaster(); }    ///< Defines if the program is in server mode (online)
            static void setIsClient     (bool val) { bIsClient_s      = val; updateIsMaster(); }    ///< Defines if the program is in client mode (online)
            static void setIsStandalone (bool val) { bIsStandalone_s  = val; updateIsMaster(); }    ///< Defines if the program is in standalone mode (offline)

        private:
            GameMode();
            GameMode(const GameMode& inst);
            ~GameMode();

            /// Checks if we're in control of the game (either standalone or server).
            static void updateIsMaster()
            {
                bIsMaster_s = (bIsServer_s || bIsStandalone_s);
            }

            static bool bShowsGraphics_s;                   //!< global variable that tells whether to show graphics
            static bool bPlaysSound_s;                      //!< global variable that tells whether to sound works
            static bool bIsServer_s;                        //!< global variable that tells whether this is a server (online)
            static bool bIsClient_s;                        //!< global variable that tells whether this is a client (online)
            static bool bIsStandalone_s;                    //!< global variable that tells whether the game is running in standalone mode (offline)
            static bool bIsMaster_s;                        //!< global variable that tells whether we're in control of the game (standalone or server)
    }; // tolua_export
} // tolua_export

#endif /* _GameMode_H__ */
