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

#ifndef _MultiTypeValue_H__
#define _MultiTypeValue_H__

#include "UtilPrereqs.h"
#include "MathConvert.h"
#include "MultiType.h"

template <typename T>
struct MT_Value : public MultiType::MT_ValueBase
{
    MT_Value(const T& value, MT_Type type) : MT_ValueBase(type), value_(value) {}

    inline MT_ValueBase* clone() const { return new MT_Value<T>(this->value_, this->type_); }
    inline void reset() { this->value_ = T(); }
    inline void assimilate(const MultiType& other) { if (other.value_) { T temp; other.getValue(&temp); this->value_ = temp; } else { this->value_ = T(); } }

    inline void setValue(const char& value)                 { this->value_ = getConvertedValue<char,                 T>(value); }
    inline void setValue(const unsigned char& value)        { this->value_ = getConvertedValue<unsigned char,        T>(value); }
    inline void setValue(const short& value)                { this->value_ = getConvertedValue<short,                T>(value); }
    inline void setValue(const unsigned short& value)       { this->value_ = getConvertedValue<unsigned short,       T>(value); }
    inline void setValue(const int& value)                  { this->value_ = getConvertedValue<int,                  T>(value); }
    inline void setValue(const unsigned int& value)         { this->value_ = getConvertedValue<unsigned int,         T>(value); }
    inline void setValue(const long& value)                 { this->value_ = getConvertedValue<long,                 T>(value); }
    inline void setValue(const unsigned long& value)        { this->value_ = getConvertedValue<unsigned long,        T>(value); }
    inline void setValue(const long long& value)            { this->value_ = getConvertedValue<long long,            T>(value); }
    inline void setValue(const unsigned long long& value)   { this->value_ = getConvertedValue<unsigned long long,   T>(value); }
    inline void setValue(const float& value)                { this->value_ = getConvertedValue<float,                T>(value); }
    inline void setValue(const double& value)               { this->value_ = getConvertedValue<double,               T>(value); }
    inline void setValue(const long double& value)          { this->value_ = getConvertedValue<long double,          T>(value); }
    inline void setValue(const bool& value)                 { this->value_ = getConvertedValue<bool,                 T>(value); }
    inline void setValue(      void* const& value)          { this->value_ = getConvertedValue<void*,                T>(value); }
    inline void setValue(const std::string& value)          { this->value_ = getConvertedValue<std::string,          T>(value); }
    inline void setValue(const orxonox::Vector2& value)     { this->value_ = getConvertedValue<orxonox::Vector2,     T>(value); }
    inline void setValue(const orxonox::Vector3& value)     { this->value_ = getConvertedValue<orxonox::Vector3,     T>(value); }
    inline void setValue(const orxonox::Vector4& value)     { this->value_ = getConvertedValue<orxonox::Vector4,     T>(value); }
    inline void setValue(const orxonox::ColourValue& value) { this->value_ = getConvertedValue<orxonox::ColourValue, T>(value); }
    inline void setValue(const orxonox::Quaternion& value)  { this->value_ = getConvertedValue<orxonox::Quaternion,  T>(value); }
    inline void setValue(const orxonox::Radian& value)      { this->value_ = getConvertedValue<orxonox::Radian,      T>(value); }
    inline void setValue(const orxonox::Degree& value)      { this->value_ = getConvertedValue<orxonox::Degree,      T>(value); }

    inline operator char()                 const { return getConvertedValue<T, char>                (this->value_, 0); }
    inline operator unsigned char()        const { return getConvertedValue<T, unsigned char>       (this->value_, 0); }
    inline operator short()                const { return getConvertedValue<T, short>               (this->value_, 0); }
    inline operator unsigned short()       const { return getConvertedValue<T, unsigned short>      (this->value_, 0); }
    inline operator int()                  const { return getConvertedValue<T, int>                 (this->value_, 0); }
    inline operator unsigned int()         const { return getConvertedValue<T, unsigned int>        (this->value_, 0); }
    inline operator long()                 const { return getConvertedValue<T, long>                (this->value_, 0); }
    inline operator unsigned long()        const { return getConvertedValue<T, unsigned long>       (this->value_, 0); }
    inline operator long long()            const { return getConvertedValue<T, long long>           (this->value_, 0); }
    inline operator unsigned long long()   const { return getConvertedValue<T, unsigned long long>  (this->value_, 0); }
    inline operator float()                const { return getConvertedValue<T, float>               (this->value_, 0); }
    inline operator double()               const { return getConvertedValue<T, double>              (this->value_, 0); }
    inline operator long double()          const { return getConvertedValue<T, long double>         (this->value_, 0); }
    inline operator bool()                 const { return getConvertedValue<T, bool>                (this->value_, false); }
    inline operator void*()                const { return getConvertedValue<T, void*>               (this->value_, 0); }
    inline operator std::string()          const { return getConvertedValue<T, std::string>         (this->value_); }
    inline operator orxonox::Vector2()     const { return getConvertedValue<T, orxonox::Vector2>    (this->value_); }
    inline operator orxonox::Vector3()     const { return getConvertedValue<T, orxonox::Vector3>    (this->value_); }
    inline operator orxonox::Vector4()     const { return getConvertedValue<T, orxonox::Vector4>    (this->value_); }
    inline operator orxonox::ColourValue() const { return getConvertedValue<T, orxonox::ColourValue>(this->value_); }
    inline operator orxonox::Quaternion()  const { return getConvertedValue<T, orxonox::Quaternion> (this->value_); }
    inline operator orxonox::Radian()      const { return getConvertedValue<T, orxonox::Radian>     (this->value_); }
    inline operator orxonox::Degree()      const { return getConvertedValue<T, orxonox::Degree>     (this->value_); }

    inline void toString(std::ostream& outstream) const { std::operator <<(outstream, conversion_cast<std::string>(this->value_)); }
    //inline friend std::ostream& operator <<(std::ostream& outstream, const MT_Value& mtv) { outstream << conversion_cast<std::string>(this->value_); return outsream; }

    T value_;
};

#endif /* _MultiTypeValue_H__ */
