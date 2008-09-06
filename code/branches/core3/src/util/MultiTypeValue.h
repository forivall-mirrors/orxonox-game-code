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
#include "Convert.h"
#include "MultiType.h"

template <typename T>
struct MT_Value : public MultiType::MT_ValueBase
{
/*
    template <class A, class B, class Btest = B>
    class StaticConversion
    {
        private:
            template <class V, bool convertable> struct Convert
            {   static inline A convert(const V& value) { return A(); }   };
            template <class V>                   struct Convert<V, true>
            {   static inline A convert(const V& value) { return ((A)value); }   };

            template <typename V, bool convertable> struct Compare
            {
                static inline bool compare_equal       (const A& value, const V& other) { return false; }
                static inline bool compare_notequal    (const A& value, const V& other) { return true;  }
                static inline bool compare_smaller     (const A& value, const V& other) { return false; }
                static inline bool compare_smallerequal(const A& value, const V& other) { return false; }
                static inline bool compare_greater     (const A& value, const V& other) { return false; }
                static inline bool compare_greaterequal(const A& value, const V& other) { return false; }
            };
            template <typename V>                   struct Compare<V, true>
            {
                static inline bool compare_equal       (const A& value, const V& other) { return (value == other); }
                static inline bool compare_notequal    (const A& value, const V& other) { return (value != other); }
                static inline bool compare_smaller     (const A& value, const V& other) { return (value <  other); }
                static inline bool compare_smallerequal(const A& value, const V& other) { return (value <= other); }
                static inline bool compare_greater     (const A& value, const V& other) { return (value >  other); }
                static inline bool compare_greaterequal(const A& value, const V& other) { return (value >= other); }
            };

            class Small { char dummy[1]; };
            class Big   { char dummy[1024]; };
            static Small Test(A);
            static Big   Test(...);
            static Btest MakeB();
            enum { exists = sizeof(Test(MakeB())) == sizeof(Small) };

        public:
            static inline A    convert(const B& value)                              { return Convert<B, exists>::convert(value); }
            static inline bool compare_equal       (const B& value, const A& other) { return Compare<B, exists>::compare_equal       (value, other); }
            static inline bool compare_notequal    (const B& value, const A& other) { return Compare<B, exists>::compare_notequal    (value, other); }
            static inline bool compare_smaller     (const B& value, const A& other) { return Compare<B, exists>::compare_smaller     (value, other); }
            static inline bool compare_smallerequal(const B& value, const A& other) { return Compare<B, exists>::compare_smallerequal(value, other); }
            static inline bool compare_greater     (const B& value, const A& other) { return Compare<B, exists>::compare_greater     (value, other); }
            static inline bool compare_greaterequal(const B& value, const A& other) { return Compare<B, exists>::compare_greaterequal(value, other); }
    };
*/
    MT_Value() {}
    MT_Value(const T& value) : value_(value) {}

    inline MT_ValueBase* clone() const { return new MT_Value<T>(this->value_); }
/*
    inline void setValue(const char& value)                 { this->value_ = StaticConversion<T, char>::convert(value);                 }
    inline void setValue(const unsigned char& value)        { this->value_ = StaticConversion<T, unsigned char>::convert(value);        }
    inline void setValue(const short& value)                { this->value_ = StaticConversion<T, short>::convert(value);                }
    inline void setValue(const unsigned short& value)       { this->value_ = StaticConversion<T, unsigned short>::convert(value);       }
    inline void setValue(const int& value)                  { this->value_ = StaticConversion<T, int>::convert(value);                  }
    inline void setValue(const unsigned int& value)         { this->value_ = StaticConversion<T, unsigned int>::convert(value);         }
    inline void setValue(const long& value)                 { this->value_ = StaticConversion<T, long>::convert(value);                 }
    inline void setValue(const unsigned long& value)        { this->value_ = StaticConversion<T, unsigned long>::convert(value);        }
    inline void setValue(const long long& value)            { this->value_ = StaticConversion<T, long long>::convert(value);            }
    inline void setValue(const unsigned long long& value)   { this->value_ = StaticConversion<T, unsigned long long>::convert(value);   }
    inline void setValue(const float& value)                { this->value_ = StaticConversion<T, float, int>::convert(value);           }
    inline void setValue(const double& value)               { this->value_ = StaticConversion<T, double, int>::convert(value);          }
    inline void setValue(const long double& value)          { this->value_ = StaticConversion<T, long double, int>::convert(value);     }
    inline void setValue(const bool& value)                 { this->value_ = StaticConversion<T, bool>::convert(value);                 }
    inline void setValue(      void* const& value)          { this->value_ = StaticConversion<T, void*>::convert(value);                }
    inline void setValue(const std::string& value)          { this->value_ = StaticConversion<T, std::string>::convert(value);          }
    inline void setValue(const orxonox::Vector2& value)     { this->value_ = StaticConversion<T, orxonox::Vector2>::convert(value);     }
    inline void setValue(const orxonox::Vector3& value)     { this->value_ = StaticConversion<T, orxonox::Vector3>::convert(value);     }
    inline void setValue(const orxonox::Vector4& value)     { this->value_ = StaticConversion<T, orxonox::Vector4>::convert(value);     }
    inline void setValue(const orxonox::ColourValue& value) { this->value_ = StaticConversion<T, orxonox::ColourValue>::convert(value); }
    inline void setValue(const orxonox::Quaternion& value)  { this->value_ = StaticConversion<T, orxonox::Quaternion>::convert(value);  }
    inline void setValue(const orxonox::Radian& value)      { this->value_ = StaticConversion<T, orxonox::Radian>::convert(value);      }
    inline void setValue(const orxonox::Degree& value)      { this->value_ = StaticConversion<T, orxonox::Degree>::convert(value);      }
*/
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

    inline void toString(std::ostream& outstream) const { outstream << this->value_; }

    T value_;
};

#endif /* _MultiTypeValue_H__ */
