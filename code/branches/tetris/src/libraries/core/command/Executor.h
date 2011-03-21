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
    @defgroup FunctorExecutor Functor and Executor
    @ingroup Command
*/

/**
    @file
    @ingroup Command FunctorExecutor
    @brief Declaration of the orxonox::Executor class and the createExecutor() functions.

    @anchor ExecutorExample

    orxonox::Executor is used to wrap an orxonox::Functor and to store default values for
    its parameters. Usually one uses the function createExecutor() to create a new executor.
    This function returns an orxonox::ExecutorPtr which is a typedef of @ref orxonox::SharedPtr
    "SharedPtr<Executor>", used to manage the pointer to the executor.

    Executors are mostly used to execute callbacks. Because some callback functions need arguments,
    Executor provides an easy interface to store these arguments as default values, so the
    executor can also be executed without arguments because it will use these default values.

    The Executor doesn't contain the function-pointer directly. Instead it wraps an instance
    of orxonox::Functor. See @ref FunctorExample "Functor.h" for more information and some
    examples.

    Example:
    @code
    void myFunction(int a, int b)                           // declare a static function
    {
        COUT(0) << "The sum is " << (a + b) << std::endl;   // print the sum of a and b to the console
    }

    FunctorPtr functor = createFunctor(&myFunction);        // create a functor that wraps the function-pointer
    ExecutorPtr executor = createExecutor(functor);         // create an executor that wraps the functor

    (*executor)(2, 5);                                      // calls the executor with arguments 2 and 5, prints "The sum is 7" to the console

    executor->setDefaultValue(1, 10);                       // sets the default-value for the second parameter (note: paramters start at index 0) to 10

    (*executor)(2);                                         // calls the executor with argument 2 and default-value 10, prints "The sum is 12"

    executor->setDefaultValue(0, 5);                        // sets the default-value for the first parameter to 5

    (*executor)();                                          // calls the executor with default-values only, prints "The sum is 15"
    @endcode

    Because executors that were created with createExecutor() are managed by an orxonox::SharedPtr,
    they don't need to be deleted after usage.
*/

#ifndef _Executor_H__
#define _Executor_H__

#include "core/CorePrereqs.h"

#include <string>
#include "util/MultiType.h"
#include "Functor.h"
#include "ExecutorPtr.h"

namespace orxonox
{
    /**
        @brief This class is used to wrap a Functor and to store default values for any of its parameters.

        @see See @ref ExecutorExample "Executor.h" for an example.
    */
    class _CoreExport Executor
    {
        public:
            Executor(const FunctorPtr& functor, const std::string& name = "");
            Executor(const Executor& other);
            virtual ~Executor();

            /// Calls the wrapped function with 0 arguments. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()() const
                { return (*this->functor_)(this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 1 argument. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(const MultiType& arg1) const
                { return (*this->functor_)(arg1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 2 arguments. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(const MultiType& arg1, const MultiType& arg2) const
                { return (*this->functor_)(arg1, arg2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 3 arguments. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3) const
                { return (*this->functor_)(arg1, arg2, arg3, this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 4 arguments. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4) const
                { return (*this->functor_)(arg1, arg2, arg3, arg4, this->defaultValue_[4]); }
            /// Calls the wrapped function with 5 arguments. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5) const
                { return (*this->functor_)(arg1, arg2, arg3, arg4, arg5); }

            MultiType parse(const std::string& arguments, int* error = 0, const std::string& delimiter = " ", bool bPrintError = false) const;
            MultiType parse(const SubString& arguments, int* error = 0, const std::string& delimiter = " ", bool bPrintError = false) const;

            int evaluateArguments(const SubString& arguments, MultiType arg[MAX_FUNCTOR_ARGUMENTS], int* error = 0, const std::string& delimiter = " ") const;

            /// Changes the functor.
            inline void setFunctor(const FunctorPtr& functor)
                { this->functor_ = functor; }
            /// Returns the functor.
            inline const FunctorPtr& getFunctor() const
                { return this->functor_; }

            /// Changes the name of the executor.
            inline void setName(const std::string& name)
                { this->name_ = name; }
            /// Returns the name of the executor.
            inline const std::string& getName() const
                { return this->name_; }

            /// Returns the number of parameters of the wrapped function.
            inline unsigned int getParamCount() const
                { return this->functor_->getParamCount(); }
            /// Returns true if the wrapped function returns a value.
            inline bool hasReturnvalue() const
                { return this->functor_->hasReturnvalue(); }
            /// Returns the type of the wrapped function (static or member).
            inline Functor::Type::Enum getType() const
                { return this->functor_->getType(); }
            /// Returns the name of the type of a parameter with given index (the first parameter has index 0).
            inline std::string getTypenameParam(unsigned int param) const
                { return this->functor_->getTypenameParam(param); }
            /// Returns the name of the type of the return value.
            inline std::string getTypenameReturnvalue() const
                { return this->functor_->getTypenameReturnvalue(); }

            void setDefaultValues(const MultiType& arg1);
            void setDefaultValues(const MultiType& arg1, const MultiType& arg2);
            void setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3);
            void setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4);
            void setDefaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5);
            void setDefaultValue(unsigned int index, const MultiType& arg);

            /// Returns the default value for a parameter with given index (the first parameter has index 0).
            inline MultiType getDefaultValue(unsigned int index) const
            {
                if (index < MAX_FUNCTOR_ARGUMENTS)
                    return this->defaultValue_[index];

                return MT_Type::Null;
            }

            bool allDefaultValuesSet() const;

            /// Returns true if the executor contains a default value for the parameter with given index (the first parameter has index 0).
            inline bool defaultValueSet(unsigned int index) const
            {
                if (index < MAX_FUNCTOR_ARGUMENTS)
                    return !this->defaultValue_[index].null();

                return false;
            }

        protected:
            FunctorPtr functor_;                                ///< The underlying Functor that wraps a function
            std::string name_;                                  ///< The name of the executor
            MultiType defaultValue_[MAX_FUNCTOR_ARGUMENTS];     ///< The default values, one for each parameter
    };

    /**
        @brief A child class of Executor, used to distinguish executors that wrap static functions from executors that wrap member-functions.

        Behaves exactly like Executor, because static functions need no special treatment.
    */
    class _CoreExport ExecutorStatic : public Executor
    {
        public:
            /// Constructor: Initializes the parent class
            ExecutorStatic(const FunctorStaticPtr& functor, const std::string& name = "") : Executor(functor, name) {}
            /// Destructor
            virtual ~ExecutorStatic() {}
    };

    /**
        @brief A child class of Executor, used for easier handling of non-static member-functions.

        Overloads some functions that call the underlying FunctorMember and additionally pass
        an object-pointer that is needed for non-static member-functions.
    */
    template <class T>
    class ExecutorMember : public Executor
    {
        public:
            /// Constructor: Initializes the parent class and the pointer to the member-functor.
            ExecutorMember(const FunctorMemberPtr<T>& functor, const std::string& name = "") : Executor(functor, name), functorMember_(functor) {}
            /// Destructor
            virtual ~ExecutorMember() {}

            using Executor::operator();

            /// Calls the wrapped function with 0 arguments and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object) const
                { return (*this->functorMember_)(object, this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 1 argument and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object, const MultiType& arg1) const
                { return (*this->functorMember_)(object, arg1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 2 arguments and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object, const MultiType& arg1, const MultiType& arg2) const
                { return (*this->functorMember_)(object, arg1, arg2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 3 arguments and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object, const MultiType& arg1, const MultiType& arg2, const MultiType& arg3) const
                { return (*this->functorMember_)(object, arg1, arg2, arg3, this->defaultValue_[3], this->defaultValue_[4]); }
            /// Calls the wrapped function with 4 arguments and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object, const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4) const
                { return (*this->functorMember_)(object, arg1, arg2, arg3, arg4, this->defaultValue_[4]); }
            /// Calls the wrapped function with 5 arguments and an object-pointer. If the function needs more arguments, the executor's default values are used.
            inline MultiType operator()(T* object, const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5) const
                { return (*this->functorMember_)(object, arg1, arg2, arg3, arg4, arg5); }

            /// Changes the object-pointer of the underlying FunctorMember.
            inline void setObject(T* object) const
                { this->functorMember_->setObject(object); }
            /// Returns the object-pointer of the underlying FunctorMember.
            inline T* getObject() const
                { return this->functorMember_->getObject(); }

            using Executor::parse;

            /// Overloads Executor::parse() with an additional object-pointer.
            MultiType parse(T* object, const std::string& arguments, int* error = 0, const std::string& delimiter = " ", bool bPrintError = false) const
            {
                T* oldobject = this->functorMember_->getObject();

                this->functorMember_->setObject(object);
                const MultiType& result = this->Executor::parse(arguments, error, delimiter, bPrintError);
                this->functorMember_->setObject(oldobject);

                return result;
            }

        protected:
            FunctorMemberPtr<T> functorMember_;     ///< A pointer to the FunctorMember is stored separately to avoid casting when executing the function.
    };

    /// Creates a new Executor that wraps a given Functor.
    inline ExecutorPtr createExecutor(const FunctorPtr& functor, const std::string& name = "")
    {
        return new Executor(functor, name);
    }

    /// Creates a new ExecutorMember that wraps a given FunctorMember.
    template <class T>
    inline ExecutorMemberPtr<T> createExecutor(const FunctorMemberPtr<T>& functor, const std::string& name = "")
    {
        return new ExecutorMember<T>(functor, name);
    }

    /// Creates a new ExecutorStatic that wraps a given FunctorStatic.
    inline ExecutorStaticPtr createExecutor(const FunctorStaticPtr& functor, const std::string& name = "")
    {
        return new ExecutorStatic(functor, name);
    }
}

#endif /* _Executor_H__ */
