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

#ifndef _Functor_H__
#define _Functor_H__

#include <typeinfo>

#include "core/CorePrereqs.h"

#include "util/Debug.h"
#include "util/MultiType.h"
#include "FunctorPtr.h"

namespace orxonox
{
    const unsigned int MAX_FUNCTOR_ARGUMENTS = 5;

    template <class T>
    inline std::string _typeToString() { return "unknown"; }

    template <> inline std::string _typeToString<void>()               { return ""; }
    template <> inline std::string _typeToString<int>()                { return "int"; }
    template <> inline std::string _typeToString<unsigned int>()       { return "uint"; }
    template <> inline std::string _typeToString<char>()               { return "char"; }
    template <> inline std::string _typeToString<unsigned char>()      { return "uchar"; }
    template <> inline std::string _typeToString<short>()              { return "short"; }
    template <> inline std::string _typeToString<unsigned short>()     { return "ushort"; }
    template <> inline std::string _typeToString<long>()               { return "long"; }
    template <> inline std::string _typeToString<unsigned long>()      { return "ulong"; }
    template <> inline std::string _typeToString<long long>()          { return "longlong"; }
    template <> inline std::string _typeToString<unsigned long long>() { return "ulonglong"; }
    template <> inline std::string _typeToString<float>()              { return "float"; }
    template <> inline std::string _typeToString<double>()             { return "double"; }
    template <> inline std::string _typeToString<long double>()        { return "longdouble"; }
    template <> inline std::string _typeToString<bool>()               { return "bool"; }
    template <> inline std::string _typeToString<std::string>()        { return "string"; }
    template <> inline std::string _typeToString<Vector2>()            { return "Vector2"; }
    template <> inline std::string _typeToString<Vector3>()            { return "Vector3"; }
    template <> inline std::string _typeToString<Quaternion>()         { return "Quaternion"; }
    template <> inline std::string _typeToString<ColourValue>()        { return "ColourValue"; }
    template <> inline std::string _typeToString<Radian>()             { return "Radian"; }
    template <> inline std::string _typeToString<Degree>()             { return "Degree"; }

    template <class T>
    inline std::string typeToString() { return _typeToString<typename Loki::TypeTraits<T>::UnqualifiedReferredType>(); }

    class _CoreExport Functor
    {
        public:
            struct Type
            {
                enum Enum
                {
                    Static,
                    Member
                };
            };

        public:
            virtual MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            virtual Type::Enum getType() const = 0;
            virtual unsigned int getParamCount() const = 0;
            virtual bool hasReturnvalue() const = 0;

            virtual std::string getTypenameParam(unsigned int param) const = 0;
            virtual std::string getTypenameReturnvalue() const = 0;

            virtual void evaluateParam(unsigned int index, MultiType& param) const = 0;

            virtual void setRawObjectPointer(void* object) {}
            virtual void* getRawObjectPointer() const { return 0; }

            template <class F>
            inline bool setFunction(F* function)
            {
                if (this->getFullIdentifier() == typeid(F*))
                {
                    modifyFunctor(this, function);
                    return true;
                }
                return false;
            }

            virtual const std::type_info& getFullIdentifier() const = 0;
            virtual const std::type_info& getHeaderIdentifier() const = 0;
            virtual const std::type_info& getHeaderIdentifier(unsigned int params) const = 0;
    };

    namespace detail
    {
        template <class O>
        struct FunctorTypeStatic
        { enum { result = false }; };
        template <>
        struct FunctorTypeStatic<void>
        { enum { result = true }; };
    }

    template <class O>
    class FunctorMember : public Functor
    {
        public:
            FunctorMember(O* object = 0) : object_(object) {}

            virtual MultiType operator()(O* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null)
            {
                if (detail::FunctorTypeStatic<O>::result || this->object_)
                    return (*this)(this->object_, param1, param2, param3, param4, param5);
                else
                {
                    COUT(1) << "Error: Can't execute FunctorMember, no object set." << std::endl;
                    return MT_Type::Null;
                }
            }

            Functor::Type::Enum getType() const
                { return detail::FunctorTypeStatic<O>::result ? Functor::Type::Static : Functor::Type::Member; }

            inline void setObject(O* object)
                { this->object_ = object;}
            inline O* getObject() const
                { return this->object_; }

            inline void setRawObjectPointer(void* object)
                { this->object_ = (O*)object; }
            inline void* getRawObjectPointer() const
                { return this->object_; }

        protected:
            O* object_;
    };

    typedef FunctorMember<void> FunctorStatic;

    template <class F, class O = void>
    class FunctorPointer : public FunctorMember<O>
    {
        public:
            FunctorPointer(F functionPointer, O* object = 0) : FunctorMember<O>(object), functionPointer_(functionPointer) {}

            inline void setFunction(F functionPointer)
                { this->functionPointer_ = functionPointer; }
            inline F getFunction() const
                { return this->functionPointer_; }

            const std::type_info& getFullIdentifier() const
                { return typeid(F); }

        protected:
            F functionPointer_;
    };

    namespace detail
    {
        template <class R, class O, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctionPointer                                            { typedef R (O::*Type)(P1, P2, P3, P4, P5); };
        template <class R, class O, class P1, class P2, class P3, class P4, class P5>               struct FunctionPointer<R, O, false, P1, P2, P3, P4, P5>           { typedef R (O::*Type)(P1, P2, P3, P4, P5); };
        template <class R, class O, class P1, class P2, class P3, class P4>                         struct FunctionPointer<R, O, false, P1, P2, P3, P4, void>         { typedef R (O::*Type)(P1, P2, P3, P4); };
        template <class R, class O, class P1, class P2, class P3>                                   struct FunctionPointer<R, O, false, P1, P2, P3, void, void>       { typedef R (O::*Type)(P1, P2, P3); };
        template <class R, class O, class P1, class P2>                                             struct FunctionPointer<R, O, false, P1, P2, void, void, void>     { typedef R (O::*Type)(P1, P2); };
        template <class R, class O, class P1>                                                       struct FunctionPointer<R, O, false, P1, void, void, void, void>   { typedef R (O::*Type)(P1); };
        template <class R, class O>                                                                 struct FunctionPointer<R, O, false, void, void, void, void, void> { typedef R (O::*Type)(); };
        template <class R, class O, class P1, class P2, class P3, class P4, class P5> struct FunctionPointer<R, O, true, P1, P2, P3, P4, P5>           { typedef R (O::*Type)(P1, P2, P3, P4, P5) const; };
        template <class R, class O, class P1, class P2, class P3, class P4>           struct FunctionPointer<R, O, true, P1, P2, P3, P4, void>         { typedef R (O::*Type)(P1, P2, P3, P4) const; };
        template <class R, class O, class P1, class P2, class P3>                     struct FunctionPointer<R, O, true, P1, P2, P3, void, void>       { typedef R (O::*Type)(P1, P2, P3) const; };
        template <class R, class O, class P1, class P2>                               struct FunctionPointer<R, O, true, P1, P2, void, void, void>     { typedef R (O::*Type)(P1, P2) const; };
        template <class R, class O, class P1>                                         struct FunctionPointer<R, O, true, P1, void, void, void, void>   { typedef R (O::*Type)(P1) const; };
        template <class R, class O>                                                   struct FunctionPointer<R, O, true, void, void, void, void, void> { typedef R (O::*Type)() const; };
        template <class R, class P1, class P2, class P3, class P4, class P5> struct FunctionPointer<R, void, false, P1, P2, P3, P4, P5>           { typedef R (*Type)(P1, P2, P3, P4, P5); };
        template <class R, class P1, class P2, class P3, class P4>           struct FunctionPointer<R, void, false, P1, P2, P3, P4, void>         { typedef R (*Type)(P1, P2, P3, P4); };
        template <class R, class P1, class P2, class P3>                     struct FunctionPointer<R, void, false, P1, P2, P3, void, void>       { typedef R (*Type)(P1, P2, P3); };
        template <class R, class P1, class P2>                               struct FunctionPointer<R, void, false, P1, P2, void, void, void>     { typedef R (*Type)(P1, P2); };
        template <class R, class P1>                                         struct FunctionPointer<R, void, false, P1, void, void, void, void>   { typedef R (*Type)(P1); };
        template <class R>                                                   struct FunctionPointer<R, void, false, void, void, void, void, void> { typedef R (*Type)(); };

        template <class R, class O, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller                                              { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1, param2, param3, param4, param5); } };
        template <class R, class O, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<R, O, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1, param2, param3, param4); } };
        template <class R, class O, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<R, O, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1, param2, param3); } };
        template <class R, class O, bool isconst, class P1, class P2>                               struct FunctorCaller<R, O, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1, param2); } };
        template <class R, class O, bool isconst, class P1>                                         struct FunctorCaller<R, O, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1); } };
        template <class R, class O, bool isconst>                                                   struct FunctorCaller<R, O, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, void, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(); } };
        template <class O, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<void, O, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1, param2, param3, param4, param5); return MT_Type::Null; } };
        template <class O, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<void, O, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, P4, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1, param2, param3, param4); return MT_Type::Null; } };
        template <class O, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<void, O, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1, param2, param3); return MT_Type::Null; } };
        template <class O, bool isconst, class P1, class P2>                               struct FunctorCaller<void, O, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1, param2); return MT_Type::Null; } };
        template <class O, bool isconst, class P1>                                         struct FunctorCaller<void, O, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1); return MT_Type::Null; } };
        template <class O, bool isconst>                                                   struct FunctorCaller<void, O, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, void, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(); return MT_Type::Null; } };
        template <class R, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<R, void, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, P4, P5>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1, param2, param3, param4, param5); } };
        template <class R, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<R, void, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, P4, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1, param2, param3, param4); } };
        template <class R, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<R, void, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1, param2, param3); } };
        template <class R, bool isconst, class P1, class P2>                               struct FunctorCaller<R, void, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1, param2); } };
        template <class R, bool isconst, class P1>                                         struct FunctorCaller<R, void, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1); } };
        template <class R, bool isconst>                                                   struct FunctorCaller<R, void, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, void, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(); } };
        template <bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<void, void, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, P4, P5>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1, param2, param3, param4, param5); return MT_Type::Null; } };
        template <bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<void, void, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, P4, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1, param2, param3, param4); return MT_Type::Null; } };
        template <bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<void, void, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1, param2, param3); return MT_Type::Null; } };
        template <bool isconst, class P1, class P2>                               struct FunctorCaller<void, void, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1, param2); return MT_Type::Null; } };
        template <bool isconst, class P1>                                         struct FunctorCaller<void, void, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1); return MT_Type::Null; } };
        template <bool isconst>                                                   struct FunctorCaller<void, void, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, void, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(); return MT_Type::Null; } };

        template <class R, class P1, class P2, class P3, class P4, class P5>
        struct FunctorHeaderIdentifier
        {};

        template <class T>
        struct FunctorHasReturnvalue
        { enum { result = true }; };
        template <>
        struct FunctorHasReturnvalue<void>
        { enum { result = false }; };

        template <class P1, class P2, class P3, class P4, class P5>
        struct FunctorParamCount
        { enum { result = 5 }; };
        template <class P1, class P2, class P3, class P4>
        struct FunctorParamCount<P1, P2, P3, P4, void>
        { enum { result = 4 }; };
        template <class P1, class P2, class P3>
        struct FunctorParamCount<P1, P2, P3, void, void>
        { enum { result = 3 }; };
        template <class P1, class P2>
        struct FunctorParamCount<P1, P2, void, void, void>
        { enum { result = 2 }; };
        template <class P1>
        struct FunctorParamCount<P1, void, void, void, void>
        { enum { result = 1 }; };
        template <>
        struct FunctorParamCount<void, void, void, void, void>
        { enum { result = 0 }; };
    }

    template <class R, class O, bool isconst, class P1, class P2, class P3, class P4, class P5>
    class FunctorTemplate : public FunctorPointer<typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type, O>
    {
        public:
            FunctorTemplate(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object = 0) : FunctorPointer<typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type, O>(functionPointer, object) {}

            MultiType operator()(O* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null)
            {
                return detail::FunctorCaller<R, O, isconst, P1, P2, P3, P4, P5>::call(this->functionPointer_, object, param1, param2, param3, param4, param5);
            }

            void evaluateParam(unsigned int index, MultiType& param) const
            {
                switch (index)
                {
                    case 0: param.convert<P1>(); break;
                    case 1: param.convert<P2>(); break;
                    case 2: param.convert<P3>(); break;
                    case 3: param.convert<P4>(); break;
                    case 4: param.convert<P5>(); break;
                }
            }

            unsigned int getParamCount() const
            {
                return detail::FunctorParamCount<P1, P2, P3, P4, P5>::result;
            }

            bool hasReturnvalue() const
            {
                return detail::FunctorHasReturnvalue<R>::result;
            }

            std::string getTypenameParam(unsigned int param) const
            {
                switch (param)
                {
                    case 0:  return typeToString<P1>();
                    case 1:  return typeToString<P2>();
                    case 2:  return typeToString<P3>();
                    case 3:  return typeToString<P4>();
                    case 4:  return typeToString<P5>();
                    default: return "";
                }
            }

            std::string getTypenameReturnvalue() const
            {
                return typeToString<R>();
            }

            const std::type_info& getHeaderIdentifier() const
            {
                return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, P3, P4, P5>);
            }

            const std::type_info& getHeaderIdentifier(unsigned int params) const
            {
                switch (params)
                {
                    case 0:  return typeid(detail::FunctorHeaderIdentifier<R, void, void, void, void, void>);
                    case 1:  return typeid(detail::FunctorHeaderIdentifier<R, P1, void, void, void, void>);
                    case 2:  return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, void, void, void>);
                    case 3:  return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, P3, void, void>);
                    case 4:  return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, P3, P4, void>);
                    default: return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, P3, P4, P5>);
                }
            }
    };

    template <class R, class O, class OO, class P1, class P2, class P3, class P4, class P5> SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>,           FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, P4, P5>::Type, O> >           createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5), OO* object) { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2, class P3, class P4>           SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, P4, void>,         FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, P4, void>::Type, O> >         createFunctor(R (O::*functionPointer)(P1, P2, P3, P4), OO* object)     { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2, class P3>                     SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, void, void>,       FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, void, void>::Type, O> >       createFunctor(R (O::*functionPointer)(P1, P2, P3), OO* object)         { return new FunctorTemplate<R, O, false, P1, P2, P3, void, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2>                               SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, void, void, void>,     FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, void, void, void>::Type, O> >     createFunctor(R (O::*functionPointer)(P1, P2), OO* object)             { return new FunctorTemplate<R, O, false, P1, P2, void, void, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1>                                         SharedChildPtr<FunctorTemplate<R, O, false, P1, void, void, void, void>,   FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, void, void, void, void>::Type, O> >   createFunctor(R (O::*functionPointer)(P1), OO* object)                 { return new FunctorTemplate<R, O, false, P1, void, void, void, void>(functionPointer, object); }
    template <class R, class O, class OO>                                                   SharedChildPtr<FunctorTemplate<R, O, false, void, void, void, void, void>, FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, void, void, void, void, void>::Type, O> > createFunctor(R (O::*functionPointer)(), OO* object)                   { return new FunctorTemplate<R, O, false, void, void, void, void, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2, class P3, class P4, class P5> SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>,           FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, P4, P5>::Type, O> >           createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5) const, OO* object) { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2, class P3, class P4>           SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, P4, void>,         FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, P4, void>::Type, O> >         createFunctor(R (O::*functionPointer)(P1, P2, P3, P4) const, OO* object)     { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2, class P3>                     SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, void, void>,       FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, void, void>::Type, O> >       createFunctor(R (O::*functionPointer)(P1, P2, P3) const, OO* object)         { return new FunctorTemplate<R, O, true, P1, P2, P3, void, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1, class P2>                               SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, void, void, void>,     FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, void, void, void>::Type, O> >     createFunctor(R (O::*functionPointer)(P1, P2) const, OO* object)             { return new FunctorTemplate<R, O, true, P1, P2, void, void, void>(functionPointer, object); }
    template <class R, class O, class OO, class P1>                                         SharedChildPtr<FunctorTemplate<R, O, true, P1, void, void, void, void>,   FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, void, void, void, void>::Type, O> >   createFunctor(R (O::*functionPointer)(P1) const, OO* object)                 { return new FunctorTemplate<R, O, true, P1, void, void, void, void>(functionPointer, object); }
    template <class R, class O, class OO>                                                   SharedChildPtr<FunctorTemplate<R, O, true, void, void, void, void, void>, FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, void, void, void, void, void>::Type, O> > createFunctor(R (O::*functionPointer)() const, OO* object)                   { return new FunctorTemplate<R, O, true, void, void, void, void, void>(functionPointer, object); }

    template <class R, class O, class P1, class P2, class P3, class P4, class P5> SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>,           FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, P4, P5>::Type, O> >           createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5)) { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>(functionPointer); }
    template <class R, class O, class P1, class P2, class P3, class P4>           SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, P4, void>,         FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, P4, void>::Type, O> >         createFunctor(R (O::*functionPointer)(P1, P2, P3, P4))     { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, void>(functionPointer); }
    template <class R, class O, class P1, class P2, class P3>                     SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, P3, void, void>,       FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, P3, void, void>::Type, O> >       createFunctor(R (O::*functionPointer)(P1, P2, P3))         { return new FunctorTemplate<R, O, false, P1, P2, P3, void, void>(functionPointer); }
    template <class R, class O, class P1, class P2>                               SharedChildPtr<FunctorTemplate<R, O, false, P1, P2, void, void, void>,     FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, P2, void, void, void>::Type, O> >     createFunctor(R (O::*functionPointer)(P1, P2))             { return new FunctorTemplate<R, O, false, P1, P2, void, void, void>(functionPointer); }
    template <class R, class O, class P1>                                         SharedChildPtr<FunctorTemplate<R, O, false, P1, void, void, void, void>,   FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, P1, void, void, void, void>::Type, O> >   createFunctor(R (O::*functionPointer)(P1))                 { return new FunctorTemplate<R, O, false, P1, void, void, void, void>(functionPointer); }
    template <class R, class O>                                                   SharedChildPtr<FunctorTemplate<R, O, false, void, void, void, void, void>, FunctorPointerPtr<typename detail::FunctionPointer<R, O, false, void, void, void, void, void>::Type, O> > createFunctor(R (O::*functionPointer)())                   { return new FunctorTemplate<R, O, false, void, void, void, void, void>(functionPointer); }
    template <class R, class O, class P1, class P2, class P3, class P4, class P5> SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>,           FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, P4, P5>::Type, O> >           createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5) const) { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>(functionPointer); }
    template <class R, class O, class P1, class P2, class P3, class P4>           SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, P4, void>,         FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, P4, void>::Type, O> >         createFunctor(R (O::*functionPointer)(P1, P2, P3, P4) const)     { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, void>(functionPointer); }
    template <class R, class O, class P1, class P2, class P3>                     SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, P3, void, void>,       FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, P3, void, void>::Type, O> >       createFunctor(R (O::*functionPointer)(P1, P2, P3) const)         { return new FunctorTemplate<R, O, true, P1, P2, P3, void, void>(functionPointer); }
    template <class R, class O, class P1, class P2>                               SharedChildPtr<FunctorTemplate<R, O, true, P1, P2, void, void, void>,     FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, P2, void, void, void>::Type, O> >     createFunctor(R (O::*functionPointer)(P1, P2) const)             { return new FunctorTemplate<R, O, true, P1, P2, void, void, void>(functionPointer); }
    template <class R, class O, class P1>                                         SharedChildPtr<FunctorTemplate<R, O, true, P1, void, void, void, void>,   FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, P1, void, void, void, void>::Type, O> >   createFunctor(R (O::*functionPointer)(P1) const)                 { return new FunctorTemplate<R, O, true, P1, void, void, void, void>(functionPointer); }
    template <class R, class O>                                                   SharedChildPtr<FunctorTemplate<R, O, true, void, void, void, void, void>, FunctorPointerPtr<typename detail::FunctionPointer<R, O, true, void, void, void, void, void>::Type, O> > createFunctor(R (O::*functionPointer)() const)                   { return new FunctorTemplate<R, O, true, void, void, void, void, void>(functionPointer); }

    template <class R, class P1, class P2, class P3, class P4, class P5> SharedChildPtr<FunctorTemplate<R, void, false, P1, P2, P3, P4, P5>,           FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, P1, P2, P3, P4, P5>::Type, void> >           createFunctor(R (*functionPointer)(P1, P2, P3, P4, P5)) { return new FunctorTemplate<R, void, false, P1, P2, P3, P4, P5>(functionPointer); }
    template <class R, class P1, class P2, class P3, class P4>           SharedChildPtr<FunctorTemplate<R, void, false, P1, P2, P3, P4, void>,         FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, P1, P2, P3, P4, void>::Type, void> >         createFunctor(R (*functionPointer)(P1, P2, P3, P4))     { return new FunctorTemplate<R, void, false, P1, P2, P3, P4, void>(functionPointer); }
    template <class R, class P1, class P2, class P3>                     SharedChildPtr<FunctorTemplate<R, void, false, P1, P2, P3, void, void>,       FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, P1, P2, P3, void, void>::Type, void> >       createFunctor(R (*functionPointer)(P1, P2, P3))         { return new FunctorTemplate<R, void, false, P1, P2, P3, void, void>(functionPointer); }
    template <class R, class P1, class P2>                               SharedChildPtr<FunctorTemplate<R, void, false, P1, P2, void, void, void>,     FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, P1, P2, void, void, void>::Type, void> >     createFunctor(R (*functionPointer)(P1, P2))             { return new FunctorTemplate<R, void, false, P1, P2, void, void, void>(functionPointer); }
    template <class R, class P1>                                         SharedChildPtr<FunctorTemplate<R, void, false, P1, void, void, void, void>,   FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, P1, void, void, void, void>::Type, void> >   createFunctor(R (*functionPointer)(P1))                 { return new FunctorTemplate<R, void, false, P1, void, void, void, void>(functionPointer); }
    template <class R>                                                   SharedChildPtr<FunctorTemplate<R, void, false, void, void, void, void, void>, FunctorPointerPtr<typename detail::FunctionPointer<R, void, false, void, void, void, void, void>::Type, void> > createFunctor(R (*functionPointer)())                   { return new FunctorTemplate<R, void, false, void, void, void, void, void>(functionPointer); }
}

#endif /* _Functor_H__ */
