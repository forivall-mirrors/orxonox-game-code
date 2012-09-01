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
    @defgroup Output Output system
    @ingroup Util
*/

/**
    @file
    @ingroup Output
    @brief Defines the helper function orxout() and includes all necessary headers to use the output system.

    The output system is used to write output to the console, the logfile, and
    other instances of orxonox::OutputListener. Each line of output is assigned
    a level and a context. The level defines the type and importance of a
    message, e.g. if it's a fatal error or just some internal information.
    The context defines to which part of the program the output belongs.
    Levels and contexts are defined in OutputDefinitions.h

    Each orxonox::OutputListener can define a mask of desired levels and
    contexts, to receive only a part of the output. Instances of
    orxonox::SubcontextOutputListener are even able to filter sub-contexts.
    A derivative of orxonox::BaseWriter is able to define these levels and
    contexts through config values.

    @attention
    A message sent to the output system MUST end with "endl" or the message
    won't be flushed.

    @code
    orxout() << "Debug output" << endl;
    orxout(user_info) << "Orxonox version 1.2.3" << endl;
    orxout(internal_status, context::input) << "Loading joystick" << endl;
    @endcode
*/

#ifndef _Output_H__
#define _Output_H__

#include "UtilPrereqs.h"
#include "output/OutputStream.h"

namespace orxonox
{
    // Just for convenience
    using std::endl;

    /**
        @brief This helper function returns a reference to a commonly used
        instance of OutputStream.

        It can be used like std::cout except that it is a function. You can
        pass level and context of the following output as function arguments.
    */
    inline OutputStream& orxout(OutputLevel level = level::debug_output, const OutputContextContainer& context = context::undefined())
    {
        static OutputStream stream;
        stream.setOutputAttributes(level, context);
        return stream;
    }

    /**
        @brief Shortcut for orxout() to allow passing contexts directly as
        functions without using "()".

        @code
        orxout(user_info, context::example) << "Hello World" << endl; // calls this function
        orxout(user_info, context::example()) << "Hello World" << endl; // calls the other orxout function
        @endcode
    */
    inline OutputStream& orxout(OutputLevel level, OutputContextFunction context)
    {
        return orxout(level, context());
    }

    // COUT() is deprecated, please use orxout()
    inline __DEPRECATED__(OutputStream& COUT(int level));

    /**
        @brief Writes output to the orxonox console. This function is deprecated, please use orxout()
        @note The output level argument is ignored since it's not supported anymore. See orxout() for the new output levels.
        @deprecated This function is deprecated. Use orxout() instead.
    */
    inline OutputStream& COUT(int)
    {
        return orxout();
    }
}

#endif /* _Output_H__ */
