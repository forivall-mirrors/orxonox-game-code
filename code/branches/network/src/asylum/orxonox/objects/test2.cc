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

#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"

namespace orxonox
{
    CreateFactory(Test2);

    Test2::Test2()
    {
        RegisterObject(Test2);

        this->usefullClass1_ = Class(Test1);
        this->usefullClass2_ = Class(Test2);
        this->usefullClass3_ = Class(Test3);

        timer1.setTimer(1, true, this, createExecutor(createFunctor(&Test2::timerFunction1)));
        timer2.setTimer(5, true, this, createExecutor(createFunctor(&Test2::timerFunction2)));
        timer3.setTimer(10, false, this, createExecutor(createFunctor(&Test2::timerFunction3)));
    }

    Test2::~Test2()
    {
    }

    void Test2::timerFunction1()
    {
        std::cout << "Test2: 1 Sekunde" << std::endl;
    }

    void Test2::timerFunction2()
    {
        std::cout << "Test2: 5 Sekunden" << std::endl;
    }

    void Test2::timerFunction3()
    {
        std::cout << "Test2: 10 Sekunden sind um!" << std::endl;
    }

    bool Test2::usefullClass1isA(Identifier* identifier)
    {
        return this->usefullClass1_->isA(identifier);
    }

    bool Test2::usefullClass2isA(Identifier* identifier)
    {
        return this->usefullClass2_->isA(identifier);
    }

    bool Test2::usefullClass3isA(Identifier* identifier)
    {
        return this->usefullClass3_.isA(identifier);
    }

    void Test2::setUsefullClass1(Identifier* identifier)
    {
        std::cout << std::endl;
        std::cout << "Test2: usefullClass1->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << std::endl;
        std::cout << "Test2: usefullClass1->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << std::endl;
        std::cout << "Test2: usefullClass1->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << std::endl;
        this->usefullClass1_ = identifier;
    }

    void Test2::setUsefullClass2(Identifier* identifier)
    {
        std::cout << std::endl;
        std::cout << "Test2: usefullClass2->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << std::endl;
        std::cout << "Test2: usefullClass2->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << std::endl;
        std::cout << "Test2: usefullClass2->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << std::endl;
        this->usefullClass2_ = identifier;
    }

    void Test2::setUsefullClassOfTypeTest3(Identifier* identifier)
    {
        std::cout << std::endl;
        std::cout << "Test2: usefullClass3->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << std::endl;
        std::cout << "Test2: usefullClass3->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << std::endl;
        std::cout << "Test2: usefullClass3->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << std::endl;
        this->usefullClass3_ = identifier;
    }
}
