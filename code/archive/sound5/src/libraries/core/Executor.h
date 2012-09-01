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

#ifndef _Executor_H__
#define _Executor_H__

#include "CorePrereqs.h"

#include <algorithm>
#include <string>

#include "util/Debug.h"
#include "util/Math.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "Functor.h"


#define EXECUTOR_PARSE_FUNCTORCALL(mode) EXECUTOR_PARSE_FUNCTORCALL##mode
#define EXECUTOR_PARSE_FUNCTORCALLnormal (*this->functor_)
#define EXECUTOR_PARSE_FUNCTORCALLobject (*((FunctorMember<T>*)this->functor_))

#define EXECUTOR_PARSE_OBJECT(mode, comma) EXECUTOR_PARSE_OBJECT##mode##comma
#define EXECUTOR_PARSE_OBJECTnormal0
#define EXECUTOR_PARSE_OBJECTnormal1
#define EXECUTOR_PARSE_OBJECTobject0 object
#define EXECUTOR_PARSE_OBJECTobject1 object,

#define EXECUTOR_PARSE(mode) \
    unsigned int paramCount = this->functor_->getParamCount(); \
    \
    if (paramCount == 0) \
    { \
        COUT(5) << "Calling Executor " << this->name_ << " through parser without parameters." << std::endl; \
        EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 0)); \
    } \
    else if (paramCount == 1) \
    { \
        const std::string& temp = getStripped(params); \
        if (!temp.empty()) \
        { \
            COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using whole string: " << params << std::endl; \
            EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) MultiType(params)); \
        } \
        else if (this->bAddedDefaultValue_[0]) \
        { \
            COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using default value: " << this->defaultValue_[0] << std::endl; \
            EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) this->defaultValue_[0]); \
        } \
        else \
        { \
            COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given (input: " << temp << ")." << std::endl; \
            return false; \
        } \
    } \
    else \
    { \
        SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', true, '"', true, '(', ')', true, '\0'); \
        \
        for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++) \
        { \
            if (!this->bAddedDefaultValue_[i]) \
            { \
                COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given (input:" << params << ")." << std::endl; \
                return false; \
            } \
        } \
        \
        MultiType param[MAX_FUNCTOR_ARGUMENTS]; \
        COUT(5) << "Calling Executor " << this->name_ << " through parser with " << paramCount << " parameters, using " << tokens.size() << " tokens ("; \
        for (unsigned int i = 0; i < tokens.size() && i < MAX_FUNCTOR_ARGUMENTS; i++) \
        { \
            param[i] = tokens[i]; \
            if (i != 0) \
            { \
                COUT(5) << ", "; \
            } \
            COUT(5) << tokens[i]; \
        } \
        COUT(5) << ") and " << std::max(static_cast<int>(paramCount) - static_cast<int>(tokens.size()), 0) << " default values ("; \
        for (unsigned int i = tokens.size(); i < paramCount; i++) \
        { \
            param[i] = this->defaultValue_[i]; \
            if (i != 0) \
            { \
                COUT(5) << ", "; \
            } \
            COUT(5) << this->defaultValue_[i]; \
        } \
        COUT(5) << ")." << std::endl; \
        \
        if ((tokens.size() > paramCount) && (this->functor_->getTypenameParam(paramCount - 1) == "string")) \
            param[paramCount - 1] = tokens.subSet(paramCount - 1).join(); \
        \
        switch(paramCount) \
        { \
            case 2: \
                EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) param[0], param[1]); \
                break; \
            case 3: \
                EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) param[0], param[1], param[2]); \
                break; \
            case 4: \
                EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) param[0], param[1], param[2], param[3]); \
                break; \
            case 5: \
                EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) param[0], param[1], param[2], param[3], param[4]); \
                break; \
        } \
    } \
    \
    return true

namespace orxonox
{
    class _CoreExport Executor
    {
        public:
            Executor(Functor* functor, const std::string& name = "");
            virtual ~Executor();

            inline void operator()() const
                { (*this->functor_)(this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiType& param1) const
                { (*this->functor_)(param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiType& param1, const MultiType& param2) const
                { (*this->functor_)(param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiType& param1, const MultiType& param2, const MultiType& param3) const
                { (*this->functor_)(param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4) const
                { (*this->functor_)(param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) const
                { (*this->functor_)(param1, param2, param3, param4, param5); }

            bool parse(const std::string& params, const std::string& delimiter = " ") const;

            bool evaluate(const std::string& params, MultiType param[5], const std::string& delimiter = " ") const;

            Executor& setDescription(const std::string& description);
            const std::string& getDescription() const;

            Executor& setDescriptionParam(unsigned int param, const std::string& description);
            const std::string& getDescriptionParam(unsigned int param) const;

            Executor& setDescriptionReturnvalue(const std::string& description);
            const std::string& getDescriptionReturnvalue(int param) const;

            inline Functor* getFunctor() const
                { return this->functor_; }
            inline unsigned int getParamCount() const
                { return this->functor_->getParamCount(); }
            inline bool hasReturnvalue() const
                { return this->functor_->hasReturnvalue(); }
            inline FunctionType::Value getType() const
                { return this->functor_->getType(); }
            inline const MultiType& getReturnvalue() const
                { return this->functor_->getReturnvalue(); }
            inline const std::string& getTypenameParam(unsigned int param) const
                { return this->functor_->getTypenameParam(param); }
            inline const std::string& getTypenameReturnvalue() const
                { return this->functor_->getTypenameReturnvalue(); }

            inline void setName(const std::string& name)
                { this->name_ = name; }
            inline const std::string& getName() const
                { return this->name_; }

            Executor& setDefaultValues(const MultiType& param1);
            Executor& setDefaultValues(const MultiType& param1, const MultiType& param2);
            Executor& setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3);
            Executor& setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4);
            Executor& setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5);
            Executor& setDefaultValue(unsigned int index, const MultiType& param);

            inline MultiType getDefaultValue(unsigned int index) const
            {
                if (index < MAX_FUNCTOR_ARGUMENTS)
                    return this->defaultValue_[index];

                return MT_Type::Null;
            }

            bool allDefaultValuesSet() const;
            inline bool defaultValueSet(unsigned int index) const
            {
                if (index < MAX_FUNCTOR_ARGUMENTS)
                    return this->bAddedDefaultValue_[index];

                return false;
            }

        protected:
            Functor* functor_;
            std::string name_;
            MultiType defaultValue_[MAX_FUNCTOR_ARGUMENTS];
            bool bAddedDefaultValue_[MAX_FUNCTOR_ARGUMENTS];

        private:
            LanguageEntryLabel description_;
            LanguageEntryLabel descriptionReturnvalue_;
            LanguageEntryLabel descriptionParam_[MAX_FUNCTOR_ARGUMENTS];

            bool bAddedDescription_;
            bool bAddedDescriptionReturnvalue_;
            bool bAddedDescriptionParam_[MAX_FUNCTOR_ARGUMENTS];
    };

    class _CoreExport ExecutorStatic : public Executor
    {
        public:
            ExecutorStatic(FunctorStatic* functor, const std::string& name = "") : Executor(functor, name) {}
            virtual ~ExecutorStatic() {}
    };

    template <class T>
    class ExecutorMember : public Executor
    {
        public:
            ExecutorMember(FunctorMember<T>* functor, const std::string& name = "") : Executor(functor, name) {}
            virtual ~ExecutorMember() {}

            using Executor::operator();

            inline void operator()(T* object) const
                { (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiType& param1) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiType& param1, const MultiType& param2) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, param5); }


            inline void operator()(const T* object) const
                { (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiType& param1) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiType& param1, const MultiType& param2) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, param5); }

            inline void setObject(T* object) const
                { ((FunctorMember<T>*)this->functor_)->setObject(object); }
            inline void setObject(const T* object) const
                { ((FunctorMember<T>*)this->functor_)->setObject(object); }

            using Executor::parse;

            bool parse(T* object, const std::string& params, const std::string& delimiter = " ") const
            {
                EXECUTOR_PARSE(object);
            }

            bool parse(const T* object, const std::string& params, const std::string& delimiter = " ") const
            {
                EXECUTOR_PARSE(object);
            }
    };

    inline Executor* createExecutor(Functor* functor, const std::string& name = "")
    {
        return new Executor(functor, name);
    }

    template <class T>
    inline ExecutorMember<T>* createExecutor(FunctorMember<T>* functor, const std::string& name = "")
    {
        return new ExecutorMember<T>(functor, name);
    }

    inline ExecutorStatic* createExecutor(FunctorStatic* functor, const std::string& name = "")
    {
        return new ExecutorStatic(functor, name);
    }
}

#endif /* _Executor_H__ */
