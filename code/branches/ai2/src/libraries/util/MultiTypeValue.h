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
    @ingroup MultiType
    @brief Declaration and Implementation of the MT_Value<T> class.

    The MT_Value<T> class is used to hold a value of type T within a MultiType.
*/

#ifndef _MultiTypeValue_H__
#define _MultiTypeValue_H__

#include "UtilPrereqs.h"

#include <cassert>
#include "MathConvert.h"
#include "MultiType.h"
#include "Serialise.h"

namespace orxonox
{
    /**
        @brief The MT_Value<T> class is used to hold a value of type T within a MultiType.
    */
    template <typename T>
    class MT_Value : public MultiType::MT_ValueBase
    {
    public:
        /// Constructor: Assigns the value and the type identifier.
        MT_Value(const T& value, MT_Type::Value type) : MT_ValueBase(type), value_(value) {}

        /// Creates a copy of itself.
        inline MT_ValueBase* clone() const { return new MT_Value<T>(this->value_, this->type_); }

        /// Resets the current value to the default.
        inline void reset() { this->value_ = zeroise<T>(); bHasDefaultValue_ = true; }

        /**
            @brief Assigns the value of the other MultiType, converted to T.
            @param other The other MultiType
        */
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

        inline bool getValue(char*                 value) const { return convertValue<T, char                >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(unsigned char*        value) const { return convertValue<T, unsigned char       >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(short*                value) const { return convertValue<T, short               >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(unsigned short*       value) const { return convertValue<T, unsigned short      >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(int*                  value) const { return convertValue<T, int                 >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(unsigned int*         value) const { return convertValue<T, unsigned int        >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(long*                 value) const { return convertValue<T, long                >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(unsigned long*        value) const { return convertValue<T, unsigned long       >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(long long*            value) const { return convertValue<T, long long           >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(unsigned long long*   value) const { return convertValue<T, unsigned long long  >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(float*                value) const { return convertValue<T, float               >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(double*               value) const { return convertValue<T, double              >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(long double*          value) const { return convertValue<T, long double         >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(bool*                 value) const { return convertValue<T, bool                >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(void**                value) const { return convertValue<T, void*               >(value, value_, 0); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(std::string*          value) const { return convertValue<T, std::string         >(value, value_, zeroise<std::string>         ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Vector2*     value) const { return convertValue<T, orxonox::Vector2    >(value, value_, zeroise<orxonox::Vector2>    ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Vector3*     value) const { return convertValue<T, orxonox::Vector3    >(value, value_, zeroise<orxonox::Vector3>    ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Vector4*     value) const { return convertValue<T, orxonox::Vector4    >(value, value_, zeroise<orxonox::Vector4>    ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::ColourValue* value) const { return convertValue<T, orxonox::ColourValue>(value, value_, zeroise<orxonox::ColourValue>()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Quaternion*  value) const { return convertValue<T, orxonox::Quaternion >(value, value_, zeroise<orxonox::Quaternion> ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Radian*      value) const { return convertValue<T, orxonox::Radian     >(value, value_, zeroise<orxonox::Radian>     ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
        inline bool getValue(orxonox::Degree*      value) const { return convertValue<T, orxonox::Degree     >(value, value_, zeroise<orxonox::Degree>     ()); } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.

        inline bool setValue(const char& value)                 { return !(bHasDefaultValue_ = !convertValue<char                , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const unsigned char& value)        { return !(bHasDefaultValue_ = !convertValue<unsigned char       , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const short& value)                { return !(bHasDefaultValue_ = !convertValue<short               , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const unsigned short& value)       { return !(bHasDefaultValue_ = !convertValue<unsigned short      , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const int& value)                  { return !(bHasDefaultValue_ = !convertValue<int                 , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const unsigned int& value)         { return !(bHasDefaultValue_ = !convertValue<unsigned int        , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const long& value)                 { return !(bHasDefaultValue_ = !convertValue<long                , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const unsigned long& value)        { return !(bHasDefaultValue_ = !convertValue<unsigned long       , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const long long& value)            { return !(bHasDefaultValue_ = !convertValue<long long           , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const unsigned long long& value)   { return !(bHasDefaultValue_ = !convertValue<unsigned long long  , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const float& value)                { return !(bHasDefaultValue_ = !convertValue<float               , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const double& value)               { return !(bHasDefaultValue_ = !convertValue<double              , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const long double& value)          { return !(bHasDefaultValue_ = !convertValue<long double         , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const bool& value)                 { return !(bHasDefaultValue_ = !convertValue<bool                , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(      void* const& value)          { return !(bHasDefaultValue_ = !convertValue<void*               , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const std::string& value)          { return !(bHasDefaultValue_ = !convertValue<std::string         , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Vector2& value)     { return !(bHasDefaultValue_ = !convertValue<orxonox::Vector2    , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Vector3& value)     { return !(bHasDefaultValue_ = !convertValue<orxonox::Vector3    , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Vector4& value)     { return !(bHasDefaultValue_ = !convertValue<orxonox::Vector4    , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::ColourValue& value) { return !(bHasDefaultValue_ = !convertValue<orxonox::ColourValue, T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Quaternion& value)  { return !(bHasDefaultValue_ = !convertValue<orxonox::Quaternion , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Radian& value)      { return !(bHasDefaultValue_ = !convertValue<orxonox::Radian     , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.
        inline bool setValue(const orxonox::Degree& value)      { return !(bHasDefaultValue_ = !convertValue<orxonox::Degree     , T>(&value_, value, zeroise<T>())); } ///< Assigns the value by converting it to T.

        inline operator char()                 const { return getConvertedValue<T, char>                (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator unsigned char()        const { return getConvertedValue<T, unsigned char>       (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator short()                const { return getConvertedValue<T, short>               (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator unsigned short()       const { return getConvertedValue<T, unsigned short>      (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator int()                  const { return getConvertedValue<T, int>                 (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator unsigned int()         const { return getConvertedValue<T, unsigned int>        (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator long()                 const { return getConvertedValue<T, long>                (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator unsigned long()        const { return getConvertedValue<T, unsigned long>       (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator long long()            const { return getConvertedValue<T, long long>           (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator unsigned long long()   const { return getConvertedValue<T, unsigned long long>  (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator float()                const { return getConvertedValue<T, float>               (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator double()               const { return getConvertedValue<T, double>              (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator long double()          const { return getConvertedValue<T, long double>         (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator bool()                 const { return getConvertedValue<T, bool>                (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator void*()                const { return getConvertedValue<T, void*>               (this->value_, 0); }     ///< Returns the current value, converted to the requested type.
        inline operator std::string()          const { return getConvertedValue<T, std::string>         (this->value_, NilValue<std::string         >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Vector2()     const { return getConvertedValue<T, orxonox::Vector2>    (this->value_, NilValue<orxonox::Vector2    >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Vector3()     const { return getConvertedValue<T, orxonox::Vector3>    (this->value_, NilValue<orxonox::Vector3    >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Vector4()     const { return getConvertedValue<T, orxonox::Vector4>    (this->value_, NilValue<orxonox::Vector4    >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::ColourValue() const { return getConvertedValue<T, orxonox::ColourValue>(this->value_, NilValue<orxonox::ColourValue>()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Quaternion()  const { return getConvertedValue<T, orxonox::Quaternion> (this->value_, NilValue<orxonox::Quaternion >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Radian()      const { return getConvertedValue<T, orxonox::Radian>     (this->value_, NilValue<orxonox::Radian     >()); } ///< Returns the current value, converted to the requested type.
        inline operator orxonox::Degree()      const { return getConvertedValue<T, orxonox::Degree>     (this->value_, NilValue<orxonox::Degree     >()); } ///< Returns the current value, converted to the requested type.

        /// Puts the current value on the stream
        inline void toString(std::ostream& outstream) const { outstream << this->value_; }

        /// loads data from the bytestream (mem) into the MT and increases the bytestream pointer by the size of the data
        inline void importData( uint8_t*& mem )         { loadAndIncrease( /*(const T&)*/this->value_, mem ); }
        /// saves data from the MT into the bytestream (mem) and increases the bytestream pointer by the size of the data
        inline void exportData( uint8_t*& mem ) const   { saveAndIncrease( /*(const T&)*/this->value_, mem ); }
        /// returns the size of the data that would be saved by exportData
        inline uint8_t getSize() const { return returnSize( this->value_ ); }

        T value_; ///< The stored value
    };

    // Import / Export specialisation
    // ColourValue
    template <> inline void MT_Value<ColourValue>::importData( uint8_t*& mem )
    {
        loadAndIncrease( this->value_.r, mem );
        loadAndIncrease( this->value_.g, mem );
        loadAndIncrease( this->value_.b, mem );
        loadAndIncrease( this->value_.a, mem );
    }
    template <> inline void MT_Value<ColourValue>::exportData( uint8_t*& mem ) const
    {
        saveAndIncrease( this->value_.r, mem );
        saveAndIncrease( this->value_.g, mem );
        saveAndIncrease( this->value_.b, mem );
        saveAndIncrease( this->value_.a, mem );
    }
    template <> inline uint8_t MT_Value<ColourValue>::getSize() const
    {
        return 4*returnSize(this->value_.r);
    }
    // Ogre::Quaternion
    template <> inline void MT_Value<Ogre::Quaternion>::importData( uint8_t*& mem )
    {
        loadAndIncrease( this->value_.x, mem );
        loadAndIncrease( this->value_.y, mem );
        loadAndIncrease( this->value_.z, mem );
        loadAndIncrease( this->value_.w, mem );
    }
    template <> inline void MT_Value<Ogre::Quaternion>::exportData( uint8_t*& mem ) const
    {
        saveAndIncrease( this->value_.x, mem );
        saveAndIncrease( this->value_.y, mem );
        saveAndIncrease( this->value_.z, mem );
        saveAndIncrease( this->value_.w, mem );
    }
    template <> inline uint8_t MT_Value<Ogre::Quaternion>::getSize() const
    {
        return 4*returnSize(this->value_.x);
    }
    // Ogre::Vector2
    template <> inline void MT_Value<Ogre::Vector2>::importData( uint8_t*& mem )
    {
        loadAndIncrease( this->value_.x, mem );
        loadAndIncrease( this->value_.y, mem );
    }
    template <> inline void MT_Value<Ogre::Vector2>::exportData( uint8_t*& mem ) const
    {
        saveAndIncrease( this->value_.x, mem );
        saveAndIncrease( this->value_.y, mem );
    }
    template <> inline uint8_t MT_Value<Ogre::Vector2>::getSize() const
    {
        return 2*returnSize(this->value_.x);
    }
    // Ogre::Vector3
    template <> inline void MT_Value<Ogre::Vector3>::importData( uint8_t*& mem )
    {
        loadAndIncrease( this->value_.x, mem );
        loadAndIncrease( this->value_.y, mem );
        loadAndIncrease( this->value_.z, mem );
    }
    template <> inline void MT_Value<Ogre::Vector3>::exportData( uint8_t*& mem ) const
    {
        saveAndIncrease( this->value_.x, mem );
        saveAndIncrease( this->value_.y, mem );
        saveAndIncrease( this->value_.z, mem );
    }
    template <> inline uint8_t MT_Value<Ogre::Vector3>::getSize() const
    {
        return 3*returnSize(this->value_.x);
    }
    // Ogre::Vector4
    template <> inline void MT_Value<Ogre::Vector4>::importData( uint8_t*& mem )
    {
        loadAndIncrease( this->value_.x, mem );
        loadAndIncrease( this->value_.y, mem );
        loadAndIncrease( this->value_.z, mem );
        loadAndIncrease( this->value_.w, mem );
    }
    template <> inline void MT_Value<Ogre::Vector4>::exportData( uint8_t*& mem ) const
    {
        saveAndIncrease( this->value_.x, mem );
        saveAndIncrease( this->value_.y, mem );
        saveAndIncrease( this->value_.z, mem );
        saveAndIncrease( this->value_.w, mem );
    }
    template <> inline uint8_t MT_Value<Ogre::Vector4>::getSize() const
    {
        return 4*returnSize(this->value_.x);
    }
    template <> inline void MT_Value<void*>::importData( uint8_t*& /*mem*/ )
    {
        assert(0);
    }
    template <> inline void MT_Value<void*>::exportData( uint8_t*& /*mem*/ ) const
    {
        assert(0);
    }
    template <> inline uint8_t MT_Value<void*>::getSize() const
    {
        assert(0); return 0;
    }
}

#endif /* _MultiTypeValue_H__ */
