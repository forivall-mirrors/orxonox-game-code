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
     - @ref orxonox::MultiType::set() "set(value)"

    If you assign another value of another type, the MultiType keeps "its" type and
    converts the new value to the old type.

    If you want to change the type, there are three possibilities:
     - @ref orxonox::MultiType::convert "convert<T>()" sets the type to T and converts the currently assigned value
     - @ref orxonox::MultiType::reset "reset<T>()" sets the type to T and resets the value to zero using zeroise<T>()
     - force<T>(value) assigns a new value and changes the type to T.

    Examples:
    @code
    MultiType a = 10;       // a has now the type int and the value 10
    a.set("3.14");          // a has still the type int and "3.14" gets converted, therefore the value is now 3
    a.force<float>("3.14"); // a has now the type float and "3.14" gets converted to 3.14f
    a.convert<bool>();      // converts 3.14f to bool, which is true
    a = false;              // assigns false, this is equivalent to a.setValue(false)
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
#include "Math.h"
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
            inline MT_ValueBase(void* data, Type::Enum type) : type_(type), bLastConversionSuccessful(true), data_(data) {}
            inline virtual ~MT_ValueBase() {}

            virtual MT_ValueBase* clone() const = 0;

            virtual void reset() = 0;

            /// Returns the type of the current value.
            inline const Type::Enum& getType() const { return this->type_; }
            /// Returns true if the type of the stored value is T.
            template <typename T> inline bool isType() const { return false; }

            /// Checks whether the value is a default one.
            inline bool lastConversionSuccessful()   const { return this->bLastConversionSuccessful; }

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

            virtual bool setValue(const MultiType& other)            = 0;

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

            template <typename T> T get() const
            {
                if (this->isType<T>())
                    return *reinterpret_cast<const T*>(this->data_);
                else
                {
                    T value;
                    this->getValue(&value);
                    return value;
                }
            }

            virtual void toString(std::ostream& outstream) const = 0;

            virtual void importData(uint8_t*& mem) = 0;
            virtual void exportData(uint8_t*& mem) const = 0;
            virtual uint8_t getSize() const = 0;

            Type::Enum type_;               ///< The type of the current value
            bool bLastConversionSuccessful; ///< True if the last conversion was successful
            void* data_;                    ///< For direct access to the value if the type is known
        };

        public:
            static const MultiType Null;

            /// Default constructor: Assigns no value and no type. The type will be determined by the first assignment of a value.
            inline MultiType()                       : value_(0) { }
            /// Constructor: Assigns the given value and sets the type.
            template <typename V>
            inline MultiType(const V& value)         : value_(0) { this->set(value); }
            /// Copyconstructor: Assigns value and type of the other MultiType.
            inline MultiType(const MultiType& other) : value_(0) { this->set(other); }

            /// Destructor: Deletes the MT_Value.
            inline ~MultiType() { if (this->value_) { delete this->value_; } }

            /// Assigns a new value. The value will be converted to the current type of the MultiType.
            template <typename V> inline MultiType& operator=(const V& value)         { this->set(value); return (*this); }
            /// Assigns a pointer.
            template <typename V> inline MultiType& operator=(V* value)               { this->set(value); return (*this); }
            /// Assigns the value of the other MultiType and converts it to the current type of the MultiType.
            inline                       MultiType& operator=(const MultiType& other) { this->set(other); return (*this); }

            /// Assigns the given value and converts it to the current type.
            template <typename V> inline bool set(const V& value)
            {
                if (this->value_)
                    return this->value_->setValue(value);

                this->assignValue(value);
                return true;
            }
            /// Assigns a pointer.
            template <typename V> inline bool set(V* value)
            {
                if (this->value_)
                    return this->value_->setValue(static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));

                this->assignValue(static_cast<void*>(const_cast<typename Loki::TypeTraits<V>::UnqualifiedType*>(value)));
                return true;
            }
            /// Assigns the value of the other MultiType and converts it to the current type.
            inline bool set(const MultiType& other)
            {
                if (this->value_)
                    return this->value_->setValue(other);
                else if (other.value_)
                    this->value_ = other.value_->clone();
                return true;
            }

            /// Changes the type to T and assigns the new value (which might be of another type than T - it gets converted).
            template <typename T, typename V> inline bool force(const V& value)
            {
                this->reset<T>();
                return this->set(value);
            }

            /// Copies the other MultiType by assigning value and type.
            inline void copy(const MultiType& other)
            {
                if (this == &other)
                    return;
                if (this->value_)
                    delete this->value_;
                this->value_ = (other.value_) ? other.value_->clone() : 0;
            }

            /// Converts the current value to type T.
            template <typename T> inline bool convert() { return this->force<T>(MultiType(*this)); }
//            template <typename T> inline bool convert() { return this->force<T>(this->get<typename Loki::TypeTraits<T>::UnqualifiedReferredType>()); }

            /// Resets value and type. Type will be void afterwards and null() returns true.
            inline void reset() { if (this->value_) delete this->value_; this->value_ = 0; }
            /// Resets the value and changes the internal type to T.
            template <typename T> inline void reset() { this->assignValue(typename Loki::TypeTraits<T>::UnqualifiedReferredType()); }
            /// Current value gets overridden with default zero value
            inline void resetValue() { if (this->value_) this->value_->reset(); }

            /// Returns true if the type of the current value is T.
            template <typename T> inline bool isType() const { return (this->value_ ? this->value_->isType<T>() : false); }
            std::string getTypename() const;

            /// Checks whether the last conversion was successful
            inline bool lastConversionSuccessful() const { return !this->value_ || this->value_->lastConversionSuccessful(); }

            /// Checks if the MT contains no value.
            inline bool null() const { return !this->value_; }

            inline operator char()                 const { return (this->value_ ? this->value_->get<char>()                 : 0); }
            inline operator unsigned char()        const { return (this->value_ ? this->value_->get<unsigned char>()        : 0); }
            inline operator short()                const { return (this->value_ ? this->value_->get<short>()                : 0); }
            inline operator unsigned short()       const { return (this->value_ ? this->value_->get<unsigned short>()       : 0); }
            inline operator int()                  const { return (this->value_ ? this->value_->get<int>()                  : 0); }
            inline operator unsigned int()         const { return (this->value_ ? this->value_->get<unsigned int>()         : 0); }
            inline operator long()                 const { return (this->value_ ? this->value_->get<long>()                 : 0); }
            inline operator unsigned long()        const { return (this->value_ ? this->value_->get<unsigned long>()        : 0); }
            inline operator long long()            const { return (this->value_ ? this->value_->get<long long>()            : 0); }
            inline operator unsigned long long()   const { return (this->value_ ? this->value_->get<unsigned long long>()   : 0); }
            inline operator float()                const { return (this->value_ ? this->value_->get<float>()                : 0); }
            inline operator double()               const { return (this->value_ ? this->value_->get<double>()               : 0); }
            inline operator long double()          const { return (this->value_ ? this->value_->get<long double>()          : 0); }
            inline operator bool()                 const { return (this->value_ ? this->value_->get<bool>()                 : 0); }
            inline operator void*()                const { return (this->value_ ? this->value_->get<void*>()                : 0); }
            inline operator std::string()          const { return (this->value_ ? this->value_->get<std::string>()          : NilValue<std::string>()); }
            inline operator orxonox::Vector2()     const { return (this->value_ ? this->value_->get<orxonox::Vector2>()     : NilValue<orxonox::Vector2>()); }
            inline operator orxonox::Vector3()     const { return (this->value_ ? this->value_->get<orxonox::Vector3>()     : NilValue<orxonox::Vector3>()); }
            inline operator orxonox::Vector4()     const { return (this->value_ ? this->value_->get<orxonox::Vector4>()     : NilValue<orxonox::Vector4>()); }
            inline operator orxonox::ColourValue() const { return (this->value_ ? this->value_->get<orxonox::ColourValue>() : NilValue<orxonox::ColourValue>()); }
            inline operator orxonox::Quaternion()  const { return (this->value_ ? this->value_->get<orxonox::Quaternion>()  : NilValue<orxonox::Quaternion>()); }
            inline operator orxonox::Radian()      const { return (this->value_ ? this->value_->get<orxonox::Radian>()      : NilValue<orxonox::Radian>()); }
            inline operator orxonox::Degree()      const { return (this->value_ ? this->value_->get<orxonox::Degree>()      : NilValue<orxonox::Degree>()); }
            /// Returns the current value, converted to a T* pointer.
            template <class T> operator T*() const { return (static_cast<T*>(this->operator void*())); }

            /// Assigns the value to the given pointer. The value gets converted if the types don't match.
            template <typename T> inline bool getValue(T* value) const { if (this->value_) { return this->value_->getValue(value); } return false; }

            /// Returns the current value, converted to the requested type.
            template <typename T> inline T get() const { return *this; }


            ///////////////////////////////
            // network-related functions //
            ///////////////////////////////
            /// Saves the value of the MT to a bytestream (pointed at by mem) and increases mem pointer by size of MT
            inline void exportData(uint8_t*& mem) const
            {
                assert(sizeof(Type::Enum) <= 8);
                *static_cast<uint8_t*>(mem) = this->getType();
                mem += sizeof(uint8_t);
                this->value_->exportData(mem);
            }
            /// Loads the value of the MT from a bytestream (pointed at by mem) and increases mem pointer by size of MT
            inline void importData(uint8_t*& mem)
            {
                assert(sizeof(Type::Enum) <= 8);
                this->setType(static_cast<Type::Enum>(*static_cast<uint8_t*>(mem)));
                mem += sizeof(uint8_t);
                this->value_->importData(mem);
            }
            /// Saves the value of the MT to a bytestream and increases pointer to bytestream by size of MT
            inline uint8_t*& operator<<(uint8_t*& mem)
            {
                importData(mem);
                return mem;
            }
            /// Loads the value of the MT to a bytestream and increases pointer to bytestream by size of MT
            inline void operator>>(uint8_t*& mem) const
            {
                exportData(mem);
            }
            inline uint32_t getNetworkSize() const
            {
                assert(this->value_);
                return this->value_->getSize() + sizeof(uint8_t);
            }

        private:
            /// Assigns a new value by changing type and creating a new container.
            template <typename T> inline void assignValue(const T& value)
            {
                if (this->isType<T>())
                    this->value_->setValue(value);
                else
                    this->changeValueContainer(value);
            }
            /// Assigns a new value by changing type and creating a new container (overload for pointers).
            template <typename T> inline void assignValue(T* const& value)
            {
                if (this->isType<void*>())
                    this->value_->setValue(static_cast<void*>(value));
                else
                    this->changeValueContainer<void*>(value);
            }

            /// Resets the value and changes the internal type to the given type.
            inline void setType(Type::Enum type) { this->reset(); this->convert(type); this->resetValue(); }
            /// Returns the current type.
            inline Type::Enum getType() const { return (this->value_) ? this->value_->type_ : Type::Null; }
            /// Converts the current value to the given type.
            bool convert(Type::Enum type);

            /// Changes the value container.
            template <typename T> inline void changeValueContainer(const T& value)
            {
                if (this->value_)
                    delete this->value_;
                this->createNewValueContainer(value);
            }
            /// Creates a new value container (works only with specialized types).
            template <typename T> inline void createNewValueContainer(const T& value) { /* STATIC ASSERT */ *****value; return false; }

            MT_ValueBase* value_; //!< A pointer to the value container
    };

    /// Puts the MultiType on a stream by using the native << operator of the current type.
    _UtilExport inline std::ostream& operator<<(std::ostream& outstream, const MultiType& mt)
    {
        if (mt.value_)
            mt.value_->toString(outstream);
        return outstream;
    }

    template <> inline bool MultiType::MT_ValueBase::isType<char>()                 const { return this->type_ == Type::Char;             }
    template <> inline bool MultiType::MT_ValueBase::isType<unsigned char>()        const { return this->type_ == Type::UnsignedChar;     }
    template <> inline bool MultiType::MT_ValueBase::isType<short>()                const { return this->type_ == Type::Short;            }
    template <> inline bool MultiType::MT_ValueBase::isType<unsigned short>()       const { return this->type_ == Type::UnsignedShort;    }
    template <> inline bool MultiType::MT_ValueBase::isType<int>()                  const { return this->type_ == Type::Int;              }
    template <> inline bool MultiType::MT_ValueBase::isType<unsigned int>()         const { return this->type_ == Type::UnsignedInt;      }
    template <> inline bool MultiType::MT_ValueBase::isType<long>()                 const { return this->type_ == Type::Long;             }
    template <> inline bool MultiType::MT_ValueBase::isType<unsigned long>()        const { return this->type_ == Type::UnsignedLong;     }
    template <> inline bool MultiType::MT_ValueBase::isType<long long>()            const { return this->type_ == Type::LongLong;         }
    template <> inline bool MultiType::MT_ValueBase::isType<unsigned long long>()   const { return this->type_ == Type::UnsignedLongLong; }
    template <> inline bool MultiType::MT_ValueBase::isType<float>()                const { return this->type_ == Type::Float;            }
    template <> inline bool MultiType::MT_ValueBase::isType<double>()               const { return this->type_ == Type::Double;           }
    template <> inline bool MultiType::MT_ValueBase::isType<long double>()          const { return this->type_ == Type::LongDouble;       }
    template <> inline bool MultiType::MT_ValueBase::isType<bool>()                 const { return this->type_ == Type::Bool;             }
    template <> inline bool MultiType::MT_ValueBase::isType<void*>()                const { return this->type_ == Type::VoidPointer;      }
    template <> inline bool MultiType::MT_ValueBase::isType<std::string>()          const { return this->type_ == Type::String;           }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Vector2>()     const { return this->type_ == Type::Vector2;          }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Vector3>()     const { return this->type_ == Type::Vector3;          }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Vector4>()     const { return this->type_ == Type::Vector4;          }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::ColourValue>() const { return this->type_ == Type::ColourValue;      }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Quaternion>()  const { return this->type_ == Type::Quaternion;       }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Radian>()      const { return this->type_ == Type::Radian;           }
    template <> inline bool MultiType::MT_ValueBase::isType<orxonox::Degree>()      const { return this->type_ == Type::Degree;           }

    template <> inline bool MultiType::set(const char* value)  { return this->set(std::string(value)); }
    template <> inline bool MultiType::set(const mbool& value) { return this->set((bool)value); }

    // Spezializations for void
    template <> inline bool MultiType::isType<void>() const { return this->null(); }
    template <> inline bool MultiType::convert<void>() { this->reset(); return true; }

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
}

#endif /* _MultiType_H__ */
