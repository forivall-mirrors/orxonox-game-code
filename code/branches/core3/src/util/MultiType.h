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

#ifndef _MultiType_H__
#define _MultiType_H__

#include "UtilPrereqs.h"

#include <boost/static_assert.hpp>

#include "Math.h"

// disable annoying warning about multiple assignment operators
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4522)
#endif

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

class _UtilExport MultiType
{
    _UtilExport friend std::ostream& operator<<(std::ostream& outstream, const MultiType& mt);
    template <typename T> friend struct MT_Value;

    struct _UtilExport MT_ValueBase
    {
        MT_ValueBase(MT_Type type) : type_(type) {}
        virtual ~MT_ValueBase() {}

        virtual MT_ValueBase* clone() const = 0;

        virtual void reset() = 0;
        virtual void assimilate(const MultiType& other) = 0;
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

        MT_Type type_;
    };

    public:
        inline MultiType()                                  : value_(0) {}
        inline MultiType(const char& value)                 : value_(0) { this->assignValue(value); }
        inline MultiType(const unsigned char& value)        : value_(0) { this->assignValue(value); }
        inline MultiType(const short& value)                : value_(0) { this->assignValue(value); }
        inline MultiType(const unsigned short& value)       : value_(0) { this->assignValue(value); }
        inline MultiType(const int& value)                  : value_(0) { this->assignValue(value); }
        inline MultiType(const unsigned int& value)         : value_(0) { this->assignValue(value); }
        inline MultiType(const long& value)                 : value_(0) { this->assignValue(value); }
        inline MultiType(const unsigned long& value)        : value_(0) { this->assignValue(value); }
        inline MultiType(const long long& value)            : value_(0) { this->assignValue(value); }
        inline MultiType(const unsigned long long& value)   : value_(0) { this->assignValue(value); }
        inline MultiType(const float& value)                : value_(0) { this->assignValue(value); }
        inline MultiType(const double& value)               : value_(0) { this->assignValue(value); }
        inline MultiType(const long double& value)          : value_(0) { this->assignValue(value); }
        inline MultiType(const bool& value)                 : value_(0) { this->assignValue(value); }
        inline MultiType(      void* const& value)          : value_(0) { this->assignValue(value); }
        inline MultiType(const std::string& value)          : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Vector2& value)     : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Vector3& value)     : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Vector4& value)     : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::ColourValue& value) : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Quaternion& value)  : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Radian& value)      : value_(0) { this->assignValue(value); }
        inline MultiType(const orxonox::Degree& value)      : value_(0) { this->assignValue(value); }
        inline MultiType(const char* value)                 : value_(0) { this->setValue(std::string(value)); }
        inline MultiType(const MultiType& other)            : value_(0) { this->setValue(other); }
        inline MultiType(MT_Type type)                      : value_(0) { this->setType(type); }

        inline ~MultiType() { if (this->value_) { delete this->value_; } }

        template <typename V> inline MultiType& operator=(const V& value)         { this->setValue(value); return (*this); }
        inline                       MultiType& operator=(const MultiType& other) { this->setValue(other); return (*this); }
        inline                       MultiType& operator=(MT_Type type)           { this->setType(type);   return (*this); }

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
        template <typename V> inline void             setValue(V* value)               { if (this->value_) { this->value_->setValue((void*)value); } else { this->assignValue((void*)value); } }
        void                                          setValue(const MultiType& other) { if (this->value_) { this->value_->assimilate(other); } else { if (other.value_) { this->value_ = other.value_->clone(); } } }
        template <typename T, typename V> inline void setValue(const V& value)         { this->setType<T>(); this->setValue(value); }
        inline void                                   setValue(const char* value);

        inline void                       copy(const MultiType& other)    { if (this->value_) { delete this->value_; } this->value_ = (other.value_) ? other.value_->clone() : 0; }

        template <typename T> inline void convert()                       { this->setValue<T>((T)(*this));  }
        inline void                       convert(const MultiType& other) { this->convert(other.getType()); }
        void                              convert(MT_Type type);

        inline void                       reset()                         { if (this->value_) { delete this->value_; this->value_ = 0; } }

        template <typename T> inline void setType()                       { this->assignValue(T());             }
        inline void                       setType(const MultiType& other) { this->setType(other.getType());     }
        inline void                       setType(MT_Type type)           { this->reset(); this->convert(type); }

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
        template <class T> operator T*() const { return ((T*)this->operator void*()); }

        inline void getValue(char*                 value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(unsigned char*        value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(short*                value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(unsigned short*       value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(int*                  value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(unsigned int*         value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(long*                 value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(unsigned long*        value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(long long*            value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(unsigned long long*   value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(float*                value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(double*               value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(long double*          value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(bool*                 value) const { if (this->value_) { (*value) = (*this->value_); } }
        inline void getValue(void*                 value) const { if (this->value_) {   value  = (*this->value_); } }
        inline void getValue(std::string*          value) const { if (this->value_) { (*value) = this->value_->operator std::string();          } }
        inline void getValue(orxonox::Vector2*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector2();     } }
        inline void getValue(orxonox::Vector3*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector3();     } }
        inline void getValue(orxonox::Vector4*     value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Vector4();     } }
        inline void getValue(orxonox::ColourValue* value) const { if (this->value_) { (*value) = this->value_->operator orxonox::ColourValue(); } }
        inline void getValue(orxonox::Quaternion*  value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Quaternion();  } }
        inline void getValue(orxonox::Radian*      value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Radian();      } }
        inline void getValue(orxonox::Degree*      value) const { if (this->value_) { (*value) = this->value_->operator orxonox::Degree();      } }

        inline MT_Type                    getType()            const { return (this->value_) ? this->value_->type_ : MT_null; }
        inline bool                       isType(MT_Type type) const { return (this->value_) ? (this->value_->type_ == type) : (type == MT_null); }
        template <typename T> inline bool isType()             const { return false; }
        std::string                       getTypename()        const;

        inline std::string toString() const { return this->operator std::string(); }

    private:
        inline void assignValue(const char& value)                 { if (this->value_ && this->value_->type_ == MT_char)        { this->value_->setValue(value); } else { this->changeValueContainer<char>(value);                 } }
        inline void assignValue(const unsigned char& value)        { if (this->value_ && this->value_->type_ == MT_uchar)       { this->value_->setValue(value); } else { this->changeValueContainer<unsigned char>(value);        } }
        inline void assignValue(const short& value)                { if (this->value_ && this->value_->type_ == MT_short)       { this->value_->setValue(value); } else { this->changeValueContainer<short>(value);                } }
        inline void assignValue(const unsigned short& value)       { if (this->value_ && this->value_->type_ == MT_ushort)      { this->value_->setValue(value); } else { this->changeValueContainer<unsigned short>(value);       } }
        inline void assignValue(const int& value)                  { if (this->value_ && this->value_->type_ == MT_int)         { this->value_->setValue(value); } else { this->changeValueContainer<int>(value);                  } }
        inline void assignValue(const unsigned int& value)         { if (this->value_ && this->value_->type_ == MT_uint)        { this->value_->setValue(value); } else { this->changeValueContainer<unsigned int>(value);         } }
        inline void assignValue(const long& value)                 { if (this->value_ && this->value_->type_ == MT_long)        { this->value_->setValue(value); } else { this->changeValueContainer<long>(value);                 } }
        inline void assignValue(const unsigned long& value)        { if (this->value_ && this->value_->type_ == MT_ulong)       { this->value_->setValue(value); } else { this->changeValueContainer<unsigned long>(value);        } }
        inline void assignValue(const long long& value)            { if (this->value_ && this->value_->type_ == MT_longlong)    { this->value_->setValue(value); } else { this->changeValueContainer<long long>(value);            } }
        inline void assignValue(const unsigned long long& value)   { if (this->value_ && this->value_->type_ == MT_ulonglong)   { this->value_->setValue(value); } else { this->changeValueContainer<unsigned long long>(value);   } }
        inline void assignValue(const float& value)                { if (this->value_ && this->value_->type_ == MT_float)       { this->value_->setValue(value); } else { this->changeValueContainer<float>(value);                } }
        inline void assignValue(const double& value)               { if (this->value_ && this->value_->type_ == MT_double)      { this->value_->setValue(value); } else { this->changeValueContainer<double>(value);               } }
        inline void assignValue(const long double& value)          { if (this->value_ && this->value_->type_ == MT_longdouble)  { this->value_->setValue(value); } else { this->changeValueContainer<long double>(value);          } }
        inline void assignValue(const bool& value)                 { if (this->value_ && this->value_->type_ == MT_bool)        { this->value_->setValue(value); } else { this->changeValueContainer<bool>(value);                 } }
        inline void assignValue(      void* const& value)          { if (this->value_ && this->value_->type_ == MT_void)        { this->value_->setValue(value); } else { this->changeValueContainer<void*>(value);                } }
        inline void assignValue(const std::string& value)          { if (this->value_ && this->value_->type_ == MT_string)      { this->value_->setValue(value); } else { this->changeValueContainer<std::string>(value);          } }
        inline void assignValue(const orxonox::Vector2& value)     { if (this->value_ && this->value_->type_ == MT_vector2)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector2>(value);     } }
        inline void assignValue(const orxonox::Vector3& value)     { if (this->value_ && this->value_->type_ == MT_vector3)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector3>(value);     } }
        inline void assignValue(const orxonox::Vector4& value)     { if (this->value_ && this->value_->type_ == MT_vector4)     { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Vector4>(value);     } }
        inline void assignValue(const orxonox::ColourValue& value) { if (this->value_ && this->value_->type_ == MT_colourvalue) { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::ColourValue>(value); } }
        inline void assignValue(const orxonox::Quaternion& value)  { if (this->value_ && this->value_->type_ == MT_quaternion)  { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Quaternion>(value);  } }
        inline void assignValue(const orxonox::Radian& value)      { if (this->value_ && this->value_->type_ == MT_radian)      { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Radian>(value);      } }
        inline void assignValue(const orxonox::Degree& value)      { if (this->value_ && this->value_->type_ == MT_degree)      { this->value_->setValue(value); } else { this->changeValueContainer<orxonox::Degree>(value);      } }

        template <typename T> inline void changeValueContainer(const T& value) { if (this->value_) { delete this->value_; } this->createNewValueContainer<T>(value); }
        template <typename T>        void createNewValueContainer(const T& value) { BOOST_STATIC_ASSERT(sizeof(T) == 0); }

        MT_ValueBase* value_;
};

_UtilExport inline std::ostream& operator<<(std::ostream& outstream, const MultiType& mt) { if (mt.value_) { mt.value_->toString(outstream); } return outstream; }

template <> inline bool MultiType::isType<char>()                 const { return (this->value_ && this->value_->type_ == MT_char);        }
template <> inline bool MultiType::isType<unsigned char>()        const { return (this->value_ && this->value_->type_ == MT_uchar);       }
template <> inline bool MultiType::isType<short>()                const { return (this->value_ && this->value_->type_ == MT_short);       }
template <> inline bool MultiType::isType<unsigned short>()       const { return (this->value_ && this->value_->type_ == MT_ushort);      }
template <> inline bool MultiType::isType<int>()                  const { return (this->value_ && this->value_->type_ == MT_int);         }
template <> inline bool MultiType::isType<unsigned int>()         const { return (this->value_ && this->value_->type_ == MT_uint);        }
template <> inline bool MultiType::isType<long>()                 const { return (this->value_ && this->value_->type_ == MT_long);        }
template <> inline bool MultiType::isType<unsigned long>()        const { return (this->value_ && this->value_->type_ == MT_ulong);       }
template <> inline bool MultiType::isType<long long>()            const { return (this->value_ && this->value_->type_ == MT_longlong);    }
template <> inline bool MultiType::isType<unsigned long long>()   const { return (this->value_ && this->value_->type_ == MT_ulonglong);   }
template <> inline bool MultiType::isType<float>()                const { return (this->value_ && this->value_->type_ == MT_float);       }
template <> inline bool MultiType::isType<double>()               const { return (this->value_ && this->value_->type_ == MT_double);      }
template <> inline bool MultiType::isType<long double>()          const { return (this->value_ && this->value_->type_ == MT_longdouble);  }
template <> inline bool MultiType::isType<bool>()                 const { return (this->value_ && this->value_->type_ == MT_bool);        }
template <> inline bool MultiType::isType<void*>()                const { return (this->value_ && this->value_->type_ == MT_void);        }
template <> inline bool MultiType::isType<std::string>()          const { return (this->value_ && this->value_->type_ == MT_string);      }
template <> inline bool MultiType::isType<orxonox::Vector2>()     const { return (this->value_ && this->value_->type_ == MT_vector2);     }
template <> inline bool MultiType::isType<orxonox::Vector3>()     const { return (this->value_ && this->value_->type_ == MT_vector3);     }
template <> inline bool MultiType::isType<orxonox::Vector4>()     const { return (this->value_ && this->value_->type_ == MT_vector4);     }
template <> inline bool MultiType::isType<orxonox::ColourValue>() const { return (this->value_ && this->value_->type_ == MT_colourvalue); }
template <> inline bool MultiType::isType<orxonox::Quaternion>()  const { return (this->value_ && this->value_->type_ == MT_quaternion);  }
template <> inline bool MultiType::isType<orxonox::Radian>()      const { return (this->value_ && this->value_->type_ == MT_radian);      }
template <> inline bool MultiType::isType<orxonox::Degree>()      const { return (this->value_ && this->value_->type_ == MT_degree);      }

template <> inline void MultiType::convert<std::string>()          { this->setValue<std::string>         (this->operator std::string());          }
template <> inline void MultiType::convert<orxonox::Vector2>()     { this->setValue<orxonox::Vector2>    (this->operator orxonox::Vector2());     }
template <> inline void MultiType::convert<orxonox::Vector3>()     { this->setValue<orxonox::Vector3>    (this->operator orxonox::Vector3());     }
template <> inline void MultiType::convert<orxonox::Vector4>()     { this->setValue<orxonox::Vector4>    (this->operator orxonox::Vector4());     }
template <> inline void MultiType::convert<orxonox::ColourValue>() { this->setValue<orxonox::ColourValue>(this->operator orxonox::ColourValue()); }
template <> inline void MultiType::convert<orxonox::Quaternion>()  { this->setValue<orxonox::Quaternion> (this->operator orxonox::Quaternion());  }
template <> inline void MultiType::convert<orxonox::Radian>()      { this->setValue<orxonox::Radian>     (this->operator orxonox::Radian());      }
template <> inline void MultiType::convert<orxonox::Degree>()      { this->setValue<orxonox::Degree>     (this->operator orxonox::Degree());      }

template <> inline void MultiType::convert<const std::string&>()          { this->convert<std::string>();          }
template <> inline void MultiType::convert<const orxonox::Vector2&>()     { this->convert<orxonox::Vector2>();     }
template <> inline void MultiType::convert<const orxonox::Vector3&>()     { this->convert<orxonox::Vector3>();     }
template <> inline void MultiType::convert<const orxonox::Vector4&>()     { this->convert<orxonox::Vector4>();     }
template <> inline void MultiType::convert<const orxonox::ColourValue&>() { this->convert<orxonox::ColourValue>(); }
template <> inline void MultiType::convert<const orxonox::Quaternion&>()  { this->convert<orxonox::Quaternion>();  }
template <> inline void MultiType::convert<const orxonox::Radian&>()      { this->convert<orxonox::Radian>();      }
template <> inline void MultiType::convert<const orxonox::Degree&>()      { this->convert<orxonox::Degree>();      }

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

inline void MultiType::setValue(const char& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const unsigned char& value)         { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const short& value)                 { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const unsigned short& value)        { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const int& value)                   { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const unsigned int& value)          { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const long& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const unsigned long& value)         { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const long long& value)             { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const unsigned long long& value)    { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const float& value)                 { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const double& value)                { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const long double& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const bool& value)                  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(      void* const& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const std::string& value)           { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Vector2& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Vector3& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Vector4& value)      { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::ColourValue& value)  { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Quaternion& value)   { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Radian& value)       { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }
inline void MultiType::setValue(const orxonox::Degree& value)       { if (this->value_) { this->value_->setValue(value); } else { this->assignValue(value); } }

inline void MultiType::setValue(const char* value)                  { if (this->value_) { this->value_->setValue(std::string(value)); } else { this->assignValue(std::string(value)); } }


#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif /* _MultiType_H__ */
