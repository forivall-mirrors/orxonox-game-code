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
 *   Inspiration: Executor by Benjamin Grauer
 */

/**
    @file
    @brief Implementation of orxonox::Executor
*/

#include "Executor.h"

#include <algorithm>

#include "util/Convert.h"
#include "util/Output.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "CommandExecutor.h"

namespace orxonox
{
    /**
        @brief Constructor: Creates an executor.
        @param functor The wrapped functor
        @param name The name of the executor (optional, used mostly for debug output)
    */
    Executor::Executor(const FunctorPtr& functor, const std::string& name)
    {
        this->functor_ = functor;
        this->name_ = name;
    }

    /**
        @brief Copy-constructor: Creates a new executor with the same values and a clone of the wrapped Functor.
    */
    Executor::Executor(const Executor& other) : name_(other.name_)
    {
        for (size_t i = 0; i < MAX_FUNCTOR_ARGUMENTS; ++i)
            defaultValue_[i] = other.defaultValue_[i];
        this->functor_ = other.functor_->clone();
    }

    /**
        @brief Destructor
    */
    Executor::~Executor()
    {
    }

    /**
        @brief Calls the wrapped function with arguments that are passed in a string.
        @param arguments The arguments that should be passed to the function, separated by @a delimiter
        @param error A pointer to a variable (or NULL) that is used to store the error code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
        @param delimiter The delimiter that is used to separate the arguments in the string @a arguments
        @param bPrintError If true, errors are printed to the console if the function couldn't be executed with the given arguments
        @return Returns the return value of the function (or MultiType::Null if there is no return value)
    */
    MultiType Executor::parse(const std::string& arguments, int* error, const std::string& delimiter, bool bPrintError) const
    {
        return this->parse(SubString(arguments, delimiter, SubString::WhiteSpaces, false, '\\', true, '"', true, '{', '}', true, '\0'), error, delimiter, bPrintError);
    }

    /**
        @brief Calls the wrapped function with arguments that are passed as tokens in a SubString
        @param arguments The arguments that should be passed to the function
        @param error A pointer to a variable (or NULL) that is used to store the error code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
        @param delimiter The delimiter that was used to separate the arguments in the SubString @a arguments (used to join the surplus arguments)
        @param bPrintError If true, errors are printed to the console if the function couldn't be executed with the given arguments
        @return Returns the return value of the function (or MultiType::Null if there is no return value)
    */
    MultiType Executor::parse(const SubString& arguments, int* error, const std::string& delimiter, bool bPrintError) const
    {
        // evaluate the arguments
        MultiType arg[MAX_FUNCTOR_ARGUMENTS];
        unsigned int argCount = this->evaluateArguments(arguments, arg, error, delimiter);

        // check if an error occurred
        if (error && *error)
        {
            if (bPrintError)
                orxout(internal_warning) << "Can't call executor " << this->name_ << " through parser: Not enough arguments or default values given (input: " << arguments.join() << ")." << endl;
            return MultiType::Null;
        }

        orxout(verbose, context::misc::executor) << "Executor::parse: \"" << arguments.join(delimiter) << "\" -> " << argCount << " arguments: " << arg[0] << " / " << arg[1] << " / " << arg[2] << " / " << arg[3] << " / " << arg[4] << endl;

        // execute the function with the evaluated arguments (the default values of the executor are also included in these arguments)
        switch (argCount)
        {
            case 0:  return (*this->functor_)();
            case 1:  return (*this->functor_)(arg[0]);
            case 2:  return (*this->functor_)(arg[0], arg[1]);
            case 3:  return (*this->functor_)(arg[0], arg[1], arg[2]);
            case 4:  return (*this->functor_)(arg[0], arg[1], arg[2], arg[3]);
            case 5:
            default: return (*this->functor_)(arg[0], arg[1], arg[2], arg[3], arg[4]);
        }
    }

    /**
        @brief Converts the arguments in a SubString to the right type, so they can be used to execute the function without further conversions.
        @param arguments The arguments that should be converted
        @param arg An array of MultiType where the converted arguments will be stored
        @param error A pointer to a variable (or NULL) that is used to store the error code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
        @param delimiter The delimiter that was used to separate the arguments in the SubString @a arguments (used to join the surplus arguments)
        @return Returns the number of evaluated arguments
    */
    int Executor::evaluateArguments(const SubString& arguments, MultiType arg[MAX_FUNCTOR_ARGUMENTS], int* error, const std::string& delimiter) const
    {
        unsigned int paramCount = this->functor_->getParamCount();
        unsigned int argumentCount = arguments.size();

        // if there are not enough params given, check if there are default values
        for (unsigned int i = argumentCount; i < paramCount; i++)
        {
            if (this->defaultValue_[i].null())
            {
                if (error)
                    *error = CommandExecutor::Incomplete;
                return 0;
            }
        }

        // assign all given arguments to the multitypes
        for (unsigned int i = 0; i < std::min(std::min(argumentCount, paramCount), MAX_FUNCTOR_ARGUMENTS); i++)
            arg[i] = arguments[i];

        // fill the remaining multitypes with default values
        for (unsigned int i = argumentCount; i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
            arg[i] = this->defaultValue_[i];

        // assign the remaining arguments all to the last parameter if it is a string
        if ((paramCount <= MAX_FUNCTOR_ARGUMENTS) &&(argumentCount > paramCount) && (paramCount == 1 || this->functor_->getTypenameParam(paramCount - 1) == "string"))
            arg[paramCount - 1] = arguments.subSet(paramCount - 1).join(delimiter);

        // evaluate the parameter types through the functor
        for (unsigned int i = 0; i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
            this->functor_->evaluateArgument(i, arg[i]);

        if (error)
            *error = CommandExecutor::Success;
        return paramCount;
    }

    /// Defines the default value for the first parameter.
    void Executor::setDefaultValues(const MultiType& arg1)
    {
        this->defaultValue_[0] = arg1;
    }

    /// Defines the default value for the first two parameters.
    void Executor::setDefaultValues(const MultiType& arg1, const MultiType& arg2)
    {
        this->defaultValue_[0] = arg1;
        this->defaultValue_[1] = arg2;
    }

    /// Defines the default value for the first three parameters.
    void Executor::setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3)
    {
        this->defaultValue_[0] = arg1;
        this->defaultValue_[1] = arg2;
        this->defaultValue_[2] = arg3;
    }

    /// Defines the default value for the first four parameters.
    void Executor::setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4)
    {
        this->defaultValue_[0] = arg1;
        this->defaultValue_[1] = arg2;
        this->defaultValue_[2] = arg3;
        this->defaultValue_[3] = arg4;
    }

    /// Defines the default value for the first five parameters.
    void Executor::setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5)
    {
        this->defaultValue_[0] = arg1;
        this->defaultValue_[1] = arg2;
        this->defaultValue_[2] = arg3;
        this->defaultValue_[3] = arg4;
        this->defaultValue_[4] = arg5;
    }

    /// Defines the default value for a parameter with given index (the first parameter has index 0).
    void Executor::setDefaultValue(unsigned int index, const MultiType& arg)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            this->defaultValue_[index] = arg;
    }

    /// Returns true if the executor has a default value for each parameter of the wrapped function, so it can be called without passing additional arguments.
    bool Executor::allDefaultValuesSet() const
    {
        for (unsigned int i = 0; i < this->functor_->getParamCount(); i++)
            if (this->defaultValue_[i].null())
                return false;

        return true;
    }
}
