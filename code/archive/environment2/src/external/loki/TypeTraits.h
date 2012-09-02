////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copyright
//     notice appear in all copies and that both that copyright notice and this
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
//
// Changes by Orxonox (Reto Grieder)
//     Removed all stdInt, etc. type traits and function pointer traits
//     and added UnqualifiedReferredType.
////////////////////////////////////////////////////////////////////////////////
#ifndef LOKI_TYPETRAITS_INC_
#define LOKI_TYPETRAITS_INC_

// $Id: TypeTraits.h 1069 2010-04-19 03:09:59Z rich_sposato $


#include <loki/NullType.h>

#if (defined _MSC_VER) && (_MSC_VER < 1400)
#include <string>
#endif


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4180 ) //qualifier applied to function type has no meaning; ignored
#endif

namespace Loki
{
////////////////////////////////////////////////////////////////////////////////
// class template IsCustomUnsignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits
//     class template defined below.
// Invocation: IsCustomUnsignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in unsigned
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct IsCustomUnsignedInt
    {
        enum { value = 0 };
    };

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomSignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits
//     class template defined below.
// Invocation: IsCustomSignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in signed
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct IsCustomSignedInt
    {
        enum { value = 0 };
    };

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomFloat
// Offers a means to integrate nonstandard floating point types with the
//     TypeTraits class template defined below.
// Invocation: IsCustomFloat<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in
//     floating point type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct IsCustomFloat
    {
        enum { value = 0 };
    };

////////////////////////////////////////////////////////////////////////////////
// Helper types for class template TypeTraits defined below
////////////////////////////////////////////////////////////////////////////////

    namespace Private
    {
        template <typename U> struct AddPointer
        {
            typedef U* Result;
        };

        template <typename U> struct AddPointer<U&>
        {
            typedef U* Result;
        };

        template <class U> struct AddReference
        {
            typedef U & Result;
        };

        template <class U> struct AddReference<U &>
        {
            typedef U & Result;
        };

        template <> struct AddReference<void>
        {
            typedef NullType Result;
        };

        template <class U> struct AddParameterType
        {
            typedef const U & Result;
        };

        template <class U> struct AddParameterType<U &>
        {
            typedef U & Result;
        };

        template <> struct AddParameterType<void>
        {
            typedef NullType Result;
        };
    }// namespace Private

////////////////////////////////////////////////////////////////////////////////
// class template TypeTraits
//
// Figures out at compile time various properties of any given type
// Invocations (T is a type, TypeTraits<T>::Property):
//
// - isPointer       : returns true if T is a pointer type
// - PointeeType     : returns the type to which T points if T is a pointer
//                     type, NullType otherwise
// - isReference     : returns true if T is a reference type
// - ReferredType    : returns the type to which T refers if T is a reference
//                     type, NullType otherwise
// - ParameterType   : returns the optimal type to be used as a parameter for
//                     functions that take Ts
// - isConst         : returns true if T is a const-qualified type
// - NonConstType    : Type with removed 'const' qualifier from T, if any
// - isVolatile      : returns true if T is a volatile-qualified type
// - NonVolatileType : Type with removed 'volatile' qualifier from T, if any
// - UnqualifiedType : Type with removed 'const' and 'volatile' qualifiers from
//                     T, if any
// - ParameterType   : returns the optimal type to be used as a parameter
//                       for functions that take 'const T's
//
////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class TypeTraits
    {
    private:

        template <class U> struct ReferenceTraits
        {
            enum { result = false };
            typedef U ReferredType;
        };

        template <class U> struct ReferenceTraits<U&>
        {
            enum { result = true };
            typedef U ReferredType;
        };

        template <class U> struct PointerTraits
        {
            enum { result = false };
            typedef NullType PointeeType;
        };

        template <class U> struct PointerTraits<U*>
        {
            enum { result = true };
            typedef U PointeeType;
        };

        template <class U> struct PointerTraits<U*&>
        {
            enum { result = true };
            typedef U PointeeType;
        };

        template <class U> struct PToMTraits
        {
            enum { result = false };
        };

        template <class U, class V> struct PToMTraits<U V::*>
        {
            enum { result = true };
        };

        template <class U, class V> struct PToMTraits<U V::*&>
        {
            enum { result = true };
        };

        template <class U> struct UnConst
        {
            typedef U Result;
            enum { isConst = 0 };
        };

        template <class U> struct UnConst<const U>
        {
            typedef U Result;
            enum { isConst = 1 };
        };

        template <class U> struct UnConst<const U&>
        {
            typedef U& Result;
            enum { isConst = 1 };
        };

        template <class U> struct UnVolatile
        {
            typedef U Result;
            enum { isVolatile = 0 };
        };

        template <class U> struct UnVolatile<volatile U>
        {
            typedef U Result;
            enum { isVolatile = 1 };
        };

        template <class U> struct UnVolatile<volatile U&>
        {
            typedef U& Result;
            enum { isVolatile = 1 };
        };

    public:
        typedef typename UnConst<T>::Result
            NonConstType;
        typedef typename UnVolatile<T>::Result
            NonVolatileType;
        typedef typename UnVolatile<typename UnConst<T>::Result>::Result
            UnqualifiedType;
        typedef typename PointerTraits<UnqualifiedType>::PointeeType
            PointeeType;
        typedef typename ReferenceTraits<T>::ReferredType
            ReferredType;
        typedef typename ReferenceTraits<typename UnVolatile<typename UnConst<T>::Result>::Result>::ReferredType
            UnqualifiedReferredType;

        enum { isConst          = UnConst<T>::isConst };
        enum { isVolatile       = UnVolatile<T>::isVolatile };
        enum { isReference      = ReferenceTraits<UnqualifiedType>::result };
        enum { isPointer        = PointerTraits<
                                        typename ReferenceTraits<UnqualifiedType>::ReferredType >::result};

    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER


#endif // end file guardian

