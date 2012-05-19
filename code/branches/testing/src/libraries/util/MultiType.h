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
    @defgroup MultiType MultiType
    @ingroup Util
*/

/**
    @file
    @ingroup MultiType
    @brief Declaration of the MultiType and some helper constructs.

    @anchor MultiTypeExamples

    The MultiType can hold a value of one of the following types:
     - all primitives (int, float, bool, etc.)
     - all pointers (void* and T*)
     - std::string
     - Vector2, Vector3, Vector4
     - Quaternion
     - ColourValue
     - Radian, Degree

    The MultiType has an internal "type" determined by the first assigned value, using one of these ways:
     - @ref orxonox::MultiType::MultiType "The constructor"
     - The assignment operator= (orxonox::MultiType::operator=())
     - @ref orxonox::MultiType::setValue() "setValue(value)"

    If you assign another value of another type, the MultiType keeps "its" type and
    converts the new value to the old type.

    If you want to change the type, there are three possibilities:
     - @ref orxonox::MultiType::convert "convert<T>()" sets the type to T and converts the currently assigned value
     - @ref orxonox::MultiType::setType "setType<T>()" sets the type to T and resets the value to zero using zeroise<T>()
     - setValue<T>(value) assigns a new value and changes the type to T.

    Examples:
    @code
    MultiType a = 10;          // a has now the type int and the value 10
    a.setValue("3.14");        // a has still the type int and "3.14" gets converted, therefore the value is now 3
    a.setValue<float>("3.14"); // a has now the type float and "3.14" gets converted to 3.14f
    a.convert<bool>();         // converts 3.14f to bool, which is true
    a = false;                 // assigns false, this is equivalent to a.setValue(false)
    @endcode

    You can pass a MultiType to a function as an argument, even if the argument is
    not of type MultiType. This works, because the MultiType is automatically converted
    to the right type.

    Example:
    @code
    void myfunction(int value)
    {
        orxout() << "doubled value is " << (2 * value) << endl;
    }

    MultiType a = "50";        // Note: We assigned a string
    myfunction(a);             // a is converted to int and passed to the function, which prints "value is 100"
    @endcode

    Note however that it is of course quite expensive to convert values, especially std::string <-> value.
    So if you can, always assign a value with the right type to avoid conversion.

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
#include <loki/TypeTraits.h>
#include "mbool.h"

namespace orxonox
{
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

        For more information and some examples see the description @ref MultiTypeExamples "here".

        @see MultiType.h
    */
    class _UtilExport MultiType
    {
        _UtilExport friend std::ostream& operator<<(std::ostream& outstream, const MultiType& mt);
        template <typename T> friend class MT_Value;

        struct Type
        {
            /**
                @brief Enum of all possible types of a MultiType.
            */
            enum Enum
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
        };

    public:
        /**
            @brief MT_ValueBase is an almost pure virtual baseclass of MT_Value<T>, which holds the value of the MultiType.
            This class is only used within the MultiType.
        */
        class _UtilExport MT_ValueBase
        {
        public:
            MT_ValueBase(Type::Enum type) : type_(type), bHasDefaultValue_(false) {}
            virtual ~MT_ValueBase() {}

            virtual MT_ValueBase* clone() const = 0;

            virtual void reset() = 0;
            virtual bool assimilate(const MultiType& other) = 0;

            /// Returns the type of the current value.
            const Type::Enum& getType() const { return this->type_; }

            /// Checks whether the value is a default one.
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

            Type::Enum type_;       ///< The type of the current value
            bool bHasDefaultValue_; ///< True if the last conversion wasn't successful
        };

        public:
            static const MultiType Null;

            inline MultiType()                                  : value_(0) {}                                      ///< Default constructor: Assigns no value and no type. The type will be determined by the first assignment of a value.
            inline MultiType(const char& value)                 : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const unsigned char& value)        : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const short& value)                : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const unsigned short& value)       : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const int& value)                  : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const unsigned int& value)         : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const long& value)                 : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const unsigned long& value)        : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const long long& value)            : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const unsigned long long& value)   : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const float& value)                : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const double& value)               : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const long double& value)          : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const bool& value)                 : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(      void* const& value)          : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const std::string& value)          : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Vector2& value)     : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Vector3& value)     : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Vector4& value)     : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::ColourValue& value) : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Quaternion& value)  : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Radian& value)      : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::Degree& value)      : value_(0) { this->assignValue(value); }           ///< Constructor: Assigns the given value and sets the type.
            inline MultiType(const orxonox::mbool& value)       : value_(0) { this->assignValue((bool)value); }     ///< Constructor: Assigns the given mbool and converts it to bool.
            inline MultiType(const char* value)                 : value_(0) { this->setValue(std::string(value)); } ///< Constructor: Converts the char array to a std::string, assigns the value and sets the type.
            inline MultiType(const MultiType& other)            : value_(0) { this->setValue(other); }              ///< Copyconstructor: Assigns value and type of the other MultiType.

            /// Destructor: Deletes the MT_Value.
            inline ~MultiType() { if (this->value_) { delete this->value_; } }

            template <typename V> inline MultiType& operator=(const V& value)         { this->setValue(value); return (*this); } ///< Assigns a new value. The value will be converted to the current type of the MultiType.
            template <typename V> inline MultiType& operator=(V* value)               { this->setValue(value); return (*this); } ///< Assigns a pointer.
            inline                       MultiType& operator=(const MultiType& other) { this->setValue(other); return (*this); } ///< Assigns the value of the other MultiType and converts it to the current type of the MultiType.

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
            /// Assigns a pointer.
            template <typename V> inline bool setValue(V* value)
            {
                if (this->value_)
                    return this->value_->setValue(static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));
                else
                    return this->assignValue     (static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));
            }
            /// Assigns the value of the other MultiType and converts it to the current type.
            bool                                          setValue(const MultiType& other) { if (this->value_) { return this->value_->assimilate(other); } else { if (other.value_) { this->value_ = other.value_->clone(); } return true; } }
            /// Changes the type to T and assigns the new value (which might be of another type than T - it gets converted).
            template <typename T, typename V> inline bool setValue(const V& value) { this->setType<T>(); return this->setValue(value); }


            /// Copies the other MultiType by assigning value and type.
            inline void                       copy(const MultiType& other) { if (this == &other) { return; } if (this->value_) { delete this->value_; } this->value_ = (other.value_) ? other.value_->clone() : 0; }

            /// Converts the current value to type T.
            template <typename T> inline bool convert()                    { return this->setValue<T>((typename Loki::TypeTraits<T>::UnqualifiedReferredType)(*this));  }

            /// Resets value and type. Type will be void afterwards and null() returns true.
            inline void                       reset()                      { if (this->value_) delete this->value_; this->value_ = 0; }
            /// Current content gets overridden with default zero value
            inline void                       resetValue()                 { if (this->value_) this->value_->reset(); }

            /// Resets the value and changes the internal type to T.
            template <typename T> inline void setType()                    { this->assignValue(typename Loki::TypeTraits<T>::UnqualifiedReferredType()); }

            /// Returns true if the current type is T.
            template <typename T> inline bool isType()               const { return false; } // Only works for specialized values - see below
            std::string                       getTypename()          const;

            /// Saves the value of the MT to a bytestream (pointed at by mem) and increases mem pointer by size of MT
            inline void                       exportData(uint8_t*& mem) const { assert(sizeof(Type::Enum)<=8); *static_cast<uint8_t*>(mem) = this->getType(); mem+=sizeof(uint8_t); this->value_->exportData(mem); }
            /// Loads the value of the MT from a bytestream (pointed at by mem) and increases mem pointer by size of MT
            inline void                       importData(uint8_t*& mem) { assert(sizeof(Type::Enum)<=8); this->setType(static_cast<Type::Enum>(*static_cast<uint8_t*>(mem))); mem+=sizeof(uint8_t); this->value_->importData(mem); }
            /// Saves the value of the MT to a bytestream and increases pointer to bytestream by size of MT
            inline uint8_t*&                  operator << (uint8_t*& mem) { importData(mem); return mem; }
            /// Loads the value of the MT to a bytestream and increases pointer to bytestream by size of MT
            inline void                       operator >> (uint8_t*& mem) const { exportData(mem); }
            inline uint32_t                   getNetworkSize() const { assert(this->value_); return this->value_->getSize() + sizeof(uint8_t); }

            /// Checks whether the value is a default one (assigned after a failed conversion)
            bool                              hasDefaultValue() const { return this->value_->hasDefaultValue(); }

            /// Checks if the MT contains no value.
            bool                              null() const { return (!this->value_); }

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
            /// Returns the current value, converted to a T* pointer.
            template <class T> operator T*() const { return (static_cast<T*>(this->operator void*())); }

            inline bool getValue(char*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(unsigned char*        value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(short*                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(unsigned short*       value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(int*                  value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(unsigned int*         value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(long*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(unsigned long*        value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(long long*            value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(unsigned long long*   value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(float*                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(double*               value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(long double*          value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(bool*                 value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(void**                value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(std::string*          value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Vector2*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Vector3*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Vector4*     value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::ColourValue* value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Quaternion*  value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Radian*      value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.
            inline bool getValue(orxonox::Degree*      value) const { if (this->value_) { return this->value_->getValue(value); } return false; } ///< Assigns the value to the given pointer. The value gets converted if the types don't match.

            inline char                     getChar()             const { return this->operator char();                 } ///< Returns the current value, converted to the requested type.
            inline unsigned char            getUnsignedChar()     const { return this->operator unsigned char();        } ///< Returns the current value, converted to the requested type.
            inline short                    getShort()            const { return this->operator short();                } ///< Returns the current value, converted to the requested type.
            inline unsigned short           getUnsignedShort()    const { return this->operator unsigned short();       } ///< Returns the current value, converted to the requested type.
            inline int                      getInt()              const { return this->operator int();                  } ///< Returns the current value, converted to the requested type.
            inline unsigned int             getUnsignedInt()      const { return this->operator unsigned int();         } ///< Returns the current value, converted to the requested type.
            inline long                     getLong()             const { return this->operator long();                 } ///< Returns the current value, converted to the requested type.
            inline unsigned long            getUnsignedLong()     const { return this->operator unsigned long();        } ///< Returns the current value, converted to the requested type.
            inline long long                getLongLong()         const { return this->operator long long();            } ///< Returns the current value, converted to the requested type.
            inline unsigned long long       getUnsignedLongLong() const { return this->operator unsigned long long();   } ///< Returns the current value, converted to the requested type.
            inline float                    getFloat()            const { return this->operator float();                } ///< Returns the current value, converted to the requested type.
            inline double                   getDouble()           const { return this->operator double();               } ///< Returns the current value, converted to the requested type.
            inline long double              getLongDouble()       const { return this->operator long double();          } ///< Returns the current value, converted to the requested type.
            inline bool                     getBool()             const { return this->operator bool();                 } ///< Returns the current value, converted to the requested type.
            inline void*                    getVoid()             const { return this->operator void*();                } ///< Returns the current value, converted to the requested type.
            inline std::string              getString()           const { return this->operator std::string();          } ///< Returns the current value, converted to the requested type.
            inline orxonox::Vector2         getVector2()          const { return this->operator orxonox::Vector2();     } ///< Returns the current value, converted to the requested type.
            inline orxonox::Vector3         getVector3()          const { return this->operator orxonox::Vector3();     } ///< Returns the current value, converted to the requested type.
            inline orxonox::Vector4         getVector4()          const { return this->operator orxonox::Vector4();     } ///< Returns the current value, converted to the requested type.
            inline orxonox::ColourValue     getColourValue()      const { return this->operator orxonox::ColourValue(); } ///< Returns the current value, converted to the requested type.
            inline orxonox::Quaternion      getQuaternion()       const { return this->operator orxonox::Quaternion();  } ///< Returns the current value, converted to the requested type.
            inline orxonox::Radian          getRadian()           const { return this->operator orxonox::Radian();      } ///< Returns the current value, converted to the requested type.
            inline orxonox::Degree          getDegree()           const { return this->operator orxonox::Degree();      } ///< Returns the current value, converted to the requested type.
            template <typename T> inline T* getPointer()          const { return static_cast<T*>(this->getVoid());      } ///< Returns the current value, converted to a T* pointer.

        private:
            inline bool assignValue(const char& value)                 { if (this->value_ && this->value_->type_ == Type::Char)             { return this->value_->setValue(value); } else { this->changeValueContainer<char>(value);                 return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const unsigned char& value)        { if (this->value_ && this->value_->type_ == Type::UnsignedChar)     { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned char>(value);        return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const short& value)                { if (this->value_ && this->value_->type_ == Type::Short)            { return this->value_->setValue(value); } else { this->changeValueContainer<short>(value);                return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const unsigned short& value)       { if (this->value_ && this->value_->type_ == Type::UnsignedShort)    { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned short>(value);       return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const int& value)                  { if (this->value_ && this->value_->type_ == Type::Int)              { return this->value_->setValue(value); } else { this->changeValueContainer<int>(value);                  return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const unsigned int& value)         { if (this->value_ && this->value_->type_ == Type::UnsignedInt)      { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned int>(value);         return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const long& value)                 { if (this->value_ && this->value_->type_ == Type::Long)             { return this->value_->setValue(value); } else { this->changeValueContainer<long>(value);                 return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const unsigned long& value)        { if (this->value_ && this->value_->type_ == Type::UnsignedLong)     { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned long>(value);        return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const long long& value)            { if (this->value_ && this->value_->type_ == Type::LongLong)         { return this->value_->setValue(value); } else { this->changeValueContainer<long long>(value);            return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const unsigned long long& value)   { if (this->value_ && this->value_->type_ == Type::UnsignedLongLong) { return this->value_->setValue(value); } else { this->changeValueContainer<unsigned long long>(value);   return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const float& value)                { if (this->value_ && this->value_->type_ == Type::Float)            { return this->value_->setValue(value); } else { this->changeValueContainer<float>(value);                return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const double& value)               { if (this->value_ && this->value_->type_ == Type::Double)           { return this->value_->setValue(value); } else { this->changeValueContainer<double>(value);               return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const long double& value)          { if (this->value_ && this->value_->type_ == Type::LongDouble)       { return this->value_->setValue(value); } else { this->changeValueContainer<long double>(value);          return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const bool& value)                 { if (this->value_ && this->value_->type_ == Type::Bool)             { return this->value_->setValue(value); } else { this->changeValueContainer<bool>(value);                 return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(      void* const& value)          { if (this->value_ && this->value_->type_ == Type::VoidPointer)      { return this->value_->setValue(value); } else { this->changeValueContainer<void*>(value);                return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const std::string& value)          { if (this->value_ && this->value_->type_ == Type::String)           { return this->value_->setValue(value); } else { this->changeValueContainer<std::string>(value);          return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Vector2& value)     { if (this->value_ && this->value_->type_ == Type::Vector2)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector2>(value);     return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Vector3& value)     { if (this->value_ && this->value_->type_ == Type::Vector3)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector3>(value);     return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Vector4& value)     { if (this->value_ && this->value_->type_ == Type::Vector4)          { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector4>(value);     return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::ColourValue& value) { if (this->value_ && this->value_->type_ == Type::ColourValue)      { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::ColourValue>(value); return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Quaternion& value)  { if (this->value_ && this->value_->type_ == Type::Quaternion)       { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Quaternion>(value);  return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Radian& value)      { if (this->value_ && this->value_->type_ == Type::Radian)           { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Radian>(value);      return true; } } ///< Assigns a new value by changing type and creating a new container.
            inline bool assignValue(const orxonox::Degree& value)      { if (this->value_ && this->value_->type_ == Type::Degree)           { return this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Degree>(value);      return true; } } ///< Assigns a new value by changing type and creating a new container.

            /// Resets the value and changes the internal type to the given type.
            inline void       setType(Type::Enum type) { this->reset(); this->convert(type); this->resetValue(); }
            /// Returns the current type.
            inline Type::Enum getType()          const { return (this->value_) ? this->value_->type_ : Type::Null; }
            /// Converts the current value to the given type.
            bool              convert(Type::Enum type);

            /// Changes the value container.
            template <typename T> inline void changeValueContainer(const T& value) { if (this->value_) { delete this->value_; } this->createNewValueContainer<T>(value); }
            /// Creates a new value container (works only with specialized types).
            template <typename T>        void createNewValueContainer(const T& value) { /* STATIC ASSERT */ *****value; return false; }

            MT_ValueBase* value_; //!< A pointer to the value container
    };

    /// Puts the MultiType on a stream by using the native << operator of the current type.
    _UtilExport inline std::ostream& operator<<(std::ostream& outstream, const MultiType& mt) { if (mt.value_) { mt.value_->toString(outstream); } return outstream; }

    template <> inline bool MultiType::isType<void>()                 const { return this->null();                                                       } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<char>()                 const { return (this->value_ && this->value_->type_ == Type::Char);             } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<unsigned char>()        const { return (this->value_ && this->value_->type_ == Type::UnsignedChar);     } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<short>()                const { return (this->value_ && this->value_->type_ == Type::Short);            } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<unsigned short>()       const { return (this->value_ && this->value_->type_ == Type::UnsignedShort);    } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<int>()                  const { return (this->value_ && this->value_->type_ == Type::Int);              } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<unsigned int>()         const { return (this->value_ && this->value_->type_ == Type::UnsignedInt);      } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<long>()                 const { return (this->value_ && this->value_->type_ == Type::Long);             } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<unsigned long>()        const { return (this->value_ && this->value_->type_ == Type::UnsignedLong);     } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<long long>()            const { return (this->value_ && this->value_->type_ == Type::LongLong);         } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<unsigned long long>()   const { return (this->value_ && this->value_->type_ == Type::UnsignedLongLong); } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<float>()                const { return (this->value_ && this->value_->type_ == Type::Float);            } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<double>()               const { return (this->value_ && this->value_->type_ == Type::Double);           } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<long double>()          const { return (this->value_ && this->value_->type_ == Type::LongDouble);       } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<bool>()                 const { return (this->value_ && this->value_->type_ == Type::Bool);             } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<void*>()                const { return (this->value_ && this->value_->type_ == Type::VoidPointer);      } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<std::string>()          const { return (this->value_ && this->value_->type_ == Type::String);           } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Vector2>()     const { return (this->value_ && this->value_->type_ == Type::Vector2);          } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Vector3>()     const { return (this->value_ && this->value_->type_ == Type::Vector3);          } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Vector4>()     const { return (this->value_ && this->value_->type_ == Type::Vector4);          } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::ColourValue>() const { return (this->value_ && this->value_->type_ == Type::ColourValue);      } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Quaternion>()  const { return (this->value_ && this->value_->type_ == Type::Quaternion);       } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Radian>()      const { return (this->value_ && this->value_->type_ == Type::Radian);           } ///< Returns true if the current type equals the given type.
    template <> inline bool MultiType::isType<orxonox::Degree>()      const { return (this->value_ && this->value_->type_ == Type::Degree);           } ///< Returns true if the current type equals the given type.

    /// Deletes the content, type becomes void.
    template <> inline bool MultiType::convert<void>()                 { this->reset(); return true; }

    // Specialization to avoid ambiguities with the conversion operator
    template <> inline bool MultiType::convert<std::string>()          { return this->setValue<std::string>         (this->operator std::string());          } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Vector2>()     { return this->setValue<orxonox::Vector2>    (this->operator orxonox::Vector2());     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Vector3>()     { return this->setValue<orxonox::Vector3>    (this->operator orxonox::Vector3());     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Vector4>()     { return this->setValue<orxonox::Vector4>    (this->operator orxonox::Vector4());     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::ColourValue>() { return this->setValue<orxonox::ColourValue>(this->operator orxonox::ColourValue()); } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Quaternion>()  { return this->setValue<orxonox::Quaternion> (this->operator orxonox::Quaternion());  } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Radian>()      { return this->setValue<orxonox::Radian>     (this->operator orxonox::Radian());      } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<orxonox::Degree>()      { return this->setValue<orxonox::Degree>     (this->operator orxonox::Degree());      } ///< Converts the current value to the given type.

    // Specialization to avoid ambiguities with the conversion operator
    template <> inline bool MultiType::convert<const std::string&>()          { return this->convert<std::string>();          } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Vector2&>()     { return this->convert<orxonox::Vector2>();     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Vector3&>()     { return this->convert<orxonox::Vector3>();     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Vector4&>()     { return this->convert<orxonox::Vector4>();     } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::ColourValue&>() { return this->convert<orxonox::ColourValue>(); } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Quaternion&>()  { return this->convert<orxonox::Quaternion>();  } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Radian&>()      { return this->convert<orxonox::Radian>();      } ///< Converts the current value to the given type.
    template <> inline bool MultiType::convert<const orxonox::Degree&>()      { return this->convert<orxonox::Degree>();      } ///< Converts the current value to the given type.

    template <> _UtilExport void MultiType::createNewValueContainer(const char& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const unsigned char& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const short& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const unsigned short& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const int& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const unsigned int& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const long& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const unsigned long& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const long long& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const unsigned long long& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const float& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const double& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const bool& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const long double& value);
    template <> _UtilExport void MultiType::createNewValueContainer(      void* const& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const std::string& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Vector2& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Vector3& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Vector4& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::ColourValue& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Quaternion& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Radian& value);
    template <> _UtilExport void MultiType::createNewValueContainer(const orxonox::Degree& value);

    inline bool MultiType::setValue(const char& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const unsigned char& value)         { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const short& value)                 { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const unsigned short& value)        { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const int& value)                   { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const unsigned int& value)          { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const long& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const unsigned long& value)         { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const long long& value)             { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const unsigned long long& value)    { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const float& value)                 { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const double& value)                { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const long double& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const bool& value)                  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(      void* const& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const std::string& value)           { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Vector2& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Vector3& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Vector4& value)      { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::ColourValue& value)  { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Quaternion& value)   { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Radian& value)       { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const orxonox::Degree& value)       { if (this->value_) { return this->value_->setValue(value); } else { return this->assignValue(value); } } ///< Assigns the given value and converts it to the current type.

    /// Assigns the given value and converts it to the current type.
    inline bool MultiType::setValue(const char* value)                  { if (this->value_) { return this->value_->setValue(std::string(value)); } else { return this->assignValue(std::string(value)); } }
}

#endif /* _MultiType_H__ */
