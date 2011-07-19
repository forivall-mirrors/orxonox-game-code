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

#ifndef _OutputStream_H__
#define _OutputStream_H__

#include "util/UtilPrereqs.h"

#include <sstream>

#include "OutputDefinitions.h"

namespace orxonox
{
namespace test
{
    class _UtilExport OutputStream : public std::ostringstream
    {
        typedef std::ostream& (*EndlType)(std::ostream&);

        public:
            OutputStream();

            void setOutputAttributes(OutputLevel level, OutputContext context);

            template <class T>
            inline OutputStream& operator<<(const T& val) { return this->output(val); }
            inline OutputStream& operator<<(std::ios_base& (*manipulator)(std::ios_base&)) { return this->output(manipulator); }
            inline OutputStream& operator<<(std::ios&      (*manipulator)(std::ios&))      { return this->output(manipulator); }
            inline OutputStream& operator<<(std::ostream&  (*manipulator)(std::ostream&))
            {
                if (this->bAcceptsOutput_)
                {
                    if (manipulator == static_cast<EndlType>(std::endl))
                        this->sendMessage();
                    else
                        return this->output(manipulator);
                }
                return *this;
            }

        private:
            template <class T>
            inline OutputStream& output(const T& val)
            {
                if (this->bAcceptsOutput_)
                    static_cast<std::ostringstream&>(*this) << val;
                return *this;
            }

            void sendMessage();

            OutputLevel level_;
            OutputContext context_;
            bool bAcceptsOutput_;
    };
}
}

#endif /* _OutputStream_H__ */
