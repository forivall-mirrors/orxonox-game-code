#include "test1.h"
#include "test2.h"
#include "test3.h"

namespace orxonox
{
    Test3::Test3()
    {
        registerObject(Test3);
    }

    Test3::~Test3()
    {
    }
        #define testandcout(code) \
          std::cout << #code << " " << code << "\n"

    bool Test3::usefullClassesIsATest(Test1* test1)
    {
        std::cout << "\n";
        std::cout << "Test1:\n";
        testandcout(test1->usefullClass1isA(Class(Test1)));
        testandcout(test1->usefullClass1isA(Class(Test2)));
        testandcout(test1->usefullClass1isA(Class(Test3)));
        std::cout << "\n";
        testandcout(test1->usefullClass2isA(Class(Test1)));
        testandcout(test1->usefullClass2isA(Class(Test2)));
        testandcout(test1->usefullClass2isA(Class(Test3)));
        std::cout << "\n";
        testandcout(test1->usefullClass3isA(Class(Test1)));
        testandcout(test1->usefullClass3isA(Class(Test2)));
        testandcout(test1->usefullClass3isA(Class(Test3)));
    }

    bool Test3::usefullClassesIsATest(Test2* test2)
    {
        std::cout << "\n";
        std::cout << "Test2:\n";
        testandcout(test2->usefullClass1isA(Class(Test1)));
        testandcout(test2->usefullClass1isA(Class(Test2)));
        testandcout(test2->usefullClass1isA(Class(Test3)));
        std::cout << "\n";
        testandcout(test2->usefullClass2isA(Class(Test1)));
        testandcout(test2->usefullClass2isA(Class(Test2)));
        testandcout(test2->usefullClass2isA(Class(Test3)));
        std::cout << "\n";
        testandcout(test2->usefullClass3isA(Class(Test1)));
        testandcout(test2->usefullClass3isA(Class(Test2)));
        testandcout(test2->usefullClass3isA(Class(Test3)));
    }
}
