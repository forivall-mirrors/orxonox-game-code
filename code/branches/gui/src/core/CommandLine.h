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

#ifndef _CommandLine_H__
#define _CommandLine_H__

#include "CorePrereqs.h"
#include <map>
#include "util/Convert.h"
#include "Debug.h"
#include "Exception.h"

#define SetCommandLineArgument(name, defaultValue) \
    BaseCommandLineArgument& CmdArgumentDummyBoolVar##name \
    = orxonox::CommandLine::addCommandLineArgument(#name, defaultValue)
#define SetCommandLineSwitch(name) \
    BaseCommandLineArgument& CmdArgumentDummyBoolVar##name \
    = orxonox::CommandLine::addCommandLineArgument(#name, false)


namespace orxonox
{
    /**
    @brief
        Template struct which creates a distinct type for every integer constant.
    @note
        This allows to select a different function depending on a boolean value
        when creating a new CommandLineArgument.
        From 'Modern C++ Design' (Alexandrescu 2001).
    */
    template <int v>
    struct Int2Type
    {
        enum { value = v };
    };

    /**
    @brief
        Base class for CommandLineArguments.
    */
    class _CoreExport BaseCommandLineArgument
    {
        friend class CommandLine;

    public:
        //! Tells whether the value has been changed by the command line.
        bool hasDefaultValue() const { return bHasDefaultValue_; }
        //! Returns the name of the argument.
        const std::string& getName() const { return name_; }

        //! Returns the shortcut (example: "-p 22" for "--port 22") of the argument.
        //! Evaluates to "" if none there is none.
        const std::string& getShortcut() const { return shortcut_; }
        //! Sets the shortcut for the argument
        BaseCommandLineArgument& setShortcut(const std::string& shortcut)
        { this->shortcut_ = shortcut; return *this; }

        //! Returns the usage information
        const std::string& getInformation() const { return this->usageInformation_; }
        //! Sets the option information when displaying orxonox usage.
        BaseCommandLineArgument& setInformation(const std::string& usage)
        { this->usageInformation_ = usage; return *this; }

    protected:
        BaseCommandLineArgument(const std::string& name)
            : name_(name)
            , bHasDefaultValue_(true)
        { }

        //! Undefined copy constructor
        BaseCommandLineArgument(const BaseCommandLineArgument& instance);
        virtual ~BaseCommandLineArgument() { }

        //! Parses the value string of a command line argument.
        virtual void parse(const std::string& value) = 0;

        //! Tells whether the value has been changed by the command line.
        bool bHasDefaultValue_;

    private:
        std::string name_;             //!< Name of the argument
        std::string shortcut_;         //!< Shortcut of the argument. @see getShortcut().
        std::string usageInformation_; //!< Tells about the usage of this parameter
    };


    /**
    @brief
        Container class for a command line argument of type T.
        
        Whenever you want to have an option specified by a command line switch,
        you need to first define it with SetCommandLineArgument(name, defaultValue).
        It is then added to a map and possibly changed when the command line is being parsed.
        If the option was not given, you can detect this by asking hasDefaultValue().
        
        There is a possibility to define a short cut so you can write "-p 20" instead of "--port 20".
        Note the difference between "-" and "--"!
        Also, there is no restriction to the number of strings you add after --name.
        So "--startVector (2, 4, 5)" is perfectly legal.

        Retrieving an argument is possible with the getCommandLineArgument function of the
        CommandLine class. It is a Singleton, but the public interface is static.
    */
    template <class T>
    class CommandLineArgument : public BaseCommandLineArgument
    {
        // Only let CommandLine change the value.
        friend class CommandLine;

    public:
        //! Returns the actual value of the argument. Can be equal to default value.
        T getValue() const { return value_; }
        //! Returns the given default value as type T.
        T getDefaultValue() const { return defaultValue_; }

    private:
        //! Constructor initialises both value_ and defaultValue_ with defaultValue.
        CommandLineArgument(const std::string& name, const T& defaultValue)
            : BaseCommandLineArgument(name)
            , value_(defaultValue)
            , defaultValue_(defaultValue)
        { }

        virtual void parse(const std::string& value);

        T value_;            //!< The actual value
        T defaultValue_;     //!< Default value. Should not be changed.
    };

    /**
    @brief
        Parses a value string for a command line argument.
        It simply uses convertValue(Output, Input) to do that.
    */
    template <class T>
    void CommandLineArgument<T>::parse(const std::string& value)
    {
        if (convertValue(&this->value_, value))
        {
            this->bHasDefaultValue_ = false;
        }
        else
        {
            ThrowException(Argument, "Could not read command line argument '" + getName() + "'.");
        }
    }

    /**
    @brief
        Parses a value string for a command line argument.
        It simply uses convertValue(Output, Input) to do that.
        This is a template specialisation for bool type. That is necessary
        so that you can have simple command line switches.
    */
    template <>
    inline void CommandLineArgument<bool>::parse(const std::string& value)
    {
        if (convertValue(&this->value_, value))
        {
            this->bHasDefaultValue_ = false;
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


    /**
    @brief
        Global interface to command line options.
        Allows to add and retrieve command line arguments. Also does the parsing.
    @note
        Internally it is a Singleton, but the public interface is static.
    @see
        CommandLineArgument
    */
    class _CoreExport CommandLine
    {
    public:

        //! Parse redirection to internal member method.
        static void parse(const std::vector<std::string>& arguments) { _getInstance()._parse(arguments); }

        static std::string getUsageInformation();

        template <class T>
        static const CommandLineArgument<T>* getCommandLineArgument(const std::string& name);
        //! Writes the argument value in the given parameter.
        template <class T>
        static void getCommandLineValue(const std::string& name, T* value)
        { *value = getCommandLineArgument<T>(name)->getValue(); }
        template <class T>
        static BaseCommandLineArgument& addCommandLineArgument(const std::string& name, T defaultValue);

    private:
        //! Constructor initialises bFirstTimeParse_ with true.
        CommandLine() : bFirstTimeParse_(true) { }
        //! Undefined copy constructor
        CommandLine(const CommandLine& instance);
        ~CommandLine();

        static CommandLine& _getInstance();

        void _parse(const std::vector<std::string>& arguments);
        void checkFullArgument(const std::string& name, const std::string& value);
        void checkShortcut(const std::string& shortcut, const std::string& value);

        template <class T>
        BaseCommandLineArgument* createArg(const std::string& name, T defaultValue, Int2Type<0> isString);
        template <class T>
        BaseCommandLineArgument* createArg(const std::string& name, T defaultValue, Int2Type<1> isString);

        /**
            Tells whether we parse the first expression. The CommmandLineArguments are added before main().
            So when we call parse() the first time, we need to create a map with all shortcuts since these
            get added after addCommandLineArgument().
        */
        bool bFirstTimeParse_;

        //! Holds all pointers to the arguments and serves as a search map by name.
        std::map<std::string, BaseCommandLineArgument*> cmdLineArgs_;
        //! Search map by chortcut for the arguments.
        std::map<std::string, BaseCommandLineArgument*> cmdLineArgsShortcut_;
    };


    /**
    @brief
        Retrieves a CommandLineArgument.
        The method throws an exception if 'name' was not found or the value could not be converted.
    @note
        You shold of course not call this method before the command line has been parsed.
    */
    template <class T>
    const CommandLineArgument<T>* CommandLine::getCommandLineArgument(const std::string& name)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.find(name);
        if (it == _getInstance().cmdLineArgs_.end())
        {
            ThrowException(Argument, "Could find command line argument '" + name + "'.");
        }
        else
        {
            CommandLineArgument<T>* arg = dynamic_cast<CommandLineArgument<T>* >(it->second);
            if (arg == 0)
            {
                ThrowException(Argument, "Could not convert command line argument value to requested type. " \
                    "You should use exactly the same type as predefined.");
            }
            else
            {
                return arg;
            }
        }
    }

    /**
    @brief
        Adds a new CommandLineArgument to the internal map.
        Note that only such arguments are actually valid.
    @param name
        Name of the argument. Shortcut can be added later.
    @param defaultValue
        Default value that is used when argument was not given.
    @note
        In order to store char* strings as std::string too, there's
        little bit of template programming involved:
        StaticConversions::exists determines whether T converts to std::string.
        Int2Type<int> is then used to call the right function. One returns
        a CommandLineArgument<T> and the other CommandLineArgument<std::string>.
    */
    template <class T>
    BaseCommandLineArgument& CommandLine::addCommandLineArgument(const std::string& name, T defaultValue)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.find(name);
        OrxAssert(it == _getInstance().cmdLineArgs_.end(),
            "Cannot add a command line argument with name '" + name + "' twice.");

        return *(_getInstance().cmdLineArgs_[name] = 
            _getInstance().createArg(name, defaultValue, Int2Type<StaticConversion<T, std::string>::exists>()));
    }

    /**
        Returns a new CommandLineArgument<T>.
    */
    template <class T>
    BaseCommandLineArgument* CommandLine::createArg(const std::string& name, T defaultValue, Int2Type<0> isPrime)
    {
        return new CommandLineArgument<T>(name, defaultValue);
    }

    /**
        Returns a new CommandLineArgument<std::string>.
    */
    template <class T>
    BaseCommandLineArgument* CommandLine::createArg(const std::string& name, T defaultValue, Int2Type<1> isPrime)
    {
        return new CommandLineArgument<std::string>(name, defaultValue);
    }
}

#endif /* _CommandLine_H__ */
