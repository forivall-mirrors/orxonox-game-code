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
    bool MultiType::convert(MT_Type type)
    {
        switch (type)
        {
            case MT_null:
                this->reset(); return true;
            case MT_char:
                return this->convert<char>(); break;
            case MT_uchar:
                return this->convert<unsigned char>(); break;
            case MT_short:
                return this->convert<short>(); break;
            case MT_ushort:
                return this->convert<unsigned short>(); break;
            case MT_int:
                return this->convert<int>(); break;
            case MT_uint:
                return this->convert<unsigned int>(); break;
            case MT_long:
                return this->convert<long>(); break;
            case MT_ulong:
                return this->convert<unsigned long>(); break;
            case MT_longlong:
                return this->convert<long long>(); break;
            case MT_ulonglong:
                return this->convert<unsigned long long>(); break;
            case MT_float:
                return this->convert<float>(); break;
            case MT_double:
                return this->convert<double>(); break;
            case MT_longdouble:
                return this->convert<long double>(); break;
            case MT_bool:
                return this->convert<bool>(); break;
            case MT_void:
                return this->convert<void*>(); break;
            case MT_string:
                return this->convert<std::string>(); break;
            case MT_vector2:
                return this->convert<orxonox::Vector2>(); break;
            case MT_vector3:
                return this->convert<orxonox::Vector3>(); break;
            case MT_vector4:
                return this->convert<orxonox::Vector4>(); break;
            case MT_colourvalue:
                return this->convert<orxonox::ColourValue>(); break;
            case MT_quaternion:
                return this->convert<orxonox::Quaternion>(); break;
            case MT_radian:
                return this->convert<orxonox::Radian>(); break;
            case MT_degree:
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
        MT_Type type = (this->value_) ? this->value_->type_ : MT_null;

        switch (type)
        {
            case MT_char:
                return "char"; break;
            case MT_uchar:
                return "unsigned char"; break;
            case MT_short:
                return "short"; break;
            case MT_ushort:
                return "unsigned short"; break;
            case MT_int:
                return "int"; break;
            case MT_uint:
                return "unsigned int"; break;
            case MT_long:
                return "long"; break;
            case MT_ulong:
                return "unsigned long"; break;
            case MT_longlong:
                return "long long"; break;
            case MT_ulonglong:
                return "unsigned long long"; break;
            case MT_float:
                return "float"; break;
            case MT_double:
                return "double"; break;
            case MT_longdouble:
                return "long double"; break;
            case MT_bool:
                return "bool"; break;
            case MT_void:
                return "void*"; break;
            case MT_string:
                return "std::string"; break;
            case MT_vector2:
                return "orxonox::Vector2"; break;
            case MT_vector3:
                return "orxonox::Vector3"; break;
            case MT_vector4:
                return "orxonox::Vector4"; break;
            case MT_colourvalue:
                return "orxonox::ColourValue"; break;
            case MT_quaternion:
                return "orxonox::Quaternion"; break;
            case MT_radian:
                return "orxonox::Radian"; break;
            case MT_degree:
                return "orxonox::Degree"; break;
            default:
                return "unknown"; break;
        };
    }

    MultiType::operator char()                 const { return (this->value_) ? ((this->value_->type_ == MT_char       ) ? (static_cast<MT_Value<char>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned char()        const { return (this->value_) ? ((this->value_->type_ == MT_uchar      ) ? (static_cast<MT_Value<unsigned char>       *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator short()                const { return (this->value_) ? ((this->value_->type_ == MT_short      ) ? (static_cast<MT_Value<short>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned short()       const { return (this->value_) ? ((this->value_->type_ == MT_ushort     ) ? (static_cast<MT_Value<unsigned short>      *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator int()                  const { return (this->value_) ? ((this->value_->type_ == MT_int        ) ? (static_cast<MT_Value<int>                 *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned int()         const { return (this->value_) ? ((this->value_->type_ == MT_uint       ) ? (static_cast<MT_Value<unsigned int>        *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long()                 const { return (this->value_) ? ((this->value_->type_ == MT_long       ) ? (static_cast<MT_Value<long>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned long()        const { return (this->value_) ? ((this->value_->type_ == MT_ulong      ) ? (static_cast<MT_Value<unsigned long>       *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long long()            const { return (this->value_) ? ((this->value_->type_ == MT_longlong   ) ? (static_cast<MT_Value<long long>           *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator unsigned long long()   const { return (this->value_) ? ((this->value_->type_ == MT_ulonglong  ) ? (static_cast<MT_Value<unsigned long long>  *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator float()                const { return (this->value_) ? ((this->value_->type_ == MT_float      ) ? (static_cast<MT_Value<float>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator double()               const { return (this->value_) ? ((this->value_->type_ == MT_double     ) ? (static_cast<MT_Value<double>              *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator long double()          const { return (this->value_) ? ((this->value_->type_ == MT_longdouble ) ? (static_cast<MT_Value<long double>         *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator bool()                 const { return (this->value_) ? ((this->value_->type_ == MT_bool       ) ? (static_cast<MT_Value<bool>                *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator void*()                const { return (this->value_) ? ((this->value_->type_ == MT_void       ) ? (static_cast<MT_Value<void*>               *>(this->value_))->value_ : (*this->value_)) : 0;                      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator std::string()          const { return (this->value_) ? ((this->value_->type_ == MT_string     ) ? (static_cast<MT_Value<std::string>         *>(this->value_))->value_ : (*this->value_)) : NilValue<std::string>();          } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector2()     const { return (this->value_) ? ((this->value_->type_ == MT_vector2    ) ? (static_cast<MT_Value<orxonox::Vector2>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector2>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector3()     const { return (this->value_) ? ((this->value_->type_ == MT_vector3    ) ? (static_cast<MT_Value<orxonox::Vector3>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector3>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Vector4()     const { return (this->value_) ? ((this->value_->type_ == MT_vector4    ) ? (static_cast<MT_Value<orxonox::Vector4>    *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Vector4>();     } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::ColourValue() const { return (this->value_) ? ((this->value_->type_ == MT_colourvalue) ? (static_cast<MT_Value<orxonox::ColourValue>*>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::ColourValue>(); } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Quaternion()  const { return (this->value_) ? ((this->value_->type_ == MT_quaternion ) ? (static_cast<MT_Value<orxonox::Quaternion> *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Quaternion>();  } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Radian()      const { return (this->value_) ? ((this->value_->type_ == MT_radian     ) ? (static_cast<MT_Value<orxonox::Radian>     *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Radian>();      } /** @brief Returns the current value, converted to the requested type. */
    MultiType::operator orxonox::Degree()      const { return (this->value_) ? ((this->value_->type_ == MT_degree     ) ? (static_cast<MT_Value<orxonox::Degree>     *>(this->value_))->value_ : (*this->value_)) : NilValue<orxonox::Degree>();      } /** @brief Returns the current value, converted to the requested type. */

    template <> void MultiType::createNewValueContainer(const char& value)                 { this->value_ = new MT_Value<char>                (value, MT_char       ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned char& value)        { this->value_ = new MT_Value<unsigned char>       (value, MT_uchar      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const short& value)                { this->value_ = new MT_Value<short>               (value, MT_short      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned short& value)       { this->value_ = new MT_Value<unsigned short>      (value, MT_ushort     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const int& value)                  { this->value_ = new MT_Value<int>                 (value, MT_int        ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned int& value)         { this->value_ = new MT_Value<unsigned int>        (value, MT_uint       ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long& value)                 { this->value_ = new MT_Value<long>                (value, MT_long       ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned long& value)        { this->value_ = new MT_Value<unsigned long>       (value, MT_ulong      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long long& value)            { this->value_ = new MT_Value<long long>           (value, MT_longlong   ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const unsigned long long& value)   { this->value_ = new MT_Value<unsigned long long>  (value, MT_ulonglong  ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const float& value)                { this->value_ = new MT_Value<float>               (value, MT_float      ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const double& value)               { this->value_ = new MT_Value<double>              (value, MT_double     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const long double& value)          { this->value_ = new MT_Value<long double>         (value, MT_longdouble ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const bool& value)                 { this->value_ = new MT_Value<bool>                (value, MT_bool       ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(      void* const& value)          { this->value_ = new MT_Value<void*>               (value, MT_void       ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const std::string& value)          { this->value_ = new MT_Value<std::string>         (value, MT_string     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector2& value)     { this->value_ = new MT_Value<orxonox::Vector2>    (value, MT_vector2    ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector3& value)     { this->value_ = new MT_Value<orxonox::Vector3>    (value, MT_vector3    ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Vector4& value)     { this->value_ = new MT_Value<orxonox::Vector4>    (value, MT_vector4    ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::ColourValue& value) { this->value_ = new MT_Value<orxonox::ColourValue>(value, MT_colourvalue); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Quaternion& value)  { this->value_ = new MT_Value<orxonox::Quaternion> (value, MT_quaternion ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Radian& value)      { this->value_ = new MT_Value<orxonox::Radian>     (value, MT_radian     ); } /** @brief Creates a new value container for the given type. */
    template <> void MultiType::createNewValueContainer(const orxonox::Degree& value)      { this->value_ = new MT_Value<orxonox::Degree>     (value, MT_degree     ); } /** @brief Creates a new value container for the given type. */
}
