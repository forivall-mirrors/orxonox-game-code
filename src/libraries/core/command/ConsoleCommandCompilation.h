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
    @ingroup Command ConsoleCommand
    @brief Declaration of some console commands.
*/

#ifndef _ConsoleCommandCompilation_H__
#define _ConsoleCommandCompilation_H__

#include "core/CorePrereqs.h"

namespace orxonox
{
    _CoreExport void source(const std::string& filename);
    _CoreExport std::string echo(const std::string& text);

    _CoreExport void orxout_level(const std::string& level_name, const std::string& text);
    _CoreExport void orxout_level_context(const std::string& level_name, const std::string& context_name, const std::string& text);

    _CoreExport void log(const std::string& text);
    _CoreExport void error(const std::string& text);
    _CoreExport void warning(const std::string& text);
    _CoreExport void status(const std::string& text);
    _CoreExport void info(const std::string& text);
    _CoreExport void debug(const std::string& text);

    _CoreExport void write(const std::string& filename, const std::string& text);
    _CoreExport void append(const std::string& filename, const std::string& text);
    _CoreExport std::string read(const std::string& filename);

    _CoreExport float calculate(const std::string& calculation);
}

#endif /* _ConsoleCommandCompilation_H__ */
