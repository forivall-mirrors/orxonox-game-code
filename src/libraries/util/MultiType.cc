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
    @brief Implementation of the MultiType.
*/

#include "MultiType.h"
#include "MultiTypeValue.h"

namespace orxonox
{
    const MultiType MultiType::Null;

    /**
        @brief Converts the current value of the MultiType to a new type.
        @param type The type
    */
    bool MultiType::convert(Type::Enum type)
    {
        switch (type)
        {
            case Type::Null:
                this->reset(); return true;
            case Type::Char:
                return this->convert<char>(); break;
            case Type::UnsignedChar:
                return this->convert<unsigned char>(); break;
            case Type::Short:
                return this->convert<short>(); break;
            case Type::UnsignedShort:
                return this->convert<unsigned short>(); break;
            case Type::Int:
                return this->convert<int>(); break;
            case Type::UnsignedInt:
                return this->convert<unsigned int>(); break;
            case Type::Long:
                return this->convert<long>(); break;
            case Type::UnsignedLong:
                return this->convert<unsigned long>(); break;
            case Type::LongLong:
                return this->convert<long long>(); break;
            case Type::UnsignedLongLong:
                return this->convert<unsigned long long>(); break;
            case Type::Float:
                return this->convert<float>(); break;
            case Type::Double:
                return this->convert<double>(); break;
            case Type::LongDouble:
                return this->convert<long double>(); break;
            case Type::Bool:
                return this->convert<bool>(); break;
            case Type::VoidPointer:
                return this->convert<void*>(); break;
            case Type::String:
                return this->convert<std::string>(); break;
            case Type::Vector2:
                return this->convert<orxonox::Vector2>(); break;
            case Type::Vector3:
                return this->convert<orxonox::Vector3>(); break;
            case Type::Vector4:
                return this->convert<orxonox::Vector4>(); break;
            case Type::ColourValue:
                return this->convert<orxonox::ColourValue>(); break;
            case Type::Quaternion:
                return this->convert<orxonox::Quaternion>(); break;
            case Type::Radian:
                return this->convert<orxonox::Radian>(); break;
            case Type::Degree:
                return this->convert<orxonox::Degree>(); break;
            default:
                this->reset(); return false; break;
        };
    }

    /**
        @brief Returns the name of the current type.
        @return The name
    */
    std::string MultiType::getTypename() const
    {
        Type::Enum type = (this->value_) ? this->value_->type_ : Type::Null;

        switch (type)
        {
            case Type::Char:
                return "char"; break;
            case Type::UnsignedChar:
                return "unsigned char"; break;
            case Type::Short:
                return "short"; break;
            case Type::UnsignedShort:
                return "unsigned short"; break;
            case Type::Int:
                return "int"; break;
            case Type::UnsignedInt:
                return "unsigned int"; break;
            case Type::Long:
                return "long"; break;
            case Type::UnsignedLong:
                return "unsigned long"; break;
            case Type::LongLong:
                return "long long"; break;
            case Type::UnsignedLongLong:
                return "unsigned long long"; break;
            case Type::Float:
                return "float"; break;
            case Type::Double:
                return "double"; break;
            case Type::LongDouble:
                return "long double"; break;
            case Type::Bool:
                return "bool"; break;
            case Type::VoidPointer:
                return "void*"; break;
            case Type::String:
                return "std::string"; break;
            case Type::Vector2:
                return "orxonox::Vector2"; break;
            case Type::Vector3:
                return "orxonox::Vector3"; break;
            case Type::Vector4:
                return "orxonox::Vector4"; break;
            case Type::ColourValue:
                return "orxonox::ColourValue"; break;
            case Type::Quaternion:
                return "orxonox::Quaternion"; break;
            case Type::Radian:
                return "orxonox::Radian"; break;
            case Type::Degree:
                return "orxonox::Degree"; break;
            default:
                return "unknown"; break;
        };
    }

    template <> void MultiType::createNewValueContainer(const char& value)                 { this->value_ = new MT_Value<char>                (value, Type::Char            ); }
    template <> void MultiType::createNewValueContainer(const unsigned char& value)        { this->value_ = new MT_Value<unsigned char>       (value, Type::UnsignedChar    ); }
    template <> void MultiType::createNewValueContainer(const short& value)                { this->value_ = new MT_Value<short>               (value, Type::Short           ); }
    template <> void MultiType::createNewValueContainer(const unsigned short& value)       { this->value_ = new MT_Value<unsigned short>      (value, Type::UnsignedShort   ); }
    template <> void MultiType::createNewValueContainer(const int& value)                  { this->value_ = new MT_Value<int>                 (value, Type::Int             ); }
    template <> void MultiType::createNewValueContainer(const unsigned int& value)         { this->value_ = new MT_Value<unsigned int>        (value, Type::UnsignedInt     ); }
    template <> void MultiType::createNewValueContainer(const long& value)                 { this->value_ = new MT_Value<long>                (value, Type::Long            ); }
    template <> void MultiType::createNewValueContainer(const unsigned long& value)        { this->value_ = new MT_Value<unsigned long>       (value, Type::UnsignedLong    ); }
    template <> void MultiType::createNewValueContainer(const long long& value)            { this->value_ = new MT_Value<long long>           (value, Type::LongLong        ); }
    template <> void MultiType::createNewValueContainer(const unsigned long long& value)   { this->value_ = new MT_Value<unsigned long long>  (value, Type::UnsignedLongLong); }
    template <> void MultiType::createNewValueContainer(const float& value)                { this->value_ = new MT_Value<float>               (value, Type::Float           ); }
    template <> void MultiType::createNewValueContainer(const double& value)               { this->value_ = new MT_Value<double>              (value, Type::Double          ); }
    template <> void MultiType::createNewValueContainer(const long double& value)          { this->value_ = new MT_Value<long double>         (value, Type::LongDouble      ); }
    template <> void MultiType::createNewValueContainer(const bool& value)                 { this->value_ = new MT_Value<bool>                (value, Type::Bool            ); }
    template <> void MultiType::createNewValueContainer(      void* const& value)          { this->value_ = new MT_Value<void*>               (value, Type::VoidPointer     ); }
    template <> void MultiType::createNewValueContainer(const std::string& value)          { this->value_ = new MT_Value<std::string>         (value, Type::String          ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Vector2& value)     { this->value_ = new MT_Value<orxonox::Vector2>    (value, Type::Vector2         ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Vector3& value)     { this->value_ = new MT_Value<orxonox::Vector3>    (value, Type::Vector3         ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Vector4& value)     { this->value_ = new MT_Value<orxonox::Vector4>    (value, Type::Vector4         ); }
    template <> void MultiType::createNewValueContainer(const orxonox::ColourValue& value) { this->value_ = new MT_Value<orxonox::ColourValue>(value, Type::ColourValue     ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Quaternion& value)  { this->value_ = new MT_Value<orxonox::Quaternion> (value, Type::Quaternion      ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Radian& value)      { this->value_ = new MT_Value<orxonox::Radian>     (value, Type::Radian          ); }
    template <> void MultiType::createNewValueContainer(const orxonox::Degree& value)      { this->value_ = new MT_Value<orxonox::Degree>     (value, Type::Degree          ); }
}
