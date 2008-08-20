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
    CommandLine::~CommandLine()
    {
        for (std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgs_.begin();
            it != cmdLineArgs_.end(); ++it)
        {
            delete it->second;
        }
    }
    
    CommandLine& CommandLine::_getInstance()
    {
        static CommandLine instance;
        return instance;
    }

    void CommandLine::_parse(int argc, char** argv)
    {
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
        std::string val;
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i][0] == '-')
            {
                if (argv[i][1] <= 57 && argv[i][1] >= 48)
                {
                    // negative number as a value
                    val += std::string(argv[i]) + " ";
                }
                else
                {
                    // save old data first
                    if (name != "")
                    {
                        checkFullArgument(name, val);
                        name = "";
                        assert(shortcut == "");
                    }
                    else if (shortcut != "")
                    {
                        checkShortcut(shortcut, val);
                        shortcut = "";
                        assert(name == "");
                    }

                    if (argv[i][1] == '-')
                    {
                        // full name argument
                        name = argv[i] + 2;
                    }
                    else
                    {
                        // short cut
                        shortcut = argv[i] + 1;
                    }
                    val = "";
                }
            }
            else // value
            {
                if (name == "" && shortcut == "")
                {
                    ThrowException(Argument, "Expected \"-\" or \"-\" in command line arguments.\n");
                }

                val += argv[i];
            }
        }
    }

    void CommandLine::checkFullArgument(const std::string& name, const std::string& value)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgs_.find(name);
        if (it == cmdLineArgs_.end())
            ThrowException(Argument, "Command line argument '" + name + "' does not exist.");

        it->second->parse(value);
    }

    void CommandLine::checkShortcut(const std::string& shortcut, const std::string& value)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = cmdLineArgsShortcut_.find(shortcut);
        if (it == cmdLineArgsShortcut_.end())
            ThrowException(Argument, "Command line shortcut '" + shortcut + "' does not exist.");

        it->second->parse(value);
    }
}
