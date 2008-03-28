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
 *   Inspiration: Executor by Benjamin Grauer
 */

#ifndef _Executor_H__
#define _Executor_H__

#include "CorePrereqs.h"
#include "Functor.h"
#include "Debug.h"
#include "util/SubString.h"
#include "util/String.h"


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
        std::string temp = getStripped(params); \
        if ((temp != "") && (temp.size() != 0)) \
        { \
            COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using whole string: " << params << std::endl; \
            EXECUTOR_PARSE_FUNCTORCALL(mode)(EXECUTOR_PARSE_OBJECT(mode, 1) MultiTypeMath(params)); \
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
        SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0'); \
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
        MultiTypeMath param[paramCount]; \
        COUT(5) << "Calling Executor " << this->name_ << " through parser with " << paramCount << " parameters, using " << tokens.size() << " tokens ("; \
        for (unsigned int i = 0; i < tokens.size(); i++) \
        { \
            param[i] = tokens[i]; \
            if (i != 0) \
            { \
                COUT(5) << ", "; \
            } \
            COUT(5) << tokens[i]; \
        } \
        COUT(5) << ") and " << (paramCount - tokens.size()) << " default values ("; \
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

namespace AccessLevel
{
    enum Level
    {
        None,
        User,
        Admin,
        Offline,
        Debug,
        Disabled
    };
}

namespace orxonox
{
    class _CoreExport Executor
    {
        public:
            Executor(Functor* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None);
            virtual ~Executor();

            inline void operator()() const
                { (*this->functor_)(this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiTypeMath& param1) const
                { (*this->functor_)(param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiTypeMath& param1, const MultiTypeMath& param2) const
                { (*this->functor_)(param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3) const
                { (*this->functor_)(param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4) const
                { (*this->functor_)(param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5) const
                { (*this->functor_)(param1, param2, param3, param4, param5); }

            bool parse(const std::string& params, const std::string& delimiter = " ") const;

            void setDescription(const std::string& description);
            const std::string& getDescription() const;

            void setDescriptionParam(int param, const std::string& description);
            const std::string& getDescriptionParam(int param) const;

            void setDescriptionReturnvalue(const std::string& description);
            const std::string& getDescriptionReturnvalue(int param) const;

            inline unsigned int getParamCount() const
                { return this->functor_->getParamCount(); }
            inline bool hasReturnvalue() const
                { return this->functor_->hasReturnvalue(); }
            inline FunctionType getType() const
                { return this->functor_->getType(); }
            inline MultiTypeMath getReturnvalue() const
                { return this->functor_->getReturnvalue(); }
            inline std::string getTypenameParam(unsigned int param) const
                { return this->functor_->getTypenameParam(param); }
            inline std::string getTypenameReturnvalue() const
                { return this->functor_->getTypenameReturnvalue(); }

            inline void setName(const std::string name)
                { this->name_ = name; }
            inline const std::string& getName() const
                { return this->name_; }

            inline void setAccessLevel(AccessLevel::Level level)
                { this->accessLevel_ = level; }
            inline AccessLevel::Level getAccessLevel() const
                { return this->accessLevel_; }

            void setDefaultValues(const MultiTypeMath& param1);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5);
            void setDefaultValue(unsigned int index, const MultiTypeMath& param);

            bool allDefaultValuesSet() const;
            inline bool defaultValueSet(unsigned int index) const
            {
                if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
                    return this->bAddedDefaultValue_[index];

                return false;
            }

        protected:
            Functor* functor_;
            std::string name_;
            MultiTypeMath defaultValue_[MAX_FUNCTOR_ARGUMENTS];
            bool bAddedDefaultValue_[MAX_FUNCTOR_ARGUMENTS];

        private:
            LanguageEntryLabel description_;
            LanguageEntryLabel descriptionReturnvalue_;
            LanguageEntryLabel descriptionParam_[MAX_FUNCTOR_ARGUMENTS];

            bool bAddedDescription_;
            bool bAddedDescriptionReturnvalue_;
            bool bAddedDescriptionParam_[MAX_FUNCTOR_ARGUMENTS];

            AccessLevel::Level accessLevel_;
    };

    class _CoreExport ExecutorStatic : public Executor
    {
        public:
            ExecutorStatic(FunctorStatic* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None) : Executor(functor, name, level) {}
            virtual ~ExecutorStatic() {}
    };

    template <class T>
    class ExecutorMember : public Executor
    {
        public:
            ExecutorMember(FunctorMember<T>* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None) : Executor(functor, name, level) {}
            virtual ~ExecutorMember() {}

            inline void operator()(T* object) const
                { (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiTypeMath& param1) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiTypeMath& param1, const MultiTypeMath& param2) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, param5); }


            inline void operator()(const T* object) const
                { (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0], this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiTypeMath& param1) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, this->defaultValue_[1], this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiTypeMath& param1, const MultiTypeMath& param2) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, this->defaultValue_[2], this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, this->defaultValue_[3], this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, this->defaultValue_[4]); }
            inline void operator()(const T* object, const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5) const
                { (*((FunctorMember<T>*)this->functor_))(object, param1, param2, param3, param4, param5); }

            inline void setObject(T* object) const
                { ((FunctorMember<T>*)this->functor_)->setObject(object); }
            inline void setObject(const T* object) const
                { ((FunctorMember<T>*)this->functor_)->setObject(object); }

            bool parse(T* object, const std::string& params, const std::string& delimiter = " ") const
            {
                EXECUTOR_PARSE(object);
            }

            bool parse(const T* object, const std::string& params, const std::string& delimiter = " ") const
            {
                EXECUTOR_PARSE(object);
            }
    };

    inline Executor* createExecutor(Functor* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None)
    {
        return new Executor(functor, name, level);
    }

    template <class T>
    inline ExecutorMember<T>* createExecutor(FunctorMember<T>* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None)
    {
        return new ExecutorMember<T>(functor, name, level);
    }

    inline ExecutorStatic* createExecutor(FunctorStatic* functor, const std::string& name = "", AccessLevel::Level level = AccessLevel::None)
    {
        return new ExecutorStatic(functor, name, level);
    }
}

#endif /* _Executor_H__ */
