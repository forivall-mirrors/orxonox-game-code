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
 *   Inspiration: MultiType by Benjamin Grauer
 */

#ifndef _MultiTypeString_H__
#define _MultiTypeString_H__

#include <string>
#include <iostream>
#include "UtilPrereqs.h"

#include "MultiTypePrimitive.h"

class _UtilExport MultiTypeString : public MultiTypePrimitive
{
    public:
        MultiTypeString(MultiType type = MT_null);
        inline MultiTypeString(void*          value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(int            value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(unsigned int   value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(char           value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(unsigned char  value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(short          value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(unsigned short value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(long           value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(unsigned long  value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(float          value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(double         value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(long double    value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(bool           value) : MultiTypePrimitive(value) {}
        inline MultiTypeString(const char*           value)   { this->setValue(value); }
        inline MultiTypeString(const std::string&    value)   { this->setValue(value); }
        inline MultiTypeString(const MultiTypeString& mts)    { this->setValue(mts);   }
        virtual inline ~MultiTypeString() {}

        using MultiTypePrimitive::operator=;
        inline MultiTypeString& operator=(const char*             value)   { this->setValue(value); return *this; }
        inline MultiTypeString& operator=(const std::string&      value)   { this->setValue(value); return *this; }
        inline MultiTypeString& operator=(const MultiTypeString& mts)      { this->setValue(mts);   return *this; }

        using MultiTypePrimitive::operator==;
        inline bool operator==(const char*             value) const { return (this->string_      == std::string(value)); }
        inline bool operator==(const std::string&      value) const { return (this->string_      == value);              }
        bool operator==(const MultiTypeString& mts) const;

        using MultiTypePrimitive::operator!=;
        inline bool operator!=(const char*             value) const { return (this->string_      != std::string(value)); }
        inline bool operator!=(const std::string&      value) const { return (this->string_      != value);              }
        bool operator!=(const MultiTypeString& mts) const;

        virtual operator void*()                const;
        virtual operator int()                  const;
        virtual operator unsigned int()         const;
        virtual operator char()                 const;
        virtual operator unsigned char()        const;
        virtual operator short()                const;
        virtual operator unsigned short()       const;
        virtual operator long()                 const;
        virtual operator unsigned long()        const;
        virtual operator float ()               const;
        virtual operator double ()              const;
        virtual operator long double()          const;
        virtual operator bool()                 const;
        virtual operator std::string()          const;
        virtual operator const char*()          const;

        using MultiTypePrimitive::setValue;
        inline void setValue(const char*             value) { this->type_ = MT_string;     this->string_     = std::string(value); }
        inline void setValue(const std::string&      value) { this->type_ = MT_string;     this->string_     = value;              }
        void setValue(const MultiTypeString& mts);

        inline std::string getString()          const { return this->string_;         }
        inline const char*  getConstChar()      const { return this->string_.c_str(); }

        inline std::string& getString()          { return this->string_;         }
        inline const char*  getConstChar()       { return this->string_.c_str(); }

        using MultiTypePrimitive::getValue;
        inline void getValue(std::string*      variable) const { (*variable) = this->string_;         }
        inline void getValue(const char**      variable) const { (*variable) = this->string_.c_str(); }

        virtual std::string getTypename() const;

        virtual std::string toString() const;
        virtual bool fromString(const std::string value);

    protected:
        std::string      string_;
};

_UtilExport std::ostream& operator<<(std::ostream& out, MultiTypeString& mts);

#endif /* _MultiTypeString_H__ */
