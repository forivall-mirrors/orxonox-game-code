/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *   Inspiration: MultiType by Benjamin Grauer
 */

#ifndef _MultiTypePrimitive_H__
#define _MultiTypePrimitive_H__

#include <ostream>

#include "UtilPrereqs.h"

#include "MultiType.h"

class _UtilExport MultiTypePrimitive
{
    public:
        MultiTypePrimitive(MultiType      type = MT_null);
        inline MultiTypePrimitive(int            value) { this->setValue(value); }
        inline MultiTypePrimitive(unsigned int   value) { this->setValue(value); }
        inline MultiTypePrimitive(char           value) { this->setValue(value); }
        inline MultiTypePrimitive(unsigned char  value) { this->setValue(value); }
        inline MultiTypePrimitive(short          value) { this->setValue(value); }
        inline MultiTypePrimitive(unsigned short value) { this->setValue(value); }
        inline MultiTypePrimitive(long           value) { this->setValue(value); }
        inline MultiTypePrimitive(unsigned long  value) { this->setValue(value); }
        inline MultiTypePrimitive(float          value) { this->setValue(value); }
        inline MultiTypePrimitive(double         value) { this->setValue(value); }
        inline MultiTypePrimitive(long double    value) { this->setValue(value); }
        inline MultiTypePrimitive(bool           value) { this->setValue(value); }
        inline MultiTypePrimitive(const MultiTypePrimitive& mtp) { this->setValue(mtp); }

        inline MultiTypePrimitive& operator=(MultiType      value) { this->type_ = MT_null; return *this; }
        inline MultiTypePrimitive& operator=(int            value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(unsigned int   value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(char           value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(unsigned char  value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(short          value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(unsigned short value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(long           value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(unsigned long  value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(float          value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(double         value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(long double    value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(bool           value) { this->setValue(value); return *this; }
        inline MultiTypePrimitive& operator=(const MultiTypePrimitive& mtp) { this->setValue(mtp); return *this; }

        inline bool operator==(int            value) const { return (this->value_.int_        == value); }
        inline bool operator==(unsigned int   value) const { return (this->value_.uint_       == value); }
        inline bool operator==(char           value) const { return (this->value_.char_       == value); }
        inline bool operator==(unsigned char  value) const { return (this->value_.uchar_      == value); }
        inline bool operator==(short          value) const { return (this->value_.short_      == value); }
        inline bool operator==(unsigned short value) const { return (this->value_.ushort_     == value); }
        inline bool operator==(long           value) const { return (this->value_.long_       == value); }
        inline bool operator==(unsigned long  value) const { return (this->value_.ulong_      == value); }
        inline bool operator==(float          value) const { return (this->value_.float_      == value); }
        inline bool operator==(double         value) const { return (this->value_.double_     == value); }
        inline bool operator==(long double    value) const { return (this->value_.longdouble_ == value); }
        inline bool operator==(bool           value) const { return (this->value_.bool_       == value); }
        bool operator==(const MultiTypePrimitive& mtp) const;

        inline bool operator!=(int            value) const { return (this->value_.int_        != value); }
        inline bool operator!=(unsigned int   value) const { return (this->value_.uint_       != value); }
        inline bool operator!=(char           value) const { return (this->value_.char_       != value); }
        inline bool operator!=(unsigned char  value) const { return (this->value_.uchar_      != value); }
        inline bool operator!=(short          value) const { return (this->value_.short_      != value); }
        inline bool operator!=(unsigned short value) const { return (this->value_.ushort_     != value); }
        inline bool operator!=(long           value) const { return (this->value_.long_       != value); }
        inline bool operator!=(unsigned long  value) const { return (this->value_.ulong_      != value); }
        inline bool operator!=(float          value) const { return (this->value_.float_      != value); }
        inline bool operator!=(double         value) const { return (this->value_.double_     != value); }
        inline bool operator!=(long double    value) const { return (this->value_.longdouble_ != value); }
        inline bool operator!=(bool           value) const { return (this->value_.bool_       != value); }
        bool operator!=(const MultiTypePrimitive& mtp) const;

        operator int()            const;
        operator unsigned int()   const;
        operator char()           const;
        operator unsigned char()  const;
        operator short()          const;
        operator unsigned short() const;
        operator long()           const;
        operator unsigned long()  const;
        operator float ()         const;
        operator double ()        const;
        operator long double()    const;
        operator bool()           const;

        inline void setValue(int            value) { this->type_ = MT_int;        this->value_.int_        = value; }
        inline void setValue(unsigned int   value) { this->type_ = MT_uint;       this->value_.uint_       = value; }
        inline void setValue(char           value) { this->type_ = MT_char;       this->value_.char_       = value; }
        inline void setValue(unsigned char  value) { this->type_ = MT_uchar;      this->value_.uchar_      = value; }
        inline void setValue(short          value) { this->type_ = MT_short;      this->value_.short_      = value; }
        inline void setValue(unsigned short value) { this->type_ = MT_ushort;     this->value_.ushort_     = value; }
        inline void setValue(long           value) { this->type_ = MT_long;       this->value_.long_       = value; }
        inline void setValue(unsigned long  value) { this->type_ = MT_ulong;      this->value_.ulong_      = value; }
        inline void setValue(float          value) { this->type_ = MT_float;      this->value_.float_      = value; }
        inline void setValue(double         value) { this->type_ = MT_double;     this->value_.double_     = value; }
        inline void setValue(long double    value) { this->type_ = MT_longdouble; this->value_.longdouble_ = value; }
        inline void setValue(bool           value) { this->type_ = MT_bool;       this->value_.bool_       = value; }
        void setValue(const MultiTypePrimitive& mtp);

        inline int            getInt()           const { return this->value_.int_;        }
        inline unsigned int   getUnsignedInt()   const { return this->value_.uint_;       }
        inline char           getChar()          const { return this->value_.char_;       }
        inline unsigned char  getUnsignedChar()  const { return this->value_.uchar_;      }
        inline short          getShort()         const { return this->value_.short_;      }
        inline unsigned short getUnsignedShort() const { return this->value_.ushort_;     }
        inline long           getLong()          const { return this->value_.long_;       }
        inline unsigned long  getUnsignedLong()  const { return this->value_.ulong_;      }
        inline float          getFloat()         const { return this->value_.float_;      }
        inline double         getDouble()        const { return this->value_.double_;     }
        inline long double    getLongDouble()    const { return this->value_.longdouble_; }
        inline bool           getBool()          const { return this->value_.bool_;       }

        inline int&            getInt()           { return this->value_.int_;        }
        inline unsigned int&   getUnsignedInt()   { return this->value_.uint_;       }
        inline char&           getChar()          { return this->value_.char_;       }
        inline unsigned char&  getUnsignedChar()  { return this->value_.uchar_;      }
        inline short&          getShort()         { return this->value_.short_;      }
        inline unsigned short& getUnsignedShort() { return this->value_.ushort_;     }
        inline long&           getLong()          { return this->value_.long_;       }
        inline unsigned long&  getUnsignedLong()  { return this->value_.ulong_;      }
        inline float&          getFloat()         { return this->value_.float_;      }
        inline double&         getDouble()        { return this->value_.double_;     }
        inline long double&    getLongDouble()    { return this->value_.longdouble_; }
        inline bool&           getBool()          { return this->value_.bool_;       }

        inline void getValue(int*            variable) const { (*variable) = this->value_.int_;        }
        inline void getValue(unsigned int*   variable) const { (*variable) = this->value_.uint_;       }
        inline void getValue(char*           variable) const { (*variable) = this->value_.char_;       }
        inline void getValue(unsigned char*  variable) const { (*variable) = this->value_.uchar_;      }
        inline void getValue(short*          variable) const { (*variable) = this->value_.short_;      }
        inline void getValue(unsigned short* variable) const { (*variable) = this->value_.ushort_;     }
        inline void getValue(long*           variable) const { (*variable) = this->value_.long_;       }
        inline void getValue(unsigned long*  variable) const { (*variable) = this->value_.ulong_;      }
        inline void getValue(float*          variable) const { (*variable) = this->value_.float_;      }
        inline void getValue(double*         variable) const { (*variable) = this->value_.double_;     }
        inline void getValue(long double*    variable) const { (*variable) = this->value_.longdouble_; }
        inline void getValue(bool*           variable) const { (*variable) = this->value_.bool_;       }

        inline MultiType getType()           const { return this->type_; }
        inline bool      isA(MultiType type) const { return (this->type_ == type); }

        std::string toString() const;
        bool fromString(const std::string value);

    protected:
        MultiTypeValue  value_;
        MultiType       type_;
};

std::ostream& operator<<(std::ostream& out, const MultiTypePrimitive& mtp);

#endif /* _MultiTypePrimitive_H__ */
