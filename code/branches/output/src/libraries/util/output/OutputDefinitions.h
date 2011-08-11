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
    const OutputContextContainer& name() { static const OutputContextContainer& context = registerContext(#name); return context; }

#define REGISTER_OUTPUT_SUBCONTEXT(name, subname) \
    const OutputContextContainer& subname() { static const OutputContextContainer& context = registerContext(#name, #subname); return context; }

namespace orxonox
{
    namespace level
    {
        enum OutputLevel
        {
            all              = 0xFFFF,
            none             = 0x0000,

            message          = 0x0001,
            debug_output     = 0x0002,
            user_error       = 0x0004,
            user_warning     = 0x0008,
            user_status      = 0x0010,
            user_info        = 0x0020,
            internal_error   = 0x0040,
            internal_warning = 0x0080,
            internal_status  = 0x0100,
            internal_info    = 0x0200,
            verbose          = 0x0400,
            verbose_more     = 0x0800,
            verbose_ultra    = 0x1000
        };
    }

    using namespace level;

    typedef uint64_t OutputContextMask;
    typedef uint16_t OutputContextSubID;

    struct OutputContextContainer
    {
        OutputContextMask mask;
        OutputContextSubID sub_id;
        std::string name;
    };

    typedef const OutputContextContainer& (OutputContextFunction)();

    extern _UtilExport const OutputContextContainer& registerContext(const std::string& name, const std::string& subname = "");

    namespace context
    {
        static const OutputContextMask all       = 0xFFFFFFFFFFFFFFFF;
        static const OutputContextMask none      = 0x0000000000000000;

        static const OutputContextSubID no_subcontext = 0;

        namespace
        {
            REGISTER_OUTPUT_CONTEXT(undefined);

            REGISTER_OUTPUT_CONTEXT(ogre);
            REGISTER_OUTPUT_CONTEXT(cegui);
            REGISTER_OUTPUT_CONTEXT(lua);
            REGISTER_OUTPUT_CONTEXT(tcl);
            REGISTER_OUTPUT_CONTEXT(identifier);
            REGISTER_OUTPUT_CONTEXT(object_list);
            REGISTER_OUTPUT_CONTEXT(super);
            REGISTER_OUTPUT_CONTEXT(language);
            REGISTER_OUTPUT_CONTEXT(commands);
            REGISTER_OUTPUT_CONTEXT(input);
            REGISTER_OUTPUT_CONTEXT(events);
            REGISTER_OUTPUT_CONTEXT(config);
            REGISTER_OUTPUT_CONTEXT(templates);
            REGISTER_OUTPUT_CONTEXT(loader);
            REGISTER_OUTPUT_CONTEXT(xml);
            REGISTER_OUTPUT_CONTEXT(network);
            REGISTER_OUTPUT_CONTEXT(packets);
            REGISTER_OUTPUT_CONTEXT(master_server);
            REGISTER_OUTPUT_CONTEXT(sound);
            REGISTER_OUTPUT_CONTEXT(lod);
            REGISTER_OUTPUT_CONTEXT(pickups);
            REGISTER_OUTPUT_CONTEXT(quests);
            REGISTER_OUTPUT_CONTEXT(notifications);
            REGISTER_OUTPUT_CONTEXT(triggers);
            REGISTER_OUTPUT_CONTEXT(docking);

            namespace misc
            {
                REGISTER_OUTPUT_SUBCONTEXT(misc, executor);
                REGISTER_OUTPUT_SUBCONTEXT(misc, factory);
                REGISTER_OUTPUT_SUBCONTEXT(misc, gui);
                REGISTER_OUTPUT_SUBCONTEXT(misc, overlays);
                REGISTER_OUTPUT_SUBCONTEXT(misc, script);
            }

        }
    }
}

#endif /* _OutputDefinitions_H__ */
