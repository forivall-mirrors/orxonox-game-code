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
    /**
        @brief Converts the current value of the MultiType to a new type.
        @param type The type
    */
    bool MultiType::convert(MT_Type::Value type)
    {
        switch (type)
        {
            case MT_Type::Null:
                this->reset(); return true;
            case MT_Type::Char:
                return this->convert<char>(); break;
            case MT_Type::UnsignedChar:
                return this->convert<unsigned char>(); break;
            case MT_Type::Short:
                return this->convert<short>(); break;
            case MT_Type::UnsignedShort:
                return this->convert<unsigned short>(); break;
            case MT_Type::Int:
                return this->convert<int>(); break;
            case MT_Type::UnsignedInt:
                return this->convert<unsigned int>(); break;
            case MT_Type::Long:
                return this->convert<long>(); break;
            case MT_Type::UnsignedLong:
                return this->convert<unsigned long>(); break;
            case MT_Type::LongLong:
                return this->convert<long long>(); break;
            case MT_Type::UnsignedLongLong:
                return this->convert<unsigned long long>(); break;
            case MT_Type::Float:
                return this->convert<float>(); break;
            case MT_Type::Double:
                return this->convert<double>(); break;
            case MT_Type::LongDouble:
                return this->convert<long double>(); break;
            case MT_Type::Bool:
                return this->convert<bool>(); break;
            case MT_Type::VoidPointer:
                return this->convert<void*>(); break;
            case MT_Type::String:
                return this->convert<std::string>(); break;
            case MT_Type::Vector2:
                return this->convert<orxonox::Vector2>(); break;
            case MT_Type::Vector3:
                return this->convert<orxonox::Vector3>(); break;
            case MT_Type::Vector4:
                return this->convert<orxonox::Vector4>(); break;
            case MT_Type::ColourValue:
                return this->convert<orxonox::ColourValue>(); break;
            case MT_Type::Quaternion:
                return this->convert<orxonox::Quaternion>(); break;
            case MT_Type::Radian:
                return this->convert<orxonox::Radian>(); break;
            case MT_Type::Degree:
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
        MT_Type::Value type = (this->value_) ? this->value_->type_ : MT_Type::Null;

        switch (type)
        {
            case MT_Type::Char:
                return "char"; break;
            case MT_Type::UnsignedChar:
                return "unsigned char"; break;
            case MT_Type::Short:
                return "short"; break;
            case MT_Type::UnsignedShort:
                return "unsigned short"; break;
            case MT_Type::Int:
                return "int"; break;
            case MT_Type::UnsignedInt:
                return "unsigned int"; break;
            case MT_Type::Long:
                return "long"; break;
            case MT_Type::UnsignedLong:
                return "unsigned long"; break;
            case MT_Type::LongLong:
                return "long long"; break;
            case MT_Type::UnsignedLongLong:
                return "unsigned long long"; break;
            case MT_Type::Float:
                return "float"; break;
            case MT_Type::Double:
                return "double"; break;
            case MT_Type::LongDouble:
                return "long double"; break;
            case MT_Type::Bool:
                return "bool"; break;
            case MT_Type::VoidPointer:
                return "void*"; break;
            case MT_Type::String:
                return "std::string"; break;
            case MT_Type::Vector2:
                return "orxonox::Vector2"; break;
            case MT_Type::Vector3:
                return "orxonox::Vector3"; break;
            case MT_Type::Vector4:
                return "orxonox::Vector4"; break;
            case MT_Type::ColourValue:
                return "orxonox::ColourValue"; break;
            case MT_Type::Quaternion:
                return "orxonox::Quaternion"; break;
            case MT_Type::Radian:
                return "orxonox::Radian"; break;
            case MT_Type::Degree:
                return "orxonox::Degree"; break;
            default:
                return "unknown"; break;
        };
    }

    MultiType::operator char()                 const { return (this->value_) ? ((this->value_->type_ == MT_Type::Char            ) ? (static_cast<MT_Value<char>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned char()        const { return (this->value_) ? ((this->value_->type_ == MT_Type::UnsignedChar    ) ? (static_cast<MT_Value<unsigned char>       *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator short()                const { return (this->value_) ? ((this->value_->type_ == MT_Type::Short           ) ? (static_cast<MT_Value<short>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned short()       const { return (this->value_) ? ((this->value_->type_ == MT_Type::UnsignedShort   ) ? (static_cast<MT_Value<unsigned short>      *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator int()                  const { return (this->value_) ? ((this->value_->type_ == MT_Type::Int             ) ? (static_cast<MT_Value<int>                 *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned int()         const { return (this->value_) ? ((this->value_->type_ == MT_Type::UnsignedInt     ) ? (static_cast<MT_Value<unsigned int>        *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long()                 const { return (this->value_) ? ((this->value_->type_ == MT_Type::Long            ) ? (static_cast<MT_Value<long>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned long()        const { return (this->value_) ? ((this->value_->type_ == MT_Type::UnsignedLong    ) ? (static_cast<MT_Value<unsigned long>       *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long long()            const { return (this->value_) ? ((this->value_->type_ == MT_Type::LongLong        ) ? (static_cast<MT_Value<long long>           *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned long long()   const { return (this->value_) ? ((this->value_->type_ == MT_Type::UnsignedLongLong) ? (static_cast<MT_Value<unsigned long long>  *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator float()                const { return (this->value_) ? ((this->value_->type_ == MT_Type::Float           ) ? (static_cast<MT_Value<float>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator double()               const { return (this->value_) ? ((this->value_->type_ == MT_Type::Double          ) ? (static_cast<MT_Value<double>              *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long double()          const { return (this->value_) ? ((this->value_->type_ == MT_Type::LongDouble      ) ? (static_cast<MT_Value<long double>         *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator bool()                 const { return (this->value_) ? ((this->value_->type_ == MT_Type::Bool            ) ? (static_cast<MT_Value<bool>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator void*()                const { return (this->value_) ? ((this->value_->type_ == MT_Type::VoidPointer     ) ? (static_cast<MT_Value<void*>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator std::string()          const { return (this->value_) ? ((this->value_->type_ == MT_Type::String          ) ? (static_cast<MT_Value<std::string>         *>(this->value_))->value_ : (*this->value_)) : NilValue<std::string>();          } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector2()     const { return (this->value_) ? ((this->value_->type_ == MT_Type::Vector2         ) ? (static_cast<MT_Value<orxonox::Vector2>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector2>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector3()     const { return (this->value_) ? ((this->value_->type_ == MT_Type::Vector3         ) ? (static_cast<MT_Value<orxonox::Vector3>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector3>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector4()     const { return (this->value_) ? ((this->value_->type_ == MT_Type::Vector4         ) ? (static_cast<MT_Value<orxonox::Vector4>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector4>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::ColourValue() const { return (this->value_) ? ((this->value_->type_ == MT_Type::ColourValue     ) ? (static_cast<MT_Value<orxonox::ColourValue>*>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::ColourValue>(); } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Quaternion()  const { return (this->value_) ? ((this->value_->type_ == MT_Type::Quaternion      ) ? (static_cast<MT_Value<orxonox::Quaternion> *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Quaternion>();  } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Radian()      const { return (this->value_) ? ((this->value_->type_ == MT_Type::Radian          ) ? (static_cast<MT_Value<orxonox::Radian>     *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Radian>();      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Degree()      const { return (this->value_) ? ((this->value_->type_ == MT_Type::Degree          ) ? (static_cast<MT_Value<orxonox::Degree>     *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Degree>();      } /** @brief Returns the current value, converted to the requested type. */

    template <> void MultiType::createNewValueContainer(const char& value)                 { this->value_ = new MT_Value<char>                (value, MT_Type::Char            ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned char& value)        { this->value_ = new MT_Value<unsigned char>       (value, MT_Type::UnsignedChar    ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const short& value)                { this->value_ = new MT_Value<short>               (value, MT_Type::Short           ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned short& value)       { this->value_ = new MT_Value<unsigned short>      (value, MT_Type::UnsignedShort   ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const int& value)                  { this->value_ = new MT_Value<int>                 (value, MT_Type::Int             ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned int& value)         { this->value_ = new MT_Value<unsigned int>        (value, MT_Type::UnsignedInt     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long& value)                 { this->value_ = new MT_Value<long>                (value, MT_Type::Long            ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned long& value)        { this->value_ = new MT_Value<unsigned long>       (value, MT_Type::UnsignedLong    ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long long& value)            { this->value_ = new MT_Value<long long>           (value, MT_Type::LongLong        ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned long long& value)   { this->value_ = new MT_Value<unsigned long long>  (value, MT_Type::UnsignedLongLong); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const float& value)                { this->value_ = new MT_Value<float>               (value, MT_Type::Float           ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const double& value)               { this->value_ = new MT_Value<double>              (value, MT_Type::Double          ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long double& value)          { this->value_ = new MT_Value<long double>         (value, MT_Type::LongDouble      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const bool& value)                 { this->value_ = new MT_Value<bool>                (value, MT_Type::Bool            ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(      void* const& value)          { this->value_ = new MT_Value<void*>               (value, MT_Type::VoidPointer     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const std::string& value)          { this->value_ = new MT_Value<std::string>         (value, MT_Type::String          ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector2& value)     { this->value_ = new MT_Value<orxonox::Vector2>    (value, MT_Type::Vector2         ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector3& value)     { this->value_ = new MT_Value<orxonox::Vector3>    (value, MT_Type::Vector3         ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector4& value)     { this->value_ = new MT_Value<orxonox::Vector4>    (value, MT_Type::Vector4         ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::ColourValue& value) { this->value_ = new MT_Value<orxonox::ColourValue>(value, MT_Type::ColourValue     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Quaternion& value)  { this->value_ = new MT_Value<orxonox::Quaternion> (value, MT_Type::Quaternion      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Radian& value)      { this->value_ = new MT_Value<orxonox::Radian>     (value, MT_Type::Radian          ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Degree& value)      { this->value_ = new MT_Value<orxonox::Degree>     (value, MT_Type::Degree          ); } /** @brief Creates a new value container for the given type. */
}
