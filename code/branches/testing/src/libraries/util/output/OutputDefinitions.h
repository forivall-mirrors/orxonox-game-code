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
    @ingroup Output
    @brief Defines output levels and output contexts.
*/

#ifndef _OutputDefinitions_H__
#define _OutputDefinitions_H__

#include "util/UtilPrereqs.h"
#include <string>

/**
    @brief Defines a context function with a given name.
    @param name Name of the context

    Context functions return a reference to a OutputContextContainer. Context
    functions (or the containers they return) can be passed to orxout() as
    context argument.
*/
#define REGISTER_OUTPUT_CONTEXT(name) \
    const OutputContextContainer& name() { static OutputContextContainer context = registerContext(#name); return context; }

/**
    @brief Defines a sub-context.
    @param name Name of the main-context
    @param subname Name of the sub-context

    Sub-contexts act like normal contexts, except that multiple sub-contexts
    share the context mask of their main-context. This allows contexts with
    more descriptive names (e.g. input::keyboard) and they can be filtered
    individually by derivatives of orxonox::SubcontextOutputListener.
*/
#define REGISTER_OUTPUT_SUBCONTEXT(name, subname) \
    const OutputContextContainer& subname() { static const OutputContextContainer& context = registerContext(#name, #subname); return context; }

// tolua_begin
namespace orxonox
{
    namespace level
    {
        /**
            @brief Output levels define type and importance of an output message.
            They can be passed to the orxout() function as level argument.
        */
        enum OutputLevel
        {
            all              = 0xFFFF, ///< Level mask with all bits set to 1
            none             = 0x0000, ///< Level mask with all bits set to 0

            message          = 0x0001, ///< Output level, used for messages directed to the user (e.g. "Press any key to continue")
            debug_output     = 0x0002, ///< Output level, used for temporary debug output while writing code
            user_error       = 0x0004, ///< Output level, used for error messages which are important for the user
            user_warning     = 0x0008, ///< Output level, used for warnings which are important for the user
            user_status      = 0x0010, ///< Output level, used to notify the user about the program's state
            user_info        = 0x0020, ///< Output level, used to provide the user with additional progress information
            internal_error   = 0x0040, ///< Output level, used for error messages which are important for developers
            internal_warning = 0x0080, ///< Output level, used for warnings which are important for developers
            internal_status  = 0x0100, ///< Output level, used to log the program's internal state in the log file
            internal_info    = 0x0200, ///< Output level, used to log information about the program's progress in the log file
            verbose          = 0x0400, ///< Output level, usually not visible, used for unimportant debug information
            verbose_more     = 0x0800, ///< Output level, usually not visible, used for unimportant debug information (less important than verbose)
            verbose_ultra    = 0x1000  ///< Output level, usually not visible, used for unimportant debug information (even less important than verbose_more)
        };
    }
// tolua_end

    using namespace level;

    typedef uint64_t OutputContextMask;  ///< Used to store the context masks. Each bit defines a context.
    typedef uint16_t OutputContextSubID; ///< Used to store the IDs of sub-contexts. Each number except context::no_subcontext defines a sub-context.

    /// @brief Stores all information about a context.
    struct OutputContextContainer
    {
        OutputContextMask mask;     ///< The mask of the context (or the mask of the main-context if this container defines a sub-context)
        OutputContextSubID sub_id;  ///< The id of the sub-context (or context::no_subcontext if this container doesn't define a sub-context)
        std::string name;           ///< The name of this context

        inline bool operator==(const OutputContextContainer& other) const
        {
            return this->mask == other.mask && this->sub_id == other.sub_id && this->name == other.name;
        }
    };

    typedef const OutputContextContainer& (OutputContextFunction)();

    /**
        @brief Registers a context.
        This is a shortcut to OutputManager::registerContext() to avoid the inclusion of its header file.
    */
    extern _UtilExport const OutputContextContainer& registerContext(const std::string& name, const std::string& subname = "");

    namespace context
    {
        static const OutputContextMask all       = 0xFFFFFFFFFFFFFFFFull; ///< Context mask, all bits set to 1
        static const OutputContextMask none      = 0x0000000000000000ull; ///< Context mask, all bits set to 0

        static const OutputContextSubID no_subcontext = 0; ///< Used as ID for contexts which are not sub-contexts

        namespace
        {
            REGISTER_OUTPUT_CONTEXT(undefined); ///< "undefined" context which is implicitly used for all output that has no explicit context

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
} // tolua_export

#endif /* _OutputDefinitions_H__ */
