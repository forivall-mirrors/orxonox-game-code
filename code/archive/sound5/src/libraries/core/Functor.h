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

#include "CorePrereqs.h"

#include "util/Debug.h"
#include "util/MultiType.h"

namespace orxonox
{
    const unsigned int MAX_FUNCTOR_ARGUMENTS = 5;

    namespace FunctionType
    {
        enum Value
        {
            Member,
            ConstMember,
            Static
        };
    }


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
        public:
            Functor() {}
            virtual ~Functor() {}

            virtual void operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            inline unsigned int getParamCount() const { return this->numParams_; }
            inline bool hasReturnvalue() const { return this->hasReturnValue_; }
            inline FunctionType::Value getType() const { return this->type_; }
            inline const MultiType& getReturnvalue() const { return this->returnedValue_; }

            const std::string& getTypenameParam(unsigned int param) const { return (param < 5) ? this->typeParam_[param] : BLANKSTRING; }
            const std::string& getTypenameReturnvalue() const { return this->typeReturnvalue_; }

            virtual void evaluateParam(unsigned int index, MultiType& param) const = 0;

        protected:
            unsigned int numParams_;
            bool hasReturnValue_;
            FunctionType::Value type_;
            MultiType returnedValue_;

            std::string typeReturnvalue_;
            std::string typeParam_[MAX_FUNCTOR_ARGUMENTS];
    };

    class _CoreExport FunctorStatic : public Functor
    {
        public:
            virtual ~FunctorStatic() {}
            virtual void operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;
    };

    template <class T>
    class FunctorMember : public Functor
    {
        public:
            FunctorMember()
            {
                constObject_ = 0;
                object_ = 0;
                bConstObject_ = false;
            }
            virtual ~FunctorMember() {}

            virtual void operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;
            virtual void operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) = 0;

            virtual void operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null)
            {
                if (this->bConstObject_)
                {
                    if (this->constObject_)
                        (*this)(this->constObject_, param1, param2, param3, param4, param5);
                    else
                    {
                        COUT(1) << "An error occurred in Functor.h:" << std::endl;
                        COUT(1) << "Error: No const object set." << std::endl;
                    }
                }
                else
                {
                    if (this->object_)
                        (*this)(this->object_, param1, param2, param3, param4, param5);
                    else
                    {
                        COUT(1) << "An error occurred in Functor.h:" << std::endl;
                        COUT(1) << "Error: No object set." << std::endl;
                    }
                }
            }

            FunctorMember<T>* setObject(T* object)
            {
                this->bConstObject_ = false;
                this->object_ = object;
                return this;
            }

            FunctorMember<T>* setObject(const T* object)
            {
                this->bConstObject_ = true;
                this->constObject_ = object;
                return this;
            }

        private:
            const T* constObject_;
            T* object_;
            bool bConstObject_;
    };



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



#define FUNCTOR_TYPENAME_PARAMS(numparams) FUNCTOR_TYPENAME_PARAMS##numparams
#define FUNCTOR_TYPENAME_PARAMS0
#define FUNCTOR_TYPENAME_PARAMS1 this->typeParam_[0] = typeToString<P1>();
#define FUNCTOR_TYPENAME_PARAMS2 this->typeParam_[0] = typeToString<P1>(); this->typeParam_[1] = typeToString<P2>();
#define FUNCTOR_TYPENAME_PARAMS3 this->typeParam_[0] = typeToString<P1>(); this->typeParam_[1] = typeToString<P2>(); this->typeParam_[2] = typeToString<P3>();
#define FUNCTOR_TYPENAME_PARAMS4 this->typeParam_[0] = typeToString<P1>(); this->typeParam_[1] = typeToString<P2>(); this->typeParam_[2] = typeToString<P3>(); this->typeParam_[3] = typeToString<P4>();
#define FUNCTOR_TYPENAME_PARAMS5 this->typeParam_[0] = typeToString<P1>(); this->typeParam_[1] = typeToString<P2>(); this->typeParam_[2] = typeToString<P3>(); this->typeParam_[3] = typeToString<P4>(); this->typeParam_[4] = typeToString<P5>();

#define FUNCTOR_TYPENAME_RETURN(returnvalue) FUNCTOR_TYPENAME_RETURN##returnvalue
#define FUNCTOR_TYPENAME_RETURN0
#define FUNCTOR_TYPENAME_RETURN1 this->typeReturnvalue_ = typeToString<R>();



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
#define FUNCTOR_STORE_RETURNVALUE0(functioncall) functioncall
#define FUNCTOR_STORE_RETURNVALUE1(functioncall) this->returnedValue_ = functioncall



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
                this->numParams_ = numparams; \
                this->hasReturnValue_ = returnvalue; \
                this->type_ = FunctionType::Static; \
                this->functionPointer_ = functionPointer; \
                \
                FUNCTOR_TYPENAME_PARAMS(numparams); \
                FUNCTOR_TYPENAME_RETURN(returnvalue); \
            } \
    \
            void operator()(const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            virtual void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
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
                this->numParams_ = numparams; \
                this->hasReturnValue_ = returnvalue; \
                this->type_ = FunctionType::Member; \
                this->functionPointer_ = functionPointer; \
            } \
    \
            void operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            void operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                COUT(1) << "An error occurred in Functor.h:" << std::endl; \
                COUT(1) << "Error: Function is not const." << std::endl; \
            } \
    \
            virtual void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
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
                this->numParams_ = numparams; \
                this->hasReturnValue_ = returnvalue; \
                this->type_ = FunctionType::ConstMember; \
                this->functionPointer_ = functionPointer; \
            } \
    \
            void operator()(T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            void operator()(const T* object, const MultiType& param1 = MT_Type::Null, const MultiType& param2 = MT_Type::Null, const MultiType& param3 = MT_Type::Null, const MultiType& param4 = MT_Type::Null, const MultiType& param5 = MT_Type::Null) \
            { \
                FUNCTOR_STORE_RETURNVALUE(returnvalue, (*object.*this->functionPointer_)(FUNCTOR_FUNCTION_CALL(numparams))); \
            } \
    \
            virtual void evaluateParam(unsigned int index, MultiType& param) const \
            { \
                FUNCTOR_EVALUATE_PARAM(numparams); \
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
