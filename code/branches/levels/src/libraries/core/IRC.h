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

#ifndef _IRC_H__
#define _IRC_H__

#include "CorePrereqs.h"

#include <string>
#include "OrxonoxClass.h"

namespace orxonox
{
    class _CoreExport IRC : public OrxonoxClass
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
            IRC(const IRC& other);
            ~IRC() {}

            Tcl::interpreter* interpreter_;
            std::string nickname_;
    };
}

#endif /* _IRC_H__ */
