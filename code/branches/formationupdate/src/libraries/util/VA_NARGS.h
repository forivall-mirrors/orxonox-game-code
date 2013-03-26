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
    @ingroup Util
    @brief Declaration of the ORXONOX_VA_NARGS macro which returns the number of arguments passed to a variadic macro.

    With this utility you can overload a macro for different numbers of arguments
    (of course overloading is not possible for different types, as macros are not
    type aware, but for different numbers of arguments is still very powerful).

    Important: The macro can not be overloaded for 0 arguments: ORXONOX_VA_NARGS() returns 1!

    Example: A macro to call functions
    @code
    myfunction();                                           // A static function
    MyClass::myStaticFunction();                            // A static class function
    MyClass::myFunction();                                  // A member function

    #define CallFunction(...) \                             // Define a variadic macro that passes the arguments to the overloaded implementations
        BOOST_PP_EXPAND(BOOST_PP_CAT(CallFunction, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__))

    #define CallFunction1(function) \                       // Overloaded macro for 1 argument
        function()                                          // Calls the static function

    #define CallFunction2(class, function) \                // Overloaded macro for 2 arguments
        class::function()                                   // Calls the static class function

    #define CallFunction3(class, function, object) \        // Overloaded macro for 3 arguments
        object->class::function()                           // Calls the function on the object


    CallFunction(myFunction);                               // Call the macro with 1 argument
    CallFunction(MyClass, myStaticFunction);                // Call the macro with 2 arguments
    CallFunction(MyClass, myFunction, new MyClass());       // Call the macro with 3 arguments
    @endcode

    Note that the first (variadic) macro concatenates the name "CallFunction" with
    the number of arguments ("1" - "N"). Then all arguments are passed to the right macro.
*/

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

/**
    @brief Returns the number of arguments passed to a variadic macro. Important: The number of arguments must be greater than zero (ORXONOX_VA_NARGS() returns 1).
*/
#define ORXONOX_VA_NARGS(...) \
    ORXONOX_VA_NARGS_CONCAT(__VA_ARGS__, ORXONOX_VA_NARGS_NUMBERS)


// some helper macros //

#define ORXONOX_VA_NARGS_CONCAT(...) \
    BOOST_PP_EXPAND(ORXONOX_VA_NARGS_INTERN(__VA_ARGS__))

#define ORXONOX_VA_NARGS_INTERN( \
    arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,  arg8,  arg9,  arg10, \
    arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, \
    arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, \
    arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39, arg40, \
    arg41, arg42, arg43, arg44, arg45, arg46, arg47, arg48, arg49, arg50, \
    arg51, arg52, arg53, arg54, arg55, arg56, arg57, arg58, arg59, arg60, \
    arg61, arg62, arg63, argn, ...) argn

#define ORXONOX_VA_NARGS_NUMBERS \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, \
    47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, \
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, \
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0
