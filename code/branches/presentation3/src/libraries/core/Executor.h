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

#include <string>
#include "util/MultiType.h"
#include "Functor.h"

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
                static_cast<FunctorMember<T>*>(this->functor_)->setObject(object);
                if (Executor::parse(params, delimiter))
                    return true;
                else
                {
                    static_cast<FunctorMember<T>*>(this->functor_)->setObject((T*)NULL);
                    return false;
                }
            }

            bool parse(const T* object, const std::string& params, const std::string& delimiter = " ") const
            {
                static_cast<FunctorMember<T>*>(this->functor_)->setObject(object);
                if (Executor::parse(params, delimiter))
                    return true;
                else
                {
                    static_cast<FunctorMember<T>*>(this->functor_)->setObject((T*)NULL);
                    return false;
                }
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
