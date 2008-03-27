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

namespace orxonox
{
    class _CoreExport Executor
    {
        public:
            Executor(Functor* functor, const std::string& name = "");
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

            void setName(const std::string name);
            const std::string& getName() const;

            void description(const std::string& description);
            const std::string& getDescription() const;

            void descriptionParam(int param, const std::string& description);
            const std::string& getDescriptionParam(int param) const;

            void descriptionReturnvalue(const std::string& description);
            const std::string& getDescriptionReturnvalue(int param) const;

            inline int getParamCount() const
                { return this->functor_->getParamCount(); }
            inline bool hasReturnvalue() const
                { return this->functor_->hasReturnvalue(); }
            inline FunctionType getType() const
                { return this->functor_->getType(); }
            inline MultiTypeMath getReturnvalue() const
                { return this->functor_->getReturnvalue(); }
            inline std::string getTypenameParam(int param) const
                { return this->functor_->getTypenameParam(param); }
            inline std::string getTypenameReturnvalue() const
                { return this->functor_->getTypenameReturnvalue(); }

            void setDefaultValues(const MultiTypeMath& param1);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4);
            void setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5);
            void setDefaultValue(int index, const MultiTypeMath& param);

        protected:
            Functor* functor_;

        private:
            std::string name_;

            LanguageEntryLabel description_;
            LanguageEntryLabel descriptionReturnvalue_;
            LanguageEntryLabel descriptionParam_[MAX_FUNCTOR_ARGUMENTS];

            bool bAddedDescription_;
            bool bAddedDescriptionReturnvalue_;
            bool bAddedDescriptionParam_[MAX_FUNCTOR_ARGUMENTS];

            MultiTypeMath defaultValue_[MAX_FUNCTOR_ARGUMENTS];
            bool bAddedDefaultValue_[MAX_FUNCTOR_ARGUMENTS];
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

            bool parse(T* object, const std::string& params, const std::string& delimiter) const
            {
                unsigned int paramCount = this->functor_->getParamCount();

                if (paramCount == 0)
                {
                    COUT(5) << "Calling Executor " << this->name_ << " through parser without parameters." << std::endl;
                    (*((FunctorMember<T>*)this->functor_))(object);
                }
                else if (paramCount == 1)
                {
                    std::string temp = getStripped(params);
                    if ((temp != "") && (temp.size() != 0))
                    {
                        COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using whole string: " << params << std::endl;
                        (*((FunctorMember<T>*)this->functor_))(object, MultiTypeMath(params));
                    }
                    else if (this->bAddedDefaultValue_[0])
                    {
                        COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using default value: " << this->defaultValue_[0] << std::endl;
                        (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0]);
                    }
                    else
                    {
                        COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given." << std::endl;
                        return false;
                    }
                }
                else
                {
                    SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

                    for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++)
                    {
                        if (!this->bAddedDefaultValue_[i])
                        {
                            COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given." << std::endl;
                            return false;
                        }
                    }

                    MultiTypeMath param[paramCount];
                    COUT(5) << "Calling Executor " << this->name_ << " through parser with " << paramCount << " parameters, using " << tokens.size() << " tokens (";
                    for (unsigned int i = 0; i < tokens.size(); i++)
                    {
                        param[i] = tokens[i];
                        if (i != 0)
                        {
                            COUT(5) << ", ";
                        }
                        COUT(5) << tokens[i];
                    }
                    COUT(5) << ") and " << (paramCount - tokens.size()) << " default values (";
                    for (unsigned int i = tokens.size(); i < paramCount; i++)
                    {
                        param[i] = this->defaultValue_[i];
                        if (i != 0)
                        {
                            COUT(5) << ", ";
                        }
                        COUT(5) << this->defaultValue_[i];
                    }
                    COUT(5) << ")." << std::endl;

                    switch(paramCount)
                    {
                        case 2:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1]);
                            break;
                        case 3:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2]);
                            break;
                        case 4:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2], param[3]);
                            break;
                        case 5:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2], param[3], param[4]);
                            break;
                    }
                }

                return true;
            }

            bool parse(const T* object, const std::string& params, const std::string& delimiter) const
            {
                unsigned int paramCount = this->functor_->getParamCount();

                if (paramCount == 0)
                {
                    COUT(5) << "Calling Executor " << this->name_ << " through parser without parameters." << std::endl;
                    (*((FunctorMember<T>*)this->functor_))(object);
                }
                else if (paramCount == 1)
                {
                    std::string temp = getStripped(params);
                    if ((temp != "") && (temp.size() != 0))
                    {
                        COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using whole string: " << params << std::endl;
                        (*((FunctorMember<T>*)this->functor_))(object, MultiTypeMath(params));
                    }
                    else if (this->bAddedDefaultValue_[0])
                    {
                        COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using default value: " << this->defaultValue_[0] << std::endl;
                        (*((FunctorMember<T>*)this->functor_))(object, this->defaultValue_[0]);
                    }
                    else
                    {
                        COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given." << std::endl;
                        return false;
                    }
                }
                else
                {
                    SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

                    for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++)
                    {
                        if (!this->bAddedDefaultValue_[i])
                        {
                            COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given." << std::endl;
                            return false;
                        }
                    }

                    MultiTypeMath param[paramCount];
                    COUT(5) << "Calling Executor " << this->name_ << " through parser with " << paramCount << " parameters, using " << tokens.size() << " tokens (";
                    for (unsigned int i = 0; i < tokens.size(); i++)
                    {
                        param[i] = tokens[i];
                        if (i != 0)
                        {
                            COUT(5) << ", ";
                        }
                        COUT(5) << tokens[i];
                    }
                    COUT(5) << ") and " << (paramCount - tokens.size()) << " default values (";
                    for (unsigned int i = tokens.size(); i < paramCount; i++)
                    {
                        param[i] = this->defaultValue_[i];
                        if (i != 0)
                        {
                            COUT(5) << ", ";
                        }
                        COUT(5) << this->defaultValue_[i];
                    }
                    COUT(5) << ")." << std::endl;

                    switch(paramCount)
                    {
                        case 2:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1]);
                            break;
                        case 3:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2]);
                            break;
                        case 4:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2], param[3]);
                            break;
                        case 5:
                            (*((FunctorMember<T>*)this->functor_))(object, param[0], param[1], param[2], param[3], param[4]);
                            break;
                    }
                }

                return true;
            }
    };
}

#endif /* _Executor_H__ */
