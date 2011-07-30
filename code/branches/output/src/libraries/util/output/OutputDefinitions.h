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

#ifndef _OutputDefinitions_H__
#define _OutputDefinitions_H__

#include "util/UtilPrereqs.h"
#include <string>

#define REGISTER_OUTPUT_CONTEXT(name) \
    OutputContext name() { static OutputContext context = registerContext(#name); return context; }

namespace orxonox
{
    typedef uint16_t OutputLevel;

    namespace level
    {
        static const OutputLevel all              = 0xFFFF;
        static const OutputLevel none             = 0x0000;
        static const OutputLevel debug_output     = 0x0001;
        static const OutputLevel user_error       = 0x0002;
        static const OutputLevel user_warning     = 0x0004;
        static const OutputLevel user_status      = 0x0008;
        static const OutputLevel user_info        = 0x0010;
        static const OutputLevel internal_error   = 0x0020;
        static const OutputLevel internal_warning = 0x0040;
        static const OutputLevel internal_status  = 0x0080;
        static const OutputLevel internal_info    = 0x0100;
        static const OutputLevel verbose          = 0x0200;
        static const OutputLevel verbose_more     = 0x0400;
        static const OutputLevel verbose_ultra    = 0x0800;
    }

    typedef uint64_t OutputContext;
    typedef OutputContext (OutputContextFunction)();

    extern _UtilExport OutputContext registerContext(const std::string& name);

    namespace context
    {
        static const OutputContext all       = 0xFFFFFFFFFFFFFFFF;
        static const OutputContext none      = 0x0000000000000000;

        namespace
        {
            REGISTER_OUTPUT_CONTEXT(undefined);

            REGISTER_OUTPUT_CONTEXT(test1);
            REGISTER_OUTPUT_CONTEXT(test2);
            REGISTER_OUTPUT_CONTEXT(output);
            REGISTER_OUTPUT_CONTEXT(lua);
            REGISTER_OUTPUT_CONTEXT(tcl);
        }
    }
}

#endif /* _OutputDefinitions_H__ */
