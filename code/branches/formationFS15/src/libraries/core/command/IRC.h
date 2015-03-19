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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @ingroup Command Tcl
    @brief Declaration of IRC helper class, used for IRC connections using Tcl.
*/

#ifndef _IRC_H__
#define _IRC_H__

#include "core/CorePrereqs.h"

#include <string>

namespace orxonox
{
    /**
        @brief The IRC class creates a Tcl-thread (see TclThreadManager) and connects to an IRC server.
        It provides different console commands to send messages and to perform other actions on the IRC server.
    */
    class _CoreExport IRC
    {
        public:
            static IRC& getInstance();

            static void say(const std::string& message);
            static void msg(const std::string& channel, const std::string& message);
            static void nick(const std::string& nickname);

            static void tcl_say(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args);
            static void tcl_privmsg(Tcl::object const &query, Tcl::object const &nick, Tcl::object const &args);
            static void tcl_action(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args);
            static void tcl_info(Tcl::object const &channel, Tcl::object const &args);

        private:
            static bool eval(const std::string& command);
            void initialize();

            IRC();
            IRC(const IRC& other);              ///< Copy-constructor: Not implemented
            ~IRC() {}                           ///< Destructor

            Tcl::interpreter* interpreter_;     ///< The Tcl interpreter that is used for the IRC connection
            std::string nickname_;              ///< The user's nickname on the IRC server
    };
}

#endif /* _IRC_H__ */
