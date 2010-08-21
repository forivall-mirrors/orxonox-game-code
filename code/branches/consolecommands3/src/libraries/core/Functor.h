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
 *   Inspiration: Functor by Benjamin Grauer
 */

#ifndef _Functor_H__
#define _Functor_H__

#include <typeinfo>

#include "CorePrereqs.h"

#include "util/Convert.h"
#include "util/Debug.h"
#include "util/MultiType.h"
#include "FunctorPtr.h"

namespace orxonox
{
    const unsigned int MAX_FUNCTOR_ARGUMENTS = 5;

    template <class T>
    inline std::string typeToString() { return "unknown"; }

#define CreateTypeToStringTemplate(type) \
    template <> \
    inline std::string typeToString<type>() { return #type; } \
    template <> \
    inline std::string typeToString<type&>() { return #type; } \
    template <> \
    inline std::string typeToString<const type>() { return #type; } \
    template <> \
    inline std::string typeToString<const type&>() { return #type; }

    CreateTypeToStringTemplate(int);
    CreateTypeToStringTemplate(unsigned int);
    CreateTypeToStringTemplate(char);
    CreateTypeToStringTemplate(unsigned char);
    CreateTypeToStringTemplate(short);
    CreateTypeToStringTemplate(unsigned short);
    CreateTypeToStringTemplate(long);
    CreateTypeToStringTemplate(unsigned long);
    CreateTypeToStringTemplate(long long);
    CreateTypeToStringTemplate(unsigned long long);
    CreateTypeToStringTemplate(float);
    CreateTypeToStringTemplate(double);
    CreateTypeToStringTemplate(long double);
    CreateTypeToStringTemplate(bool);
    CreateTypeToStringTemplate(Vector2);
    CreateTypeToStringTemplate(Vector3);
    CreateTypeToStringTemplate(Quaternion);
    CreateTypeToStringTemplate(ColourValue);
    CreateTypeToStringTemplate(Radian);
    CreateTypeToStringTemplate(Degree);

    template <>
    inline std::string typeToString<std::string>() { return "string"; }
    template <>
    inline std::string typeToString<std::string&>() { return "string"; }
    template <>
    inline std::string typeToString<const std::string>() { return "string"; }
    template <>
    inline std::string typeToString<const std::string&>() { return "string"; }

    class _CoreExport Functor
    {
        friend class SharedPtr<Functor>;

        public:
            struct Type
            {
                enum Enum
                {
                    Member,
                    ConstMember,
                    Static,
                    Lua
                };
            };

        public:
            Functor() { ++instances_s; COUT(0) << "functor ++: " << instances_s << std::endl; }
            virtual ~Functor() { --instances_s; COUT(0) << "functor --: " << instances_s << std::endl; }

            virtual MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            virtual Type::Enum getType() const = 0;
            virtual unsigned int getParamCount() const = 0;
            virtual bool hasReturnvalue() const = 0;

            virtual std::string getTypenameParam(unsigned int param) const = 0;
            virtual std::string getTypenameReturnvalue() const = 0;

            virtual void evaluateParam(unsigned int index, MultiType& param) const = 0;

            virtual void setRawObjectPointer(void* object) {}
            virtual void* getRawObjectPointer() const { return 0; }

            virtual const std::type_info& getHeaderIdentifier() const = 0;

        private:
            static int instances_s;
    };

    class _CoreExport FunctorStatic : public Functor
    {
        public:
            virtual ~FunctorStatic() {}
            virtual MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;
    };

    template <class T>
    class FunctorMember : public Functor
    {
        public:
            FunctorMember()
            {
                this->object_ = 0;
                this->constObject_ = 0;
            }
            virtual ~FunctorMember() {}

            virtual MultiType operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;
            virtual MultiType operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            virtual MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null)
            {
                if (this->object_)
                    return (*this)(this->object_, param1, param2, param3, param4, param5);
                else if (this->constObject_)
                    return (*this)(this->constObject_, param1, param2, param3, param4, param5);
                else
                {
                    COUT(1) << "An error occurred in Functor.h:" << std::endl;
                    COUT(1) << "Error: No object set." << std::endl;
                    return MT_Type::Null;
                }
            }

            inline FunctorMember<T>* setObject(T* object)
            {
                this->object_ = object;
                this->constObject_ = 0;
                return this;
            }

            inline FunctorMember<T>* setObject(const T* object)
            {
                this->object_ = 0;
                this->constObject_ = object;
                return this;
            }

            void setRawObjectPointer(void* object)
            {
                this->object_ = (T*)object;
                this->constObject_ = 0;
            }

            void* getRawObjectPointer() const
            {
                if (this->object_)
                    return (void*)this->object_;
                else
                    return (void*)this->constObject_;
            }

            typedef std::pair<T*, const T*> Objects;

            inline Objects getObjects() const
            {
                return Objects(this->object_, this->constObject_);
            }

            inline void setObjects(const Objects& objects)
            {
                this->object_ = objects.first;
                this->constObject_ = objects.second;
            }

        private:
            T* object_;
            const T* constObject_;
    };



    template <class R, class P1, class P2, class P3, class P4, class P5>
    struct FunctorHeaderIdentifier {};



    inline Functor* createFunctor(Functor* functor)
    {
        return functor;
    }



#define FUNCTOR_TEMPLATE(ismember, returnvalue, numparams, additionalobject) FUNCTOR_TEMPLATE##ismember##returnvalue##numparams(additionalobject)
#define FUNCTOR_TEMPLATE000(additionalobject)
#define FUNCTOR_TEMPLATE001(additionalobject) template <class P1>
#define FUNCTOR_TEMPLATE002(additionalobject) template <class P1, class P2>
#define FUNCTOR_TEMPLATE003(additionalobject) template <class P1, class P2, class P3>
#define FUNCTOR_TEMPLATE004(additionalobject) template <class P1, class P2, class P3, class P4>
#define FUNCTOR_TEMPLATE005(additionalobject) template <class P1, class P2, class P3, class P4, class P5>
#define FUNCTOR_TEMPLATE010(additionalobject) template <class R>
#define FUNCTOR_TEMPLATE011(additionalobject) template <class R, class P1>
#define FUNCTOR_TEMPLATE012(additionalobject) template <class R, class P1, class P2>
#define FUNCTOR_TEMPLATE013(additionalobject) template <class R, class P1, class P2, class P3>
#define FUNCTOR_TEMPLATE014(additionalobject) template <class R, class P1, class P2, class P3, class P4>
#define FUNCTOR_TEMPLATE015(additionalobject) template <class R, class P1, class P2, class P3, class P4, class P5>
#define FUNCTOR_TEMPLATE100(additionalobject) template <class T FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE101(additionalobject) template <class T, class P1 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE102(additionalobject) template <class T, class P1, class P2 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE103(additionalobject) template <class T, class P1, class P2, class P3 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE104(additionalobject) template <class T, class P1, class P2, class P3, class P4 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE105(additionalobject) template <class T, class P1, class P2, class P3, class P4, class P5 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE110(additionalobject) template <class T, class R FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE111(additionalobject) template <class T, class R, class P1 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE112(additionalobject) template <class T, class R, class P1, class P2 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE113(additionalobject) template <class T, class R, class P1, class P2, class P3 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE114(additionalobject) template <class T, class R, class P1, class P2, class P3, class P4 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >
#define FUNCTOR_TEMPLATE115(additionalobject) template <class T, class R, class P1, class P2, class P3, class P4, class P5 FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) >



#define FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT(additionalobject) FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT##additionalobject
#define FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT0
#define FUNCTOR_TEMPLATE_ADDITIONAL_OBJECT1 , class O



#define FUNCTOR_TEMPLATE_CLASSES(ismember, returnvalue, numparams) FUNCTOR_TEMPLATE_CLASSES##ismember##returnvalue##numparams
#define FUNCTOR_TEMPLATE_CLASSES000
#define FUNCTOR_TEMPLATE_CLASSES001 <P1>
#define FUNCTOR_TEMPLATE_CLASSES002 <P1, P2>
#define FUNCTOR_TEMPLATE_CLASSES003 <P1, P2, P3>
#define FUNCTOR_TEMPLATE_CLASSES004 <P1, P2, P3, P4>
#define FUNCTOR_TEMPLATE_CLASSES005 <P1, P2, P3, P4, P5>
#define FUNCTOR_TEMPLATE_CLASSES010 <R>
#define FUNCTOR_TEMPLATE_CLASSES011 <R, P1>
#define FUNCTOR_TEMPLATE_CLASSES012 <R, P1, P2>
#define FUNCTOR_TEMPLATE_CLASSES013 <R, P1, P2, P3>
#define FUNCTOR_TEMPLATE_CLASSES014 <R, P1, P2, P3, P4>
#define FUNCTOR_TEMPLATE_CLASSES015 <R, P1, P2, P3, P4, P5>
#define FUNCTOR_TEMPLATE_CLASSES100 <T>
#define FUNCTOR_TEMPLATE_CLASSES101 <T, P1>
#define FUNCTOR_TEMPLATE_CLASSES102 <T, P1, P2>
#define FUNCTOR_TEMPLATE_CLASSES103 <T, P1, P2, P3>
#define FUNCTOR_TEMPLATE_CLASSES104 <T, P1, P2, P3, P4>
#define FUNCTOR_TEMPLATE_CLASSES105 <T, P1, P2, P3, P4, P5>
#define FUNCTOR_TEMPLATE_CLASSES110 <T, R>
#define FUNCTOR_TEMPLATE_CLASSES111 <T, R, P1>
#define FUNCTOR_TEMPLATE_CLASSES112 <T, R, P1, P2>
#define FUNCTOR_TEMPLATE_CLASSES113 <T, R, P1, P2, P3>
#define FUNCTOR_TEMPLATE_CLASSES114 <T, R, P1, P2, P3, P4>
#define FUNCTOR_TEMPLATE_CLASSES115 <T, R, P1, P2, P3, P4, P5>



#define FUNCTOR_TYPENAME_PARAM(numparams) FUNCTOR_TYPENAME_PARAM##numparams
#define FUNCTOR_TYPENAME_PARAM0 \
    return BLANKSTRING
#define FUNCTOR_TYPENAME_PARAM1 \
    if (param == 0) { return typeToString<P1>(); } \
    else { return BLANKSTRING; }
#define FUNCTOR_TYPENAME_PARAM2 \
    if (param == 0) { return typeToString<P1>(); } \
    else if (param == 1) { return typeToString<P2>(); } \
    else { return BLANKSTRING; }
#define FUNCTOR_TYPENAME_PARAM3 \
    if (param == 0) { return typeToString<P1>(); } \
    else if (param == 1) { return typeToString<P2>(); } \
    else if (param == 2) { return typeToString<P3>(); } \
    else { return BLANKSTRING; }
#define FUNCTOR_TYPENAME_PARAM4 \
    if (param == 0) { return typeToString<P1>(); } \
    else if (param == 1) { return typeToString<P2>(); } \
    else if (param == 2) { return typeToString<P3>(); } \
    else if (param == 3) { return typeToString<P4>(); } \
    else { return BLANKSTRING; }
#define FUNCTOR_TYPENAME_PARAM5 \
    if (param == 0) { return typeToString<P1>(); } \
    else if (param == 1) { return typeToString<P2>(); } \
    else if (param == 2) { return typeToString<P3>(); } \
    else if (param == 3) { return typeToString<P4>(); } \
    else if (param == 4) { return typeToString<P5>(); } \
    else { return BLANKSTRING; }

#define FUNCTOR_TYPENAME_RETURN(returnvalue) FUNCTOR_TYPENAME_RETURN##returnvalue
#define FUNCTOR_TYPENAME_RETURN0 BLANKSTRING
#define FUNCTOR_TYPENAME_RETURN1 typeToString<R>()



#define FUNCTOR_FUNCTION_PARAMS(numparams) FUNCTOR_FUNCTION_PARAMS##numparams
#define FUNCTOR_FUNCTION_PARAMS0
#define FUNCTOR_FUNCTION_PARAMS1 P1 param1
#define FUNCTOR_FUNCTION_PARAMS2 P1 param1, P2 param2
#define FUNCTOR_FUNCTION_PARAMS3 P1 param1, P2 param2, P3 param3
#define FUNCTOR_FUNCTION_PARAMS4 P1 param1, P2 param2, P3 param3, P4 param4
#define FUNCTOR_FUNCTION_PARAMS5 P1 param1, P2 param2, P3 param3, P4 param4, P5 param5

#define FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) FUNCTOR_FUNCTION_RETURNVALUE##returnvalue
#define FUNCTOR_FUNCTION_RETURNVALUE0 void
#define FUNCTOR_FUNCTION_RETURNVALUE1 R



#define FUNCTOR_FUNCTION_CALL(numparams) FUNCTOR_FUNCTION_CALL##numparams
#define FUNCTOR_FUNCTION_CALL0
#define FUNCTOR_FUNCTION_CALL1 param1
#define FUNCTOR_FUNCTION_CALL2 param1, param2
#define FUNCTOR_FUNCTION_CALL3 param1, param2, param3
#define FUNCTOR_FUNCTION_CALL4 param1, param2, param3, param4
#define FUNCTOR_FUNCTION_CALL5 param1, param2, param3, param4, param5

#define FUNCTOR_STORE_RETURNVALUE(returnvalue, functioncall) FUNCTOR_STORE_RETURNVALUE##returnvalue(functioncall)
#define FUNCTOR_STORE_RETURNVALUE0(functioncall) functioncall; return MT_Type::Null
#define FUNCTOR_STORE_RETURNVALUE1(functioncall) return functioncall



#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES(returnvalue, numparams) FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES##numparams(returnvalue)
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES0(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), void, void, void, void, void>
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES1(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), P1, void, void, void, void>
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES2(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), P1, P2, void, void, void>
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES3(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), P1, P2, P3, void, void>
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES4(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), P1, P2, P3, P4, void>
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES5(returnvalue) <FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue), P1, P2, P3, P4, P5>

#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE(returnvalue) FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE##returnvalue
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE0 void
#define FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES_RETURNVALUE1 R



#define FUNCTOR_EVALUATE_PARAM(numparams) FUNCTOR_EVALUATE_PARAM##numparams
#define FUNCTOR_EVALUATE_PARAM0
#define FUNCTOR_EVALUATE_PARAM1 \
    if (index == 0) { param.convert<P1>(); }
#define FUNCTOR_EVALUATE_PARAM2 \
    if (index == 0) { param.convert<P1>(); } \
    else if (index == 1) { param.convert<P2>(); }
#define FUNCTOR_EVALUATE_PARAM3 \
    if (index == 0) { param.convert<P1>(); } \
    else if (index == 1) { param.convert<P2>(); } \
    else if (index == 2) { param.convert<P3>(); }
#define FUNCTOR_EVALUATE_PARAM4 \
    if (index == 0) { param.convert<P1>(); } \
    else if (index == 1) { param.convert<P2>(); } \
    else if (index == 2) { param.convert<P3>(); } \
    else if (index == 3) { param.convert<P4>(); }
#define FUNCTOR_EVALUATE_PARAM5 \
    if (index == 0) { param.convert<P1>(); } \
    else if (index == 1) { param.convert<P2>(); } \
    else if (index == 2) { param.convert<P3>(); } \
    else if (index == 3) { param.convert<P4>(); } \
    else if (index == 4) { param.convert<P5>(); }




#define CREATE_STATIC_FUNCTOR(returnvalue, numparams) \
    FUNCTOR_TEMPLATE(0, returnvalue, numparams, 0) \
    class FunctorStatic##returnvalue##numparams : public FunctorStatic \
    { \
        public: \
            FunctorStatic##returnvalue##numparams(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams))) \
            { \
                this->functionPointer_ = functionPointer; \
            } \
    \
            MultiType operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
            } \
    \
            Functor::Type::Enum getType() const { return Functor::Type::Static; } \
            unsigned int getParamCount() const { return numparams; } \
            bool hasReturnvalue() const { return returnvalue; } \
            std::string getTypenameParam(unsigned int param) const { FUNCTOR_TYPENAME_PARAM(numparams); } \
            std::string getTypenameReturnvalue() const { return FUNCTOR_TYPENAME_RETURN(returnvalue); } \
    \
            const std::type_info& getHeaderIdentifier() const \
            { \
                return typeid(FunctorHeaderIdentifier FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES(returnvalue, numparams)); \
            } \
    \
        private: \
            FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (*functionPointer_)(FUNCTOR_FUNCTION_PARAMS(numparams)); \
    }; \
    \
    \
    FUNCTOR_TEMPLATE(0, returnvalue, numparams, 0) \
    inline FunctorStatic##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(0, returnvalue, numparams)* createFunctor(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams))) \
    { \
        return new FunctorStatic##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(0, returnvalue, numparams) (functionPointer); \
    }





#define CREATE_MEMBER_FUNCTOR(returnvalue, numparams) \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 0) \
    class FunctorMember##returnvalue##numparams : public FunctorMember<T> \
    { \
        public: \
            FunctorMember##returnvalue##numparams(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams))) \
            { \
                this->functionPointer_ = functionPointer; \
            } \
    \
            MultiType operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            MultiType operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                COUT(1) << "An error occurred in Functor.h:" << std::endl; \
                COUT(1) << "Error: Function is not const." << std::endl; \
                return MT_Type::Null; \
            } \
    \
            void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
            } \
    \
            Functor::Type::Enum getType() const { return Functor::Type::Member; } \
            unsigned int getParamCount() const { return numparams; } \
            bool hasReturnvalue() const { return returnvalue; } \
            std::string getTypenameParam(unsigned int param) const { FUNCTOR_TYPENAME_PARAM(numparams); } \
            std::string getTypenameReturnvalue() const { return FUNCTOR_TYPENAME_RETURN(returnvalue); } \
    \
            const std::type_info& getHeaderIdentifier() const \
            { \
                return typeid(FunctorHeaderIdentifier FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES(returnvalue, numparams)); \
            } \
    \
        private: \
            FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer_)(FUNCTOR_FUNCTION_PARAMS(numparams)); \
    }; \
    \
    \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 0) \
    class FunctorConstMember##returnvalue##numparams : public FunctorMember<T> \
    { \
        public: \
            FunctorConstMember##returnvalue##numparams(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams)) const) \
            { \
                this->functionPointer_ = functionPointer; \
            } \
    \
            MultiType operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            MultiType operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
            } \
    \
            Functor::Type::Enum getType() const { return Functor::Type::ConstMember; } \
            unsigned int getParamCount() const { return numparams; } \
            bool hasReturnvalue() const { return returnvalue; } \
            std::string getTypenameParam(unsigned int param) const { FUNCTOR_TYPENAME_PARAM(numparams); } \
            std::string getTypenameReturnvalue() const { return FUNCTOR_TYPENAME_RETURN(returnvalue); } \
    \
            const std::type_info& getHeaderIdentifier() const \
            { \
                return typeid(FunctorHeaderIdentifier FUNCTOR_HEADER_IDENTIFIER_TEMPLATE_CLASSES(returnvalue, numparams)); \
            } \
    \
        private: \
            FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer_)(FUNCTOR_FUNCTION_PARAMS(numparams)) const; \
    }; \
    \
    \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 0) \
    inline FunctorMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* createFunctor(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams))) \
    { \
        return new FunctorMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams) (functionPointer); \
    } \
    \
    \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 0) \
    inline FunctorConstMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* createFunctor(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams)) const) \
    { \
        return new FunctorConstMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams) (functionPointer); \
    } \
    \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 1) \
    inline FunctorMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* createFunctor(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams)), O* object) \
    { \
        FunctorMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* functor = new FunctorMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams) (functionPointer); \
        functor->setObject(object); \
        return functor; \
    } \
    \
    \
    FUNCTOR_TEMPLATE(1, returnvalue, numparams, 1) \
    inline FunctorConstMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* createFunctor(FUNCTOR_FUNCTION_RETURNVALUE(returnvalue) (T::*functionPointer)(FUNCTOR_FUNCTION_PARAMS(numparams)) const, O* object) \
    { \
        FunctorConstMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams)* functor = new FunctorConstMember##returnvalue##numparams FUNCTOR_TEMPLATE_CLASSES(1, returnvalue, numparams) (functionPointer); \
        functor->setObject(object); \
        return functor; \
    }



// disable annoying warning about forcing value to boolean
#ifdef ORXONOX_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4100 4800)
#endif

#define CREATE_ALL_STATIC_FUNCTORS() \
    CREATE_STATIC_FUNCTOR(0, 0); \
    CREATE_STATIC_FUNCTOR(0, 1); \
    CREATE_STATIC_FUNCTOR(0, 2); \
    CREATE_STATIC_FUNCTOR(0, 3); \
    CREATE_STATIC_FUNCTOR(0, 4); \
    CREATE_STATIC_FUNCTOR(0, 5); \
    CREATE_STATIC_FUNCTOR(1, 0); \
    CREATE_STATIC_FUNCTOR(1, 1); \
    CREATE_STATIC_FUNCTOR(1, 2); \
    CREATE_STATIC_FUNCTOR(1, 3); \
    CREATE_STATIC_FUNCTOR(1, 4); \
    CREATE_STATIC_FUNCTOR(1, 5)


#define CREATE_ALL_MEMBER_FUNCTORS() \
    CREATE_MEMBER_FUNCTOR(0, 0); \
    CREATE_MEMBER_FUNCTOR(0, 1); \
    CREATE_MEMBER_FUNCTOR(0, 2); \
    CREATE_MEMBER_FUNCTOR(0, 3); \
    CREATE_MEMBER_FUNCTOR(0, 4); \
    CREATE_MEMBER_FUNCTOR(0, 5); \
    CREATE_MEMBER_FUNCTOR(1, 0); \
    CREATE_MEMBER_FUNCTOR(1, 1); \
    CREATE_MEMBER_FUNCTOR(1, 2); \
    CREATE_MEMBER_FUNCTOR(1, 3); \
    CREATE_MEMBER_FUNCTOR(1, 4); \
    CREATE_MEMBER_FUNCTOR(1, 5)


    CREATE_ALL_STATIC_FUNCTORS();
    CREATE_ALL_MEMBER_FUNCTORS();
}

#ifdef ORXONOX_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif /* _Functor_H__ */
