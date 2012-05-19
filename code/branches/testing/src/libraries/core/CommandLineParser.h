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
    @defgroup CmdArgs Commandline arguments
    @ingroup Config
    @brief For a reference of all commandline arguments see @ref cmdargspage
*/

/**
    @file
    @ingroup Config CmdArgs
    @brief Declaration of CommandLineParser and CommandLineArgument, definition of the SetCommandLineArgument() macros.
*/

#ifndef _CommandLine_H__
#define _CommandLine_H__

#include "CorePrereqs.h"

#include <fstream>
#include <map>
#include "util/OrxAssert.h"
#include "util/MultiType.h"

#define SetCommandLineArgument(name, defaultValue) \
    orxonox::CommandLineArgument& CmdArgumentDummyBoolVar##name \
    = orxonox::CommandLineParser::addArgument(#name, defaultValue)
#define SetCommandLineSwitch(name) \
    orxonox::CommandLineArgument& CmdArgumentDummyBoolVar##name \
    = orxonox::CommandLineParser::addArgument(#name, false)

namespace orxonox
{
    /**
    @brief
        Container class for a command line argument of any type supported by MultiType.

        Whenever you want to have an option specified by a command line switch,
        you need to first define it with SetCommandLineArgument(name, defaultValue).
        It is then added to a map and possibly changed when the command line is being parsed.
        If the option was not given, you can detect this by asking hasDefaultValue().

        There is a possibility to define a short cut so you can write "-p 20" instead of "--port 20".
        Note the difference between "-" and "--"!
        Also, there is no restriction to the number of strings you add after --name.
        So "--startVector {2, 4, 5}" is perfectly legal.

        Retrieving an argument is possible with the getCommandLineArgument function of the
        CommandLineParser class. It is a Singleton, but the public interface is static.
    */
    class _CoreExport CommandLineArgument
    {
        friend class CommandLineParser;

    public:
        //! Tells whether the value has been changed by the command line.
        bool hasDefaultValue() const { return bHasDefaultValue_; }
        //! Returns the name of the argument.
        const std::string& getName() const { return name_; }

        //! Returns the shortcut (example: "-p 22" for "--port 22") of the argument.
        //! Evaluates to "" if there is none.
        const std::string& getShortcut() const { return shortcut_; }
        //! Sets the shortcut for the argument
        CommandLineArgument& shortcut(const std::string& shortcut)
        { this->shortcut_ = shortcut; return *this; }

        //! Returns the usage information
        const std::string& getInformation() const { return this->usageInformation_; }
        //! Sets the option information when displaying orxonox usage.
        CommandLineArgument& information(const std::string& usage)
        { this->usageInformation_ = usage; return *this; }

        //! Returns the actual value of the argument. Can be equal to default value.
        const MultiType& getValue() const { return value_; }
        //! Returns the given default value as type T.
        const MultiType& getDefaultValue() const { return defaultValue_; }

    private:
        //! Constructor initialises both value_ and defaultValue_ with defaultValue.
        CommandLineArgument(const std::string& name, const MultiType& defaultValue)
            : bHasDefaultValue_(true)
            , name_(name)
            , value_(defaultValue)
            , defaultValue_(defaultValue)
        { }

        //! Undefined copy constructor
        CommandLineArgument(const CommandLineArgument& instance);
        ~CommandLineArgument() { }

        //! Parses the value string of a command line argument.
        void parse(const std::string& value);

        //! Tells whether the value has been changed by the command line.
        bool bHasDefaultValue_;

    private:
        std::string name_;             //!< Name of the argument
        std::string shortcut_;         //!< Shortcut of the argument. @see getShortcut().
        std::string usageInformation_; //!< Tells about the usage of this parameter

        MultiType   value_;            //!< The actual value
        MultiType   defaultValue_;     //!< Default value. Should not be changed.
    };


    /**
    @brief
        Global interface to command line options.
        Allows to add and retrieve command line arguments. Also does the parsing.
    @note
        Internally it is a Singleton, but the public interface is static.
    @see
        CommandLineArgument
    */
    class _CoreExport CommandLineParser
    {
    public:

        //! Parse redirection to internal member method.
        static void parse(const std::string& cmdLine)
        { _getInstance()._parse(cmdLine); }

        static std::string getUsageInformation();

        static const CommandLineArgument* getArgument(const std::string& name);
        //! Writes the argument value in the given parameter.
        template <class T>
        static void getValue(const std::string& name, T* value)
        { *value = (T)(getArgument(name)->getValue()); }
        static const MultiType& getValue(const std::string& name)
        { return getArgument(name)->getValue(); }
        template <class T>
        static CommandLineArgument& addArgument(const std::string& name, T defaultValue);

        static bool existsArgument(const std::string& name)
        {
            std::map<std::string, CommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.find(name);
            return !(it == _getInstance().cmdLineArgs_.end());
        }

        static void destroyAllArguments();

        static void generateDoc(std::ofstream& file);

    private:
        //! Constructor initialises bFirstTimeParse_ with true.
        CommandLineParser() : bFirstTimeParse_(true) { }
        //! Undefined copy constructor
        CommandLineParser(const CommandLineParser& instance);
        ~CommandLineParser();

        static CommandLineParser& _getInstance();

        void _parse(const std::string& cmdLine);
        void checkFullArgument(const std::string& name, const std::string& value);
        void checkShortcut(const std::string& shortcut, const std::string& value);

        /**
            Tells whether we parsed for the first time. The CommmandLineArguments are added before main().
            So when we call parse() the first time, we need to create a map with all shortcuts since these
            get added after addCommandLineArgument().
        */
        bool bFirstTimeParse_;

        //! Holds all pointers to the arguments and serves as a search map by name.
        std::map<std::string, CommandLineArgument*> cmdLineArgs_;
        //! Search map by shortcut for the arguments.
        std::map<std::string, CommandLineArgument*> cmdLineArgsShortcut_;
    };

    template <>
    inline void CommandLineParser::getValue<std::string>(const std::string& name, std::string* value)
    {
        *value = getArgument(name)->getValue().get<std::string>();
    }

    /**
    @brief
        Adds a new CommandLineArgument to the internal map.
        Note that only such arguments are actually valid.
    @param name
        Name of the argument. Shortcut can be added later.
    @param defaultValue
        Default value that is used when argument was not given.
    */
    template <class T>
    CommandLineArgument& CommandLineParser::addArgument(const std::string& name, T defaultValue)
    {
        OrxAssert(!_getInstance().existsArgument(name),
            "Cannot add a command line argument with name '" + name + "' twice.");
        OrxAssert(!MultiType(defaultValue).isType<bool>() || MultiType(defaultValue).get<bool>() != true,
               "Boolean command line arguments with positive default values are not supported." << endl
            << "Please use SetCommandLineSwitch and adjust your argument: " << name);

        return *(_getInstance().cmdLineArgs_[name] = new CommandLineArgument(name, defaultValue));
    }
}

#endif /* _CommandLine_H__ */
