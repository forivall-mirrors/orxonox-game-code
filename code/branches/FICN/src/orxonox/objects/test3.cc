#include "test1.h"
#include "test2.h"
#include "test3.h"

namespace orxonox
{
    CreateFactory(Test3);

    Test3::Test3()
    {
        RegisterObject(Test3);

        this->setConfigValues();
    }

    void Test3::setConfigValues()
    {
        SetConfigValue(value_int_, 1);
        SetConfigValue(value_uint_, 1);
        SetConfigValue(value_char_, 1);
        SetConfigValue(value_uchar_, 1);
        SetConfigValue(value_float_, 1);
        SetConfigValue(value_double_, 1);
        SetConfigValue(value_bool_, 1);
        SetConfigValue(value_string_, "This is a test");
        SetConfigValue(value_constchar_, "This is another test");
        SetConfigValue(value_vector2_, Vector2(101, 202));
        SetConfigValue(value_vector3_, Vector3(13, 26, 39));
        SetConfigValue(value_colourvalue_, ColourValue(1.0, 0.5, 0.25, 0.887));
    }

    Test3::~Test3()
    {
    }

    void Test3::configOutput()
    {
        std::cout << this->value_int_ << std::endl;
        std::cout << this->value_uint_ << std::endl;
        std::cout << (int)this->value_char_ << std::endl;
        std::cout << (int)this->value_uchar_ << std::endl;
        std::cout << this->value_float_ << std::endl;
        std::cout << this->value_double_ << std::endl;
        std::cout << this->value_bool_ << std::endl;
        std::cout << this->value_string_ << std::endl;
        std::cout << this->value_constchar_ << std::endl;
        std::cout << this->value_vector2_ << std::endl;
        std::cout << this->value_vector3_ << std::endl;
        std::cout << this->value_colourvalue_ << std::endl;
    }

    #define testandcout(code) \
      std::cout << #code << " " << code << "\n"

    void Test3::usefullClassesIsATest(Test1* test1)
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

    void Test3::usefullClassesIsATest(Test2* test2)
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
