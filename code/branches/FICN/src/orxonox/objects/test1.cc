#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "../../tinyxml/tinyxml.h"
#include "../Orxonox.h"
#include <OgreSceneManager.h>

namespace orxonox
{
    CreateFactory(Test1);

    Test1::Test1()
    {
        RegisterObject(Test1);

        this->usefullClass1_ = Class(Test1);
        this->usefullClass2_ = Class(Test2);
        this->usefullClass3_ = Class(Test3);
    }

    Test1::~Test1()
    {
    }

    void Test1::tick(float dt)
    {
        std::cout << "Test1: " << this << "\n";
    }

    bool Test1::usefullClass1isA(Identifier* identifier)
    {
        return this->usefullClass1_->isA(identifier);
    }

    bool Test1::usefullClass2isA(Identifier* identifier)
    {
        return this->usefullClass2_->isA(identifier);
    }

    bool Test1::usefullClass3isA(Identifier* identifier)
    {
        return this->usefullClass3_.isA(identifier);
    }

    void Test1::setUsefullClass1(Identifier* identifier)
    {
        std::cout << "\n";
        std::cout << "Test1: usefullClass1->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test1: usefullClass1->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test1: usefullClass1->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass1_ = identifier;
    }

    void Test1::setUsefullClass2(Identifier* identifier)
    {
        std::cout << "\n";
        std::cout << "Test1: usefullClass2->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test1: usefullClass2->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test1: usefullClass2->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass2_ = identifier;
    }

    void Test1::setUsefullClassOfTypeTest3(Identifier* identifier)
    {
        std::cout << "\n";
        std::cout << "Test1: usefullClass3->isA(Class(Test1)): " << identifier->isA(Class(Test1)) << "\n";
        std::cout << "Test1: usefullClass3->isA(Class(Test2)): " << identifier->isA(Class(Test2)) << "\n";
        std::cout << "Test1: usefullClass3->isA(Class(Test3)): " << identifier->isA(Class(Test3)) << "\n";
        this->usefullClass3_ = identifier;
    }

    void Test1::loadParams(TiXmlElement* xmlElem)
    {



    }
}
