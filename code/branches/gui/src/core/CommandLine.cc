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

#include "CommandLine.h"

namespace orxonox
{
    /**
    @brief
        Destructor destroys all CommandLineArguments with it.
    */
    CommandLine::~CommandLine()
    {
        for (std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgs_.begin();
            it != cmdLineArgs_.end(); ++it)
        {
            delete it->second;
        }
    }

    /**
    @brief
        Returns a unique instance (Meyers Singleton).
    */
    CommandLine& CommandLine::_getInstance()
    {
        static CommandLine instance;
        return instance;
    }

    /**
    @brief
        Reads the command line parses the values of each argument.
        It is then stored in the corresponding CommandLineArgument.
    @note
        The reason that you have to provide the string to be parsed as
        space separted list is because of argc and argv. If you only have
        a whole string, simply use getAllStrings() of SubString.
    @param arguments
        Vector of space separated strings.
    */
    void CommandLine::_parse(const std::vector<std::string>& arguments)
    {
        // why this? See bFirstTimeParse_ declaration.
        if (bFirstTimeParse_)
        {
            // first shove all the shortcuts in a map
            for (std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgs_.begin();
                it != cmdLineArgs_.end(); ++it)
            {
                OrxAssert(cmdLineArgsShortcut_.find(it->second->getShortcut()) == cmdLineArgsShortcut_.end(),
                    "Cannot have two command line shortcut with the same name.");
                if (it->second->getShortcut() != "")
                    cmdLineArgsShortcut_[it->second->getShortcut()] = it->second;
            }
            bFirstTimeParse_ = false;
        }

        std::string name;
        std::string shortcut;
        std::string value;
        for (unsigned int i = 0; i < arguments.size(); ++i)
        {
            if (arguments[i].size() != 0)
            {
                // sure not ""
                if (arguments[i][0] == '-')
                {
                    // start with "-"
                    if (arguments[i].size() == 1)
                    {
                        // argument[i] is "-", probably a minus sign
                        value += "- ";
                    }
                    else if (arguments[i][1] <= 57 && arguments[i][1] >= 48)
                    {
                        // negative number as a value
                        value += arguments[i] + " ";
                    }
                    else
                    {
                        // can be shortcut or full name argument

                        // save old data first
                        if (name != "")
                        {
                            checkFullArgument(name, value);
                            name = "";
                            assert(shortcut == "");
                        }
                        else if (shortcut != "")
                        {
                            checkShortcut(shortcut, value);
                            shortcut = "";
                            assert(name == "");
                        }

                        if (arguments[i][1] == '-')
                        {
                            // full name argument with "--name"
                            name = arguments[i].substr(2);
                        }
                        else
                        {
                            // shortcut with "-s"
                            shortcut = arguments[i].substr(1);
                        }

                        // reset value string
                        value = "";
                    }
                }
                else
                {
                    // value string

                    if (name == "" && shortcut == "")
                    {
                        ThrowException(Argument, "Expected \"-\" or \"-\" in command line arguments.\n");
                    }

                    // Concatenate strings as long as there's no new argument by "-" or "--"
                    value += arguments[i] + ' ';
                }
            }
        }

        // parse last argument
        if (name != "")
        {
            checkFullArgument(name, value);
            assert(shortcut == "");
        }
        else if (shortcut != "")
        {
            checkShortcut(shortcut, value);
            assert(name == "");
        }
    }

    /**
    @brief
        Parses an argument based on its full name.
    @param name
        Full name of the argument
    @param value
        String containing the value
    */
    void CommandLine::checkFullArgument(const std::string& name, const std::string& value)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgs_.find(name);
        if (it == cmdLineArgs_.end())
            ThrowException(Argument, "Command line argument '" + name + "' does not exist.");

        it->second->parse(value);
    }

    /**
    @brief
        Parses an argument based on its shortcut.
    @param shortcut
        Shotcut to the argument
    @param value
        String containing the value
    */
    void CommandLine::checkShortcut(const std::string& shortcut, const std::string& value)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgsShortcut_.find(shortcut);
        if (it == cmdLineArgsShortcut_.end())
            ThrowException(Argument, "Command line shortcut '" + shortcut + "' does not exist.");

        it->second->parse(value);
    }

    std::string CommandLine::getUsageInformation()
    {
        CommandLine* inst = &_getInstance();
        std::string infoStr;
        for (std::map<std::string, BaseCommandLineArgument*>::const_iterator it = inst->cmdLineArgs_.begin();
            it != inst->cmdLineArgs_.end(); ++it)
        {
            infoStr += "[--" + it->second->getName() + " " + it->second->getInformation() + "] ";
        }
        return infoStr;
    }

}
