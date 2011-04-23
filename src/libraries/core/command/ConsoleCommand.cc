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
    @brief Implementation of the ConsoleCommand class.
*/

#include "ConsoleCommand.h"

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "core/Language.h"
#include "core/GameMode.h"
#include "core/input/KeyBinder.h"
#include "core/input/KeyBinderManager.h"

namespace orxonox
{
    /**
        @brief Constructor: Initializes all values and registers the command.
        @param group The group of the command
        @param name The name of the command
        @param executor The executor of the command
        @param bInitialized If true, the executor is used for both, the definition of the function-header AND to executute the command. If false, the command is inactive and needs to be assigned a function before it can be used.
    */
    ConsoleCommand::ConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized)
    {
        this->bActive_ = true;
        this->bHidden_ = false;
        this->accessLevel_ = AccessLevel::All;

        this->baseName_ = name;
        this->baseFunctor_ = executor->getFunctor();

        for (size_t i = 0; i < MAX_FUNCTOR_ARGUMENTS; ++i)
            this->argumentCompleter_[i] = 0;

        this->keybindMode_ = KeybindMode::OnPress;
        this->inputConfiguredParam_ = -1;

        if (bInitialized)
            this->executor_ = executor;

        ConsoleCommand::registerCommand(group, name, this);
    }

    /**
        @brief Destructor: Unregisters the command.
    */
    ConsoleCommand::~ConsoleCommand()
    {
        ConsoleCommand::unregisterCommand(this);
    }

    /**
        @brief Registers the command with the same name, but without group, as shortcut.
    */
    ConsoleCommand& ConsoleCommand::addShortcut()
    {
        ConsoleCommand::registerCommand("", this->baseName_, this);
        return *this;
    }

    /**
        @brief Registers the command with an alias as shortcut.
    */
    ConsoleCommand& ConsoleCommand::addShortcut(const std::string&  name)
    {
        ConsoleCommand::registerCommand("", name, this);
        return *this;
    }

    /**
        @brief Registers the command in a different group but with the same name.
    */
    ConsoleCommand& ConsoleCommand::addGroup(const std::string& group)
    {
        ConsoleCommand::registerCommand(group, this->baseName_, this);
        return *this;
    }

    /**
        @brief Registers an alias of the command in a different group with a different name.
    */
    ConsoleCommand& ConsoleCommand::addGroup(const std::string& group, const std::string&  name)
    {
        ConsoleCommand::registerCommand(group, name, this);
        return *this;
    }

    /**
        @brief Returns true if the command can be executed right now.

        This returns only true, if the following conditions are met:
         - The command is active
         - The command has an executor
         - The executor has a functor
         - The functor is static or has an object
    */
    bool ConsoleCommand::isActive() const
    {
        return (this->bActive_ && this->executor_ && this->executor_->getFunctor() && (this->executor_->getFunctor()->getType() == Functor::Type::Static || this->executor_->getFunctor()->getRawObjectPointer()));
    }

    /**
        @brief Returns true if the current state of the game matches the required access level.
    */
    bool ConsoleCommand::hasAccess() const
    {
        switch (this->accessLevel_)
        {
            case AccessLevel::All:        return true;
            case AccessLevel::Standalone: return GameMode::isStandalone();
            case AccessLevel::Master:     return GameMode::isMaster();
            case AccessLevel::Server:     return GameMode::isServer();
            case AccessLevel::Client:     return GameMode::isClient();
            case AccessLevel::Online:     return (GameMode::isServer() || GameMode::isClient());
            case AccessLevel::Offline:    return GameMode::isStandalone();
            case AccessLevel::None:       return false;
            default:                      return false;
        }
    }

    /**
        @brief Returns true if the headers of the given functor match the declaration of this command.
    */
    bool ConsoleCommand::headersMatch(const FunctorPtr& functor)
    {
        // get the minimum of the number of parameters of both commands
        unsigned int minparams = std::min(this->baseFunctor_->getParamCount(), functor->getParamCount());

        // if the reduced headers don't match -> return false
        if (this->baseFunctor_->getHeaderIdentifier(minparams) != functor->getHeaderIdentifier(minparams))
            return false;
        // if the reduced headers match and the new functor has less or equal parameters -> return true
        else if (functor->getParamCount() <= this->baseFunctor_->getParamCount())
            return true;
        // the headers match but the new functor has more arguments and there is no executor with default-values -> return false
        else if (!this->executor_)
            return false;
        // the headers match but the new functor has more arguments, check if the executor has enough default-values
        else
        {
            for (unsigned int i = this->baseFunctor_->getParamCount(); i < functor->getParamCount(); ++i)
            {
                if (!this->executor_->defaultValueSet(i))
                {
                    COUT(2) << "Default value " << i << " is missing" << std::endl;
                    return false;
                }
            }

            return true;
        }
    }

    /**
        @brief Returns true if the headers of the given executor match the declaration of this command.
    */
    bool ConsoleCommand::headersMatch(const ExecutorPtr& executor)
    {
        // get the minimum of the number of parameters of both commands
        unsigned int minparams = std::min(this->baseFunctor_->getParamCount(), executor->getParamCount());

        // if the reduced headers don't match -> return false
        if (this->baseFunctor_->getHeaderIdentifier(minparams) != executor->getFunctor()->getHeaderIdentifier(minparams))
            return false;
        // if the reduced headers match and the new functor has less or equal parameters -> return true
        else if (executor->getParamCount() <= this->baseFunctor_->getParamCount())
            return true;
        // the headers match but the new functor has more arguments, check if the new executor has enough default-values
        else
        {
            for (unsigned int i = this->baseFunctor_->getParamCount(); i < executor->getParamCount(); ++i)
            {
                if (!executor->defaultValueSet(i))
                {
                    COUT(2) << "Default value " << i << " is missing" << std::endl;
                    return false;
                }
            }

            return true;
        }
    }

    /**
        @brief Changes the executor.
        @param executor The new executor
        @param bForce If true, the executor is always assigned, even if the headers don't match
        @return Returns true if the assignment was successful
    */
    bool ConsoleCommand::setFunction(const ExecutorPtr& executor, bool bForce)
    {
        // assign the executor if a) it's a null-pointer, b) its functor is a null-pointer, c) it's forced, d) the headers match
        if (!executor || !executor->getFunctor() || bForce || this->headersMatch(executor))
        {
            // assign the executor and clear the object stack (because it's also a new function)
            this->executor_ = executor;
            this->objectStack_.clear();
            return true;
        }
        else
        {
            COUT(1) << "Error: Couldn't assign new executor to console command \"" << this->baseName_ << "\", headers don't match." << std::endl;
            return false;
        }
    }

    /**
        @brief Changes the functor of the current executor.
        @param functor The new functor
        @param bForce If true, the functor is always assigned, even if the headers don't match
        @return Returns true if the assignment was successful
    */
    bool ConsoleCommand::setFunction(const FunctorPtr& functor, bool bForce)
    {
        // assign the functor if a) it's a null-pointer, b) it's forced, c) the headers match
        if (!functor || bForce || this->headersMatch(functor))
        {
            // assign the functor (create a new executor if necessary) and clear the object stack
            if (this->executor_)
                this->executor_->setFunctor(functor);
            else if (functor)
                this->executor_ = createExecutor(functor);
            this->objectStack_.clear();

            return true;
        }
        else
        {
            COUT(1) << "Error: Couldn't assign new functor to console command \"" << this->baseName_ << "\", headers don't match." << std::endl;
            return false;
        }
    }

    /**
        @brief Pushes a new executor to the command-stack.
        @param executor The new executor
        @param bForce If true, the executor is always assigned, even if the headers don't match
    */
    void ConsoleCommand::pushFunction(const ExecutorPtr& executor, bool bForce)
    {
        // prepare the old function to be put on the stack
        Command command;
        command.executor_ = this->executor_;
        if (command.executor_)
            command.functor_ = this->executor_->getFunctor();
        command.objectStack_ = this->objectStack_;

        // check if the new executor can be assigned and push the old function to the stack
        if (this->setFunction(executor, bForce))
            this->commandStack_.push(command);
    }

    /**
        @brief Pushes a new functor to the command-stack.
        @param functor The new functor
        @param bForce If true, the functor is always assigned, even if the headers don't match
    */
    void ConsoleCommand::pushFunction(const FunctorPtr& functor, bool bForce)
    {
        // prepare the old function to be put on the stack
        Command command;
        command.executor_ = this->executor_;
        if (command.executor_)
            command.functor_ = this->executor_->getFunctor();
        command.objectStack_ = this->objectStack_;

        // check if the new functor can be assigned and push the old function to the stack
        if (this->setFunction(functor, bForce))
            this->commandStack_.push(command);
    }

    /**
        @brief Pushes a copy of the current executor and functor on the stack.
    */
    void ConsoleCommand::pushFunction()
    {
        if (this->executor_)
            this->pushFunction(new Executor(*this->executor_.get()));
        else
            COUT(1) << "Error: Couldn't push copy of executor in console command \"" << this->baseName_ << "\", no executor set." << std::endl;
    }

    /**
        @brief Removes the current function from the stack and restores the old state.
    */
    void ConsoleCommand::popFunction()
    {
        Command command;

        // check if there's a function on the stack
        if (!this->commandStack_.empty())
        {
            // yes it is - assign it to command and remove it from the stack
            command = this->commandStack_.top();
            this->commandStack_.pop();
        }

        // restore the old executor (and also restore its functor in case this was changed in the meantime)
        this->executor_ = command.executor_;
        if (command.executor_)
            this->executor_->setFunctor(command.functor_);
        this->objectStack_ = command.objectStack_;
    }

    /**
        @brief Sets the functor to NULL (which also deactivates the command).
    */
    void ConsoleCommand::resetFunction()
    {
        if (this->executor_)
            this->executor_->setFunctor(0);
        this->objectStack_.clear();
    }

    /**
        @brief Returns the current executor which can be used to execute the command.
    */
    const ExecutorPtr& ConsoleCommand::getExecutor() const
    {
        return this->executor_;
    }

    /**
        @brief Changes the current object that is used to execute member-functions.
        @return Returns true if the object was successfully changed
    */
    bool ConsoleCommand::setObject(void* object)
    {
        // check if there's an executor
        if (this->executor_)
        {
            // check if there's a functor
            if (this->executor_->getFunctor())
            {
                // change the object
                this->executor_->getFunctor()->setRawObjectPointer(object);
                return true;
            }
            else if (object)
                COUT(1) << "Error: Can't assign object to console command \"" << this->baseName_ << "\", no functor set." << std::endl;
        }
        else if (object)
            COUT(1) << "Error: Can't assign object to console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return false;
    }

    /**
        @brief Push a new object to the object-stack.
    */
    void ConsoleCommand::pushObject(void* object)
    {
        void* oldobject = this->getObject();
        if (this->setObject(object))
            this->objectStack_.push_back(oldobject);
    }

    /**
        @brief Removes the current object from the stack an restores the old object.
    */
    void ConsoleCommand::popObject()
    {
        void* newobject = 0;
        if (!this->objectStack_.empty())
        {
            newobject = this->objectStack_.back();
            this->objectStack_.pop_back();
        }
        this->setObject(newobject);
    }

    /**
        @brief Returns the current object pointer that is used to execute member-functions.
    */
    void* ConsoleCommand::getObject() const
    {
        if (this->executor_ && this->executor_->getFunctor())
            return this->executor_->getFunctor()->getRawObjectPointer();
        else
            return 0;
    }

    /**
        @brief Changes the default values of the current executor.
    */
    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& arg1)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(arg1);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the default values of the current executor.
    */
    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& arg1, const MultiType& arg2)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(arg1, arg2);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the default values of the current executor.
    */
    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(arg1, arg2, arg3);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the default values of the current executor.
    */
    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(arg1, arg2, arg3, arg4);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the default values of the current executor.
    */
    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(arg1, arg2, arg3, arg4, arg5);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the default value of the argument with given index of the current executor.
        @param index The index of the argument (the first argument has index 0)
        @param arg The new default value
    */
    ConsoleCommand& ConsoleCommand::defaultValue(unsigned int index, const MultiType& arg)
    {
        if (this->executor_)
            this->executor_->setDefaultValue(index, arg);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    /**
        @brief Changes the argument completer for the given argument.
        @param index The index of the argument (the first argument has index 0)
        @param completer The new argument completer
    */
    ConsoleCommand& ConsoleCommand::argumentCompleter(unsigned int index, ArgumentCompleter* completer)
    {
        if (index < 5)
            this->argumentCompleter_[index] = completer;
        else
            COUT(2) << "Warning: Couldn't add autocompletion-function for index " << index << " in console command \"" << this->baseName_ << "\": index out of bound." << std::endl;

        return *this;
    }

    /**
        @brief Returns the argument completer for the argument with given index.
    */
    ArgumentCompleter* ConsoleCommand::getArgumentCompleter(unsigned int index) const
    {
        if (index < 5)
            return this->argumentCompleter_[index];
        else
            return 0;
    }

    /**
        @brief Sets the description of this command.
    */
    ConsoleCommand& ConsoleCommand::description(const std::string& description)
    {
        this->description_ = std::string("ConsoleCommandDescription::" + this->baseName_ + "::function");
        AddLanguageEntry(this->description_, description);
        return *this;
    }

    /**
        @brief Returns the description of this command.
    */
    const std::string& ConsoleCommand::getDescription() const
    {
        return GetLocalisation_noerror(this->description_);
    }

    /**
        @brief Sets the description for an argument with given index.
    */
    ConsoleCommand& ConsoleCommand::descriptionParam(unsigned int index, const std::string& description)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
        {
            this->descriptionParam_[index] = std::string("ConsoleCommandDescription::" + this->baseName_ + "::param" + multi_cast<std::string>(index));
            AddLanguageEntry(this->descriptionParam_[index], description);
        }
        return *this;
    }

    /**
        @brief Returns the description for the argument with given index.
    */
    const std::string& ConsoleCommand::getDescriptionParam(unsigned int index) const
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            return GetLocalisation_noerror(this->descriptionParam_[index]);

        return this->descriptionParam_[0];
    }

    /**
        @brief Sets the description for the return-value.
    */
    ConsoleCommand& ConsoleCommand::descriptionReturnvalue(const std::string& description)
    {
        this->descriptionReturnvalue_ = std::string("ConsoleCommandDescription::" + this->baseName_ + "::returnvalue");
        AddLanguageEntry(this->descriptionReturnvalue_, description);
        return *this;
    }

    /**
        @brief Returns the description for the return-value.
    */
    const std::string& ConsoleCommand::getDescriptionReturnvalue(int index) const
    {
        return GetLocalisation_noerror(this->descriptionReturnvalue_);
    }

    /**
        @brief Changes the keybind mode.
    */
    ConsoleCommand& ConsoleCommand::changeKeybindMode(KeybindMode::Value mode)
    {
        if( KeyBinderManager::exists() )
            KeyBinderManager::getInstance().getCurrent()->changeMode(this, mode);

        this->keybindMode(mode);
        return *this;
    }

    /**
        @brief Returns the command with given group an name.
        @param group The group of the requested command
        @param name The group of the requested command
        @param bPrintError If true, an error is printed if the command doesn't exist
    */
    /* static */ const ConsoleCommand* ConsoleCommand::getCommand(const std::string& group, const std::string& name, bool bPrintError)
    {
        // find the group
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandMap().find(group);
        if (it_group != ConsoleCommand::getCommandMap().end())
        {
            // find the name
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(name);
            if (it_name != it_group->second.end())
            {
                // return the pointer
                return it_name->second;
            }
        }
        if (bPrintError)
        {
            if (group == "")
                COUT(1) << "Error: Couldn't find console command with shortcut \"" << name << "\"" << std::endl;
            else
                COUT(1) << "Error: Couldn't find console command with group \"" << group << "\" and name \"" << name << "\"" << std::endl;
        }
        return 0;
    }

    /**
        @brief Returns the command with given group an name in lowercase.
        @param group The group of the requested command in lowercase
        @param name The group of the requested command in lowercase
        @param bPrintError If true, an error is printed if the command doesn't exist
    */
    /* static */ const ConsoleCommand* ConsoleCommand::getCommandLC(const std::string& group, const std::string& name, bool bPrintError)
    {
        std::string groupLC = getLowercase(group);
        std::string nameLC = getLowercase(name);

        // find the group
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandMapLC().find(groupLC);
        if (it_group != ConsoleCommand::getCommandMapLC().end())
        {
            // find the name
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(nameLC);
            if (it_name != it_group->second.end())
            {
                // return the pointer
                return it_name->second;
            }
        }
        if (bPrintError)
        {
            if (group == "")
                COUT(1) << "Error: Couldn't find console command with shortcut \"" << name << "\"" << std::endl;
            else
                COUT(1) << "Error: Couldn't find console command with group \"" << group << "\" and name \"" << name << "\"" << std::endl;
        }
        return 0;
    }

    /**
        @brief Returns the static map that stores all console commands.
    */
    /* static */ std::map<std::string, std::map<std::string, ConsoleCommand*> >& ConsoleCommand::getCommandMap()
    {
        static std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMap;
        return commandMap;
    }

    /**
        @brief Returns the static map that stores all console commands in lowercase.
    */
    /* static */ std::map<std::string, std::map<std::string, ConsoleCommand*> >& ConsoleCommand::getCommandMapLC()
    {
        static std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMapLC;
        return commandMapLC;
    }

    /**
        @brief Registers a new command with given group an name by adding it to the command map.
    */
    /* static */ void ConsoleCommand::registerCommand(const std::string& group, const std::string& name, ConsoleCommand* command)
    {
        if (name == "")
            return;

        // check if a command with this name already exists
        if (ConsoleCommand::getCommand(group, name) != 0)
        {
            if (group == "")
                COUT(2) << "Warning: A console command with shortcut \"" << name << "\" already exists." << std::endl;
            else
                COUT(2) << "Warning: A console command with name \"" << name << "\" already exists in group \"" << group << "\"." << std::endl;
        }
        else
        {
            // add the command to the map
            ConsoleCommand::getCommandMap()[group][name] = command;
            ConsoleCommand::getCommandMapLC()[getLowercase(group)][getLowercase(name)] = command;
        }
    }

    /**
        @brief Removes the command from the command map.
    */
    /* static */ void ConsoleCommand::unregisterCommand(ConsoleCommand* command)
    {
        // iterate through all groups
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = ConsoleCommand::getCommandMap().begin(); it_group != ConsoleCommand::getCommandMap().end(); )
        {
            // iterate through all commands of each group
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                // erase the command
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            // erase the group if it is empty now
            if (it_group->second.empty())
                ConsoleCommand::getCommandMap().erase(it_group++);
            else
                ++it_group;
        }

        // now the same for the lowercase-map:

        // iterate through all groups
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = ConsoleCommand::getCommandMapLC().begin(); it_group != ConsoleCommand::getCommandMapLC().end(); )
        {
            // iterate through all commands of each group
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                // erase the command
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            // erase the group if it is empty now
            if (it_group->second.empty())
                ConsoleCommand::getCommandMapLC().erase(it_group++);
            else
                ++it_group;
        }
    }

    /**
        @brief Deletes all commands
    */
    /* static */ void ConsoleCommand::destroyAll()
    {
        // delete entries until the map is empty
        while (!ConsoleCommand::getCommandMap().empty() && !ConsoleCommand::getCommandMap().begin()->second.empty())
            delete ConsoleCommand::getCommandMap().begin()->second.begin()->second;
    }
}
