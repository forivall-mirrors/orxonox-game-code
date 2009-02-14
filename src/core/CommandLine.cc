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

#include "util/String.h"
#include "util/SubString.h"

namespace orxonox
{
    SetCommandLineArgument(optionsFile, "start.ini").shortcut("o");

    /**
    @brief
        Parses a value string for a command line argument.
        It simply uses convertValue(Output, Input) to do that.
        Bools are treated specially. That is necessary
        so that you can have simple command line switches.
    */
    void CommandLineArgument::parse(const std::string& value)
    {
        if (value_.getType() == MT_bool)
        {
            // simulate command line switch
            bool temp;
            if (convertValue(&temp, value))
            {
                this->bHasDefaultValue_ = false;
                this->value_ = temp;
            }
            else if (value == "")
            {
                this->bHasDefaultValue_ = false;
                this->value_ = true;
            }
            else
            {
                ThrowException(Argument, "Could not read command line argument '" + getName() + "'.");
            }
        }
        else
        {
            if (!value_.setValue(value))
            {
                value_.setValue(defaultValue_);
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
    CommandLine::~CommandLine()
    {
        CommandLine::destroyAllArguments();
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
        Destroys all command line arguments. This should be called at the end
        of main. Do not use before that.
    */
    void CommandLine::destroyAllArguments()
    {
        for (std::map<std::string, CommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.begin();
            it != _getInstance().cmdLineArgs_.end(); ++it)
            delete it->second;
        _getInstance().cmdLineArgs_.clear();
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
            for (std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgs_.begin();
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
                        value = removeTrailingWhitespaces(value);
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
        value = removeTrailingWhitespaces(value);
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
        std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgs_.find(name);
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
        std::map<std::string, CommandLineArgument*>::const_iterator it = cmdLineArgsShortcut_.find(shortcut);
        if (it == cmdLineArgsShortcut_.end())
            ThrowException(Argument, "Command line shortcut '" + shortcut + "' does not exist.");

        it->second->parse(value);
    }

    std::string CommandLine::getUsageInformation()
    {
        CommandLine* inst = &_getInstance();
        std::string infoStr;
        for (std::map<std::string, CommandLineArgument*>::const_iterator it = inst->cmdLineArgs_.begin();
            it != inst->cmdLineArgs_.end(); ++it)
        {
            infoStr += "[--" + it->second->getName() + " " + it->second->getInformation() + "] ";
        }
        return infoStr;
    }

    /**
    @brief
        Retrieves a CommandLineArgument.
        The method throws an exception if 'name' was not found or the value could not be converted.
    @note
        You shold of course not call this method before the command line has been parsed.
    */
    const CommandLineArgument* CommandLine::getArgument(const std::string& name)
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

    /**
    @brief
        Parses both command line and start.ini for CommandLineArguments.
    */
    void CommandLine::_parseAll(int argc, char** argv)
    {
        // parse command line first
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i)
            args.push_back(argv[i]);
        this->_parse(args);

        // look for additional arguments in given file or start.ini as default
        // They will not overwrite the arguments given directly
        std::ifstream file;
        std::string filename = CommandLine::getValue("optionsFile").getString();
        file.open(filename.c_str());
        args.clear();
        if (file)
        {
            while (!file.eof())
            {
                std::string line;
                std::getline(file, line);
                line = removeTrailingWhitespaces(line);
                //if (!(line[0] == '#' || line[0] == '%'))
                //{
                SubString tokens(line, " ", " ", false, 92, false, 34, false, 40, 41, false, '#');
                for (unsigned i = 0; i < tokens.size(); ++i)
                    if (tokens[i][0] != '#')
                        args.push_back(tokens[i]);
                //args.insert(args.end(), tokens.getAllStrings().begin(), tokens.getAllStrings().end());
                //}
            }
            file.close();
        }

        try
        {
            _parse(args);
        }
        catch (orxonox::ArgumentException& ex)
        {
            COUT(1) << "An Exception occured while parsing " << filename << std::endl;
            throw(ex);
        }
    }
}
