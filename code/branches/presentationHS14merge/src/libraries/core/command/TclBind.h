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
    @defgroup Tcl Tcl
    @ingroup Command
*/

/**
    @file
    @ingroup Command Tcl
    @brief Declaration of the TclBind class.

    @anchor TclBindExample

    orxonox::TclBind is a wrapper class for a Tcl interpreter. It is implemented as
    singleton, so it can be accessed by everyone, but all share the same static
    Tcl interpreter. If you need a Tcl interpreter at your own, see orxonox::TclThreadManager
    for more information.

    orxonox::TclBind is used by orxonox::CommandExecutor to execute Tcl commands. It can
    also be used to execute Tcl commands from different sources, but note that they may
    interfer with the ingame console if used improperly. By no means execute blocking
    commands such as endless loops or the tcl command @c vwait. Use orxonox::TclThreadManager
    and execute these commands in a multithreaded Tcl interpreter instead.

    TclBind also defines different callback functions to return commands from the
    Tcl interpreter back to Orxonox. Because of that it's possible to send a mixture
    of Orxonox- and Tcl-commands to TclBind::eval() and still get the desired behavior.

    Example:
    @code
    TclBind::eval("puts \"Hello World\"");                              // calls the tcl command "puts", prints "Hello World" to the console
    TclBind::eval("log Hello World");                                   // calls the orxonox command "log", prints "Hello World" to the console

    TclBind::eval("log [expr 1+1]");                                    // prints "2" to the console (which is the result of the tcl command "expr")

    TclBind::eval("puts -nonewline Hello; log World");                  // prints "HelloWorld" to the console

    TclBind::eval("for {set i 0} {$i < 10} {incr i} {log test: $i}");   // prints "test: 0", ..., "test: 9" to the console
    @endcode

    Note that @c puts and @c log behave slightly different, even though both can print
    text to the console. @c puts needs quotation marks around multi-word output, while
    @c log doesn't. @c puts on the other hand supports the flag @c -nonewline.

    TclBind::eval() can also be used to obtain the return-value of a Tcl command:
    @code
    std::string result = TclBind::eval("expr 1+1");                     // result == "2"
    @endcode
*/

#ifndef _TclBind_H__
#define _TclBind_H__

#include "core/CorePrereqs.h"

#include <cassert>
#include <string>
#include "util/Singleton.h"

namespace orxonox
{
    /**
        @brief A wrapper class for a Tcl interpreter. Used to execute Tcl commands.

        TclBind is used to execute Tcl commands, for example if sent to CommandExecutor::execute().
        It also defines different callbacks for Tcl, which allows to combine Orxonox-console-commands
        and Tcl-function without problems.

        @see See @ref TclBindExample "TclBind.h" for more information and an example.
    */
    class _CoreExport TclBind : public Singleton<TclBind>
    {
        friend class Singleton<TclBind>;
        public:
            TclBind(const std::string& datapath);
            ~TclBind();

            static std::string tcl(const std::string& tclcode);
            static void bgerror(const std::string& error);

            void setDataPath(const std::string& datapath);
            static std::string getTclLibraryPath();
            /// Returns the path to the Orxonox-specific Tcl-files.
            inline const std::string& getTclDataPath() const
                { return this->tclDataPath_; }

            void initializeTclInterpreter();
            static Tcl::interpreter* createTclInterpreter();
            /// Returns the Tcl-interpreter
            inline Tcl::interpreter* getTclInterpreter() const
                { return this->interpreter_; }

            static std::string tcl_query(Tcl::object const &args);
            static void tcl_execute(Tcl::object const &args);

            static std::string eval(const std::string& tclcode, int* error = 0);

        private:
            TclBind(const TclBind& other);      ///< Copy-constructor, not implemented

            static std::string tcl_helper(Tcl::object const &args, bool bQuery);

            Tcl::interpreter* interpreter_;     ///< The wrapped Tcl interpreter
            std::string tclDataPath_;           ///< The path to the directory that contains the Orxonox-specific Tcl-files
            bool bSetTclDataPath_;              ///< True if tclDataPath_ was defined (after a call to setDataPath())

            static TclBind* singletonPtr_s;     ///< The singleton pointer
    };
}

#endif /* _TclBind_H__ */
