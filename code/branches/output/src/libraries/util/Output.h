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

#ifndef _Output_H__
#define _Output_H__

#include "UtilPrereqs.h"
#include "output/OutputStream.h"

namespace orxonox
{
    // Just for convenience
    using std::endl;

    inline OutputStream& orxout(OutputLevel level = level::debug_output, const OutputContextContainer& context = context::undefined())
    {
        static OutputStream stream;
        stream.setOutputAttributes(level, context);
        return stream;
    }

    inline OutputStream& orxout(OutputLevel level, OutputContextFunction context)
    {
        return orxout(level, context());
    }

    // COUT() is deprecated, please use orxout()
    inline __DEPRECATED__(OutputStream& COUT(int level));

    inline OutputStream& COUT(int)
    {
        return orxout();
    }
}

#endif /* _Output_H__ */
