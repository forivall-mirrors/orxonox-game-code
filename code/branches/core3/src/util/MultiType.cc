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

#include "MultiType.h"
#include "MultiTypeValue.h"

void MultiType::setType(MT_Type type)
{
    switch (type)
    {
        case MT_char:
            this->setType<char>(); break;
        case MT_uchar:
            this->setType<unsigned char>(); break;
        case MT_short:
            this->setType<short>(); break;
        case MT_ushort:
            this->setType<unsigned short>(); break;
        case MT_int:
            this->setType<int>(); break;
        case MT_uint:
            this->setType<unsigned int>(); break;
        case MT_long:
            this->setType<long>(); break;
        case MT_ulong:
            this->setType<unsigned long>(); break;
        case MT_longlong:
            this->setType<long long>(); break;
        case MT_ulonglong:
            this->setType<unsigned long long>(); break;
        case MT_float:
            this->setType<float>(); break;
        case MT_double:
            this->setType<double>(); break;
        case MT_longdouble:
            this->setType<long double>(); break;
        case MT_bool:
            this->setType<bool>(); break;
        case MT_void:
            this->setType<void*>(); break;
        case MT_string:
            this->setType<std::string>(); break;
        case MT_vector2:
            this->setType<orxonox::Vector2>(); break;
        case MT_vector3:
            this->setType<orxonox::Vector3>(); break;
        case MT_vector4:
            this->setType<orxonox::Vector4>(); break;
        case MT_colourvalue:
            this->setType<orxonox::ColourValue>(); break;
        case MT_quaternion:
            this->setType<orxonox::Quaternion>(); break;
        case MT_radian:
            this->setType<orxonox::Radian>(); break;
        case MT_degree:
            this->setType<orxonox::Degree>(); break;
        default:
            this->reset(); break;
    };
}

std::string MultiType::getTypename() const
{
    switch (this->type_)
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

MultiType::operator char()                 const { return (this->type_ == MT_char       ) ? ((MT_Value<char>                *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator unsigned char()        const { return (this->type_ == MT_uchar      ) ? ((MT_Value<unsigned char>       *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator short()                const { return (this->type_ == MT_short      ) ? ((MT_Value<short>               *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator unsigned short()       const { return (this->type_ == MT_ushort     ) ? ((MT_Value<unsigned short>      *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator int()                  const { return (this->type_ == MT_int        ) ? ((MT_Value<int>                 *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator unsigned int()         const { return (this->type_ == MT_uint       ) ? ((MT_Value<unsigned int>        *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator long()                 const { return (this->type_ == MT_long       ) ? ((MT_Value<long>                *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator unsigned long()        const { return (this->type_ == MT_ulong      ) ? ((MT_Value<unsigned long>       *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator long long()            const { return (this->type_ == MT_longlong   ) ? ((MT_Value<long long>           *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator unsigned long long()   const { return (this->type_ == MT_ulonglong  ) ? ((MT_Value<unsigned long long>  *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator float()                const { return (this->type_ == MT_float      ) ? ((MT_Value<float>               *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator double()               const { return (this->type_ == MT_double     ) ? ((MT_Value<double>              *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator long double()          const { return (this->type_ == MT_longdouble ) ? ((MT_Value<long double>         *)this->value_)->value_ : (this->value_) ? (*this->value_) : 0;                      }
MultiType::operator bool()                 const { return (this->type_ == MT_bool       ) ? ((MT_Value<bool>                *)this->value_)->value_ : (this->value_) ? (*this->value_) : false;                  }
MultiType::operator void*()                const { return (this->type_ == MT_void       ) ? ((MT_Value<void*>               *)this->value_)->value_ : (this->value_) ? (*this->value_) : (void*)0;               }
MultiType::operator std::string()          const { return (this->type_ == MT_string     ) ? ((MT_Value<std::string>         *)this->value_)->value_ : (this->value_) ? (*this->value_) : std::string();          }
MultiType::operator orxonox::Vector2()     const { return (this->type_ == MT_vector2    ) ? ((MT_Value<orxonox::Vector2>    *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Vector2();     }
MultiType::operator orxonox::Vector3()     const { return (this->type_ == MT_vector3    ) ? ((MT_Value<orxonox::Vector3>    *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Vector3();     }
MultiType::operator orxonox::Vector4()     const { return (this->type_ == MT_vector4    ) ? ((MT_Value<orxonox::Vector4>    *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Vector4();     }
MultiType::operator orxonox::ColourValue() const { return (this->type_ == MT_colourvalue) ? ((MT_Value<orxonox::ColourValue>*)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::ColourValue(); }
MultiType::operator orxonox::Quaternion()  const { return (this->type_ == MT_quaternion ) ? ((MT_Value<orxonox::Quaternion> *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Quaternion();  }
MultiType::operator orxonox::Radian()      const { return (this->type_ == MT_radian     ) ? ((MT_Value<orxonox::Radian>     *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Radian();      }
MultiType::operator orxonox::Degree()      const { return (this->type_ == MT_degree     ) ? ((MT_Value<orxonox::Degree>     *)this->value_)->value_ : (this->value_) ? (*this->value_) : orxonox::Degree();      }

template <> void MultiType::createNewValueContainer(const char& value)                 { this->value_ = new MT_Value<char>                (value); }
template <> void MultiType::createNewValueContainer(const unsigned char& value)        { this->value_ = new MT_Value<unsigned char>       (value); }
template <> void MultiType::createNewValueContainer(const short& value)                { this->value_ = new MT_Value<short>               (value); }
template <> void MultiType::createNewValueContainer(const unsigned short& value)       { this->value_ = new MT_Value<unsigned short>      (value); }
template <> void MultiType::createNewValueContainer(const int& value)                  { this->value_ = new MT_Value<int>                 (value); }
template <> void MultiType::createNewValueContainer(const unsigned int& value)         { this->value_ = new MT_Value<unsigned int>        (value); }
template <> void MultiType::createNewValueContainer(const long& value)                 { this->value_ = new MT_Value<long>                (value); }
template <> void MultiType::createNewValueContainer(const unsigned long& value)        { this->value_ = new MT_Value<unsigned long>       (value); }
template <> void MultiType::createNewValueContainer(const long long& value)            { this->value_ = new MT_Value<long long>           (value); }
template <> void MultiType::createNewValueContainer(const unsigned long long& value)   { this->value_ = new MT_Value<unsigned long long>  (value); }
template <> void MultiType::createNewValueContainer(const float& value)                { this->value_ = new MT_Value<float>               (value); }
template <> void MultiType::createNewValueContainer(const double& value)               { this->value_ = new MT_Value<double>              (value); }
template <> void MultiType::createNewValueContainer(const long double& value)          { this->value_ = new MT_Value<long double>         (value); }
template <> void MultiType::createNewValueContainer(const bool& value)                 { this->value_ = new MT_Value<bool>                (value); }
template <> void MultiType::createNewValueContainer(      void* const& value)          { this->value_ = new MT_Value<void*>               (value); }
template <> void MultiType::createNewValueContainer(const std::string& value)          { this->value_ = new MT_Value<std::string>         (value); }
template <> void MultiType::createNewValueContainer(const orxonox::Vector2& value)     { this->value_ = new MT_Value<orxonox::Vector2>    (value); }
template <> void MultiType::createNewValueContainer(const orxonox::Vector3& value)     { this->value_ = new MT_Value<orxonox::Vector3>    (value); }
template <> void MultiType::createNewValueContainer(const orxonox::Vector4& value)     { this->value_ = new MT_Value<orxonox::Vector4>    (value); }
template <> void MultiType::createNewValueContainer(const orxonox::ColourValue& value) { this->value_ = new MT_Value<orxonox::ColourValue>(value); }
template <> void MultiType::createNewValueContainer(const orxonox::Quaternion& value)  { this->value_ = new MT_Value<orxonox::Quaternion> (value); }
template <> void MultiType::createNewValueContainer(const orxonox::Radian& value)      { this->value_ = new MT_Value<orxonox::Radian>     (value); }
template <> void MultiType::createNewValueContainer(const orxonox::Degree& value)      { this->value_ = new MT_Value<orxonox::Degree>     (value); }
