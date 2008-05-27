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

#ifndef _ArgumentCompletionFunctions_H__
#define _ArgumentCompletionFunctions_H__

#include <list>
#include <string>

#include "ArgumentCompleter.h"
#include "CorePrereqs.h"


#define ARGUMENT_COMPLETION_FUNCTION_DECLARATION(functionname) \
    ArgumentCompleter* functionname(); \
    _CoreExport std::list<std::pair<std::string, std::string> > acf_##functionname

#define ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(functionname) \
    ArgumentCompleter* functionname() \
    { \
        static ArgumentCompleter completer = ArgumentCompleter(&acf_##functionname); \
        return &completer; \
    } \
    \
    std::list<std::pair<std::string, std::string> > acf_##functionname


namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(fallback)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(files)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(configvalueclasses)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(configvalues)(const std::string& fragment, const std::string& classname);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(configvalue)(const std::string& fragment, const std::string& varname, const std::string& classname);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(tclthreads)();
    }
}

#endif /* _ArgumentCompletionFunctions_H__ */
