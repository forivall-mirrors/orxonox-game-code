#include "test1.h"
#include "test2.h"
#include "test3.h"

namespace orxonox
{
    CreateFactory(Test2);

    Test2::Test2()
    {
        registerObject(Test2);

        this->usefullClass1_ = Class(Test1);
        this->usefullClass2_ = Class(Test2);
        this->usefullClass3_ = Class(Test3);
    }

    Test2::~Test2()
    {
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
        std::cout << "\n";
        std::cout << "Test2: usefullClass1->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test2: usefullClass1->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test2: usefullClass1->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass1_ = identifier;
    }

    void Test2::setUsefullClass2(Identifier* identifier)
    {
        std::cout << "\n";
        std::cout << "Test2: usefullClass2->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test2: usefullClass2->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test2: usefullClass2->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass2_ = identifier;
    }

    void Test2::setUsefullClassOfTypeTest3(Identifier* identifier)
    {
        std::cout << "\n";
        std::cout << "Test2: usefullClass3->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test2: usefullClass3->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test2: usefullClass3->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass3_ = identifier;
    }
}
