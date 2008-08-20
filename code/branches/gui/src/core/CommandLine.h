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

namespace orxonox
{
    class _CoreExport BaseCommandLineArgument
    {
        friend class CommandLine;

    public:
        bool hasDefaultValue() const { return bHasDefaultValue_; }
        const std::string& getName() const { return name_; }
        const std::string& getShortcut() const { return shortcut_; }
        BaseCommandLineArgument& setShortcut(const std::string& shortcut)
        { this->shortcut_ = shortcut; return *this; }

    protected:
        BaseCommandLineArgument(const std::string& name)
            : name_(name)
            , bHasDefaultValue_(true)
        { }

        BaseCommandLineArgument(const BaseCommandLineArgument& instance);
        virtual ~BaseCommandLineArgument() { }

        virtual void parse(const std::string& value) = 0;

        bool bHasDefaultValue_;

    private:
        std::string name_;
        std::string shortcut_;
    };


    template <class T>
    class CommandLineArgument : public BaseCommandLineArgument
    {
        friend class CommandLine;

    public:
        T getValue() const { return value_; }
        T getDefaultValue() const { return defaultValue_; }

    private:
        CommandLineArgument(const std::string& name, const T& defaultValue)
            : BaseCommandLineArgument(name)
            , value_(defaultValue)
            , defaultValue_(defaultValue)
        { }

        virtual void parse(const std::string& value);

        T value_;
        T defaultValue_;
    };

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


    class _CoreExport CommandLine
    {
    public:

        static void parse(int argc, char** argv) { _getInstance()._parse(argc, argv); }

        template <class T>
        static const CommandLineArgument<T>* getCommandLineArgument(const std::string& name);
        template <class T>
        static BaseCommandLineArgument& addCommandLineArgument(const std::string& name,
            const T& defaultValue);

    private:
        CommandLine() : bFirstTimeParse_(true) { }
        CommandLine(const CommandLine& instance);
        ~CommandLine();

        static CommandLine& _getInstance();

        void _parse(int argc, char** argv);
        void checkFullArgument(const std::string& name, const std::string& value);
        void checkShortcut(const std::string& shortcut, const std::string& value);

        bool bFirstTimeParse_;

        std::map<std::string, BaseCommandLineArgument*> cmdLineArgs_;
        std::map<std::string, BaseCommandLineArgument*> cmdLineArgsShortcut_;
    };


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

    template <class T>
    BaseCommandLineArgument& CommandLine::addCommandLineArgument(const std::string& name,
                                             const T& defaultValue)
    {
        std::map<std::string, BaseCommandLineArgument*>::const_iterator it = _getInstance().cmdLineArgs_.find(name);
        OrxAssert(it == _getInstance().cmdLineArgs_.end(),
            "Cannot add a command line argument with name '" + name + "' twice.");

        return *(_getInstance().cmdLineArgs_[name] = new CommandLineArgument<T>(name, defaultValue));
    }
}

#endif /* _CommandLine_H__ */
