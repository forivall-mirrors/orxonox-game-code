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

namespace orxonox
{
    class _CoreExport Executor
    {
        public:
            Executor(Functor* functor, const std::string& name = "");
            virtual ~Executor();

            inline virtual void operator()(const MultiTypeMath& param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
                { (*this->functor_)(param1, param2, param3, param4, param5); }

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

        protected:
            Functor* functor_;

        private:
            std::string name_;

            LanguageEntryLabel description_;
            LanguageEntryLabel descriptionReturnvalue_;
            LanguageEntryLabel descriptionParam_[5];

            bool bAddedDescription_;
            bool bAddedDescriptionReturnvalue_;
            bool bAddedDescriptionParam_[5];
    };

    class _CoreExport ExecutorStatic : public Executor
    {
        public:
            ExecutorStatic(FunctorStatic* functor, const std::string& name = "") : Executor(functor, name) {}
            virtual ~ExecutorStatic() {}

            inline virtual void operator()(const MultiTypeMath& param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
                { (*this->functor_)(param1, param2, param3, param4, param5); }
    };

    template <class T>
    class ExecutorMember : public Executor
    {
        public:
            ExecutorMember(FunctorMember<T>* functor, const std::string& name = "") : Executor(functor, name) {}
            virtual ~ExecutorMember() {}

            inline virtual void operator()(T* object, const MultiTypeMath& param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
                { (*this->functor_)(object, param1, param2, param3, param4, param5); }
            inline virtual void operator()(const T* object, const MultiTypeMath param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
                { (*this->functor_)(object, param1, param2, param3, param4, param5); }
            inline virtual void operator()(const MultiTypeMath& param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
                { (*this->functor_)(param1, param2, param3, param4, param5); }

            inline void setObject(T* object)
                { this->functor_->setObject(object); }
            inline void setObject(const T* object)
                { this->functor_->setObject(object); }
    };
}

#endif /* _Executor_H__ */
