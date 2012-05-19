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

#include "CommandLineParser.h"

#include <algorithm>
#include <sstream>

#include "util/Convert.h"
#include "util/Output.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "PathConfig.h"

namespace orxonox
{
    /**
    @brief
        Parses a value string for a command line argument.
        It simply uses convertValue(Output, Input) to do that.
        Bools are treated specially. That is necessary
        so that you can have simple command line switches.
    */
    void CommandLineArgument::parse(const std::string& value)
    {
        if (value_.isType<bool>())
        {
            // simulate command line switch
            bool temp;
            if (convertValue(&temp, value))
            {
                this->bHasDefaultValue_ = false;
                this->value_ = temp;
            }
            else if (value.empty())
            {
                this->bHasDefaultValue_ = false;
                this->value_ = true;
            }
            else
                ThrowException(Argument, "Could not read command line argument '" + getName() + "'.");
        }
        else
        {
            if (!value_.set(value))
            {
                value_.set(defaultValue_);
                ThrowException(Argument, "Could not read command line argument '" + getName() + "'.");
            }
            else
                this->bHasDefaultValue_ = false;
        }
    }


    /**
    @brief
        Destructor destroys all CommandLineArguments with it.
    */
    CommandLineParser::~CommandLineParser()
    {
        CommandLineParser::destroyAllArguments();
    }

    /**
    @brief
        Returns a unique instance (Meyers Singleton).
    */
    CommandLineParser& CommandLineParser::_getInstance()
    {
        static CommandLineParser instance;
        return instance;
    }

    /**
    @brief
        Destroys all command line arguments. This should be called at the end
        of main. Do not use before that.
    */
    void CommandLineParser::destroyAllArguments()
    {
        for (std::map<std::string, CommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.begin();
            it != _getInstance().cmdLineArgs_.end(); ++it)
            delete it->second;
        _getInstance().cmdLineArgs_.clear();
    }

    /** Parses the command line string for arguments and stores these.
    @note
        The reason that you have to provide the string to be parsed as
        space separated list is because of argc and argv. If you only have
        a whole string, simply use getAllStrings() of SubString.
    @param cmdLine
        Command line string WITHOUT the execution path.
    */
    void CommandLineParser::_parse(const std::string& cmdLine)
    {
        std::vector<std::string> arguments;
        SubString tokens(cmdLine, " ", " ", false, '\\', true, '"', true, '\0', '\0', false);
        for (unsigned i = 0; i < tokens.size(); ++i)
            arguments.push_back(tokens[i]);

        try
        {
            // why this? See bFirstTimeParse_ declaration.
            if (bFirstTimeParse_)
            {
                // first shove all the shortcuts in a map
                for (std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgs_.begin();
                    it != cmdLineArgs_.end(); ++it)
                {
                    OrxAssert(cmdLineArgsShortcut_.find(it->second->getShortcut()) == cmdLineArgsShortcut_.end(),
                        "Cannot have two command line shortcut with the same name.");
                    if (!it->second->getShortcut().empty())
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
                            value += arguments[i] + ' ';
                        }
                        else
                        {
                            // can be shortcut or full name argument

                            // save old data first
                            value = removeTrailingWhitespaces(value);
                            if (!name.empty())
                            {
                                checkFullArgument(name, value);
                                name.clear();
                                assert(shortcut.empty());
                            }
                            else if (!shortcut.empty())
                            {
                                checkShortcut(shortcut, value);
                                shortcut.clear();
                                assert(name.empty());
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
                            value.clear();
                        }
                    }
                    else
                    {
                        // value string

                        if (name.empty() && shortcut.empty())
                        {
                            ThrowException(Argument, "Expected \"-\" or \"-\" in command line arguments.\n");
                        }

                        // Concatenate strings as long as there's no new argument by "-" or "--"
                        value += arguments[i] + ' ';
                    }
                }
            }

            // parse last argument
            value = removeTrailingWhitespaces(value);
            if (!name.empty())
            {
                checkFullArgument(name, value);
                assert(shortcut.empty());
            }
            else if (!shortcut.empty())
            {
                checkShortcut(shortcut, value);
                assert(name.empty());
            }
        }
        catch (const ArgumentException& ex)
        {
            orxout(user_error) << "Could not parse command line: " << ex.what() << endl;
            orxout(user_error) << CommandLineParser::getUsageInformation() << endl;
            throw GeneralException("");
        }
    }

    /**
    @brief
        Parses an argument based on its full name.
    @param name
        Full name of the argument
    @param value
        String containing the value
    @param bParsingFile
        Parsing a file or the command line itself
    */
    void CommandLineParser::checkFullArgument(const std::string& name, const std::string& value)
    {
        std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgs_.find(name);
        if (it == cmdLineArgs_.end())
            ThrowException(Argument, "Command line argument '" + name + "' does not exist.");

        it->second->parse(value);
    }

    /**
    @brief
        Parses an argument based on its shortcut.
    @param shortcut
        Shortcut to the argument
    @param value
        String containing the value
    @param bParsingFile
        Parsing a file or the command line itself
    */
    void CommandLineParser::checkShortcut(const std::string& shortcut, const std::string& value)
    {
        std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgsShortcut_.find(shortcut);
        if (it == cmdLineArgsShortcut_.end())
            ThrowException(Argument, "Command line shortcut '" + shortcut + "' does not exist.");

        it->second->parse(value);
    }

    std::string CommandLineParser::getUsageInformation()
    {
        CommandLineParser& inst = _getInstance();
        std::ostringstream infoStr;

        // determine maximum name size
        size_t maxNameSize = 0;
        for (std::map<std::string, CommandLineArgument*>::const_iterator it = inst.cmdLineArgs_.begin();
            it != inst.cmdLineArgs_.end(); ++it)
        {
            maxNameSize = std::max(it->second->getName().size(), maxNameSize);
        }

        infoStr << endl;
        infoStr << "Usage: orxonox [options]" << endl;
        infoStr << "Available options:" << endl;

        for (std::map<std::string, CommandLineArgument*>::const_iterator it = inst.cmdLineArgs_.begin();
            it != inst.cmdLineArgs_.end(); ++it)
        {
            if (!it->second->getShortcut().empty())
                infoStr << " [-" << it->second->getShortcut() << "] ";
            else
                infoStr << "      ";
            infoStr << "--" << it->second->getName() << ' ';
            if (it->second->getValue().isType<bool>())
                infoStr << "    ";
            else
                infoStr << "ARG ";
            // fill with the necessary amount of blanks
            infoStr << std::string(maxNameSize - it->second->getName().size(), ' ');
            infoStr << ": " << it->second->getInformation();
            infoStr << endl;
        }
        return infoStr.str();
    }

    void CommandLineParser::generateDoc(std::ofstream& file)
    {
        file << "/** @page cmdargspage Command Line Arguments Reference" << endl;
        file << "    @verbatim"; /*no endl*/
        file << getUsageInformation(); /*no endl*/
        file << "    @endverbatim" << endl;
        file << "*/" << endl;
    }

    /**
    @brief
        Retrieves a CommandLineArgument.
        The method throws an exception if 'name' was not found or the value could not be converted.
    @note
        You should of course not call this method before the command line has been parsed.
    */
    const CommandLineArgument* CommandLineParser::getArgument(const std::string& name)
    {
        std::map<std::string, CommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.find(name);
        if (it == _getInstance().cmdLineArgs_.end())
        {
            ThrowException(Argument, "Could find command line argument '" + name + "'.");
        }
        else
        {
            return it->second;
        }
    }
}
