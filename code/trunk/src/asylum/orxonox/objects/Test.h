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
 */

#ifndef _Test_H__
#define _Test_H__

#include "core/BaseObject.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    class Interface1 : virtual public OrxonoxClass
    {
        protected:
            Interface1() { RegisterRootObject(Interface1); }
    };

    class Interface2 : virtual public OrxonoxClass
    {
        protected:
            Interface2() { RegisterRootObject(Interface2); }
    };

    class A1 : public BaseObject
    {
        public:
            A1() { RegisterObject(A1); }
    };

    class A2 : public BaseObject
    {
        public:
            A2() { RegisterObject(A2); }
    };

    class A3: public BaseObject, public Interface1
    {
        public:
            A3() { RegisterObject(A3); }
    };

    class A1B1 : public A1
    {
        public:
            A1B1() { RegisterObject(A1B1); }
    };

    class A1B2 : public A1
    {
        public:
            A1B2() { RegisterObject(A1B2); }
//            virtual void test() = 0;
    };

    class A2B1 : public A2
    {
        public:
            A2B1() { RegisterObject(A2B1); }
    };

    class A2B2 : public A2, Interface1
    {
        public:
            A2B2() { RegisterObject(A2B2); }
    };

    class A3B1 : public A3
    {
        public:
            A3B1() { RegisterObject(A3B1); }
    };

    class A3B2 : public A3, Interface2
    {
        public:
            A3B2() { RegisterObject(A3B2); }
    };

    class A1B1C1 : public A1B1
    {
        public:
            A1B1C1() { RegisterObject(A1B1C1); }
    };

    class A1B1C2 : public A1B1
    {
        public:
            A1B1C2() { RegisterObject(A1B1C2); }
    };

    class A1B2C1 : public A1B2
    {
        public:
            A1B2C1() { RegisterObject(A1B2C1); }
//            void test() { std::cout << "test!\n"; }
    };

    class A2B1C1 : public A2B1, Interface2
    {
        public:
            A2B1C1() { RegisterObject(A2B1C1); }
    };

    class A2B2C1 : public A2B2
    {
        public:
            A2B2C1() { RegisterObject(A2B2C1); }
    };

    class A3B1C1 : public A3B1
    {
        public:
            A3B1C1() { RegisterObject(A3B1C1); }
    };

    class A3B1C2 : public A3B1, Interface2
    {
        public:
            A3B1C2() { RegisterObject(A3B1C2); }
    };

    class A3B2C1 : public A3B2
    {
        public:
            A3B2C1() { RegisterObject(A3B2C1); }
    };

    class A3B2C2 : public A3B2
    {
        public:
            A3B2C2() { RegisterObject(A3B2C2); }
    };

    CreateFactory(A1);
    CreateFactory(A2);
    CreateFactory(A3);
    CreateFactory(A1B1);
    CreateFactory(A1B2);
    CreateFactory(A2B1);
    CreateFactory(A2B2);
    CreateFactory(A3B1);
    CreateFactory(A3B2);
    CreateFactory(A1B1C1);
    CreateFactory(A1B1C2);
    CreateFactory(A1B2C1);
    CreateFactory(A2B1C1);
    CreateFactory(A2B2C1);
    CreateFactory(A3B1C1);
    CreateFactory(A3B1C2);
    CreateFactory(A3B2C1);
    CreateFactory(A3B2C2);
}

#endif
