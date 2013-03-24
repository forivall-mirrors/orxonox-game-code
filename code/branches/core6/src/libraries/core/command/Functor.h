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

/**
    @file
    @ingroup Command FunctorExecutor
    @brief Definition of orxonox::Functor and its specialized subclasses, as well as the createFunctor() functions.

    @anchor FunctorExample

    Functors can be used to wrap function-pointers. While function-pointers have a very
    complicated syntax in C++, Functors are always the same and you can call the wrapped
    function-pointer independently of its parameter with arguments of type MultiType. These
    arguments are then automatically converted to the right type.

    To create a Functor, the helper function createFunctor() is used. It returns an instance
    of orxonox::FunctorPtr which is simply a typedef of @ref orxonox::SharedPtr "SharedPtr<Functor>".
    This means you don't have to delete the Functor after using it, because it is managed
    by the SharedPtr.

    Example:
    @code
    int myStaticFunction(int value)                         // Definition of a static function
    {
        return (value * 2);                                 // Return the double of the value
    }

    FunctorPtr functor = createFunctor(&myStaticFunction);  // Create a Functor

    int result = (*functor)(5);                             // Calls the functor with value = 5, result == 10

    int result = (*functor)("7");                           // Calls the functor with a string which is converted to an integer, result == 14
    @endcode

    Functors can also be used if you work with member-functions. In this case createFunctor()
    returns an instance of orxonox::FunctorMemberPtr - this allows you to define the object
    that will be used to call the function.

    Example:
    @code
    class MyClass                                                   // Define a class
    {
        public:
            MyClass(const std::string& text)                        // Constructor
            {
                this->text_ = text;
            }

            bool contains(const std::string& word)                  // Function that searches for "word" in "text"
            {
                return (this->text_.find(word) != std::string::npos);
            }

        private:
            std::string text_;                                      // Member variable
    };

    MyClass* object = new MyClass("Hello World");                   // Create an object of type MyClass and set its text to "Hello World"

    FunctorPtr functor = createFunctor(&MyClass:contains, object);  // Create a Functor (note the object!)

    bool result = (*functor)("World");                              // result == true
    bool result = (*functor)("test");                               // result == false
    @endcode

    Instead of assigning the object directly to the functor when creating it, you can also define
    it at any later point or when you call the functor. Note however that this works only with
    orxonox::FunctorMember.

    @code
    MyClass* object1 = new MyClass("Hello World");                  // Create an object
    MyClass* object2 = new MyClass("this is a test");               // Create another object

    FunctorMemberPtr functor = createFunctor(&MyClass:contains);    // Create a FunctorMember (note: no object this time)

    bool result = (*functor)("World");                              // result == false and an error: "Error: Can't execute FunctorMember, no object set."

    bool result = (*functor)(object1, "World");                     // result == true
    bool result = (*functor)(object1, "test");                      // result == false
    bool result = (*functor)(object2, "test");                      // result == true

    functor->setObject(object1);                                    // Assign an object to the FunctorMember

    bool result = (*functor)("World");                              // result == true (no error this time, because the object was set using setObject())
    @endcode
*/

#ifndef _Functor_H__
#define _Functor_H__

#include "core/CorePrereqs.h"

#include <typeinfo>

#include "util/Output.h"
#include "util/MultiType.h"
#include "core/class/OrxonoxClass.h"
#include "FunctorPtr.h"

namespace orxonox
{
    const unsigned int MAX_FUNCTOR_ARGUMENTS = 5;   ///< The maximum number of parameters of a function that is supported by Functor

    namespace detail
    {
        template <class T>
        inline std::string _typeToString() { return "unknown"; }

        template <> inline std::string _typeToString<void>()               { return "void"; }
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
    }

    /// Returns the name of type @a T as string.
    template <class T>
    inline std::string typeToString() { return detail::_typeToString<typename Loki::TypeTraits<T>::UnqualifiedReferredType>(); }

    /**
        @brief The Functor classes are used to wrap function pointers.

        Function-pointers in C++ have a pretty complicated syntax and you can't store
        and call them unless you know the exact type. A Functor can be used to wrap
        a function-pointer and to store it independent of its type. You can also call
        it independently of its parameters by passing the arguments as MultiType. They
        are converted automatically to the right type.

        Functor is a pure virtual base class.

        @see See @ref FunctorExample "Functor.h" for some examples.
    */
    class _CoreExport Functor
    {
        public:
            struct Type
            {
                /// Defines the type of a function (static or member)
                enum Enum
                {
                    Static,
                    Member
                };
            };

        public:
            virtual ~Functor() {}

            /// Calls the function-pointer with up to five arguments. In case of a member-function, the assigned object-pointer is used to call the function. @return Returns the return-value of the function (if any; MultiType::Null otherwise)
            virtual MultiType operator()(const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null) = 0;

            /// Creates a new instance of Functor with the same values like this (used instead of a copy-constructor)
            virtual FunctorPtr clone() = 0;

            /// Returns the type of the function: static or member.
            virtual Type::Enum getType() const = 0;
            /// Returns the number of parameters of the function.
            virtual unsigned int getParamCount() const = 0;
            /// Returns true if the function has a return-value.
            virtual bool hasReturnvalue() const = 0;

            /// Returns the type-name of the parameter with given index (the first parameter has index 0).
            virtual std::string getTypenameParam(unsigned int index) const = 0;
            /// Returns the type-name of the return-value.
            virtual std::string getTypenameReturnvalue() const = 0;

            /// Converts a given argument to the type of the parameter with given index (the first parameter has index 0).
            virtual void evaluateArgument(unsigned int index, MultiType& argument) const = 0;

            /// Assigns an object-pointer to the functor which is used to execute a member-function.
            virtual void setRawObjectPointer(void* object) = 0;
            /// Returns the object-pointer.
            virtual void* getRawObjectPointer() const = 0;

            /// Enables or disables the safe mode which causes the functor to change the object pointer to NULL if the object is deleted (only member functors).
            virtual void setSafeMode(bool bSafeMode) = 0;

            /// Returns the full identifier of the function-pointer which is defined as typeid(@a F), where @a F is the type of the stored function-pointer. Used to compare functors.
            virtual const std::type_info& getFullIdentifier() const = 0;
            /// Returns an identifier of the header of the function (doesn't include the function's class). Used to compare functors.
            virtual const std::type_info& getHeaderIdentifier() const = 0;
            /// Returns an identifier of the header of the function (doesn't include the function's class), but regards only the first @a params parameters. Used to compare functions if an Executor provides default-values for the other parameters.
            virtual const std::type_info& getHeaderIdentifier(unsigned int params) const = 0;
    };

    /**
        @brief FunctorMember is a child class of Functor and expands it with an object-pointer, that
        is used for member-functions, as well as an overloaded execution operator.

        @param O The type of the function's class (or void if it's a static function)

        Note that FunctorMember is also used for static functions, but with T = void. FunctorStatic
        is a typedef of FunctorMember<void>. The void* object-pointer is ignored in this case.

        @see See @ref FunctorExample "Functor.h" for some examples.
    */
    template <class O>
    class FunctorMember : public Functor, public DestructionListener
    {
        public:
            /// Constructor: Stores the object-pointer.
            FunctorMember(O* object = 0) : object_(object), bSafeMode_(false) {}
            virtual ~FunctorMember() { if (this->bSafeMode_) { this->unregisterObject(this->object_); } }

            /// Calls the function-pointer with up to five arguments and an object. In case of a static-function, the object can be NULL. @return Returns the return-value of the function (if any; MultiType::Null otherwise)
            virtual MultiType operator()(O* object, const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null) = 0;

            // see Functor::operator()()
            MultiType operator()(const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null)
            {
                // call the function if an object was assigned
                if (this->object_)
                    return (*this)(this->object_, param1, param2, param3, param4, param5);
                else
                {
                    orxout(internal_error) << "Can't execute FunctorMember, no object set." << endl;
                    return MultiType::Null;
                }
            }

            // see Functor::getType()
            inline Functor::Type::Enum getType() const
                { return Functor::Type::Member; }

            /// Assigns an object-pointer to the functor which is used to execute a member-function.
            inline void setObject(O* object)
            {
                if (this->bSafeMode_ && object != this->object_)
                {
                    this->unregisterObject(this->object_);
                    this->registerObject(object);
                }
                this->object_ = object;
            }
            /// Returns the object-pointer.
            inline O* getObject() const
                { return this->object_; }

            // see Functor::setRawObjectPointer()
            inline void setRawObjectPointer(void* object)
                { this->setObject((O*)object); }
            // see Functor::getRawObjectPointer()
            inline void* getRawObjectPointer() const
                { return this->object_; }

            // see Functor::setSafeMode()
            inline void setSafeMode(bool bSafeMode)
            {
                if (bSafeMode == this->bSafeMode_)
                    return;

                this->bSafeMode_ = bSafeMode;

                if (bSafeMode)
                    this->registerObject(this->object_);
                else
                    this->unregisterObject(this->object_);
            }

        protected:
            /// Casts the object and registers as destruction listener.
            inline void registerObject(O* object)
                { OrxonoxClass* base = dynamic_cast<OrxonoxClass*>(object); if (base) { this->registerAsDestructionListener(base); } }
            /// Casts the object and unregisters as destruction listener.
            inline void unregisterObject(O* object)
                { OrxonoxClass* base = dynamic_cast<OrxonoxClass*>(object); if (base) { this->unregisterAsDestructionListener(base); } }

            /// Will be called by OrxonoxClass::~OrxonoxClass() if the stored object is deleted and the Functor is in safe mode.
            inline void objectDeleted()
                { this->object_ = 0; }

            O* object_;     ///< The stored object-pointer, used to execute a member-function (or NULL for static functions)
            bool bSafeMode_; ///< If true, the functor is in safe mode and registers itself as listener at the object and changes the pointer to NULL if the object is deleted
    };

    /// Specialization of FunctorMember with @a T = void.
    template <>
    class FunctorMember<void> : public Functor
    {
        public:
            /// Constructor: Stores the object-pointer.
            FunctorMember(void* object = 0) {}

            /// Calls the function-pointer with up to five arguments and an object. In case of a static-function, the object can be NULL. @return Returns the return-value of the function (if any; MultiType::Null otherwise)
            virtual MultiType operator()(void* object, const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null) = 0;

            // see Functor::operator()()
            MultiType operator()(const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null)
            {
                return (*this)((void*)0, param1, param2, param3, param4, param5);
            }

            // see Functor::getType()
            inline Functor::Type::Enum getType() const
                { return Functor::Type::Static; }

            // see Functor::setRawObjectPointer()
            inline void setRawObjectPointer(void*)
                { orxout(internal_warning) << "Can't assign an object pointer to a static functor" << endl; }
            // see Functor::getRawObjectPointer()
            inline void* getRawObjectPointer() const
                { return 0; }

            // see Functor::setSafeMode()
            inline void setSafeMode(bool) {}
    };

    /// FunctorStatic is just a typedef of FunctorMember with @a T = void.
    typedef FunctorMember<void> FunctorStatic;

    /**
        @brief FunctorPointer is a child class of FunctorMember and expands it with a function-pointer.
        @param F The type of the function-pointer
        @param O The type of the function's class (or void if it's a static function)

        The template FunctorPointer has an additional template parameter that defines the type
        of the function-pointer. This can be handy if you want to get or set the function-pointer.
        You can then use a static_cast to cast a Functor to FunctorPointer if you know the type
        of the function-pointer.

        However FunctorPointer is not aware of the types of the different parameters or the
        return value.
    */
    template <class F, class O = void>
    class FunctorPointer : public FunctorMember<O>
    {
        public:
            /// Constructor: Initializes the base class and stores the function-pointer.
            FunctorPointer(F functionPointer, O* object = 0) : FunctorMember<O>(object), functionPointer_(functionPointer) {}

            /// Changes the function-pointer.
            inline void setFunction(F functionPointer)
                { this->functionPointer_ = functionPointer; }
            /// Returns the function-pointer.
            inline F getFunction() const
                { return this->functionPointer_; }

            // see Functor::getFullIdentifier()
            const std::type_info& getFullIdentifier() const
                { return typeid(F); }

        protected:
            F functionPointer_;     ///< The stored function-pointer
    };

    namespace detail
    {
        // Helper class to get the type of the function pointer with the given class, parameters, return-value, and constness
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

        // Helper class, used to call a function-pointer with a given object and parameters and to return its return-value (if available)
        template <class R, class O, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller                                              { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (object->*functionPointer)(param1, param2, param3, param4, param5); } };
        template <class R, class O, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<R, O, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType&) { return (object->*functionPointer)(param1, param2, param3, param4); } };
        template <class R, class O, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<R, O, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType&, const MultiType&) { return (object->*functionPointer)(param1, param2, param3); } };
        template <class R, class O, bool isconst, class P1, class P2>                               struct FunctorCaller<R, O, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, P2, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType&, const MultiType&, const MultiType&) { return (object->*functionPointer)(param1, param2); } };
        template <class R, class O, bool isconst, class P1>                                         struct FunctorCaller<R, O, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, P1, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { return (object->*functionPointer)(param1); } };
        template <class R, class O, bool isconst>                                                   struct FunctorCaller<R, O, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<R, O, isconst, void, void, void, void, void>::Type functionPointer, O* object, const MultiType&, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { return (object->*functionPointer)(); } };
        template <class O, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<void, O, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (object->*functionPointer)(param1, param2, param3, param4, param5); return MultiType::Null; } };
        template <class O, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<void, O, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, P4, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType&) { (object->*functionPointer)(param1, param2, param3, param4); return MultiType::Null; } };
        template <class O, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<void, O, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, P3, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType&, const MultiType&) { (object->*functionPointer)(param1, param2, param3); return MultiType::Null; } };
        template <class O, bool isconst, class P1, class P2>                               struct FunctorCaller<void, O, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, P2, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType& param2, const MultiType&, const MultiType&, const MultiType&) { (object->*functionPointer)(param1, param2); return MultiType::Null; } };
        template <class O, bool isconst, class P1>                                         struct FunctorCaller<void, O, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, P1, void, void, void, void>::Type functionPointer, O* object, const MultiType& param1, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { (object->*functionPointer)(param1); return MultiType::Null; } };
        template <class O, bool isconst>                                                   struct FunctorCaller<void, O, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<void, O, isconst, void, void, void, void, void>::Type functionPointer, O* object, const MultiType&, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { (object->*functionPointer)(); return MultiType::Null; } };
        template <class R, bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<R, void, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, P4, P5>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { return (*functionPointer)(param1, param2, param3, param4, param5); } };
        template <class R, bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<R, void, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, P4, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType&) { return (*functionPointer)(param1, param2, param3, param4); } };
        template <class R, bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<R, void, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, P3, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType&, const MultiType&) { return (*functionPointer)(param1, param2, param3); } };
        template <class R, bool isconst, class P1, class P2>                               struct FunctorCaller<R, void, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, P2, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType&, const MultiType&, const MultiType&) { return (*functionPointer)(param1, param2); } };
        template <class R, bool isconst, class P1>                                         struct FunctorCaller<R, void, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, P1, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { return (*functionPointer)(param1); } };
        template <class R, bool isconst>                                                   struct FunctorCaller<R, void, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<R, void, isconst, void, void, void, void, void>::Type functionPointer, void*, const MultiType&, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { return (*functionPointer)(); } };
        template <bool isconst, class P1, class P2, class P3, class P4, class P5> struct FunctorCaller<void, void, isconst, P1, P2, P3, P4, P5>           { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, P4, P5>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) { (*functionPointer)(param1, param2, param3, param4, param5); return MultiType::Null; } };
        template <bool isconst, class P1, class P2, class P3, class P4>           struct FunctorCaller<void, void, isconst, P1, P2, P3, P4, void>         { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, P4, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType&) { (*functionPointer)(param1, param2, param3, param4); return MultiType::Null; } };
        template <bool isconst, class P1, class P2, class P3>                     struct FunctorCaller<void, void, isconst, P1, P2, P3, void, void>       { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, P3, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType&, const MultiType&) { (*functionPointer)(param1, param2, param3); return MultiType::Null; } };
        template <bool isconst, class P1, class P2>                               struct FunctorCaller<void, void, isconst, P1, P2, void, void, void>     { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, P2, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType& param2, const MultiType&, const MultiType&, const MultiType&) { (*functionPointer)(param1, param2); return MultiType::Null; } };
        template <bool isconst, class P1>                                         struct FunctorCaller<void, void, isconst, P1, void, void, void, void>   { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, P1, void, void, void, void>::Type functionPointer, void*, const MultiType& param1, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { (*functionPointer)(param1); return MultiType::Null; } };
        template <bool isconst>                                                   struct FunctorCaller<void, void, isconst, void, void, void, void, void> { static inline MultiType call(typename detail::FunctionPointer<void, void, isconst, void, void, void, void, void>::Type functionPointer, void*, const MultiType&, const MultiType&, const MultiType&, const MultiType&, const MultiType&) { (*functionPointer)(); return MultiType::Null; } };

        // Helper class, used to identify the header of a function-pointer (independent of its class)
        template <class R, class P1, class P2, class P3, class P4, class P5>
        struct FunctorHeaderIdentifier
        {};

        // Helper class to determine if a function has a returnvalue
        template <class T>
        struct FunctorHasReturnvalue
        { enum { result = true }; };
        template <>
        struct FunctorHasReturnvalue<void>
        { enum { result = false }; };

        // Helper class to count the number of parameters
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

    /**
        @brief FunctorTemplate is a child class of FunctorPointer and implements all functions
        that need to know the exact types of the parameters, return-value, and class.

        @param R The type of the return-value of the function
        @param O The class of the function
        @param isconst True if the function is a const member-function
        @param P1 The type of the first parameter
        @param P2 The type of the second parameter
        @param P3 The type of the third parameter
        @param P4 The type of the fourth parameter
        @param P5 The type of the fifth parameter

        This template has many parameters and is usually not used directly. It is created by
        createFunctor(), but only the base-classes Functor, FunctorMember, and FunctorPointer
        are used directly. It implements all the virtual functions that are declared by its
        base classes.

        All template arguments can be void.
    */
    template <class R, class O, bool isconst, class P1, class P2, class P3, class P4, class P5>
    class FunctorTemplate : public FunctorPointer<typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type, O>
    {
        public:
            /// Constructor: Initializes the base class.
            FunctorTemplate(typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type functionPointer, O* object = 0) : FunctorPointer<typename detail::FunctionPointer<R, O, isconst, P1, P2, P3, P4, P5>::Type, O>(functionPointer, object) {}

            // see FunctorMember::operator()()
            MultiType operator()(O* object, const MultiType& param1 = MultiType::Null, const MultiType& param2 = MultiType::Null, const MultiType& param3 = MultiType::Null, const MultiType& param4 = MultiType::Null, const MultiType& param5 = MultiType::Null)
            {
                return detail::FunctorCaller<R, O, isconst, P1, P2, P3, P4, P5>::call(this->functionPointer_, object, param1, param2, param3, param4, param5);
            }

            // see Functor::clone()
            FunctorPtr clone()
            {
                return new FunctorTemplate(*this);
            }

            // see Functor::evaluateArgument()
            void evaluateArgument(unsigned int index, MultiType& argument) const
            {
                switch (index)
                {
                    case 0: argument.convert<P1>(); break;
                    case 1: argument.convert<P2>(); break;
                    case 2: argument.convert<P3>(); break;
                    case 3: argument.convert<P4>(); break;
                    case 4: argument.convert<P5>(); break;
                }
            }

            // see Functor::getParamCount()
            unsigned int getParamCount() const
            {
                return detail::FunctorParamCount<P1, P2, P3, P4, P5>::result;
            }

            // see Functor::hasReturnvalue()
            bool hasReturnvalue() const
            {
                return detail::FunctorHasReturnvalue<R>::result;
            }

            // see Functor::getTypenameParam()
            std::string getTypenameParam(unsigned int index) const
            {
                switch (index)
                {
                    case 0:  return typeToString<P1>();
                    case 1:  return typeToString<P2>();
                    case 2:  return typeToString<P3>();
                    case 3:  return typeToString<P4>();
                    case 4:  return typeToString<P5>();
                    default: return "";
                }
            }

            // see Functor::getTypenameReturnvalue()
            std::string getTypenameReturnvalue() const
            {
                return typeToString<R>();
            }

            // see Functor::getHeaderIdentifier()
            const std::type_info& getHeaderIdentifier() const
            {
                return typeid(detail::FunctorHeaderIdentifier<R, P1, P2, P3, P4, P5>);
            }

            // see Functor::getHeaderIdentifier(unsigned int)
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

    template <class R, class O, class OO, class P1, class P2, class P3, class P4, class P5> inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5), OO* object) { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2, class P3, class P4>           inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4), OO* object)     { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2, class P3>                     inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3), OO* object)         { return new FunctorTemplate<R, O, false, P1, P2, P3, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2>                               inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2), OO* object)             { return new FunctorTemplate<R, O, false, P1, P2, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1>                                         inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1), OO* object)                 { return new FunctorTemplate<R, O, false, P1, void, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO>                                                   inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(), OO* object)                   { return new FunctorTemplate<R, O, false, void, void, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2, class P3, class P4, class P5> inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5) const, OO* object) { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2, class P3, class P4>           inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4) const, OO* object)     { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2, class P3>                     inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3) const, OO* object)         { return new FunctorTemplate<R, O, true, P1, P2, P3, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1, class P2>                               inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2) const, OO* object)             { return new FunctorTemplate<R, O, true, P1, P2, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO, class P1>                                         inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1) const, OO* object)                 { return new FunctorTemplate<R, O, true, P1, void, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object
    template <class R, class O, class OO>                                                   inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)() const, OO* object)                   { return new FunctorTemplate<R, O, true, void, void, void, void, void>(functionPointer, object); }   ///< Creates a new FunctorMember with the given function-pointer and an assigned object

    template <class R, class O, class P1, class P2, class P3, class P4, class P5> inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5)) { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, P5>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2, class P3, class P4>           inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4))     { return new FunctorTemplate<R, O, false, P1, P2, P3, P4, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2, class P3>                     inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3))         { return new FunctorTemplate<R, O, false, P1, P2, P3, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2>                               inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2))             { return new FunctorTemplate<R, O, false, P1, P2, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1>                                         inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1))                 { return new FunctorTemplate<R, O, false, P1, void, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O>                                                   inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)())                   { return new FunctorTemplate<R, O, false, void, void, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2, class P3, class P4, class P5> inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4, P5) const) { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, P5>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2, class P3, class P4>           inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3, P4) const)     { return new FunctorTemplate<R, O, true, P1, P2, P3, P4, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2, class P3>                     inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2, P3) const)         { return new FunctorTemplate<R, O, true, P1, P2, P3, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1, class P2>                               inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1, P2) const)             { return new FunctorTemplate<R, O, true, P1, P2, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O, class P1>                                         inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)(P1) const)                 { return new FunctorTemplate<R, O, true, P1, void, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer
    template <class R, class O>                                                   inline FunctorMemberPtr<O> createFunctor(R (O::*functionPointer)() const)                   { return new FunctorTemplate<R, O, true, void, void, void, void, void>(functionPointer); }   ///< Creates a new FunctorMember with the given function-pointer

    template <class R, class P1, class P2, class P3, class P4, class P5> inline FunctorStaticPtr createFunctor(R (*functionPointer)(P1, P2, P3, P4, P5)) { return new FunctorTemplate<R, void, false, P1, P2, P3, P4, P5>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
    template <class R, class P1, class P2, class P3, class P4>           inline FunctorStaticPtr createFunctor(R (*functionPointer)(P1, P2, P3, P4))     { return new FunctorTemplate<R, void, false, P1, P2, P3, P4, void>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
    template <class R, class P1, class P2, class P3>                     inline FunctorStaticPtr createFunctor(R (*functionPointer)(P1, P2, P3))         { return new FunctorTemplate<R, void, false, P1, P2, P3, void, void>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
    template <class R, class P1, class P2>                               inline FunctorStaticPtr createFunctor(R (*functionPointer)(P1, P2))             { return new FunctorTemplate<R, void, false, P1, P2, void, void, void>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
    template <class R, class P1>                                         inline FunctorStaticPtr createFunctor(R (*functionPointer)(P1))                 { return new FunctorTemplate<R, void, false, P1, void, void, void, void>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
    template <class R>                                                   inline FunctorStaticPtr createFunctor(R (*functionPointer)())                   { return new FunctorTemplate<R, void, false, void, void, void, void, void>(functionPointer); }   ///< Creates a new Functor with the given function-pointer
}

#endif /* _Functor_H__ */
