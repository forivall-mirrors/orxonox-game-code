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
    @brief Declaration of the MultiType and some helper constructs.

    The MultiType can hold a value of one of the following types:
     - all primitives
     - all pointers
     - string
     - Vector2, Vector3, Vector4
     - Quaternion
     - ColourValue
     - Radian, Degree

    The MultiType has a "type" determined by the first assigned value, either through
     - the constructor,
     - the assignment operator= or
     - setValue(value).
    If you assign another value of another type, the MultiType keeps "it's" type and
    converts the new value to this type.

    If you want to change the type, there are three possibilities:
     - convert<T>() set's the type to T and converts the currently assigned value
     - setType<T>() set's the type to T and resets the value
     - setValue<T>(value) assigns a new value and changes the type to T.

    @example
    MultiType a = 10;;         // a has now the type int and the value 10
    a.setValue("3.14");        // a has still the type int and "3.14" gets converted, therefore the value is now 3
    a.setValue<float>("3.14"); // a has now the type float and "3.14" gets converted to 3.14
    a.convert<bool>();         // converts 3.14 to bool, which is true
    a = false;                 // assigns false, this is equivalent to a.setValue(false)

    @note
    Whenever a value gets converted, there is a boolean return value telling you whether it was
    successful or not. If it wasn't a zero value is assigned with the help of zeroise<T>().
*/

#ifndef _MultiType_H__
#define _MultiType_H__

#include "UtilPrereqs.h"

#include <cassert>
#include <string>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>

#include "TypeTraits.h"

namespace orxonox
{
    /**
        @brief Enum of all possible types of a MultiType.
    */
    namespace MT_Type
    {
        enum Value
        {
            Null,
            Char,
            UnsignedChar,
            Short,
            UnsignedShort,
            Int,
            UnsignedInt,
            Long,
            UnsignedLong,
            LongLong,
            UnsignedLongLong,
            Float,
            Double,
            LongDouble,
            Bool,
            VoidPointer,
            String,
            Vector2,
            Vector3,
            Vector4,
            ColourValue,
            Quaternion,
            Radian,
            Degree
        };
    }

    /**
        @brief The MultiType can hold a value of many possible types and convert them to other types.

        The following types are supported by the MultiType:
         - all primitves
         - all pointers
         - string
         - Vector2, Vector3, Vector4
         - Quaternion
         - ColourValue
         - Radian, Degree

        The internal type of a MultiType is determined by the first assigned value, but can be
        changed by using setType<T>(), convert<T>() or setValue<T>(value). If a value gets assigned
        the normal way (operator=, setValue(value)), the value gets converted to the current internal
        type of the MultiType.
    */
    class _UtilExport MultiType
    {
        _UtilExport friend std::ostream& operator<<(std::ostream& outstream, const MultiType& mt);
        template <typename T> friend class MT_Value;

    public:
        /**
            @brief MT_ValueBase is an almost pure virtual baseclass of MT_Value<T>, which holds the value of the MultiType.
            This class is only used within the MultiType.
        */
        class _UtilExport MT_ValueBase
        {
        public:
            MT_ValueBase(MT_Type::Value type) : type_(type), bHasDefaultValue_(false) {}
            virtual ~MT_ValueBase() {}

            virtual MT_ValueBase* clone() const = 0;

            virtual void reset() = 0;
            virtual bool assimilate(const MultiType& other) = 0;

            /** @brief Returns the type of the current value. */
            const MT_Type::Value& getType() const { return this->type_; }

            /** @brief Checks whether the value is a default one. */
            bool hasDefaultValue()   const { return this->bHasDefaultValue_; }

            virtual bool setValue(const char& value)                 = 0;
            virtual bool setValue(const unsigned char& value)        = 0;
            virtual bool setValue(const short& value)                = 0;
            virtual bool setValue(const unsigned short& value)       = 0;
            virtual bool setValue(const int& value)                  = 0;
            virtual bool setValue(const unsigned int& value)         = 0;
            virtual bool setValue(const long& value)                 = 0;
            virtual bool setValue(const unsigned long& value)        = 0;
            virtual bool setValue(const long long& value)            = 0;
            virtual bool setValue(const unsigned long long& value)   = 0;
            virtual bool setValue(const float& value)                = 0;
            virtual bool setValue(const double& value)               = 0;
            virtual bool setValue(const long double& value)          = 0;
            virtual bool setValue(const bool& value)                 = 0;
            virtual bool setValue(      void* const& value)          = 0;
            virtual bool setValue(const std::string& value)          = 0;
            virtual bool setValue(const orxonox::Vector2& value)     = 0;
            virtual bool setValue(const orxonox::Vector3& value)     = 0;
            virtual bool setValue(const orxonox::Vector4& value)     = 0;
            virtual bool setValue(const orxonox::ColourValue& value) = 0;
            virtual bool setValue(const orxonox::Quaternion& value)  = 0;
            virtual bool setValue(const orxonox::Radian& value)      = 0;
            virtual bool setValue(const orxonox::Degree& value)      = 0;

            virtual bool getValue(char*                 value) const = 0;
            virtual bool getValue(unsigned char*        value) const = 0;
            virtual bool getValue(short*                value) const = 0;
            virtual bool getValue(unsigned short*       value) const = 0;
            virtual bool getValue(int*                  value) const = 0;
            virtual bool getValue(unsigned int*         value) const = 0;
            virtual bool getValue(long*                 value) const = 0;
            virtual bool getValue(unsigned long*        value) const = 0;
            virtual bool getValue(long long*            value) const = 0;
            virtual bool getValue(unsigned long long*   value) const = 0;
            virtual bool getValue(float*                value) const = 0;
            virtual bool getValue(double*               value) const = 0;
            virtual bool getValue(long double*          value) const = 0;
            virtual bool getValue(bool*                 value) const = 0;
            virtual bool getValue(void**                value) const = 0;
            virtual bool getValue(std::string*          value) const = 0;
            virtual bool getValue(orxonox::Vector2*     value) const = 0;
            virtual bool getValue(orxonox::Vector3*     value) const = 0;
            virtual bool getValue(orxonox::Vector4*     value) const = 0;
            virtual bool getValue(orxonox::ColourValue* value) const = 0;
            virtual bool getValue(orxonox::Quaternion*  value) const = 0;
            virtual bool getValue(orxonox::Radian*      value) const = 0;
            virtual bool getValue(orxonox::Degree*      value) const = 0;

            virtual operator char()                 const = 0;
            virtual operator unsigned char()        const = 0;
            virtual operator short()                const = 0;
            virtual operator unsigned short()       const = 0;
            virtual operator int()                  const = 0;
            virtual operator unsigned int()         const = 0;
            virtual operator long()                 const = 0;
            virtual operator unsigned long()        const = 0;
            virtual operator long long()            const = 0;
            virtual operator unsigned long long()   const = 0;
            virtual operator float()                const = 0;
            virtual operator double()               const = 0;
            virtual operator long double()          const = 0;
            virtual operator bool()                 const = 0;
            virtual operator void*()                const = 0;
            virtual operator std::string()          const = 0;
            virtual operator orxonox::Vector2()     const = 0;
            virtual operator orxonox::Vector3()     const = 0;
            virtual operator orxonox::Vector4()     const = 0;
            virtual operator orxonox::ColourValue() const = 0;
            virtual operator orxonox::Quaternion()  const = 0;
            virtual operator orxonox::Radian()      const = 0;
            virtual operator orxonox::Degree()      const = 0;

            virtual void toString(std::ostream& outstream) const = 0;

            virtual void importData( uint8_t*& mem )=0;
            virtual void exportData( uint8_t*& mem ) const=0;
            virtual uint8_t getSize() const=0;

            MT_Type::Value type_;   //!< The type of the current value
            bool bHasDefaultValue_; //!< True if the last conversion wasn't successful
        };

        public:
            inline MultiType()                                  : value_(0) {}                                      /** @brief Default constructor: Assigns no value and no type. The type will be determined by the first assignment of a value. */
            inline MultiType(const char& value)                 : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const unsigned char& value)        : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const short& value)                : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const unsigned short& value)       : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const int& value)                  : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const unsigned int& value)         : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const long& value)                 : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const unsigned long& value)        : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const long long& value)            : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const unsigned long long& value)   : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const float& value)                : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const double& value)               : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const long double& value)          : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const bool& value)                 : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(      void* const& value)          : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const std::string& value)          : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Vector2& value)     : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Vector3& value)     : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Vector4& value)     : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::ColourValue& value) : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Quaternion& value)  : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Radian& value)      : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const orxonox::Degree& value)      : value_(0) { this->assignValue(value); }           /** @brief Constructor: Assigns the given value and sets the type. */
            inline MultiType(const char* value)                 : value_(0) { this->setValue(std::string(value)); } /** @brief Constructor: Converts the char array to a std::string, assigns the value and sets the type. */
            inline MultiType(const MultiType& other)            : value_(0) { this->setValue(other); }              /** @brief Copyconstructor: Assigns value and type of the other MultiType. */
            inline MultiType(MT_Type::Value type)               : value_(0) { this->setType(type); }                /** @brief Constructor: Sets the type, the next assignment will determine the value. */

            /** @brief Destructor: Deletes the MT_Value. */
            inline ~MultiType() { if (this->value_) { delete this->value_; } }

            template <typename V> inline const MultiType& operator=(const V& value)         { this->setValue(value); return (*this); } /** @brief Assigns a new value. The value will be converted to the current type of the MultiType. */
            template <typename V> inline const MultiType& operator=(V* value)               { this->setValue(value); return (*this); } /** @brief Assigns a pointer. */
            inline                       const MultiType& operator=(const MultiType& other) { this->setValue(other); return (*this); } /** @brief Assigns the value of the other MultiType and converts it to the current type of the MultiType. */
            inline                       const MultiType& operator=(MT_Type::Value type)    { this->setType(type);   return (*this); } /** @brief Resets the value and changes the type. */

            inline bool                                   setValue(const char& value);
            inline bool                                   setValue(const unsigned char& value);
            inline bool                                   setValue(const short& value);
            inline bool                                   setValue(const unsigned short& value);
            inline bool                                   setValue(const int& value);
            inline bool                                   setValue(const unsigned int& value);
            inline bool                                   setValue(const long& value);
            inline bool                                   setValue(const unsigned long& value);
            inline bool                                   setValue(const long long& value);
            inline bool                                   setValue(const unsigned long long& value);
            inline bool                                   setValue(const float& value);
            inline bool                                   setValue(const double& value);
            inline bool                                   setValue(const long double& value);
            inline bool                                   setValue(const bool& value);
            inline bool                                   setValue(      void* const& value);
            inline bool                                   setValue(const std::string& value);
            inline bool                                   setValue(const orxonox::Vector2& value);
            inline bool                                   setValue(const orxonox::Vector3& value);
            inline bool                                   setValue(const orxonox::Vector4& value);
            inline bool                                   setValue(const orxonox::ColourValue& value);
            inline bool                                   setValue(const orxonox::Quaternion& value);
            inline bool                                   setValue(const orxonox::Radian& value);
            inline bool                                   setValue(const orxonox::Degree& value);
            inline bool                                   setValue(const char* value);
            /** @brief Assigns a pointer. */
            template <typename V> inline bool setValue(V* value)
            {
                if (this->value_)
                    return this->value_->setValue(static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));
                else
                    return this->assignValue     (static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));
            }
            /** @brief Assigns the value of the other MultiType and converts it to the current type. */
            bool                                          setValue(const MultiType& other) { if (this->value_) { return this->value_->assimilate(other); } else { if (other.value_) { this->value_ = other.value_->clone(); } return true; } }
            /** @brief Changes the type to T and assigns the new value (which might be of another type than T - it gets converted). */
            template <typename T, typename V> inline bool setValue(const V& value) { this->setType<T>(); return this->setValue(value); }


            /** @brief Copies the other MultiType by assigning value and type. */
            inline void                       copy(const MultiType& other)    { if (this == &other) { return; } if (this->value_) { delete this->value_; } this->value_ = (other.value_) ? other.value_->clone() : 0; }

            template <typename T> inline bool convert()                       { return this->setValue<T>((T)(*this));  } /** @brief Converts the current value to type T. */
            inline bool                       convert(const MultiType& other) { return this->convert(other.getType()); } /** @brief Converts the current value to the type of the other MultiType. */
            bool                              convert(MT_Type::Value type);

            /** @brief Current content gets deleted. New type is MT_Type::Null */
            inline void                       reset()                         { if (this->value_) delete this->value_; this->value_ = 0; }
            /** @brief Current content gets overridden with default zero value */
            inline void                       resetValue()                    { if (this->value_) this->value_->reset(); }

            template <typename T> inline void setType()                       { this->assignValue(typename Loki::TypeTraits<T>::UnqualifiedReferredType()); } /** @brief Resets the value and changes the internal type to T. */
            inline void                       setType(const MultiType& other) { this->setType(other.getType());                                             } /** @brief Resets the value and changes the internal type to the type of the other MultiType. */
            inline void                       setType(MT_Type::Value type)    { this->reset(); this->convert(type); this->resetValue();                     } /** @brief Resets the value and changes the internal type to the given type. */

            /** @brief Returns the current type. */
            inline MT_Type::Value             getType()                   const { return (this->value_) ? this->value_->type_ : MT_Type::Null; }
            /** @brief Returns true if the current type equals the given type. */
            inline bool                       isType(MT_Type::Value type) const { return (this->value_) ? (this->value_->type_ == type) : (type == MT_Type::Null); }
            /** @brief Returns true if the current type is T. */
            template <typename T> inline bool isType()                    const { return false; } // Only works for specialized values - see below
            std::string                       getTypename()               const;

            /** @brief Saves the value of the MT to a bytestream (pointed at by mem) and increases mem pointer by size of MT */
            inline void                       exportData(uint8_t*& mem) const { assert(sizeof(MT_Type::Value)<=8); *static_cast<uint8_t*>(mem) = this->getType(); mem+=sizeof(uint8_t); this->value_->exportData(mem); }
            /** @brief Loads the value of the MT from a bytestream (pointed at by mem) and increases mem pointer by size of MT */
            inline void                       importData(uint8_t*& mem) { assert(sizeof(MT_Type::Value)<=8); this->setType(static_cast<MT_Type::Value>(*static_cast<uint8_t*>(mem))); mem+=sizeof(uint8_t); this->value_->importData(mem); }
            /** @brief Saves the value of the MT to a bytestream and increases pointer to bytestream by size of MT */
            inline uint8_t*&                  operator << (uint8_t*& mem) { importData(mem); return mem; }
            /** @brief Loads the value of the MT to a bytestream and increases pointer to bytestream by size of MT */
            inline void                       operator >> (uint8_t*& mem) const { exportData(mem); }
            inline uint32_t                   getNetworkSize() const { assert(this->value_); return this->value_->getSize() + sizeof(uint8_t); }

            /** @brief Checks whether the value is a default one. */
            bool                              hasDefaultValue()         const { return this->value_->hasDefaultValue(); }

            operator char()                  const;
            operator unsigned char()         const;
            operator short()                 const;
            operator unsigned short()        const;
            operator int()                   const;
            operator unsigned int()          const;
            operator long()                  const;
            operator unsigned long()         const;
            operator long long()             const;
            operator unsigned long long()    const;
            operator float()                 const;
            operator double()                const;
            operator long double()           const;
            operator bool()                  const;
            operator void*()                 const;
            operator std::string()           const;
            operator orxonox::Vector2()      const;
            operator orxonox::Vector3()      const;
            operator orxonox::Vector4()      const;
            operator orxonox::ColourValue()  const;
            operator orxonox::Quaternion()   const;
            operator orxonox::Radian()       const;
            operator orxonox::Degree()       const;
            /** @brief Returns the current value, converted to a T* pointer. */
            template <class T> operator T*() const { return (static_cast<T*>(this->operator void*())); }

            inline bool getValue(char*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(unsigned char*        value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(short*                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(unsigned short*       value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(int*                  value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(unsigned int*         value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(long*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(unsigned long*        value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(long long*            value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(unsigned long long*   value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(float*                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(double*               value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(long double*          value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(bool*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(void**                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(std::string*          value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Vector2*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Vector3*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Vector4*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::ColourValue* value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Quaternion*  value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Radian*      value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
            inline bool getValue(orxonox::Degree*      value) const { if (this->value_) { return this->value_->getValue(value); } return false; } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */

            inline char                     getChar()             const { return this->operator char();                 } /** @brief Returns the current value, converted to the requested type. */
            inline unsigned char            getUnsignedChar()     const { return this->operator unsigned char();        } /** @brief Returns the current value, converted to the requested type. */
            inline short                    getShort()            const { return this->operator short();                } /** @brief Returns the current value, converted to the requested type. */
            inline unsigned short           getUnsignedShort()    const { return this->operator unsigned short();       } /** @brief Returns the current value, converted to the requested type. */
            inline int                      getInt()              const { return this->operator int();                  } /** @brief Returns the current value, converted to the requested type. */
            inline unsigned int             getUnsignedInt()      const { return this->operator unsigned int();         } /** @brief Returns the current value, converted to the requested type. */
            inline long                     getLong()             const { return this->operator long();                 } /** @brief Returns the current value, converted to the requested type. */
            inline unsigned long            getUnsignedLong()     const { return this->operator unsigned long();        } /** @brief Returns the current value, converted to the requested type. */
            inline long long                getLongLong()         const { return this->operator long long();            } /** @brief Returns the current value, converted to the requested type. */
            inline unsigned long long       getUnsignedLongLong() const { return this->operator unsigned long long();   } /** @brief Returns the current value, converted to the requested type. */
            inline float                    getFloat()            const { return this->operator float();                } /** @brief Returns the current value, converted to the requested type. */
            inline double                   getDouble()           const { return this->operator double();               } /** @brief Returns the current value, converted to the requested type. */
            inline long double              getLongDouble()       const { return this->operator long double();          } /** @brief Returns the current value, converted to the requested type. */
            inline bool                     getBool()             const { return this->operator bool();                 } /** @brief Returns the current value, converted to the requested type. */
            inline void*                    getVoid()             const { return this->operator void*();                } /** @brief Returns the current value, converted to the requested type. */
            inline std::string              getString()           const { return this->operator std::string();          } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Vector2         getVector2()          const { return this->operator orxonox::Vector2();     } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Vector3         getVector3()          const { return this->operator orxonox::Vector3();     } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Vector4         getVector4()          const { return this->operator orxonox::Vector4();     } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::ColourValue     getColourValue()      const { return this->operator orxonox::ColourValue(); } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Quaternion      getQuaternion()       const { return this->operator orxonox::Quaternion();  } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Radian          getRadian()           const { return this->operator orxonox::Radian();      } /** @brief Returns the current value, converted to the requested type. */
            inline orxonox::Degree          getDegree()           const { return this->operator orxonox::Degree();      } /** @brief Returns the current value, converted to the requested type. */
            template <typename T> inline T* getPointer()          const { return static_cast<T*>(this->getVoid());      } /** @brief Returns the current value, converted to a T* pointer. */

        private:
            inline bool assignValue(const char& value)                 { if (this->value_ && this->value_->type_ == MT_Type::Char)             { return this->value_->setValue(value); } else { this->changeValueContainer<char>(value);                 return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const unsigned char& value)        { if (this->value_ && this->value_->type_ == MT_Type::UnsignedChar)     { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned char>(value);        return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const short& value)                { if (this->value_ && this->value_->type_ == MT_Type::Short)            { return this->value_->setValue(value); } else { this->changeValueContainer<short>(value);                return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const unsigned short& value)       { if (this->value_ && this->value_->type_ == MT_Type::UnsignedShort)    { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned short>(value);       return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const int& value)                  { if (this->value_ && this->value_->type_ == MT_Type::Int)              { return this->value_->setValue(value); } else { this->changeValueContainer<int>(value);                  return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const unsigned int& value)         { if (this->value_ && this->value_->type_ == MT_Type::UnsignedInt)      { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned int>(value);         return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const long& value)                 { if (this->value_ && this->value_->type_ == MT_Type::Long)             { return this->value_->setValue(value); } else { this->changeValueContainer<long>(value);                 return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const unsigned long& value)        { if (this->value_ && this->value_->type_ == MT_Type::UnsignedLong)     { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned long>(value);        return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const long long& value)            { if (this->value_ && this->value_->type_ == MT_Type::LongLong)         { return this->value_->setValue(value); } else { this->changeValueContainer<long long>(value);            return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const unsigned long long& value)   { if (this->value_ && this->value_->type_ == MT_Type::UnsignedLongLong) { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned long long>(value);   return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const float& value)                { if (this->value_ && this->value_->type_ == MT_Type::Float)            { return this->value_->setValue(value); } else { this->changeValueContainer<float>(value);                return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const double& value)               { if (this->value_ && this->value_->type_ == MT_Type::Double)           { return this->value_->setValue(value); } else { this->changeValueContainer<double>(value);               return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const long double& value)          { if (this->value_ && this->value_->type_ == MT_Type::LongDouble)       { return this->value_->setValue(value); } else { this->changeValueContainer<long double>(value);          return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const bool& value)                 { if (this->value_ && this->value_->type_ == MT_Type::Bool)             { return this->value_->setValue(value); } else { this->changeValueContainer<bool>(value);                 return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(      void* const& value)          { if (this->value_ && this->value_->type_ == MT_Type::VoidPointer)      { return this->value_->setValue(value); } else { this->changeValueContainer<void*>(value);                return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const std::string& value)          { if (this->value_ && this->value_->type_ == MT_Type::String)           { return this->value_->setValue(value); } else { this->changeValueContainer<std::string>(value);          return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Vector2& value)     { if (this->value_ && this->value_->type_ == MT_Type::Vector2)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector2>(value);     return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Vector3& value)     { if (this->value_ && this->value_->type_ == MT_Type::Vector3)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector3>(value);     return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Vector4& value)     { if (this->value_ && this->value_->type_ == MT_Type::Vector4)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector4>(value);     return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::ColourValue& value) { if (this->value_ && this->value_->type_ == MT_Type::ColourValue)      { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::ColourValue>(value); return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Quaternion& value)  { if (this->value_ && this->value_->type_ == MT_Type::Quaternion)       { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Quaternion>(value);  return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Radian& value)      { if (this->value_ && this->value_->type_ == MT_Type::Radian)           { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Radian>(value);      return true; } } /** @brief Assigns a new value by changing type and creating a new container. */
            inline bool assignValue(const orxonox::Degree& value)      { if (this->value_ && this->value_->type_ == MT_Type::Degree)           { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Degree>(value);      return true; } } /** @brief Assigns a new value by changing type and creating a new container. */

            /** @brief Changes the value container. */
            template <typename T> inline void changeValueContainer(const T& value) { if (this->value_) { delete this->value_; } this->createNewValueContainer<T>(value); }
            /** @brief Creates a new value container (works only with specialized types). */
            template <typename T>        void createNewValueContainer(const T& value) { /* STATIC ASSERT */ *****value; return false; }

            MT_ValueBase* value_; //!< A pointer to the value container
    };

    /** @brief Puts the MultiType on a stream by using the native << operator of the current type. */
    _UtilExport inline std::ostream& operator<<(std::ostream& outstream, const MultiType& mt) { if (mt.value_) { mt.value_->toString(outstream); } return outstream; }

    template <> inline bool MultiType::isType<char>()                 const { return (this->value_ && this->value_->type_ == MT_Type::Char);             } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<unsigned char>()        const { return (this->value_ && this->value_->type_ == MT_Type::UnsignedChar);     } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<short>()                const { return (this->value_ && this->value_->type_ == MT_Type::Short);            } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<unsigned short>()       const { return (this->value_ && this->value_->type_ == MT_Type::UnsignedShort);    } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<int>()                  const { return (this->value_ && this->value_->type_ == MT_Type::Int);              } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<unsigned int>()         const { return (this->value_ && this->value_->type_ == MT_Type::UnsignedInt);      } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<long>()                 const { return (this->value_ && this->value_->type_ == MT_Type::Long);             } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<unsigned long>()        const { return (this->value_ && this->value_->type_ == MT_Type::UnsignedLong);     } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<long long>()            const { return (this->value_ && this->value_->type_ == MT_Type::LongLong);         } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<unsigned long long>()   const { return (this->value_ && this->value_->type_ == MT_Type::UnsignedLongLong); } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<float>()                const { return (this->value_ && this->value_->type_ == MT_Type::Float);            } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<double>()               const { return (this->value_ && this->value_->type_ == MT_Type::Double);           } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<long double>()          const { return (this->value_ && this->value_->type_ == MT_Type::LongDouble);       } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<bool>()                 const { return (this->value_ && this->value_->type_ == MT_Type::Bool);             } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<void*>()                const { return (this->value_ && this->value_->type_ == MT_Type::VoidPointer);      } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<std::string>()          const { return (this->value_ && this->value_->type_ == MT_Type::String);           } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Vector2>()     const { return (this->value_ && this->value_->type_ == MT_Type::Vector2);          } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Vector3>()     const { return (this->value_ && this->value_->type_ == MT_Type::Vector3);          } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Vector4>()     const { return (this->value_ && this->value_->type_ == MT_Type::Vector4);          } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::ColourValue>() const { return (this->value_ && this->value_->type_ == MT_Type::ColourValue);      } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Quaternion>()  const { return (this->value_ && this->value_->type_ == MT_Type::Quaternion);       } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Radian>()      const { return (this->value_ && this->value_->type_ == MT_Type::Radian);           } /** @brief Returns true if the current type equals the given type. */
    template <> inline bool MultiType::isType<orxonox::Degree>()      const { return (this->value_ && this->value_->type_ == MT_Type::Degree);           } /** @brief Returns true if the current type equals the given type. */

    // Specialization to avoid ambiguities with the conversion operator
    template <> inline bool MultiType::convert<std::string>()          { return this->setValue<std::string>         (this->operator std::string());          } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Vector2>()     { return this->setValue<orxonox::Vector2>    (this->operator orxonox::Vector2());     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Vector3>()     { return this->setValue<orxonox::Vector3>    (this->operator orxonox::Vector3());     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Vector4>()     { return this->setValue<orxonox::Vector4>    (this->operator orxonox::Vector4());     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::ColourValue>() { return this->setValue<orxonox::ColourValue>(this->operator orxonox::ColourValue()); } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Quaternion>()  { return this->setValue<orxonox::Quaternion> (this->operator orxonox::Quaternion());  } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Radian>()      { return this->setValue<orxonox::Radian>     (this->operator orxonox::Radian());      } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<orxonox::Degree>()      { return this->setValue<orxonox::Degree>     (this->operator orxonox::Degree());      } /** @brief Converts the current value to the given type. */

    // Specialization to avoid ambiguities with the conversion operator
    template <> inline bool MultiType::convert<const std::string&>()          { return this->convert<std::string>();          } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Vector2&>()     { return this->convert<orxonox::Vector2>();     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Vector3&>()     { return this->convert<orxonox::Vector3>();     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Vector4&>()     { return this->convert<orxonox::Vector4>();     } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::ColourValue&>() { return this->convert<orxonox::ColourValue>(); } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Quaternion&>()  { return this->convert<orxonox::Quaternion>();  } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Radian&>()      { return this->convert<orxonox::Radian>();      } /** @brief Converts the current value to the given type. */
    template <> inline bool MultiType::convert<const orxonox::Degree&>()      { return this->convert<orxonox::Degree>();      } /** @brief Converts the current value to the given type. */

    template <> void MultiType::createNewValueContainer(const char& value);
    template <> void MultiType::createNewValueContainer(const unsigned char& value);
    template <> void MultiType::createNewValueContainer(const short& value);
    template <> void MultiType::createNewValueContainer(const unsigned short& value);
    template <> void MultiType::createNewValueContainer(const int& value);
    template <> void MultiType::createNewValueContainer(const unsigned int& value);
    template <> void MultiType::createNewValueContainer(const long& value);
    template <> void MultiType::createNewValueContainer(const unsigned long& value);
    template <> void MultiType::createNewValueContainer(const long long& value);
    template <> void MultiType::createNewValueContainer(const unsigned long long& value);
    template <> void MultiType::createNewValueContainer(const float& value);
    template <> void MultiType::createNewValueContainer(const double& value);
    template <> void MultiType::createNewValueContainer(const bool& value);
    template <> void MultiType::createNewValueContainer(const long double& value);
    template <> void MultiType::createNewValueContainer(      void* const& value);
    template <> void MultiType::createNewValueContainer(const std::string& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Vector2& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Vector3& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Vector4& value);
    template <> void MultiType::createNewValueContainer(const orxonox::ColourValue& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Quaternion& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Radian& value);
    template <> void MultiType::createNewValueContainer(const orxonox::Degree& value);

    inline bool MultiType::setValue(const char& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const unsigned char& value)         { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const short& value)                 { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const unsigned short& value)        { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const int& value)                   { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const unsigned int& value)          { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const long& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const unsigned long& value)         { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const long long& value)             { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const unsigned long long& value)    { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const float& value)                 { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const double& value)                { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const long double& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const bool& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(      void* const& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const std::string& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Vector2& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Vector3& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Vector4& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::ColourValue& value)  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Quaternion& value)   { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Radian& value)       { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
    inline bool MultiType::setValue(const orxonox::Degree& value)       { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */

    inline bool MultiType::setValue(const char* value)                  { if (this->value_) { return this->value_->setValue(std::string(value)); } else { return this->assignValue(std::string(value)); } }  /** @brief Assigns the given value and converts it to the current type. */
}

#endif /* _MultiType_H__ */
