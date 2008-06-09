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
#include "core/ConfigValueIncludes.h"

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
        SetConfigValueVector(vector_int_, std::vector<int>(1, 13));
        SetConfigValueVector(vector_string_, std::vector<std::string>(3, "nothing"));
        SetConfigValueVector(vector_vector3_, std::vector<Vector3>(1, Vector3(3, 2, 1)));
    }

    Test3::~Test3()
    {
    }

    void Test3::configOutput()
    {
        std::cout << "int:             " << this->value_int_ << std::endl;
        std::cout << "uint:            " << this->value_uint_ << std::endl;
        std::cout << "char:            " << (int)this->value_char_ << std::endl;
        std::cout << "uchar:           " << (int)this->value_uchar_ << std::endl;
        std::cout << "float:           " << this->value_float_ << std::endl;
        std::cout << "double:          " << this->value_double_ << std::endl;
        std::cout << "bool:            " << this->value_bool_ << std::endl;
        std::cout << "string:         >" << this->value_string_ << "<" << std::endl;
        std::cout << "constchar:      >" << this->value_constchar_ << "<" << std::endl;
        std::cout << "vector2:         " << this->value_vector2_ << std::endl;
        std::cout << "vector3:         " << this->value_vector3_ << std::endl;
        std::cout << "colourvalue:     " << this->value_colourvalue_ << std::endl;
        std::cout << std::endl;
        for (unsigned int i = 0; i < this->vector_int_.size(); i++)
        std::cout << "vector<int>:     " << i << ": " << this->vector_int_[i] << std::endl;
        for (unsigned int i = 0; i < this->vector_string_.size(); i++)
        std::cout << "vector<string>:  " << i << ":>" << this->vector_string_[i] << "<" << std::endl;
        for (unsigned int i = 0; i < this->vector_vector3_.size(); i++)
        std::cout << "vector<vector3>: " << i << ": " << this->vector_vector3_[i] << std::endl;

        ModifyConfigValue(value_int_, tset, "100");
        std::cout << std::endl;
        std::cout << "int:             " << this->value_int_ << std::endl;

        ModifyConfigValue(value_int_, update);
        std::cout << std::endl;
        std::cout << "int:             " << this->value_int_ << std::endl;
    }

    #define testandcout(code) \
      std::cout << #code << " " << code << std::endl

    void Test3::usefullClassesIsATest(Test1* test1)
    {
        std::cout << std::endl;
        std::cout << "Test1:" << std::endl;
        testandcout(test1->usefullClass1isA(Class(Test1)));
        testandcout(test1->usefullClass1isA(Class(Test2)));
        testandcout(test1->usefullClass1isA(Class(Test3)));
        std::cout << std::endl;
        testandcout(test1->usefullClass2isA(Class(Test1)));
        testandcout(test1->usefullClass2isA(Class(Test2)));
        testandcout(test1->usefullClass2isA(Class(Test3)));
        std::cout << std::endl;
        testandcout(test1->usefullClass3isA(Class(Test1)));
        testandcout(test1->usefullClass3isA(Class(Test2)));
        testandcout(test1->usefullClass3isA(Class(Test3)));
    }

    void Test3::usefullClassesIsATest(Test2* test2)
    {
        std::cout << std::endl;
        std::cout << "Test2:" << std::endl;
        testandcout(test2->usefullClass1isA(Class(Test1)));
        testandcout(test2->usefullClass1isA(Class(Test2)));
        testandcout(test2->usefullClass1isA(Class(Test3)));
        std::cout << std::endl;
        testandcout(test2->usefullClass2isA(Class(Test1)));
        testandcout(test2->usefullClass2isA(Class(Test2)));
        testandcout(test2->usefullClass2isA(Class(Test3)));
        std::cout << std::endl;
        testandcout(test2->usefullClass3isA(Class(Test1)));
        testandcout(test2->usefullClass3isA(Class(Test2)));
        testandcout(test2->usefullClass3isA(Class(Test3)));
    }
}
