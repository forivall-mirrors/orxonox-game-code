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
    @defgroup Super Super
    @ingroup Class
*/

/**
    @file
    @ingroup Class Super
    @brief Definition of all super-function related macros, used to call functions of the base class.

    This file defines all macros needed to add a new "super-function". If you add
    a super-function, you can call <tt>SUPER(myclass, functionname, arguments)</tt>
    inside your code and the function of the parent-class gets called. This is comparable
    to <tt>super.functionname(arguments)</tt> in Java or other languages.

    This works only with virtual functions that return nothing (@c void) and belong to
    classes that have an @ref orxonox::Identifier "Identifier". Arguments however are
    supported, there's no limitation for their number and type, except that the type has
    to be known in Super.h.

    To add a new super-function, you have to process 4 steps:

    -# Add a new @c SUPER macro <br />
       This allows you to call the super-function in your code. <br />
       Location: This file (Super.h), marked with "--> HERE <--" comments (1/3)
    -# Call the @c SUPER_FUNCTION_GLOBAL_DECLARATION_PART1/2 macros. <br />
       This defines some global classes and templates, needed to create and call the super-functions. <br />
       Location: This file (Super.h), marked with "--> HERE <--" comments (2/3)
    -# Call the @c SUPER_INTRUSIVE_DECLARATION macro. <br />
       This will be included into the declaration of @c ClassIdentifier<T>. <br />
       Location: This file (Super.h), marked with "--> HERE <--" comments (3/3)
    -# Call the @c SUPER_FUNCTION macro. <br />
       This defines a partially specialized template that will decide if a class is "super" to another class.
       If the check returns true, a @c SuperFunctionCaller gets created, which will be used by the @c SUPER macro.
       You have to add this into the header-file of the baseclass of the super-function (the class that first
       implements the function), below the class declaration. You can't call it directly in this file, because
       otherwise you had to include the headerfile right here, which would cause some ugly back-dependencies,
       include loops and slower compilation. <br />
       Dont forget to include Super.h in the header-file. <br />
       Location: The header-file of the baseclass (Baseclass.h), below the class declaration
*/

#ifndef _Super_H__
#define _Super_H__

#include "CorePrereqs.h"
#include "util/Output.h"

///////////////////////
// Macro definitions //
///////////////////////

//// Common macros ////

    /**
        @brief Declares a new super-function by creating a specialized template. Add this below the class declaration of the baseclass.
        @param functionnumber Each super-function needs a unique number, starting with zero, increasing by one
        @param baseclass The baseclass of the super-function (~the root)
        @param functionname The name of the super-function
        @param purevirtualbase "true" if the function is pure virtual in the baseclass, "false" if the function is implemented (without "")
    */
    #define SUPER_FUNCTION(functionnumber, baseclass, functionname, purevirtualbase) \
        template <class T, int templatehack2> \
        struct SuperFunctionCondition<functionnumber, T, 0, templatehack2> \
        { \
            static void superCheck() \
            { \
                SuperFunctionCondition<functionnumber, T, 0, templatehack2>::apply(static_cast<T*>(0)); \
                SuperFunctionCondition<functionnumber + 1, T, 0, templatehack2>::superCheck(); \
            } \
            \
            static void apply(void*) {} \
            \
            static void apply(baseclass*) \
            { \
                ClassIdentifier<T>* identifier = ClassIdentifier<T>::getIdentifier(); \
                for (std::set<const Identifier*>::iterator it = identifier->getDirectChildrenIntern().begin(); it != identifier->getDirectChildrenIntern().end(); ++it) \
                { \
                    if (((ClassIdentifier<T>*)(*it))->bSuperFunctionCaller_##functionname##_isFallback_ && ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_) \
                    { \
                        delete ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_; \
                        ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_ = 0; \
                        ((ClassIdentifier<T>*)(*it))->bSuperFunctionCaller_##functionname##_isFallback_ = false; \
                    } \
                    \
                    if (!((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_) \
                    { \
                        orxout(verbose, context::super) << "Added SuperFunctionCaller for " << #functionname << ": " << ClassIdentifier<T>::getIdentifier()->getName() << " <- " << ((ClassIdentifier<T>*)(*it))->getName() << endl; \
                        ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_ = new SuperFunctionClassCaller_##functionname <T>; \
                    } \
                    else if (((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_->getParentIdentifier() != ClassIdentifier<T>::getIdentifier()) \
                        orxout(internal_warning, context::super) << "SuperFunctionCaller for " << #functionname << " in " << ((ClassIdentifier<T>*)(*it))->getName() << " calls function of " << ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_->getParentIdentifier()->getName() << " but " << ClassIdentifier<T>::getIdentifier()->getName() << " is also possible (do you use multiple inheritance?)" << endl; \
                } \
            } \
        }; \
        \
        SUPER_FUNCTION_PUREVIRTUAL_WORKAROUND##purevirtualbase(functionnumber, baseclass)

    #define SUPER_FUNCTION_PUREVIRTUAL_WORKAROUND0(functionnumber, baseclass) SUPER_FUNCTION_PUREVIRTUAL_WORKAROUNDfalse(functionnumber, baseclass)
    #define SUPER_FUNCTION_PUREVIRTUAL_WORKAROUND1(functionnumber, baseclass) SUPER_FUNCTION_PUREVIRTUAL_WORKAROUNDtrue(functionnumber, baseclass)
    #define SUPER_FUNCTION_PUREVIRTUAL_WORKAROUNDfalse(functionnumber, baseclass)
    #define SUPER_FUNCTION_PUREVIRTUAL_WORKAROUNDtrue(functionnumber, baseclass) \
        template <int templatehack2> \
        struct SuperFunctionCondition<functionnumber, baseclass, 0, templatehack2> \
        { \
            static void superCheck() \
            { \
                SuperFunctionCondition<functionnumber + 1, baseclass, 0, templatehack2>::superCheck(); \
            } \
        };


    /*
    //// Comments about the macro ////

        // Partially specialized template (templatehack is now specialized too).
        //
        // This ensures the compiler takes THIS template if the header-file of the super-function
        // is included. In any other case, the compiler just uses the fallback template which is
        // defined in this file.
        template <class T, templatehack2>
        struct SuperFunctionCondition<functionnumber, T, 0, templatehack2>
        {
            static void superCheck()
            {
                // This call to the apply-function is the whole check. By calling the function with
                // a T* pointer, the right function get's called.
                SuperFunctionCondition<functionnumber, T, 0, templatehack2>::apply(static_cast<T*>(0));

                // Go go the superCheck for of next super-function (functionnumber + 1)
                SuperFunctionCondition<functionnumber + 1, T, 0, templatehack2>::superCheck();
            }

            // This function gets called if T is not a child of the baseclass.
            // The function does nothing.
            static void apply(void* temp) {}

            // This function gets called if T is a child of the baseclass and can therefore be converted.
            // The function adds a SuperFunctionCaller to the Identifier of all subclasses of T.
            static void apply(baseclass* temp)
            {
                ClassIdentifier<T>* identifier = ClassIdentifier<T>::getIdentifier();

                // Iterate through all children
                for (std::set<const Identifier*>::iterator it = identifier->getDirectChildrenIntern().begin(); it != identifier->getDirectChildrenIntern().end(); ++it)
                {
                    // Check if the caller is a fallback-caller
                    if (((ClassIdentifier<T>*)(*it))->bSuperFunctionCaller_##functionname##_isFallback_ && ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_)
                    {
                        // Delete the fallback caller an prepare to get a real caller
                        delete ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_;
                        ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_ = 0;
                        ((ClassIdentifier<T>*)(*it))->bSuperFunctionCaller_##functionname##_isFallback_ = false;
                    }

                    // Check if there's not already a caller
                    if (!((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_)
                    {
                        // Add the SuperFunctionCaller
                        orxout(verbose, context::super) << "adding functionpointer to " << ((ClassIdentifier<T>*)(*it))->getName() << endl;
                        ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_ = new SuperFunctionClassCaller_##functionname <T>;
                    }

                    // If there is already a caller, but for another parent, print a warning
                    else if (((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_->getParentIdentifier() != ClassIdentifier<T>::getIdentifier())
                        orxout(internal_warning, context::super) << "SuperFunctionCaller for " << #functionname << " in " << ((ClassIdentifier<T>*)(*it))->getName() << " calls function of " << ((ClassIdentifier<T>*)(*it))->superFunctionCaller_##functionname##_->getParentIdentifier()->getName() << " but " << ClassIdentifier<T>::getIdentifier()->getName() << " is also possible (do you use multiple inheritance?)" << endl;
                }
            }
        };
        SUPER_FUNCTION_PUREVIRTUAL_WORKAROUND##purevirtualbase


        // The following piece of code is only added if purevirtualbase = true

        // Explicit specialization of the Condition template for the baseclass to avoid
        // errors if the function is pure virtual in the baseclass.
        template <int templatehack2> \
        struct SuperFunctionCondition<functionnumber, baseclass, 0, templatehack2> \
        { \
            // The superCheck function acts like the fallback - it advances to the check for the next super-function (functionnumber + 1)
            static void superCheck() \
            { \
                SuperFunctionCondition<functionnumber + 1, baseclass, 0, templatehack2>::superCheck(); \
            } \
        };
    */

    /// SUPER-macro: Calls Parent::functionname(...) where Parent is the direct parent of @a classname
    #ifdef ORXONOX_COMPILER_MSVC
        #define SUPER(classname, functionname, ...) \
            __super::functionname(__VA_ARGS__)
    #else
        #define SUPER(classname, functionname, ...) \
            SUPER_##functionname(classname, functionname, __VA_ARGS__)
    #endif

    // helper macro: for functions without arguments
    #define SUPER_NOARGS(classname, functionname) \
        (*ClassIdentifier<classname>::getIdentifier()->superFunctionCaller_##functionname##_)(this)

    // helper macro: for functions with arguments
    #define SUPER_ARGS(classname, functionname, ...) \
        (*ClassIdentifier<classname>::getIdentifier()->superFunctionCaller_##functionname##_)(this, __VA_ARGS__)


//// Function-specific macros ////

    /*
        Add a macro for each super-function

        Example (no arguments):
        #define SUPER_myfunction(classname, functionname, ...) \
            SUPER_NOARGS(classname, functionname)

        Example (with arguments):
        #define SUPER_myfunction(classname, functionname, ...) \
            SUPER_ARGS(classname, functionname, __VA_ARGS__)
    */

    // (1/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--
    #define SUPER_XMLPort(classname, functionname, ...) \
        SUPER_ARGS(classname, functionname, __VA_ARGS__)

    #define SUPER_tick(classname, functionname, ...) \
        SUPER_ARGS(classname, functionname, __VA_ARGS__)

    #define SUPER_changedActivity(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedVisibility(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_XMLEventPort(classname, functionname, ...) \
        SUPER_ARGS(classname, functionname, __VA_ARGS__)

    #define SUPER_changedScale(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedOwner(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedOverlayGroup(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedName(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedGametype(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedUsed(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_clone(classname, functionname, ...) \
        SUPER_ARGS(classname, functionname, __VA_ARGS__)

    #define SUPER_changedCarrier(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    #define SUPER_changedPickedUp(classname, functionname, ...) \
        SUPER_NOARGS(classname, functionname)

    // (1/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--


namespace orxonox
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // This code gets included by Identifier.h and every other header file that needs a super-function //
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    //// Common code ////

        // Base templates
        /**
            @brief Creates the SuperFunctionCaller if T is a child of the super-functions baseclass.
        */
        template <int functionnumber, class T, int templatehack1, int templatehack2>
        struct SuperFunctionCondition
        {
            static void superCheck() {}
        };

        /**
            @brief Initializes the SuperFunctionCaller-pointer with zero.
        */
        template <int functionnumber, class T>
        struct SuperFunctionInitialization
        {
            static void initialize(ClassIdentifier<T>*) {}
        };

        /**
            @brief Deletes the SuperFunctionCaller.
        */
        template <int functionnumber, class T>
        struct SuperFunctionDestruction
        {
            static void destroy(ClassIdentifier<T>*) {}
        };


    //// Function-specific code ////

        /**
            @brief Creates the needed objects and templates to call a super-function.
            @param functionnumber Each super-function needs a unique number, starting with zero, increasing by one
            @param functionname The name of the super-function
            @param hasarguments "false" if the function doesn't take any arguments, "true" if it does (without "")
            @param ... Variadic: If the function takes arguments, add them here with type and name. Example: int myvalue, float myothervalue
        */
        #define SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(functionnumber, functionname, hasarguments, ...) \
            template <class T, int templatehack1, int templatehack2> \
            struct SuperFunctionCondition<functionnumber, T, templatehack1, templatehack2> \
            { \
                static void superCheck() \
                { \
                    SuperFunctionCondition<functionnumber + 1, T, templatehack1, templatehack2>::superCheck(); \
                } \
            }; \
            \
            class _CoreExport SuperFunctionCaller_##functionname \
            { \
                public: \
                    virtual void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) ) = 0; \
                    virtual ~SuperFunctionCaller_##functionname () {} \
                    virtual Identifier* getParentIdentifier() const = 0; \
            }; \
            \
            template <class T> \
            class SuperFunctionClassCaller_purevirtualfallback_##functionname : public SuperFunctionCaller_##functionname \
            { \
                public: \
                    inline void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) ) \
                    { \
                    } \
                    \
                    Identifier* getParentIdentifier() const \
                    { \
                        return ClassIdentifier<T>::getIdentifier(); \
                    } \
            }; \
            \
            template <class T> \
            struct SuperFunctionInitialization<functionnumber, T> \
            { \
                static void initialize(ClassIdentifier<T>* identifier) \
                { \
                    identifier->superFunctionCaller_##functionname##_ = new SuperFunctionClassCaller_purevirtualfallback_##functionname <T>; \
                    identifier->bSuperFunctionCaller_##functionname##_isFallback_ = true; \
                    SuperFunctionInitialization<functionnumber + 1, T>::initialize(identifier); \
                } \
            }; \
            \
            template <class T> \
            struct SuperFunctionDestruction<functionnumber, T> \
            { \
                static void destroy(ClassIdentifier<T>* identifier) \
                { \
                    if (identifier->superFunctionCaller_##functionname##_) \
                        delete identifier->superFunctionCaller_##functionname##_; \
                    SuperFunctionDestruction<functionnumber + 1, T>::destroy(identifier); \
                } \
            }; \
            \
            template <class T> \
            class SuperFunctionClassCaller_##functionname : public SuperFunctionCaller_##functionname \
            { \
                public: \
                    inline void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) ) \
                    { \
                        (dynamic_cast<T*>(object))->T:: functionname

        /*
            JUST ADD THE FUNCTION ARGUMENTS BETWEEN BOTH MACROS, ENCLOSED BY BRACKETS
            EXAMPLE:

              SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(0, myfunction, true, int myvalue, float myothervalue) <-- !!! DONT ADD A SEMICOLON HERE !!!
                (myvalue, myothervalue)
              SUPER_FUNCTION_GLOBAL_DECLARATION_PART2
        */

        #define SUPER_FUNCTION_GLOBAL_DECLARATION_PART2 \
                                                        ; \
                    } \
                    \
                    Identifier* getParentIdentifier() const \
                    { \
                        return ClassIdentifier<T>::getIdentifier(); \
                    } \
            };

        #define SUPER_CALL_ARGUMENTSfalse(...) OrxonoxClass* object
        #define SUPER_CALL_ARGUMENTS0(...)     OrxonoxClass* object
        #define SUPER_CALL_ARGUMENTStrue(...) OrxonoxClass* object, __VA_ARGS__
        #define SUPER_CALL_ARGUMENTS1(...)    OrxonoxClass* object, __VA_ARGS__


    /*
    //// COMMENTS ABOUT THE MACRO ////

        // Partially specialized template (templatehack not yet specialized, this
        // will be done by the real condition in the header-file of the super-function)
        // Only used as fallback
        template <class T, int templatehack1, int templatehack2>
        struct SuperFunctionCondition<functionnumber, T, templatehack1, templatehack2>
        {
            // If this function gets called, the header-file of the super function is not
            // included, so this fallback template (templatehack not specialized) is used
            static void superCheck()
            {
                // Calls the condition-check of the next super-function (functionnumber + 1)
                SuperFunctionCondition<functionnumber + 1, T, templatehack1, templatehack2>::superCheck();
            }
        };

        // Baseclass of the super-function caller. The real call will be done by a
        // templatized subclass through the virtual () operator.
        class _CoreExport SuperFunctionCaller_##functionname
        {
            public:
                virtual void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) ) = 0;
                virtual ~SuperFunctionCaller_##functionname () {}
                virtual Identifier* getParentIdentifier() const = 0;
        };

        // Fallback if the base is pure virtual
        template <class T>
        class SuperFunctionClassCaller_purevirtualfallback_##functionname : public SuperFunctionCaller_##functionname
        {
            public:
                // Fallback does nothing
                inline void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) )
                {
                }

                Identifier* getParentIdentifier() const
                {
                    return ClassIdentifier<T>::getIdentifier();
                }
        };

        // Initializes the SuperFunctionCaller-pointer with a fallback caller in case the base function is pure virtual
        template <class T>
        struct SuperFunctionInitialization<functionnumber, T>
        {
            static void initialize(ClassIdentifier<T>* identifier)
            {
                identifier->superFunctionCaller_##functionname##_ = new SuperFunctionClassCaller_purevirtualfallback_##functionname <T>;
                identifier->bSuperFunctionCaller_##functionname##_isFallback_ = true;

                // Calls the initialization of the next super-function (functionnumber + 1)
                SuperFunctionInitialization<functionnumber + 1, T>::initialize(identifier);
            }
        };

        // Deletes the SuperFunctionCaller.
        template <class T>
        struct SuperFunctionDestruction<functionnumber, T>
        {
            static void destroy(ClassIdentifier<T>* identifier)
            {
                if (identifier->superFunctionCaller_##functionname##_)
                    delete identifier->superFunctionCaller_##functionname##_;

                // Calls the destruction of the next super-function (functionnumber + 1)
                SuperFunctionDestruction<functionnumber + 1, T>::destroy(identifier);
            }
        };

        // The real super-function caller: Calls T::functionname()
        // T should be the parent, but this will be done by the spezialized condition template
        template <class T>
        class SuperFunctionClassCaller_##functionname : public SuperFunctionCaller_##functionname
        {
            public:
                // @brief Calls the function.
                // @param object The object to call the function on
                // @param ... The arguments of the function
                inline void operator()( SUPER_CALL_ARGUMENTS##hasarguments(__VA_ARGS__) )
                {
                    (dynamic_cast<T*>(object))->T:: functionname ( Call the function with it's arguments );
                }

                Identifier* getParentIdentifier() const
                {
                    return ClassIdentifier<T>::getIdentifier();
                }
        }
    */


    //// Execute the code for each super-function ////

        // (2/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(0, XMLPort, true, Element& xmlelement, XMLPort::Mode mode)
            (xmlelement, mode)
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(1, tick, true, float dt)
            (dt)
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(2, changedActivity, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(3, changedVisibility, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(4, XMLEventPort, true, Element& xmlelement, XMLPort::Mode mode)
            (xmlelement, mode)
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(5, changedScale, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(6, changedOwner, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(7, changedOverlayGroup, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(8, changedName, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(9, changedGametype, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(10, changedUsed, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(11, clone, true, OrxonoxClass*& item)
            (item)
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(12, changedCarrier, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        SUPER_FUNCTION_GLOBAL_DECLARATION_PART1(13, changedPickedUp, false)
            ()
        SUPER_FUNCTION_GLOBAL_DECLARATION_PART2;

        // (2/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--

}

#else /* _Super_H__ */
  #ifdef SUPER_INTRUSIVE_DECLARATION_INCLUDE

//////////////////////////////////////////////////////////////////////////
// This code gets included within the declaration of ClassIdentifier<T> //
//////////////////////////////////////////////////////////////////////////

//// Common code ////

    private:

        template <int functionnumber, class TT, int templatehack1, int templatehack2>
        friend struct SuperFunctionCondition;

        // Creates the super-function-callers by calling the first SuperFunctionCondition check
        // This get's called within the initialization of an Identifier
        virtual void createSuperFunctionCaller() const
        {
            SuperFunctionCondition<0, T, 0, 0>::superCheck();
        }


//// Function-specific code ////

    public:
        /**
            @brief Adds a pointer to the SuperFunctionCaller as a member of ClassIdentifier.
            @param functionname The name of the super-function
        */
        #ifndef SUPER_INTRUSIVE_DECLARATION
          #define SUPER_INTRUSIVE_DECLARATION(functionname) \
            SuperFunctionCaller_##functionname * superFunctionCaller_##functionname##_; \
            bool bSuperFunctionCaller_##functionname##_isFallback_
        #endif


//// Execute the code for each super-function ////

    // (3/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--
    SUPER_INTRUSIVE_DECLARATION(XMLPort);
    SUPER_INTRUSIVE_DECLARATION(tick);
    SUPER_INTRUSIVE_DECLARATION(changedActivity);
    SUPER_INTRUSIVE_DECLARATION(changedVisibility);
    SUPER_INTRUSIVE_DECLARATION(XMLEventPort);
    SUPER_INTRUSIVE_DECLARATION(changedScale);
    SUPER_INTRUSIVE_DECLARATION(changedOwner);
    SUPER_INTRUSIVE_DECLARATION(changedOverlayGroup);
    SUPER_INTRUSIVE_DECLARATION(changedName);
    SUPER_INTRUSIVE_DECLARATION(changedGametype);
    SUPER_INTRUSIVE_DECLARATION(changedUsed);
    SUPER_INTRUSIVE_DECLARATION(clone);
    SUPER_INTRUSIVE_DECLARATION(changedCarrier);
    SUPER_INTRUSIVE_DECLARATION(changedPickedUp);
    // (3/3) --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <-- --> HERE <--


    #undef SUPER_INTRUSIVE_DECLARATION_INCLUDE
  #endif /* SUPER_INTRUSIVE_DECLARATION_INCLUDE */
#endif /* _Super_H__ */
