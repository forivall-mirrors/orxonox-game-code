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

#include "ConsoleCommand.h"

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "core/Language.h"
#include "core/GameMode.h"

namespace orxonox
{
    ConsoleCommand::ConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized)
    {
        this->bActive_ = true;
        this->bHidden_ = false;
        this->accessLevel_ = AccessLevel::All;

        this->baseName_ = name;
        this->baseExecutor_ = executor;

        this->argumentCompleter_[0] = 0;
        this->argumentCompleter_[1] = 0;
        this->argumentCompleter_[2] = 0;
        this->argumentCompleter_[3] = 0;
        this->argumentCompleter_[4] = 0;

        this->keybindMode_ = KeybindMode::OnPress;
        this->inputConfiguredParam_ = -1;

        if (bInitialized)
            this->executor_ = executor;

        ConsoleCommand::registerCommand(group, name, this);
    }

    ConsoleCommand::~ConsoleCommand()
    {
        ConsoleCommand::unregisterCommand(this);
    }

    ConsoleCommand& ConsoleCommand::addShortcut()
    {
        ConsoleCommand::registerCommand("", this->baseName_, this);
        return *this;
    }

    ConsoleCommand& ConsoleCommand::addShortcut(const std::string&  name)
    {
        ConsoleCommand::registerCommand("", name, this);
        return *this;
    }

    ConsoleCommand& ConsoleCommand::addGroup(const std::string& group)
    {
        ConsoleCommand::registerCommand(group, this->baseName_, this);
        return *this;
    }

    ConsoleCommand& ConsoleCommand::addGroup(const std::string& group, const std::string&  name)
    {
        ConsoleCommand::registerCommand(group, name, this);
        return *this;
    }

    bool ConsoleCommand::isActive() const
    {
        return (this->bActive_ && this->executor_ && this->executor_->getFunctor() && (this->executor_->getFunctor()->getType() == Functor::Type::Static || this->executor_->getFunctor()->getRawObjectPointer()));
    }

    bool ConsoleCommand::hasAccess() const
    {
        switch (this->accessLevel_)
        {
            case AccessLevel::All:        return true;
            case AccessLevel::Standalone: return GameMode::isStandalone();
            case AccessLevel::Master:     return GameMode::isMaster();
            case AccessLevel::Server:     return GameMode::hasServer();
            case AccessLevel::Client:     return GameMode::isClient();
            case AccessLevel::Online:     return (GameMode::hasServer() || GameMode::isClient());
            case AccessLevel::Offline:    return GameMode::isStandalone();
            case AccessLevel::None:       return false;
            default:                      return false;
        }
    }

    bool ConsoleCommand::headersMatch(const FunctorPtr& functor)
    {
        unsigned int minparams = std::min(this->baseExecutor_->getParamCount(), functor->getParamCount());

        if (this->baseExecutor_->getFunctor()->getHeaderIdentifier(minparams) != functor->getHeaderIdentifier(minparams))
            return false;
        else if (functor->getParamCount() <= this->baseExecutor_->getParamCount())
            return true;
        else if (!this->executor_)
            return false;
        else
        {
            for (unsigned int i = this->baseExecutor_->getParamCount(); i < functor->getParamCount(); ++i)
                if (!this->executor_->defaultValueSet(i))
                    return false;

            return true;
        }
    }

    bool ConsoleCommand::headersMatch(const ExecutorPtr& executor)
    {
        unsigned int minparams = std::min(this->baseExecutor_->getParamCount(), executor->getParamCount());

        if (this->baseExecutor_->getFunctor()->getHeaderIdentifier(minparams) != executor->getFunctor()->getHeaderIdentifier(minparams))
            return false;
        else if (executor->getParamCount() <= this->baseExecutor_->getParamCount())
            return true;
        else
        {
            for (unsigned int i = this->baseExecutor_->getParamCount(); i < executor->getParamCount(); ++i)
                if (!executor->defaultValueSet(i))
                    return false;

            return true;
        }
    }

    bool ConsoleCommand::setFunction(const ExecutorPtr& executor, bool bForce)
    {
        if (!executor || !executor->getFunctor() || bForce || this->headersMatch(executor))
        {
            this->executor_ = executor;
            return true;
        }
        else
        {
            COUT(1) << "Error: Couldn't assign new executor to console command \"" << this->baseName_ << "\", headers don't match." << std::endl;
            return false;
        }
    }

    bool ConsoleCommand::setFunction(const FunctorPtr& functor, bool bForce)
    {
        if (!functor || bForce || this->headersMatch(functor))
        {
            if (this->executor_)
                this->executor_->setFunctor(functor);
            else if (functor)
                this->executor_ = createExecutor(functor);

            return true;
        }
        else
        {
            COUT(1) << "Error: Couldn't assign new functor to console command \"" << this->baseName_ << "\", headers don't match." << std::endl;
            return false;
        }
    }

    void ConsoleCommand::pushFunction(const ExecutorPtr& executor, bool bForce)
    {
        Command command;
        command.executor_ = this->getExecutor();
        if (command.executor_)
            command.functor_ = this->getExecutor()->getFunctor();

        if (this->setFunction(executor, bForce))
            this->commandStack_.push(command);
    }

    void ConsoleCommand::pushFunction(const FunctorPtr& functor, bool bForce)
    {
        Command command;
        command.executor_ = this->getExecutor();
        if (command.executor_)
            command.functor_ = this->getExecutor()->getFunctor();

        if (this->setFunction(functor, bForce))
            this->commandStack_.push(command);
    }

    void ConsoleCommand::pushFunction()
    {
        if (this->executor_)
            this->pushFunction(new Executor(*this->executor_.get()));
        else
            COUT(1) << "Error: Couldn't push copy of executor in console command \"" << this->baseName_ << "\", no executor set." << std::endl;
    }

    void ConsoleCommand::popFunction()
    {
        Command command;
        if (!this->commandStack_.empty())
        {
            command = this->commandStack_.top();
            this->commandStack_.pop();
        }

        this->executor_ = command.executor_;
        if (command.executor_)
            this->executor_->setFunctor(command.functor_);
    }

    void ConsoleCommand::resetFunction()
    {
        if (this->executor_)
            this->executor_->setFunctor(0);
    }

    const ExecutorPtr& ConsoleCommand::getExecutor() const
    {
        return this->executor_;
    }

    bool ConsoleCommand::setObject(void* object)
    {
        if (this->executor_)
        {
            if (this->executor_->getFunctor())
            {
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

    void ConsoleCommand::pushObject(void* object)
    {
        void* oldobject = this->getObject();
        if (this->setObject(object))
            this->objectStack_.push(oldobject);
    }

    void ConsoleCommand::popObject()
    {
        void* newobject = 0;
        if (!this->objectStack_.empty())
        {
            newobject = this->objectStack_.top();
            this->objectStack_.pop();
        }
        this->setObject(newobject);
    }

    void* ConsoleCommand::getObject() const
    {
        if (this->executor_ && this->executor_->getFunctor())
            return this->executor_->getFunctor()->getRawObjectPointer();
        else
            return 0;
    }

    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& param1)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(param1);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& param1, const MultiType& param2)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(param1, param2);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(param1, param2, param3);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(param1, param2, param3, param4);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
    {
        if (this->executor_)
            this->executor_->setDefaultValues(param1, param2, param3, param4, param5);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::defaultValue(unsigned int index, const MultiType& param)
    {
        if (this->executor_)
            this->executor_->setDefaultValue(index, param);
        else
            COUT(1) << "Error: Can't set default values in console command \"" << this->baseName_ << "\", no executor set." << std::endl;

        return *this;
    }

    ConsoleCommand& ConsoleCommand::argumentCompleter(unsigned int param, ArgumentCompleter* completer)
    {
        if (param < 5)
            this->argumentCompleter_[param] = completer;
        else
            COUT(2) << "Warning: Couldn't add autocompletion-function for param " << param << " in console command \"" << this->baseName_ << "\": index out of bound." << std::endl;

        return *this;
    }

    ArgumentCompleter* ConsoleCommand::getArgumentCompleter(unsigned int param) const
    {
        if (param < 5)
            return this->argumentCompleter_[param];
        else
            return 0;
    }

    ConsoleCommand& ConsoleCommand::description(const std::string& description)
    {
        this->description_ = std::string("ConsoleCommandDescription::" + this->baseName_ + "::function");
        AddLanguageEntry(this->description_, description);
        return *this;
    }

    const std::string& ConsoleCommand::getDescription() const
    {
        return GetLocalisation_noerror(this->description_);
    }

    ConsoleCommand& ConsoleCommand::descriptionParam(unsigned int param, const std::string& description)
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
        {
            this->descriptionParam_[param] = std::string("ConsoleCommandDescription::" + this->baseName_ + "::param" + multi_cast<std::string>(param));
            AddLanguageEntry(this->descriptionParam_[param], description);
        }
        return *this;
    }

    const std::string& ConsoleCommand::getDescriptionParam(unsigned int param) const
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
            return GetLocalisation_noerror(this->descriptionParam_[param]);

        return this->descriptionParam_[0];
    }

    ConsoleCommand& ConsoleCommand::descriptionReturnvalue(const std::string& description)
    {
        this->descriptionReturnvalue_ = std::string("ConsoleCommandDescription::" + this->baseName_ + "::returnvalue");
        AddLanguageEntry(this->descriptionReturnvalue_, description);
        return *this;
    }

    const std::string& ConsoleCommand::getDescriptionReturnvalue(int param) const
    {
        return GetLocalisation_noerror(this->descriptionReturnvalue_);
    }

    /* static */ ConsoleCommand* ConsoleCommand::getCommand(const std::string& group, const std::string& name, bool bPrintError)
    {
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandMap().find(group);
        if (it_group != ConsoleCommand::getCommandMap().end())
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(name);
            if (it_name != it_group->second.end())
            {
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

    /* static */ ConsoleCommand* ConsoleCommand::getCommandLC(const std::string& group, const std::string& name, bool bPrintError)
    {
        std::string groupLC = getLowercase(group);
        std::string nameLC = getLowercase(name);

        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandMapLC().find(groupLC);
        if (it_group != ConsoleCommand::getCommandMapLC().end())
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(nameLC);
            if (it_name != it_group->second.end())
            {
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

    /* static */ std::map<std::string, std::map<std::string, ConsoleCommand*> >& ConsoleCommand::getCommandMap()
    {
        static std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMap;
        return commandMap;
    }

    /* static */ std::map<std::string, std::map<std::string, ConsoleCommand*> >& ConsoleCommand::getCommandMapLC()
    {
        static std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMapLC;
        return commandMapLC;
    }

    /* static */ void ConsoleCommand::registerCommand(const std::string& group, const std::string& name, ConsoleCommand* command)
    {
        if (name == "")
            return;

        if (ConsoleCommand::getCommand(group, name) != 0)
        {
            if (group == "")
                COUT(2) << "Warning: A console command with shortcut \"" << name << "\" already exists." << std::endl;
            else
                COUT(2) << "Warning: A console command with name \"" << name << "\" already exists in group \"" << group << "\"." << std::endl;
        }
        else
        {
            ConsoleCommand::getCommandMap()[group][name] = command;
            ConsoleCommand::getCommandMapLC()[getLowercase(group)][getLowercase(name)] = command;
        }
    }

    /* static */ void ConsoleCommand::unregisterCommand(ConsoleCommand* command)
    {
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = ConsoleCommand::getCommandMap().begin(); it_group != ConsoleCommand::getCommandMap().end(); )
        {
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            if (it_group->second.empty())
                ConsoleCommand::getCommandMap().erase(it_group++);
            else
                ++it_group;
        }

        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = ConsoleCommand::getCommandMapLC().begin(); it_group != ConsoleCommand::getCommandMapLC().end(); )
        {
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            if (it_group->second.empty())
                ConsoleCommand::getCommandMapLC().erase(it_group++);
            else
                ++it_group;
        }
    }

    /* static */ void ConsoleCommand::destroyAll()
    {
        while (!ConsoleCommand::getCommandMap().empty() && !ConsoleCommand::getCommandMap().begin()->second.empty())
            delete ConsoleCommand::getCommandMap().begin()->second.begin()->second;
    }
}
