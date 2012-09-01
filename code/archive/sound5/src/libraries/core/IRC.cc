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

#include "IRC.h"

#include <cpptcl/cpptcl.h>

#include "util/Convert.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "CoreIncludes.h"
#include "TclThreadManager.h"

namespace orxonox
{
    static const unsigned int IRC_TCL_THREADID  = 1421421421;

    SetConsoleCommand(IRC, say,  true).accessLevel(AccessLevel::User);
    SetConsoleCommand(IRC, msg,  false).accessLevel(AccessLevel::User);
    SetConsoleCommand(IRC, nick, false).accessLevel(AccessLevel::User);

    IRC::IRC()
    {
        RegisterRootObject(IRC);
        this->interpreter_ = 0;
    }

    void IRC::initialize()
    {
        unsigned int threadID = IRC_TCL_THREADID;
        this->interpreter_ = TclThreadManager::createWithId(threadID);

        try
        {
            this->interpreter_->def("::orxonox::irc::say", IRC::tcl_say, Tcl::variadic());
            this->interpreter_->def("::orxonox::irc::privmsg", IRC::tcl_privmsg, Tcl::variadic());
            this->interpreter_->def("::orxonox::irc::action", IRC::tcl_action, Tcl::variadic());
            this->interpreter_->def("::orxonox::irc::info", IRC::tcl_info, Tcl::variadic());
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl (IRC) error: " << e.what();   }
        catch (...)
        {   COUT(1) << "Error while initializing Tcl (IRC): " << Exception::handleMessage();   }

        this->nickname_ = "orx" + multi_cast<std::string>(static_cast<unsigned int>(rand()));
        TclThreadManager::execute(threadID, "set nickname " + this->nickname_);
        TclThreadManager::execute(threadID, "source irc.tcl");
    }

    IRC& IRC::getInstance()
    {
        static IRC instance;
        return instance;
    }

    bool IRC::eval(const std::string& command)
    {
        if (!IRC::getInstance().interpreter_)
        {
            IRC::getInstance().initialize();
            COUT(1) << "Error: IRC client wasn't yet initialized, please try again." << std::endl;
            return false;
        }

        try
        {
            IRC::getInstance().interpreter_->eval(command);
            return true;
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl (IRC) error: " << e.what();   }
        catch (...)
        {   COUT(1) << "Error while executing Tcl (IRC): " << Exception::handleMessage();   }

        return false;
    }

    void IRC::say(const std::string& message)
    {
        if (IRC::eval("irk::say $conn #orxonox {" + message + '}'))
            IRC::tcl_say(Tcl::object(), Tcl::object(IRC::getInstance().nickname_), Tcl::object(message));
    }

    void IRC::msg(const std::string& channel, const std::string& message)
    {
        if (IRC::eval("irk::say $conn " + channel + " {" + message + '}'))
            IRC::tcl_privmsg(Tcl::object(channel), Tcl::object(IRC::getInstance().nickname_), Tcl::object(message));
    }

    void IRC::nick(const std::string& nickname)
    {
        if (IRC::eval("irk::nick $conn " + nickname))
            IRC::getInstance().nickname_ = nickname;
    }

    void IRC::tcl_say(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC> " << nick.get() << ": " << stripEnclosingBraces(args.get()) << std::endl;
    }

    void IRC::tcl_privmsg(Tcl::object const &query, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC (" << query.get() << ")> " << nick.get() << ": " << stripEnclosingBraces(args.get()) << std::endl;
    }

    void IRC::tcl_action(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC> * " << nick.get() << ' ' << stripEnclosingBraces(args.get()) << std::endl;
    }

    void IRC::tcl_info(Tcl::object const &channel, Tcl::object const &args)
    {
        COUT(0) << "IRC> --> " << stripEnclosingBraces(args.get()) << std::endl;
    }
}
