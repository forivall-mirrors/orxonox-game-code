/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "util/String.h"
#include "Identifier.h"
#include "Language.h"
#include "Debug.h"
#include "Executor.h"
#include "ConfigValueContainer.h"

#define COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE "set"
#define COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY "tset"
#define COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND "bind"

namespace orxonox
{
    ConsoleCommandShortcutGeneric(keyword1, createExecutor((FunctorStatic*)0, "set", AccessLevel::User));
    ConsoleCommandShortcutGeneric(keyword2, createExecutor((FunctorStatic*)0, "tset", AccessLevel::User));
    ConsoleCommandShortcutGeneric(keyword3, createExecutor((FunctorStatic*)0, "bind", AccessLevel::User));

    ConsoleCommandShortcutExtern(exec, AccessLevel::None);
    ConsoleCommandShortcutExtern(echo, AccessLevel::None);

    ConsoleCommandShortcutExtern(read, AccessLevel::None);
    ConsoleCommandShortcutExtern(append, AccessLevel::None);
    ConsoleCommandShortcutExtern(write, AccessLevel::None);

    void exec(const std::string& filename)
    {
        static std::set<std::string> executingFiles;

        std::set<std::string>::const_iterator it = executingFiles.find(filename);
        if (it != executingFiles.end())
        {
            COUT(1) << "Error: Recurring exec command in \"" << filename << "\". Stopped execution." << std::endl;
            return;
        }

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't execute file \"" << filename << "\"." << std::endl;
            return;
        }

        executingFiles.insert(filename);

        // Iterate through the file and put the lines into the CommandExecutor
        char line[1024];
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            CommandExecutor::execute(line);
        }

        executingFiles.erase(filename);
        file.close();
    }

    std::string echo(const std::string& text)
    {
        return text;
    }

    void write(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't write to file \"" << filename << "\"." << std::endl;
            return;
        }

        file << text << std::endl;
        file.close();
    }

    void append(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::app);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't append to file \"" << filename << "\"." << std::endl;
            return;
        }

        file << text << std::endl;
        file.close();
    }

    std::string read(const std::string& filename)
    {
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't read from file \"" << filename << "\"." << std::endl;
            return "";
        }

        std::string output = "";
        char line[1024];
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            output += line;
            output += "\n";
        }

        file.close();

        return output;
    }


    ///////////////////////
    // CommandEvaluation //
    ///////////////////////
    CommandEvaluation::CommandEvaluation()
    {
        this->processedCommand_ = "";
        this->additionalParameter_ = "";

        this->functionclass_ = 0;
        this->configvalueclass_ = 0;
        this->shortcut_ = 0;
        this->function_ = 0;
        this->configvalue_ = 0;
        this->key_ = 0;

        this->errorMessage_ = "";
        this->state_ = CS_Uninitialized;

        this->bEvaluatedParams_ = false;
        this->evaluatedExecutor_ = 0;
    }

    KeybindMode CommandEvaluation::getKeybindMode()
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
//            if (this->shortcut_ != 0)
//                return this->shortcut_->getKeybindMode();
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
//            if (this->function_ != 0)
//                return this->function_->getKeybindMode();
        }
        else if (this->state_ == CS_ConfigValueType || this->state_ == CS_ConfigValueFinished)
        {
//            return KeybindMode::onPress;
        }
        else if (this->state_ == CS_KeybindCommand || this->state_ == CS_KeybindFinished)
        {
//            return KeybindMode::onPress;
        }
        else
        {
//            return KeybindMode::onPress;
        }
    }

    bool CommandEvaluation::isValid() const
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            return this->shortcut_;
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            return (this->functionclass_ && this->function_);
        }
        else if (this->state_ == CS_ConfigValueType || this->state_ == CS_ConfigValueFinished)
        {
            return (this->configvalueclass_ && this->configvalue_);
        }
        else if (this->state_ == CS_KeybindCommand || this->state_ == CS_KeybindFinished)
        {
            return this->key_;
        }
        else
        {
            return false;
        }
    }

    void CommandEvaluation::evaluateParams()
    {
        this->bEvaluatedParams_ = false;
        this->evaluatedExecutor_ = 0;

        for (unsigned int i = 0; i < MAX_FUNCTOR_ARGUMENTS; i++)
            this->param_[i] = MT_null;

        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            if (this->shortcut_)
            {
                if (this->shortcut_->evaluate(this->processedCommand_ + this->getAdditionalParameter(), this->param_, " "))
                {
                    this->bEvaluatedParams_ = true;
                    this->evaluatedExecutor_ = this->shortcut_;
                }
            }
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            if (this->function_)
            {
                if (this->function_->evaluate(this->processedCommand_ + this->getAdditionalParameter(), this->param_, " "))
                {
                    this->bEvaluatedParams_ = true;
                    this->evaluatedExecutor_ = this->function_;
                }
            }
        }
    }

    void CommandEvaluation::setEvaluatedParameter(unsigned int index, MultiTypeMath param)
    {
        if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
            this->param_[index] = param;
    }

    MultiTypeMath CommandEvaluation::getEvaluatedParameter(unsigned int index) const
    {
        if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
            return this->param_[index];

        return MT_null;
    }

    MultiTypeMath CommandEvaluation::getReturnvalue() const
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            if (this->shortcut_)
                return this->shortcut_->getReturnvalue();
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            if (this->function_)
                return this->function_->getReturnvalue();
        }

        return MT_null;
    }


    /////////////////////
    // CommandExecutor //
    /////////////////////
    CommandExecutor& CommandExecutor::getInstance()
    {
        static CommandExecutor instance;
        return instance;
    }

    CommandEvaluation& CommandExecutor::getEvaluation()
    {
        return CommandExecutor::getInstance().evaluation_;
    }

    Executor& CommandExecutor::addConsoleCommandShortcut(ExecutorStatic* executor)
    {
        CommandExecutor::getInstance().consoleCommandShortcuts_[executor->getName()] = executor;
        CommandExecutor::getInstance().consoleCommandShortcuts_LC_[getLowercase(executor->getName())] = executor;
        return (*executor);
    }

    /**
        @brief Returns the executor of a console command shortcut with given name.
        @brief name The name of the requested console command shortcut
        @return The executor of the requested console command shortcut
    */
    ExecutorStatic* CommandExecutor::getConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_.end())
            return (*it).second;
        else
            return 0;
    }

    /**
        @brief Returns the executor of a console command shortcut with given name in lowercase.
        @brief name The name of the requested console command shortcut in lowercase
        @return The executor of the requested console command shortcut
    */
    ExecutorStatic* CommandExecutor::getLowercaseConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_LC_.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_LC_.end())
            return (*it).second;
        else
            return 0;
    }

    bool CommandExecutor::execute(const std::string& command)
    {
        std::string strippedCommand = getStrippedEnclosingQuotes(command);

        SubString tokensIO(strippedCommand, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
        if (tokensIO.size() >= 2)
        {
            if (tokensIO[tokensIO.size() - 2] == ">")
            {
                bool success = CommandExecutor::execute(tokensIO.subSet(0, tokensIO.size() - 2).join());
                write(tokensIO[tokensIO.size() - 1], CommandExecutor::getEvaluation().getReturnvalue());
                return success;
            }
            else if (tokensIO[tokensIO.size() - 2] == "<")
            {
                std::string input = read(tokensIO[tokensIO.size() - 1]);
                if (input == "" || input.size() == 0)
                    return CommandExecutor::execute(tokensIO.subSet(0, tokensIO.size() - 2).join());
                else
                    return CommandExecutor::execute(tokensIO.subSet(0, tokensIO.size() - 2).join() + " " + input);
            }
        }


        SubString tokensPipeline(strippedCommand, "|", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
        if (tokensPipeline.size() > 1)
        {
            bool success = true;
            std::string returnValue = "";
            for (int i = tokensPipeline.size() - 1; i >= 0; i--)
            {
                if (returnValue == "" || returnValue.size() == 0)
                {
                    //CommandEvaluation evaluation = CommandExecutor::evaluate(tokens[i]);
                    if (!CommandExecutor::execute(tokensPipeline[i]))
                        success = false;
                }
                else
                {
                    //CommandEvaluation evaluation = CommandExecutor::evaluate(tokens[i] + " " + returnValue);
                    if (!CommandExecutor::execute(tokensPipeline[i] + " " + returnValue))
                        success = false;
                }

                //CommandExecutor::execute(evaluation);
                //returnValue = evaluation.getReturnvalue();
                returnValue = CommandExecutor::getEvaluation().getReturnvalue().toString();
            }
            return success;
        }

        if ((CommandExecutor::getEvaluation().processedCommand_ != strippedCommand) || (CommandExecutor::getEvaluation().state_ == CS_Uninitialized))
            CommandExecutor::parse(strippedCommand);

        return CommandExecutor::execute(CommandExecutor::getEvaluation());
    }


    bool CommandExecutor::execute(const CommandEvaluation& evaluation)
    {
        SubString tokens(evaluation.processedCommand_, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');

        if (evaluation.bEvaluatedParams_ && evaluation.evaluatedExecutor_)
        {
            (*evaluation.evaluatedExecutor_)(evaluation.param_[0], evaluation.param_[1], evaluation.param_[2], evaluation.param_[3], evaluation.param_[4]);
        }

        switch (evaluation.state_)
        {
            case CS_Uninitialized:
                break;
            case CS_Empty:
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                // not enough parameters but lets hope there are some additional parameters and go on
            case CS_Shortcut_Finished:
                // call the shortcut
                if (evaluation.shortcut_)
                {
                    if (tokens.size() >= 2)
                        return evaluation.shortcut_->parse(tokens.subSet(1).join() + evaluation.getAdditionalParameter());
                    else
                        return evaluation.shortcut_->parse(evaluation.additionalParameter_);
                }
                break;
            case CS_Function:
                break;
            case CS_Function_Params:
                // not enough parameters but lets hope there are some additional parameters and go on
            case CS_Function_Finished:
                // call the shortcut
                if (evaluation.function_)
                {
                    if (tokens.size() >= 3)
                        return evaluation.function_->parse(tokens.subSet(2).join() + evaluation.getAdditionalParameter());
                    else
                        return evaluation.function_->parse(evaluation.additionalParameter_);
                }
                break;
            case CS_ConfigValueClass:
                break;
            case CS_ConfigValue:
                break;
            case CS_ConfigValueType:
                // not enough parameters but lets hope there are some additional parameters and go on
            case CS_ConfigValueFinished:
                // set the config value
                if (evaluation.configvalue_)
                {
                    if ((tokens.size() >= 1) && (tokens[0] == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE))
                    {
                        if (tokens.size() >= 4)
                            return evaluation.configvalue_->set(tokens.subSet(3).join() + evaluation.getAdditionalParameter());
                        else
                            return evaluation.configvalue_->set(evaluation.additionalParameter_);
                    }
                    else if ((tokens.size() >= 1) && (tokens[0] == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY))
                    {
                        if (tokens.size() >= 4)
                            return evaluation.configvalue_->tset(tokens.subSet(3).join() + evaluation.getAdditionalParameter());
                        else
                            return evaluation.configvalue_->tset(evaluation.additionalParameter_);
                    }
                }
                break;
            case CS_KeybindKey:
                break;
            case CS_KeybindCommand:
                // not enough parameters but lets hope there are some additional parameters and go on
            case CS_KeybindFinished:
                // set the keybind
                // ...todo
                break;
            case CS_Error:
                break;
        }

        return false;
    }

    std::string CommandExecutor::complete(const std::string& command)
    {
        if ((CommandExecutor::getEvaluation().processedCommand_ != command) || (CommandExecutor::getEvaluation().state_ == CS_Uninitialized))
            CommandExecutor::parse(command);

        return CommandExecutor::complete(CommandExecutor::getEvaluation());
    }

    std::string CommandExecutor::complete(const CommandEvaluation& evaluation)
    {
        SubString tokens(evaluation.processedCommand_, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');

        std::list<std::pair<const std::string*, const std::string*> > temp;
        if (evaluation.state_ == CS_Empty)
        {
            temp.insert(temp.end(), evaluation.listOfPossibleShortcuts_.begin(), evaluation.listOfPossibleShortcuts_.end());
            temp.insert(temp.end(), evaluation.listOfPossibleFunctionClasses_.begin(), evaluation.listOfPossibleFunctionClasses_.end());
        }

        switch (evaluation.state_)
        {
            case CS_Uninitialized:
                break;
            case CS_Empty:
                return (CommandExecutor::getCommonBegin(temp));
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                if (evaluation.shortcut_)
                    return (evaluation.shortcut_->getName() + " ");
                break;
            case CS_Shortcut_Finished:
                if (evaluation.shortcut_)
                {
                    if (evaluation.shortcut_->getParamCount() == 0)
                        return (evaluation.shortcut_->getName());
                    else if (tokens.size() >= 2)
                        return (evaluation.shortcut_->getName() + " " + tokens.subSet(1).join());
                }
                break;
            case CS_Function:
                if (evaluation.functionclass_)
                    return (evaluation.functionclass_->getName() + " " + CommandExecutor::getCommonBegin(evaluation.listOfPossibleFunctions_));
                break;
            case CS_Function_Params:
                if (evaluation.functionclass_ && evaluation.function_)
                    return (evaluation.functionclass_->getName() + " " + evaluation.function_->getName() + " ");
                break;
            case CS_Function_Finished:
                if (evaluation.functionclass_ && evaluation.function_)
                {
                    if (evaluation.function_->getParamCount() == 0)
                        return (evaluation.functionclass_->getName() + " " + evaluation.function_->getName());
                    else if (tokens.size() >= 3)
                        return (evaluation.functionclass_->getName() + " " + evaluation.function_->getName() + " " + tokens.subSet(2).join());
                }
                break;
            case CS_ConfigValueClass:
                if (tokens.size() >= 1)
                    return (tokens[0] + " " + CommandExecutor::getCommonBegin(evaluation.listOfPossibleConfigValueClasses_));
                break;
            case CS_ConfigValue:
                if ((tokens.size() >= 1) && evaluation.configvalueclass_)
                    return (tokens[0] + " " + evaluation.configvalueclass_->getName() + " " + CommandExecutor::getCommonBegin(evaluation.listOfPossibleConfigValues_));
                break;
            case CS_ConfigValueType:
                if ((tokens.size() >= 1) && evaluation.configvalueclass_ && evaluation.configvalue_)
                    return (tokens[0] + " " + evaluation.configvalueclass_->getName() + " " + evaluation.configvalue_->getName() + " ");
                break;
            case CS_ConfigValueFinished:
                if ((tokens.size() >= 1) && evaluation.configvalueclass_ && evaluation.configvalue_ && (tokens.size() >= 4))
                    return (tokens[0] + " " + evaluation.configvalueclass_->getName() + " " + evaluation.configvalue_->getName() + " " + tokens.subSet(3).join());
                break;
            case CS_KeybindKey:
                if (tokens.size() >= 1)
                    return (tokens[0] + " " + CommandExecutor::getCommonBegin(evaluation.listOfPossibleKeys_));
                break;
            case CS_KeybindCommand:
                if ((evaluation.processedCommand_.size() >= 1) && (evaluation.processedCommand_[evaluation.processedCommand_.size() - 1] != ' '))
                    return (evaluation.processedCommand_ + " ");
                break;
            case CS_KeybindFinished:
                break;
            case CS_Error:
                break;
        }

        return evaluation.processedCommand_;
    }

    std::string CommandExecutor::hint(const std::string& command)
    {
        if ((CommandExecutor::getEvaluation().processedCommand_ != command) || (CommandExecutor::getEvaluation().state_ == CS_Uninitialized))
            CommandExecutor::parse(command);

        return CommandExecutor::hint(CommandExecutor::getEvaluation());
    }

    std::string CommandExecutor::hint(const CommandEvaluation& evaluation)
    {
        SubString tokens(evaluation.processedCommand_, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');

        switch (evaluation.state_)
        {
            case CS_Uninitialized:
                break;
            case CS_Empty:
                return (CommandExecutor::dump(evaluation.listOfPossibleShortcuts_) + "\n" + CommandExecutor::dump(evaluation.listOfPossibleFunctionClasses_));
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                if (evaluation.shortcut_)
                    return CommandExecutor::dump(evaluation.shortcut_);
                break;
            case CS_Shortcut_Finished:
                if (evaluation.shortcut_)
                    return CommandExecutor::dump(evaluation.shortcut_);
                break;
            case CS_Function:
                return CommandExecutor::dump(evaluation.listOfPossibleFunctions_);
                break;
            case CS_Function_Params:
                if (evaluation.function_)
                    return CommandExecutor::dump(evaluation.function_);
                break;
            case CS_Function_Finished:
                if (evaluation.function_)
                    return CommandExecutor::dump(evaluation.function_);
                break;
            case CS_ConfigValueClass:
                return CommandExecutor::dump(evaluation.listOfPossibleConfigValueClasses_);
                break;
            case CS_ConfigValue:
                return CommandExecutor::dump(evaluation.listOfPossibleConfigValues_);
                break;
            case CS_ConfigValueType:
                if (evaluation.configvalue_)
                    return CommandExecutor::dump(evaluation.configvalue_);
                break;
            case CS_ConfigValueFinished:
                if (evaluation.configvalue_)
                    return CommandExecutor::dump(evaluation.configvalue_);
                break;
            case CS_KeybindKey:
                return CommandExecutor::dump(evaluation.listOfPossibleKeys_);
                break;
            case CS_KeybindCommand:
                if (evaluation.key_)
                    return CommandExecutor::dump(evaluation.key_);
                break;
            case CS_KeybindFinished:
                if (evaluation.key_)
                    return CommandExecutor::dump(evaluation.key_);
                break;
            case CS_Error:
                return CommandExecutor::getEvaluation().errorMessage_;
                break;
        }

        return "";
    }

    CommandEvaluation CommandExecutor::evaluate(const std::string& command)
    {
        CommandExecutor::parse(command, true);
        CommandExecutor::getEvaluation().evaluateParams();
        return CommandExecutor::getEvaluation();
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
        CommandExecutor::getEvaluation().tokens_.split((command + COMMAND_EXECUTOR_CURSOR), " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
        CommandExecutor::getEvaluation().processedCommand_ = command;

        if (bInitialize)
            CommandExecutor::initialize(command);

        switch (CommandExecutor::getEvaluation().state_)
        {
            case CS_Uninitialized:
                // Impossible
                break;
            case CS_Empty:
                if (CommandExecutor::argumentsGiven() == 0)
                {
                    // We want a hint for the first token
                    // Check if there is already a perfect match
                    CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    CommandExecutor::getEvaluation().shortcut_ = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((CommandExecutor::getEvaluation().functionclass_) || (CommandExecutor::getEvaluation().shortcut_))
                    {
                        // Yes, there is a class or a shortcut with the searched name
                        // Add a whitespace and continue parsing
                        CommandExecutor::getEvaluation().state_ = CS_FunctionClass_Or_Shortcut_Or_Keyword;
                        CommandExecutor::parse(command + " ", false);
                        return;
                    }

                    // No perfect match: Create the lists of all possible classes and shortcuts and return
                    CommandExecutor::createListOfPossibleFunctionClasses(CommandExecutor::getToken(0));
                    CommandExecutor::createListOfPossibleShortcuts(CommandExecutor::getToken(0));

                    // Check if there's only one possiblility
                    if ((CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.size() == 1) && (CommandExecutor::getEvaluation().listOfPossibleShortcuts_.size() == 0))
                    {
                        // There's only one possible class
                        CommandExecutor::getEvaluation().state_ = CS_Function;
                        CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getIdentifierOfPossibleFunctionClass(*(*CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.begin()).first);
                        CommandExecutor::parse(*(*CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.begin()).first + " ", false);
                        return;
                    }
                    else if ((CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.size() == 0) && (CommandExecutor::getEvaluation().listOfPossibleShortcuts_.size() == 1))
                    {
                        if ((*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first != COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE)
                         && (*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first != COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)
                         && (*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first != COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND))
                        {
                            // There's only one possible shortcut
                            CommandExecutor::getEvaluation().state_ = CS_Shortcut_Params;
                            CommandExecutor::getEvaluation().shortcut_ = CommandExecutor::getExecutorOfPossibleShortcut(*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first);
                        }
                        else if ((*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE)
                              || (*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY))
                        {
                            // It's the 'set' or 'tset' keyword
                            CommandExecutor::getEvaluation().state_ = CS_ConfigValueClass;
                        }
                        else if (*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first != COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND)
                        {
                            // It's the 'bind' keyword
                            CommandExecutor::getEvaluation().state_ = CS_KeybindKey;
                        }

                        CommandExecutor::parse(*(*CommandExecutor::getEvaluation().listOfPossibleShortcuts_.begin()).first + " ", false);
                        return;
                    }

                    // It's ambiguous
                    return;
                }
                else
                {
                    // There is at least one argument: Check if it's a shortcut, a classname or a special keyword
                    CommandExecutor::getEvaluation().state_ = CS_FunctionClass_Or_Shortcut_Or_Keyword;
                    CommandExecutor::parse(command, false);
                    return;
                }
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                if (CommandExecutor::argumentsGiven() >= 1)
                {
                    if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY))
                    {
                        // We want to set a config value
                        CommandExecutor::getEvaluation().state_ = CS_ConfigValueClass;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else if (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND)
                    {
                        // We want to set a keybinding
                        CommandExecutor::getEvaluation().state_ = CS_KeybindKey;
                        CommandExecutor::parse(command, false);
                        return;
                    }

                    if (!CommandExecutor::getEvaluation().functionclass_)
                        CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    if (!CommandExecutor::getEvaluation().shortcut_)
                        CommandExecutor::getEvaluation().shortcut_ = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((!CommandExecutor::getEvaluation().functionclass_) && (!CommandExecutor::getEvaluation().shortcut_))
                    {
                        // Argument 1 seems to be wrong
                        AddLanguageEntry("CommandExecutor::NoSuchCommandOrClassName", "No such command or classname");
                        CommandExecutor::getEvaluation().errorMessage_ = (CommandExecutor::getToken(0) + ": " + GetLocalisation("CommandExecutor::NoSuchCommandOrClassName"));
                        CommandExecutor::getEvaluation().state_ = CS_Error;
                        return;
                    }
                    else if (CommandExecutor::getEvaluation().shortcut_)
                    {
                        // Argument 1 is a shortcut: Return the needed parameter types
                        CommandExecutor::getEvaluation().state_ = CS_Shortcut_Params;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else
                    {
                        // Argument 1 is a classname: Return the possible functions
                        CommandExecutor::getEvaluation().state_ = CS_Function;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                }
                else
                {
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_Shortcut_Params:
                if (CommandExecutor::getEvaluation().shortcut_)
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(1, CommandExecutor::getEvaluation().shortcut_))
                    {
                        CommandExecutor::getEvaluation().state_ = CS_Shortcut_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_Function:
                if (CommandExecutor::getEvaluation().functionclass_)
                {
                    // We have a valid classname
                    // Check if there is a second argument
                    if (CommandExecutor::argumentsGiven() >= 2)
                    {
                        // There is a second argument: Check if it's a valid functionname
                        CommandExecutor::getEvaluation().function_ = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::getEvaluation().functionclass_);
                        if (!CommandExecutor::getEvaluation().function_)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchFunctionnameIn", "No such functionname in");
                            CommandExecutor::getEvaluation().errorMessage_ = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchFunctionnameIn") + " " + CommandExecutor::getEvaluation().functionclass_->getName());
                            CommandExecutor::getEvaluation().state_ = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid functionname: Get the parameters
                            CommandExecutor::getEvaluation().state_ = CS_Function_Params;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getEvaluation().tokens_.size() >= 2)
                        {
                            CommandExecutor::getEvaluation().function_ = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::getEvaluation().functionclass_);
                            if (CommandExecutor::getEvaluation().function_)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getEvaluation().state_ = CS_Function_Params;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible functions and return
                        CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getToken(1), CommandExecutor::getEvaluation().functionclass_);

                        // Check if there's only one possiblility
                        if (CommandExecutor::getEvaluation().listOfPossibleFunctions_.size() == 1)
                        {
                            // There's only one possible function
                            CommandExecutor::getEvaluation().state_ = CS_Function_Params;
                            CommandExecutor::getEvaluation().function_ = CommandExecutor::getExecutorOfPossibleFunction(*(*CommandExecutor::getEvaluation().listOfPossibleFunctions_.begin()).first, CommandExecutor::getEvaluation().functionclass_);
                            CommandExecutor::parse(CommandExecutor::getToken(0) + " " + *(*CommandExecutor::getEvaluation().listOfPossibleFunctions_.begin()).first + " ", false);
                            return;
                        }

                        // It's ambiguous
                        return;
                    }
                }
                else
                {
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_Function_Params:
                if (CommandExecutor::getEvaluation().functionclass_ && CommandExecutor::getEvaluation().function_)
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(2, CommandExecutor::getEvaluation().function_))
                    {
                        CommandExecutor::getEvaluation().state_ = CS_Function_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValueClass:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)))
                {
                    // We want to set a config value
                    // Check if there is a second argument
                    if (CommandExecutor::argumentsGiven() >= 2)
                    {
                        // There is a second argument: Check if it's a valid classname
                        CommandExecutor::getEvaluation().configvalueclass_ = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                        if (!CommandExecutor::getEvaluation().configvalueclass_)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchClassWithConfigValues", "No such class with config values");
                            CommandExecutor::getEvaluation().errorMessage_ = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchClassWithConfigValues"));
                            CommandExecutor::getEvaluation().state_ = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid classname: Search for possible config values
                            CommandExecutor::getEvaluation().state_ = CS_ConfigValue;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There's no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getEvaluation().tokens_.size() >= 2)
                        {
                            CommandExecutor::getEvaluation().configvalueclass_ = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                            if (CommandExecutor::getEvaluation().configvalueclass_)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getEvaluation().state_ = CS_ConfigValue;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible classnames and return
                        CommandExecutor::createListOfPossibleConfigValueClasses(CommandExecutor::getToken(1));

                        // Check if there's only one possiblility
                        if (CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.size() == 1)
                        {
                            // There's only one possible classname
                            CommandExecutor::getEvaluation().state_ = CS_ConfigValue;
                            CommandExecutor::getEvaluation().configvalueclass_ = CommandExecutor::getIdentifierOfPossibleConfigValueClass(*(*CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.begin()).first);
                            CommandExecutor::parse(CommandExecutor::getToken(0) + " " + *(*CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.begin()).first + " ", false);
                            return;
                        }

                        // It's ambiguous
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValue:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && (CommandExecutor::getEvaluation().configvalueclass_))
                {
                    // Check if there is a third argument
                    if (CommandExecutor::argumentsGiven() >= 3)
                    {
                        // There is a third argument: Check if it's a valid config value
                        CommandExecutor::getEvaluation().configvalue_ = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::getEvaluation().configvalueclass_);
                        if (!CommandExecutor::getEvaluation().configvalue_)
                        {
                            // Argument 3 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchConfigValueIn", "No such config value in");
                            CommandExecutor::getEvaluation().errorMessage_ = (CommandExecutor::getToken(2) + ": " + GetLocalisation("CommandExecutor::NoSuchConfigValueIn") + " " + CommandExecutor::getEvaluation().configvalueclass_->getName());
                            CommandExecutor::getEvaluation().state_ = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 3 seems to be a valid config value: Get the type
                            CommandExecutor::getEvaluation().state_ = CS_ConfigValueType;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished third argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getEvaluation().tokens_.size() >= 3)
                        {
                            CommandExecutor::getEvaluation().configvalue_ = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::getEvaluation().configvalueclass_);
                            if (CommandExecutor::getEvaluation().configvalue_)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getEvaluation().state_ = CS_ConfigValueType;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible config values
                        CommandExecutor::createListOfPossibleConfigValues(CommandExecutor::getToken(2), CommandExecutor::getEvaluation().configvalueclass_);

                        // Check if there's only one possiblility
                        if (CommandExecutor::getEvaluation().listOfPossibleConfigValues_.size() == 1)
                        {
                            // There's only one possible config value
                            CommandExecutor::getEvaluation().state_ = CS_ConfigValueType;
                            CommandExecutor::getEvaluation().configvalue_ = CommandExecutor::getContainerOfPossibleConfigValue(*(*CommandExecutor::getEvaluation().listOfPossibleConfigValues_.begin()).first, CommandExecutor::getEvaluation().configvalueclass_);
                            CommandExecutor::parse(CommandExecutor::getToken(0) + " " + CommandExecutor::getToken(1) + " " + *(*CommandExecutor::getEvaluation().listOfPossibleConfigValues_.begin()).first + " ", false);
                            return;
                        }

                        // It's ambiguous
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValueType:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && CommandExecutor::getEvaluation().configvalueclass_ && CommandExecutor::getEvaluation().configvalue_)
                {
                    // Valid command
                    // Check if there are enough parameters
                    if ((CommandExecutor::getEvaluation().tokens_.size() >= 4) && (CommandExecutor::getEvaluation().tokens_[3] != COMMAND_EXECUTOR_CURSOR))
                    {
                        CommandExecutor::getEvaluation().state_ = CS_ConfigValueFinished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_KeybindKey:
                if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND))
                {
                    // todo
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_KeybindCommand:
                if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND) && (false)) // todo
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::getEvaluation().tokens_.size() >= 3)
                    {
                        CommandExecutor::getEvaluation().state_ = CS_KeybindFinished;
                        return;
                    }

                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getEvaluation().state_ = CS_Error;
                    return;
                }
                break;
            case CS_Shortcut_Finished:
                // Nothing to do
                break;
            case CS_Function_Finished:
                // Nothing to do
                break;
            case CS_ConfigValueFinished:
                // Nothing to do
                break;
            case CS_KeybindFinished:
                // Nothing to do
                break;
            case CS_Error:
                // This is bad
                break;
        }
    }

    void CommandExecutor::initialize(const std::string& command)
    {
        CommandExecutor::getEvaluation().processedCommand_ = command;
        CommandExecutor::getEvaluation().additionalParameter_ = "";

        CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.clear();
        CommandExecutor::getEvaluation().listOfPossibleShortcuts_.clear();
        CommandExecutor::getEvaluation().listOfPossibleFunctions_.clear();
        CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.clear();
        CommandExecutor::getEvaluation().listOfPossibleConfigValues_.clear();
        CommandExecutor::getEvaluation().listOfPossibleKeys_.clear();

        CommandExecutor::getEvaluation().functionclass_ = 0;
        CommandExecutor::getEvaluation().configvalueclass_ = 0;
        CommandExecutor::getEvaluation().shortcut_ = 0;
        CommandExecutor::getEvaluation().function_ = 0;
        CommandExecutor::getEvaluation().configvalue_ = 0;
        CommandExecutor::getEvaluation().key_ = 0;

        CommandExecutor::getEvaluation().errorMessage_ = "";
        CommandExecutor::getEvaluation().state_ = CS_Empty;
    }

    bool CommandExecutor::argumentsGiven(unsigned int num)
    {
        // Because we added a cursor we have +1 arguments
        // There are num arguments given if there are at least num arguments + one cursor
        return (CommandExecutor::getEvaluation().tokens_.size() >= (num + 1));
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        // Because we added a cursor we have +1 arguments
        if (CommandExecutor::getEvaluation().tokens_.size() >= 1)
            return (CommandExecutor::getEvaluation().tokens_.size() - 1);
        else
            return 0;
    }

    std::string CommandExecutor::getToken(unsigned int index)
    {
        if ((index >= 0) && (index < (CommandExecutor::getEvaluation().tokens_.size() - 1)))
            return CommandExecutor::getEvaluation().tokens_[index];
        else if (index == (CommandExecutor::getEvaluation().tokens_.size() - 1))
            return CommandExecutor::getEvaluation().tokens_[index].substr(0, CommandExecutor::getEvaluation().tokens_[index].size() - 1);
        else
            return "";
    }

    bool CommandExecutor::enoughParametersGiven(unsigned int head, Executor* executor)
    {
        unsigned int neededParams = head + executor->getParamCount();
        /*
        for (unsigned int i = executor->getParamCount() - 1; i >= 0; i--)
        {
            if (executor->defaultValueSet(i))
                neededParams--;
            else
                break;
        }
        */
        return ((CommandExecutor::getEvaluation().tokens_.size() >= neededParams) && (CommandExecutor::getEvaluation().tokens_[neededParams - 1] != COMMAND_EXECUTOR_CURSOR));
    }

    void CommandExecutor::createListOfPossibleFunctionClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConsoleCommands())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                {
                    CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
                }
            }
        }

        CommandExecutor::getEvaluation().listOfPossibleFunctionClasses_.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleShortcuts(const std::string& fragment)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
            {
                CommandExecutor::getEvaluation().listOfPossibleShortcuts_.push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
            }
        }

        CommandExecutor::getEvaluation().listOfPossibleShortcuts_.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
            {
                CommandExecutor::getEvaluation().listOfPossibleFunctions_.push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
            }
        }

        CommandExecutor::getEvaluation().listOfPossibleFunctions_.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValueClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConfigValues())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                {
                    CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
                }
            }
        }

        CommandExecutor::getEvaluation().listOfPossibleConfigValueClasses_.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValues(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->getLowercaseConfigValueMapBegin(); it != identifier->getLowercaseConfigValueMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
            {
                CommandExecutor::getEvaluation().listOfPossibleConfigValues_.push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
            }
        }

        CommandExecutor::getEvaluation().listOfPossibleConfigValues_.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleKeys(const std::string& fragment)
    {
        // todo

        CommandExecutor::getEvaluation().listOfPossibleKeys_.sort(CommandExecutor::compareStringsInList);
    }

    bool CommandExecutor::compareStringsInList(const std::pair<const std::string*, const std::string*>& first, const std::pair<const std::string*, const std::string*>& second)
    {
        return ((*first.first) < (*second.first));
    }

    Identifier* CommandExecutor::getIdentifierOfPossibleFunctionClass(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(getLowercase(name));
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConsoleCommands())
            return (*it).second;

        return 0;
    }

    ExecutorStatic* CommandExecutor::getExecutorOfPossibleShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMap().find(getLowercase(name));
        if (it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd())
            return (*it).second;

        return 0;
    }

    ExecutorStatic* CommandExecutor::getExecutorOfPossibleFunction(const std::string& name, Identifier* identifier)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = identifier->getLowercaseConsoleCommandMap().find(getLowercase(name));
        if (it != identifier->getLowercaseConsoleCommandMapEnd())
            return (*it).second;

        return 0;
    }

    Identifier* CommandExecutor::getIdentifierOfPossibleConfigValueClass(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(getLowercase(name));
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConfigValues())
            return (*it).second;

        return 0;
    }

    ConfigValueContainer* CommandExecutor::getContainerOfPossibleConfigValue(const std::string& name, Identifier* identifier)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->getLowercaseConfigValueMap().find(getLowercase(name));
        if (it != identifier->getLowercaseConfigValueMapEnd())
        {
            return (*it).second;
        }

        return 0;
    }

    ConfigValueContainer* CommandExecutor::getContainerOfPossibleKey(const std::string& name)
    {
        // todo

        return 0;
    }

    std::string CommandExecutor::dump(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        std::string output = "";
        for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += " ";

            output += *(*it).second;
        }
        return output;
    }

    std::string CommandExecutor::dump(const ExecutorStatic* executor)
    {
        std::string output = "";
        for (unsigned int i = 0; i < executor->getParamCount(); i++)
        {
            if (i != 0)
                output += " ";

            if (executor->defaultValueSet(i))
                output += "[";
            else
                output += "{";

            output += executor->getTypenameParam(i);

            if (executor->defaultValueSet(i))
                output += "=" + executor->getDefaultValue(i).toString() + "]";
            else
                output += "}";
        }
        return output;
    }

    std::string CommandExecutor::dump(const ConfigValueContainer* container)
    {
        AddLanguageEntry("CommandExecutor::oldvalue", "old value");
        return "{" + container->getTypename() + "} (" + GetLocalisation("CommandExecutor::oldvalue") + ": " + container->toString() + ")";
    }

    std::string CommandExecutor::getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            return ((*(*list.begin()).first) + " ");
        }
        else
        {
            std::string output = "";
            for (unsigned int i = 0; true; i++)
            {
                char temp = 0;
                for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    if ((*(*it).first).size() > i)
                    {
                        if (it == list.begin())
                        {
                            temp = (*(*it).first)[i];
                        }
                        else
                        {
                            if (temp != (*(*it).first)[i])
                                return output;
                        }
                    }
                    else
                    {
                        return output;
                    }
                }
                output += temp;
            }
            return output;
        }
    }
}
