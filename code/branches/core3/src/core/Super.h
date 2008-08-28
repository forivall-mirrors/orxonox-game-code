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

#ifndef _Super_H__
#define _Super_H__

#include <iostream>

//////////////////
// Common macro //
//////////////////
#define SUPER(classname, functionname, ...) \
    SUPER_##functionname(classname, functionname, __VA_ARGS__)

/////////////////////////////
// Function-specific macro //
/////////////////////////////
#define SUPER_testfunction(classname, functionname, ...) \
    (*ClassIdentifier<classname>::getIdentifier()->superFunctionCaller_##functionname##_)(this)

namespace orxonox
{
    /////////////////
    // Common code //
    /////////////////
    class SuperDummy {};

    // Base template
    template <int functionnumber, int templatehack, class T>
    struct SuperFunctionCondition
    {
        static void check() {}
    };

    ////////////////////////////
    // Function-specific code //
    ////////////////////////////
    // Partially specialized template (templatehack not yet specialized, this will be done by the real condition in the header file of the super-function)
    template <int templatehack, class T>
    struct SuperFunctionCondition<0, templatehack, T>
    {
        // Calls the condition of the next super-function
        static void check()
        {
            std::cout << "ignore superfunction \"testfunction\" in " << ClassIdentifier<T>::getIdentifier()->getName() << std::endl;
            SuperFunctionCondition<0 + 1, templatehack, T>::check();
        }
    };

    class SuperFunctionCaller_testfunction
    {
        public:
            virtual void operator()(void* object) = 0;
    };

    template <class T>
    class SuperFunctionClassCaller_testfunction : public SuperFunctionCaller_testfunction
    {
        public:
            inline virtual void operator()(void* object)
            {
                ((T*)object)->T::testfunction();
            }
    };
}

#else /* _Super_H__ */
  #ifdef SUPER_INTRUSIVE_DECLARATION

/////////////////
// Common code //
/////////////////
private:
    template <int functionnumber, int templatehack, class TT>
    friend struct SuperFunctionCondition;

    // Creates the function pointers by calling the first SuperFunctionCondition check
    virtual void createSuperFunctionCaller() const
    {
        SuperFunctionCondition<0, 0, T>::check();
    }


////////////////////////////
// Function-specific code //
////////////////////////////
public:
    // The function caller
    SuperFunctionCaller_testfunction* superFunctionCaller_testfunction_;

    #undef SUPER_INTRUSIVE_DECLARATION
  #endif /* SUPER_INTRUSIVE_DECLARATION */

  #ifdef SUPER_INTRUSIVE_CONSTRUCTOR

////////////////////////////
// Function-specific code //
////////////////////////////
this->superFunctionCaller_testfunction_ = 0;

    #undef SUPER_INTRUSIVE_CONSTRUCTOR
  #endif /* SUPER_INTRUSIVE_CONSTRUCTOR */
#endif /* _Super_H__ */
