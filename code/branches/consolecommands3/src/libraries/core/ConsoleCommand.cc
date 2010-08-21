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
#include <cassert>

#include "Language.h"

namespace orxonox
{
    ConsoleCommand::ConsoleCommand(const FunctorPtr& functor, const std::string& name) : Executor(functor, name)
    {
        this->accessLevel_ = AccessLevel::None;
        this->argumentCompleter_[0] = 0;
        this->argumentCompleter_[1] = 0;
        this->argumentCompleter_[2] = 0;
        this->argumentCompleter_[3] = 0;
        this->argumentCompleter_[4] = 0;

        this->keybindMode_ = KeybindMode::OnPress;
        this->inputConfiguredParam_ = -1;
    }

    ConsoleCommand& ConsoleCommand::argumentCompleter(unsigned int param, ArgumentCompleter* completer)
    {
        if (param < 5)
            this->argumentCompleter_[param] = completer;
        else
        {
            COUT(2) << "Warning: Couldn't add autocompletion-function for param " << param << ": index out of bound." << std::endl;
        }
        return (*this);
    }

    ArgumentCompleter* ConsoleCommand::getArgumentCompleter(unsigned int param) const
    {
        if (param < 5)
            return this->argumentCompleter_[param];
        else
            return 0;
    }

    void ConsoleCommand::createArgumentCompletionList(unsigned int param, const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5)
    {
        if (param < 5 && this->argumentCompleter_[param])
            this->argumentList_ = (*this->argumentCompleter_[param])(param1, param2, param3, param4, param5);
        else
            this->argumentList_.clear();
    }

    ConsoleCommand& ConsoleCommand::description(const std::string& description)
    {
        this->description_ = std::string("ConsoleCommandDescription::" + this->name_ + "::function");
        AddLanguageEntry(this->description_, description);
        return (*this);
    }

    const std::string& ConsoleCommand::getDescription() const
    {
        return GetLocalisation_noerror(this->description_);
    }

    ConsoleCommand& ConsoleCommand::descriptionParam(unsigned int param, const std::string& description)
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
        {
            this->descriptionParam_[param] = std::string("ConsoleCommandDescription::" + this->name_ + "::param" + multi_cast<std::string>(param));
            AddLanguageEntry(this->descriptionParam_[param], description);
        }
        return (*this);
    }

    const std::string& ConsoleCommand::getDescriptionParam(unsigned int param) const
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
            return GetLocalisation_noerror(this->descriptionParam_[param]);

        return this->descriptionParam_[0];
    }

    ConsoleCommand& ConsoleCommand::descriptionReturnvalue(const std::string& description)
    {
        this->descriptionReturnvalue_ = std::string("ConsoleCommandDescription::" + this->name_ + "::returnvalue");
        AddLanguageEntry(this->descriptionReturnvalue_, description);
        return (*this);
    }

    const std::string& ConsoleCommand::getDescriptionReturnvalue(int param) const
    {
        return GetLocalisation_noerror(this->descriptionReturnvalue_);
    }
}

#include "BaseObject.h" // remove this

namespace orxonox
{
    _SetConsoleCommand("BaseObject", "setName", &BaseObject::setName, (BaseObject*)0);
    _ConsoleCommand::_ConsoleCommandManipulator test(_ModifyConsoleCommand("BaseObject", "setName").setFunction(&BaseObject::setActive));

    _ConsoleCommand::_ConsoleCommand(const std::string& group, const std::string& name, const FunctorPtr& functor, bool bInitialized) : Executor(functor, name), functionHeader_(functor->getHeaderIdentifier())
    {
        this->bActive_ = true;
        this->bInitialized_ = bInitialized;
        _ConsoleCommand::registerCommand(group, name, this);
    }

    _ConsoleCommand& _ConsoleCommand::addShortcut()
    {
        _ConsoleCommand::registerCommand("", this->getName(), this);
        return *this;
    }

    _ConsoleCommand& _ConsoleCommand::addShortcut(const std::string&  name)
    {
        _ConsoleCommand::registerCommand("", name, this);
        return *this;
    }

    _ConsoleCommand& _ConsoleCommand::addGroup(const std::string& group)
    {
        _ConsoleCommand::registerCommand(group, this->getName(), this);
        return *this;
    }

    _ConsoleCommand& _ConsoleCommand::addGroup(const std::string& group, const std::string&  name)
    {
        _ConsoleCommand::registerCommand(group, name, this);
        return *this;
    }

    bool _ConsoleCommand::setFunctor(const FunctorPtr& functor, bool bForce)
    {
        if (!functor)
        {
            this->bInitialized_ = false;
            return true;
        }

        if (!bForce && !this->functionHeaderMatches(functor))
        {
            COUT(1) << "Error: Couldn't assign new function to console command with name \"" << this->getName() << "\", headers don't match." << std::endl;
            return false;
        }

        this->functor_ = functor;
        this->bInitialized_ = true;
        return true;
    }

    void _ConsoleCommand::pushFunctor(const FunctorPtr& functor, bool bForce)
    {
        const FunctorPtr& oldfunctor = this->getFunctor();

        if (this->setFunctor(functor, bForce));
            this->functorStack_.push(oldfunctor);
    }

    void _ConsoleCommand::popFunctor()
    {
        FunctorPtr newfunctor;
        if (!this->functorStack_.empty())
        {
            newfunctor = this->functorStack_.top();
            this->functorStack_.pop();
        }
        this->setFunctor(newfunctor);
    }

    const FunctorPtr& _ConsoleCommand::getFunctor() const
    {
//        if (this->bInitialized_) // FIXME
            return this->functor_;
//        else
//            return 0;
    }

    bool _ConsoleCommand::functionHeaderMatches(const FunctorPtr& functor) const
    {
        if (!this->functor_)
        {
            assert(false);
            return true;
        }
        return (functor->getHeaderIdentifier() == this->functionHeader_);
    }

    void _ConsoleCommand::setObject(void* object)
    {
        if (this->functor_)
            this->functor_->setRawObjectPointer(object);
        else if (object)
            COUT(1) << "Error: Can't set object in console command \"" << this->getName() << "\", no functor set." << std::endl;
    }

    void _ConsoleCommand::pushObject(void* object)
    {
        if (this->functor_)
        {
            this->objectStack_.push(this->getObject());
            this->setObject(object);
        }
        else
            COUT(1) << "Error: Can't set object in console command \"" << this->getName() << "\", no functor set." << std::endl;
    }

    void _ConsoleCommand::popObject()
    {
        void* newobject = 0;
        if (!this->objectStack_.empty())
        {
            newobject = this->objectStack_.top();
            this->objectStack_.pop();
        }
        this->setObject(newobject);
    }

    void* _ConsoleCommand::getObject() const
    {
        if (this->functor_)
            return this->functor_->getRawObjectPointer();
        else
            return 0;
    }

    /* static */ const _ConsoleCommand* _ConsoleCommand::getCommand(const std::string& group, const std::string& name, bool bPrintError)
    {
        std::map<std::string, std::map<std::string, _ConsoleCommand*> >::const_iterator it_group = _ConsoleCommand::getCommandMap().find(group);
        if (it_group != _ConsoleCommand::getCommandMap().end())
        {
            std::map<std::string, _ConsoleCommand*>::const_iterator it_name = it_group->second.find(name);
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

    /* static */ std::map<std::string, std::map<std::string, _ConsoleCommand*> >& _ConsoleCommand::getCommandMap()
    {
        static std::map<std::string, std::map<std::string, _ConsoleCommand*> > commandMap;
        return commandMap;
    }

    /* static */ void _ConsoleCommand::registerCommand(const std::string& group, const std::string& name, _ConsoleCommand* command)
    {
        if (name == "")
            return;

        if (_ConsoleCommand::getCommand(group, name) != 0)
        {
            if (group == "")
                COUT(2) << "Warning: A console command with shortcut name \"" << name << "\" already exists." << std::endl;
            else
                COUT(2) << "Warning: A console command with group \"" << group << "\" and name \"" << name << "\" already exists." << std::endl;
        }
        else
        {
            _ConsoleCommand::getCommandMap()[group][name] = command;
        }
    }
}
