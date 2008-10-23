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
    @file MultiTypeValue.h
    @brief Declaration and Implementation of the MT_Value<T> class.

    The MT_Value<T> class is used to hold a value of type T within a MultiType.
*/

#ifndef _MultiTypeValue_H__
#define _MultiTypeValue_H__

#include "UtilPrereqs.h"
#include "Convert.h"
#include "MultiType.h"

/**
    @brief The MT_Value<T> class is used to hold a value of type T within a MultiType.
*/
template <typename T>
struct MT_Value : public MultiType::MT_ValueBase
{
    /** @brief Constructor: Assigns the value and the type identifier. */
    MT_Value(const T& value, MT_Type type) : MT_ValueBase(type), value_(value) {}

    /** @brief Creates a copy of itself. */
    inline MT_ValueBase* clone() const { return new MT_Value<T>(this->value_, this->type_); }

    /** @brief Resets the current value to the default. */
    inline void reset() { this->value_ = zeroise<T>(); bHasDefaultValue_ = true; }

    /** @brief Assigns the value of the other MultiType, converted to T. @param other The other MultiType */
    inline bool assimilate(const MultiType& other)
    {
        if (other.value_)
        {
            return !(bHasDefaultValue_ = !other.value_->getValue(&value_));
        }
        else
        {
            this->value_ = zeroise<T>();
            return !(bHasDefaultValue_ = true);
        }
    }

    inline bool getValue(char*                 value) const { return ConvertValue<T, char                >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(unsigned char*        value) const { return ConvertValue<T, unsigned char       >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(short*                value) const { return ConvertValue<T, short               >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(unsigned short*       value) const { return ConvertValue<T, unsigned short      >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(int*                  value) const { return ConvertValue<T, int                 >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(unsigned int*         value) const { return ConvertValue<T, unsigned int        >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(long*                 value) const { return ConvertValue<T, long                >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(unsigned long*        value) const { return ConvertValue<T, unsigned long       >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(long long*            value) const { return ConvertValue<T, long long           >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(unsigned long long*   value) const { return ConvertValue<T, unsigned long long  >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(float*                value) const { return ConvertValue<T, float               >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(double*               value) const { return ConvertValue<T, double              >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(long double*          value) const { return ConvertValue<T, long double         >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(bool*                 value) const { return ConvertValue<T, bool                >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(void**                value) const { return ConvertValue<T, void*               >(value, value_, 0); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(std::string*          value) const { return ConvertValue<T, std::string         >(value, value_, zeroise<std::string>         ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Vector2*     value) const { return ConvertValue<T, orxonox::Vector2    >(value, value_, zeroise<orxonox::Vector2>    ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Vector3*     value) const { return ConvertValue<T, orxonox::Vector3    >(value, value_, zeroise<orxonox::Vector3>    ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Vector4*     value) const { return ConvertValue<T, orxonox::Vector4    >(value, value_, zeroise<orxonox::Vector4>    ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::ColourValue* value) const { return ConvertValue<T, orxonox::ColourValue>(value, value_, zeroise<orxonox::ColourValue>()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Quaternion*  value) const { return ConvertValue<T, orxonox::Quaternion >(value, value_, zeroise<orxonox::Quaternion> ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Radian*      value) const { return ConvertValue<T, orxonox::Radian     >(value, value_, zeroise<orxonox::Radian>     ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
    inline bool getValue(orxonox::Degree*      value) const { return ConvertValue<T, orxonox::Degree     >(value, value_, zeroise<orxonox::Degree>     ()); } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */

    inline bool setValue(const char& value)                 { return !(bHasDefaultValue_ = !ConvertValue<char                , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const unsigned char& value)        { return !(bHasDefaultValue_ = !ConvertValue<unsigned char       , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const short& value)                { return !(bHasDefaultValue_ = !ConvertValue<short               , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const unsigned short& value)       { return !(bHasDefaultValue_ = !ConvertValue<unsigned short      , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const int& value)                  { return !(bHasDefaultValue_ = !ConvertValue<int                 , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const unsigned int& value)         { return !(bHasDefaultValue_ = !ConvertValue<unsigned int        , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const long& value)                 { return !(bHasDefaultValue_ = !ConvertValue<long                , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const unsigned long& value)        { return !(bHasDefaultValue_ = !ConvertValue<unsigned long       , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const long long& value)            { return !(bHasDefaultValue_ = !ConvertValue<long long           , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const unsigned long long& value)   { return !(bHasDefaultValue_ = !ConvertValue<unsigned long long  , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const float& value)                { return !(bHasDefaultValue_ = !ConvertValue<float               , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const double& value)               { return !(bHasDefaultValue_ = !ConvertValue<double              , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const long double& value)          { return !(bHasDefaultValue_ = !ConvertValue<long double         , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const bool& value)                 { return !(bHasDefaultValue_ = !ConvertValue<bool                , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(      void* const& value)          { return !(bHasDefaultValue_ = !ConvertValue<void*               , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const std::string& value)          { return !(bHasDefaultValue_ = !ConvertValue<std::string         , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Vector2& value)     { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Vector2    , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Vector3& value)     { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Vector3    , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Vector4& value)     { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Vector4    , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::ColourValue& value) { return !(bHasDefaultValue_ = !ConvertValue<orxonox::ColourValue, T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Quaternion& value)  { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Quaternion , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Radian& value)      { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Radian     , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */
    inline bool setValue(const orxonox::Degree& value)      { return !(bHasDefaultValue_ = !ConvertValue<orxonox::Degree     , T>(&value_, value, zeroise<T>())); } /** @brief Assigns the value by converting it to T. */

    inline operator char()                 const { return getConvertedValue<T, char>                (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator unsigned char()        const { return getConvertedValue<T, unsigned char>       (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator short()                const { return getConvertedValue<T, short>               (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator unsigned short()       const { return getConvertedValue<T, unsigned short>      (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator int()                  const { return getConvertedValue<T, int>                 (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator unsigned int()         const { return getConvertedValue<T, unsigned int>        (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator long()                 const { return getConvertedValue<T, long>                (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator unsigned long()        const { return getConvertedValue<T, unsigned long>       (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator long long()            const { return getConvertedValue<T, long long>           (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator unsigned long long()   const { return getConvertedValue<T, unsigned long long>  (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator float()                const { return getConvertedValue<T, float>               (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator double()               const { return getConvertedValue<T, double>              (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator long double()          const { return getConvertedValue<T, long double>         (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator bool()                 const { return getConvertedValue<T, bool>                (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator void*()                const { return getConvertedValue<T, void*>               (this->value_, 0); }     /** @brief Returns the current value, converted to the requested type. */
    inline operator std::string()          const { return getConvertedValue<T, std::string>         (this->value_, zeroise<std::string         >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Vector2()     const { return getConvertedValue<T, orxonox::Vector2>    (this->value_, zeroise<orxonox::Vector2    >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Vector3()     const { return getConvertedValue<T, orxonox::Vector3>    (this->value_, zeroise<orxonox::Vector3    >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Vector4()     const { return getConvertedValue<T, orxonox::Vector4>    (this->value_, zeroise<orxonox::Vector4    >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::ColourValue() const { return getConvertedValue<T, orxonox::ColourValue>(this->value_, zeroise<orxonox::ColourValue>()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Quaternion()  const { return getConvertedValue<T, orxonox::Quaternion> (this->value_, zeroise<orxonox::Quaternion >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Radian()      const { return getConvertedValue<T, orxonox::Radian>     (this->value_, zeroise<orxonox::Radian     >()); } /** @brief Returns the current value, converted to the requested type. */
    inline operator orxonox::Degree()      const { return getConvertedValue<T, orxonox::Degree>     (this->value_, zeroise<orxonox::Degree     >()); } /** @brief Returns the current value, converted to the requested type. */

    /** @brief Puts the current value on the stream */
    inline void toString(std::ostream& outstream) const { outstream << this->value_; }

    T value_; //!< The stored value
};

#endif /* _MultiTypeValue_H__ */
