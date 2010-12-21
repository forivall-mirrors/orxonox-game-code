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
    @brief Implementation of the IRC class and the IRC console commands.
*/

#include "IRC.h"

#include <cpptcl/cpptcl.h>

#include "util/Convert.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "TclThreadManager.h"

namespace orxonox
{
    static const unsigned int IRC_TCL_THREADID  = 1421421421;   ///< The ID of the thread in TclThreadManager that is used for the IRC connection

    SetConsoleCommand("IRC", "say",  &IRC::say);
    SetConsoleCommand("IRC", "msg",  &IRC::msg);
    SetConsoleCommand("IRC", "nick", &IRC::nick);

    /**
        @brief Returns the only existing instance of IRC.
    */
    IRC& IRC::getInstance()
    {
        static IRC instance;
        return instance;
    }

    /**
        @brief Constructor: Doesn't yet connect to IRC nor does it create a Tcl interpreter.
        The IRC object will automatically connect to the IRC server if one of the registered
        console commands is used the first time.
    */
    IRC::IRC()
    {
        this->interpreter_ = 0;
    }

    /**
        @brief Creates and initializes a new multithreaded Tcl-interpreter and defines some callbacks to display IRC-messages in the console.
    */
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

        this->nickname_ = "orx" + multi_cast<std::string>(static_cast<unsigned int>(rand()));
        TclThreadManager::execute(threadID, "set nickname " + this->nickname_);
        TclThreadManager::execute(threadID, "source irc.tcl");
    }

    /**
        @brief Executes a Tcl-command on the Tcl-interpreter.
    */
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

        return false;
    }

    /// Console command: Sends a message to the current channel on the IRC server.
    void IRC::say(const std::string& message)
    {
        if (IRC::eval("irk::say $conn #orxonox {" + message + '}'))
            IRC::tcl_say(Tcl::object(), Tcl::object(IRC::getInstance().nickname_), Tcl::object(message));
    }

    /// Console command: Sends a message to a given channel or nickname on the IRC server.
    void IRC::msg(const std::string& channel, const std::string& message)
    {
        if (IRC::eval("irk::say $conn " + channel + " {" + message + '}'))
            IRC::tcl_privmsg(Tcl::object(channel), Tcl::object(IRC::getInstance().nickname_), Tcl::object(message));
    }

    /// Console command: Changes the nickname on the IRC server.
    void IRC::nick(const std::string& nickname)
    {
        if (IRC::eval("irk::nick $conn " + nickname))
            IRC::getInstance().nickname_ = nickname;
    }

    /// Tcl-callback: Prints a message that was received from the current IRC channel to the console.
    void IRC::tcl_say(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC> " << nick.get() << ": " << stripEnclosingBraces(args.get()) << std::endl;
    }

    /// Tcl-callback: Prints a private message that was received from a user to the console.
    void IRC::tcl_privmsg(Tcl::object const &query, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC (" << query.get() << ")> " << nick.get() << ": " << stripEnclosingBraces(args.get()) << std::endl;
    }

    /// Tcl-callback: Prints an action-message (usually /me ...) that was received from the current IRC channel to the console.
    void IRC::tcl_action(Tcl::object const &channel, Tcl::object const &nick, Tcl::object const &args)
    {
        COUT(0) << "IRC> * " << nick.get() << ' ' << stripEnclosingBraces(args.get()) << std::endl;
    }

    /// Tcl-callback: Prints all kinds of information that were received from the IRC server or channel (connection info, join, part, modes, ...) to the console.
    void IRC::tcl_info(Tcl::object const &channel, Tcl::object const &args)
    {
        COUT(0) << "IRC> --> " << stripEnclosingBraces(args.get()) << std::endl;
    }
}
