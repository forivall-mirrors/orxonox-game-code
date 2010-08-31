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

#include "core/CorePrereqs.h"
#include "ArgumentCompleter.h"


#define ARGUMENT_COMPLETION_FUNCTION_DECLARATION(functionname) \
    _CoreExport ArgumentCompleter* functionname(); \
    _CoreExport ArgumentCompletionList acf_##functionname

#define ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(functionname) \
    _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, false)
#define ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(functionname) \
    _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, true)

#define _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, bUseMultipleWords) \
    ArgumentCompleter* functionname() \
    { \
        static ArgumentCompleter completer = ArgumentCompleter(&acf_##functionname, bUseMultipleWords); \
        return &completer; \
    } \
    \
    ArgumentCompletionList acf_##functionname

#define ARGUMENT_COMPLETION_FUNCTION_CALL(functionname) acf_##functionname


namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(fallback)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(groupsandcommands)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(subcommands)(const std::string& fragment, const std::string& group);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(command)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(hiddencommand)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(files)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingssections)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingsentries)(const std::string& fragment, const std::string& section);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingsvalue)(const std::string& fragment, const std::string& entry, const std::string& section);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(tclthreads)();
    }
}

#endif /* _ArgumentCompletionFunctions_H__ */
