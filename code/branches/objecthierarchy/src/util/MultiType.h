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
    @file MultiType.h
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
*/

#ifndef _MultiType_H__
#define _MultiType_H__

#include "UtilPrereqs.h"

#include <boost/static_assert.hpp>

#include "Math.h"

/**
    @brief Enum of all possible types of a MultiType.
*/
enum MT_Type
{
    MT_null,
    MT_char,
    MT_uchar,
    MT_short,
    MT_ushort,
    MT_int,
    MT_uint,
    MT_long,
    MT_ulong,
    MT_longlong,
    MT_ulonglong,
    MT_float,
    MT_double,
    MT_longdouble,
    MT_bool,
    MT_void,
    MT_string,
    MT_vector2,
    MT_vector3,
    MT_vector4,
    MT_colourvalue,
    MT_quaternion,
    MT_radian,
    MT_degree
};

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
    template <typename T> friend struct MT_Value;

    /**
        @brief MT_ValueBase is an almost pure virtual baseclass of MT_Value<T>, which holds the value of the MultiType.
        This class is only used within the MultiType.
    */
    struct _UtilExport MT_ValueBase
    {
        MT_ValueBase(MT_Type type) : type_(type) {}
        virtual ~MT_ValueBase() {}

        virtual MT_ValueBase* clone() const = 0;

        virtual void reset() = 0;
        virtual void assimilate(const MultiType& other) = 0;

        /** @brief Returns the type of the current value. */
        const MT_Type& getType() const { return this->type_; }

        virtual void setValue(const char& value)                 = 0;
        virtual void setValue(const unsigned char& value)        = 0;
        virtual void setValue(const short& value)                = 0;
        virtual void setValue(const unsigned short& value)       = 0;
        virtual void setValue(const int& value)                  = 0;
        virtual void setValue(const unsigned int& value)         = 0;
        virtual void setValue(const long& value)                 = 0;
        virtual void setValue(const unsigned long& value)        = 0;
        virtual void setValue(const long long& value)            = 0;
        virtual void setValue(const unsigned long long& value)   = 0;
        virtual void setValue(const float& value)                = 0;
        virtual void setValue(const double& value)               = 0;
        virtual void setValue(const long double& value)          = 0;
        virtual void setValue(const bool& value)                 = 0;
        virtual void setValue(      void* const& value)          = 0;
        virtual void setValue(const std::string& value)          = 0;
        virtual void setValue(const orxonox::Vector2& value)     = 0;
        virtual void setValue(const orxonox::Vector3& value)     = 0;
        virtual void setValue(const orxonox::Vector4& value)     = 0;
        virtual void setValue(const orxonox::ColourValue& value) = 0;
        virtual void setValue(const orxonox::Quaternion& value)  = 0;
        virtual void setValue(const orxonox::Radian& value)      = 0;
        virtual void setValue(const orxonox::Degree& value)      = 0;

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

        MT_Type type_; //!< The type of the current value
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
        inline MultiType(MT_Type type)                      : value_(0) { this->setType(type); }                /** @brief Constructor: Sets the type, the next assignment will determine the value. */

        /** @brief Destructor: Deletes the MT_Value. */
        inline ~MultiType() { if (this->value_) { delete this->value_; } }

        template <typename V> inline const MultiType& operator=(const V& value)         { this->setValue(value); return (*this); } /** @brief Assigns a new value. The value will be converted to the current type of the MultiType. */
        template <typename V> inline const MultiType& operator=(V* value)               { this->setValue(value); return (*this); } /** @brief Assigns a pointer. */
        inline                       const MultiType& operator=(const MultiType& other) { this->setValue(other); return (*this); } /** @brief Assigns the value of the other MultiType and converts it to the current type of the MultiType. */
        inline                       const MultiType& operator=(MT_Type type)           { this->setType(type);   return (*this); } /** @brief Resets the value and changes the type. */

        inline void                                   setValue(const char& value);
        inline void                                   setValue(const unsigned char& value);
        inline void                                   setValue(const short& value);
        inline void                                   setValue(const unsigned short& value);
        inline void                                   setValue(const int& value);
        inline void                                   setValue(const unsigned int& value);
        inline void                                   setValue(const long& value);
        inline void                                   setValue(const unsigned long& value);
        inline void                                   setValue(const long long& value);
        inline void                                   setValue(const unsigned long long& value);
        inline void                                   setValue(const float& value);
        inline void                                   setValue(const double& value);
        inline void                                   setValue(const long double& value);
        inline void                                   setValue(const bool& value);
        inline void                                   setValue(      void* const& value);
        inline void                                   setValue(const std::string& value);
        inline void                                   setValue(const orxonox::Vector2& value);
        inline void                                   setValue(const orxonox::Vector3& value);
        inline void                                   setValue(const orxonox::Vector4& value);
        inline void                                   setValue(const orxonox::ColourValue& value);
        inline void                                   setValue(const orxonox::Quaternion& value);
        inline void                                   setValue(const orxonox::Radian& value);
        inline void                                   setValue(const orxonox::Degree& value);
        inline void                                   setValue(const char* value);
        /** @brief Assigns a pointer. */
        template <typename V> inline void             setValue(V* value)               { if (this->value_) { this->value_->setValue((void*)value); } else { this->assignValue((void*)value); } }
        /** @brief Assigns the value of the other MultiType and converts it to the current type. */
        void                                          setValue(const MultiType& other) { if (this->value_) { this->value_->assimilate(other); } else { if (other.value_) { this->value_ = other.value_->clone(); } } }
        /** @brief Changes the type to T and assigns the new value (which might be of another type than T - it gets converted). */
        template <typename T, typename V> inline void setValue(const V& value)         { this->setType<T>(); this->setValue(value); }


        /** @brief Copies the other MultiType by assigning value and type. */
        inline void                       copy(const MultiType& other)    { if (this == &other) { return; } if (this->value_) { delete this->value_; } this->value_ = (other.value_) ? other.value_->clone() : 0; }

        template <typename T> inline void convert()                       { this->setValue<T>((T)(*this));  } /** @brief Converts the current value to type T. */
        inline void                       convert(const MultiType& other) { this->convert(other.getType()); } /** @brief Converts the current value to the type of the other MultiType. */
        void                              convert(MT_Type type);

        /** @brief Resets the value to the defaultvalue of the current type. */
        inline void                       reset()                         { if (this->value_) { delete this->value_; this->value_ = 0; } }

        template <typename T> inline void setType()                       { this->assignValue(T());                            } /** @brief Resets the value and changes the internal type to T. */
        inline void                       setType(const MultiType& other) { this->setType(other.getType());                    } /** @brief Resets the value and changes the internal type to the type of the other MultiType. */
        inline void                       setType(MT_Type type)           { this->reset(); this->convert(type); this->reset(); } /** @brief Resets the value and changes the internal type to the given type. */

        /** @brief Returns the current type. */
        inline MT_Type                    getType()                 const { return (this->value_) ? this->value_->type_ : MT_null; }
        /** @brief Returns true if the current type equals the given type. */
        inline bool                       isType(MT_Type type)      const { return (this->value_) ? (this->value_->type_ == type) : (type == MT_null); }
        /** @brief Returns true if the current type is T. */
        template <typename T> inline bool isType()                  const { return false; } // Only works for specialized values - see below
        std::string                       getTypename()             const;

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
        template <class T> operator T*() const { return ((T*)this->operator void*()); }

        inline void getValue(char*                 value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(unsigned char*        value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(short*                value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(unsigned short*       value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(int*                  value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(unsigned int*         value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(long*                 value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(unsigned long*        value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(long long*            value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(unsigned long long*   value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(float*                value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(double*               value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(long double*          value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(bool*                 value) const { if (this->value_) { (*value) = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(void*                 value) const { if (this->value_) {   value  = (*this->value_); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(std::string*          value) const { if (this->value_) { (*value) = this->value_->operator std::string();          } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Vector2*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector2();     } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Vector3*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector3();     } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Vector4*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector4();     } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::ColourValue* value) const { if (this->value_) { (*value) = this->value_->operator orxonox::ColourValue(); } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Quaternion*  value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Quaternion();  } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Radian*      value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Radian();      } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */
        inline void getValue(orxonox::Degree*      value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Degree();      } } /** @brief Assigns the value to the given pointer. The value gets converted if the types don't match. */

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
        template <typename T> inline T* getPointer()          const { return ((T*)this->getVoid());                 } /** @brief Returns the current value, converted to a T* pointer. */

    private:
        inline void assignValue(const char& value)                 { if (this->value_ && this->value_->type_ == MT_char)        { this->value_->setValue(value); } else { this->changeValueContainer<char>(value);                 } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const unsigned char& value)        { if (this->value_ && this->value_->type_ == MT_uchar)       { this->value_->setValue(value); } else { this->changeValueContainer<unsigned char>(value);        } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const short& value)                { if (this->value_ && this->value_->type_ == MT_short)       { this->value_->setValue(value); } else { this->changeValueContainer<short>(value);                } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const unsigned short& value)       { if (this->value_ && this->value_->type_ == MT_ushort)      { this->value_->setValue(value); } else { this->changeValueContainer<unsigned short>(value);       } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const int& value)                  { if (this->value_ && this->value_->type_ == MT_int)         { this->value_->setValue(value); } else { this->changeValueContainer<int>(value);                  } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const unsigned int& value)         { if (this->value_ && this->value_->type_ == MT_uint)        { this->value_->setValue(value); } else { this->changeValueContainer<unsigned int>(value);         } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const long& value)                 { if (this->value_ && this->value_->type_ == MT_long)        { this->value_->setValue(value); } else { this->changeValueContainer<long>(value);                 } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const unsigned long& value)        { if (this->value_ && this->value_->type_ == MT_ulong)       { this->value_->setValue(value); } else { this->changeValueContainer<unsigned long>(value);        } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const long long& value)            { if (this->value_ && this->value_->type_ == MT_longlong)    { this->value_->setValue(value); } else { this->changeValueContainer<long long>(value);            } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const unsigned long long& value)   { if (this->value_ && this->value_->type_ == MT_ulonglong)   { this->value_->setValue(value); } else { this->changeValueContainer<unsigned long long>(value);   } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const float& value)                { if (this->value_ && this->value_->type_ == MT_float)       { this->value_->setValue(value); } else { this->changeValueContainer<float>(value);                } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const double& value)               { if (this->value_ && this->value_->type_ == MT_double)      { this->value_->setValue(value); } else { this->changeValueContainer<double>(value);               } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const long double& value)          { if (this->value_ && this->value_->type_ == MT_longdouble)  { this->value_->setValue(value); } else { this->changeValueContainer<long double>(value);          } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const bool& value)                 { if (this->value_ && this->value_->type_ == MT_bool)        { this->value_->setValue(value); } else { this->changeValueContainer<bool>(value);                 } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(      void* const& value)          { if (this->value_ && this->value_->type_ == MT_void)        { this->value_->setValue(value); } else { this->changeValueContainer<void*>(value);                } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const std::string& value)          { if (this->value_ && this->value_->type_ == MT_string)      { this->value_->setValue(value); } else { this->changeValueContainer<std::string>(value);          } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Vector2& value)     { if (this->value_ && this->value_->type_ == MT_vector2)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector2>(value);     } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Vector3& value)     { if (this->value_ && this->value_->type_ == MT_vector3)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector3>(value);     } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Vector4& value)     { if (this->value_ && this->value_->type_ == MT_vector4)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector4>(value);     } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::ColourValue& value) { if (this->value_ && this->value_->type_ == MT_colourvalue) { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::ColourValue>(value); } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Quaternion& value)  { if (this->value_ && this->value_->type_ == MT_quaternion)  { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Quaternion>(value);  } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Radian& value)      { if (this->value_ && this->value_->type_ == MT_radian)      { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Radian>(value);      } } /** @brief Assigns a new value by changing type and creating a new container. */
        inline void assignValue(const orxonox::Degree& value)      { if (this->value_ && this->value_->type_ == MT_degree)      { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Degree>(value);      } } /** @brief Assigns a new value by changing type and creating a new container. */

        /** @brief Changes the value container. */
        template <typename T> inline void changeValueContainer(const T& value) { if (this->value_) { delete this->value_; } this->createNewValueContainer<T>(value); }
        /** @brief Creates a new value container (works only with specialized types). */
        template <typename T>        void createNewValueContainer(const T& value) { BOOST_STATIC_ASSERT(sizeof(T) == 0); }

        MT_ValueBase* value_; //!< A pointer to the value container
};

/** @brief Puts the MultiType on a stream by using the native << operator of the current type. */
_UtilExport inline std::ostream& operator<<(std::ostream& outstream, const MultiType& mt) { if (mt.value_) { mt.value_->toString(outstream); } return outstream; }

template <> inline bool MultiType::isType<char>()                 const { return (this->value_ && this->value_->type_ == MT_char);        } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<unsigned char>()        const { return (this->value_ && this->value_->type_ == MT_uchar);       } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<short>()                const { return (this->value_ && this->value_->type_ == MT_short);       } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<unsigned short>()       const { return (this->value_ && this->value_->type_ == MT_ushort);      } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<int>()                  const { return (this->value_ && this->value_->type_ == MT_int);         } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<unsigned int>()         const { return (this->value_ && this->value_->type_ == MT_uint);        } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<long>()                 const { return (this->value_ && this->value_->type_ == MT_long);        } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<unsigned long>()        const { return (this->value_ && this->value_->type_ == MT_ulong);       } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<long long>()            const { return (this->value_ && this->value_->type_ == MT_longlong);    } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<unsigned long long>()   const { return (this->value_ && this->value_->type_ == MT_ulonglong);   } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<float>()                const { return (this->value_ && this->value_->type_ == MT_float);       } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<double>()               const { return (this->value_ && this->value_->type_ == MT_double);      } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<long double>()          const { return (this->value_ && this->value_->type_ == MT_longdouble);  } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<bool>()                 const { return (this->value_ && this->value_->type_ == MT_bool);        } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<void*>()                const { return (this->value_ && this->value_->type_ == MT_void);        } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<std::string>()          const { return (this->value_ && this->value_->type_ == MT_string);      } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Vector2>()     const { return (this->value_ && this->value_->type_ == MT_vector2);     } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Vector3>()     const { return (this->value_ && this->value_->type_ == MT_vector3);     } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Vector4>()     const { return (this->value_ && this->value_->type_ == MT_vector4);     } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::ColourValue>() const { return (this->value_ && this->value_->type_ == MT_colourvalue); } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Quaternion>()  const { return (this->value_ && this->value_->type_ == MT_quaternion);  } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Radian>()      const { return (this->value_ && this->value_->type_ == MT_radian);      } /** @brief Returns true if the current type equals the given type. */
template <> inline bool MultiType::isType<orxonox::Degree>()      const { return (this->value_ && this->value_->type_ == MT_degree);      } /** @brief Returns true if the current type equals the given type. */

// Specialization to avoid ambiguities with the conversion operator
template <> inline void MultiType::convert<std::string>()          { this->setValue<std::string>         (this->operator std::string());          } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Vector2>()     { this->setValue<orxonox::Vector2>    (this->operator orxonox::Vector2());     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Vector3>()     { this->setValue<orxonox::Vector3>    (this->operator orxonox::Vector3());     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Vector4>()     { this->setValue<orxonox::Vector4>    (this->operator orxonox::Vector4());     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::ColourValue>() { this->setValue<orxonox::ColourValue>(this->operator orxonox::ColourValue()); } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Quaternion>()  { this->setValue<orxonox::Quaternion> (this->operator orxonox::Quaternion());  } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Radian>()      { this->setValue<orxonox::Radian>     (this->operator orxonox::Radian());      } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<orxonox::Degree>()      { this->setValue<orxonox::Degree>     (this->operator orxonox::Degree());      } /** @brief Converts the current value to the given type. */

// Specialization to avoid ambiguities with the conversion operator
template <> inline void MultiType::convert<const std::string&>()          { this->convert<std::string>();          } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Vector2&>()     { this->convert<orxonox::Vector2>();     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Vector3&>()     { this->convert<orxonox::Vector3>();     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Vector4&>()     { this->convert<orxonox::Vector4>();     } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::ColourValue&>() { this->convert<orxonox::ColourValue>(); } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Quaternion&>()  { this->convert<orxonox::Quaternion>();  } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Radian&>()      { this->convert<orxonox::Radian>();      } /** @brief Converts the current value to the given type. */
template <> inline void MultiType::convert<const orxonox::Degree&>()      { this->convert<orxonox::Degree>();      } /** @brief Converts the current value to the given type. */

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

inline void MultiType::setValue(const char& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const unsigned char& value)         { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const short& value)                 { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const unsigned short& value)        { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const int& value)                   { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const unsigned int& value)          { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const long& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const unsigned long& value)         { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const long long& value)             { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const unsigned long long& value)    { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const float& value)                 { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const double& value)                { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const long double& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const bool& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(      void* const& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const std::string& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Vector2& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Vector3& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Vector4& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::ColourValue& value)  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Quaternion& value)   { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Radian& value)       { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */
inline void MultiType::setValue(const orxonox::Degree& value)       { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } } /** @brief Assigns the given value and converts it to the current type. */

inline void MultiType::setValue(const char* value)                  { if (this->value_) { this->value_->setValue(std::string(value)); } else { this->assignValue(std::string(value)); } }  /** @brief Assigns the given value and converts it to the current type. */

#endif /* _MultiType_H__ */
