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

#include <iostream>
#include <map>

#include "ArgumentCompletionFunctions.h"
#include "CoreIncludes.h"
#include "Identifier.h"
#include "ConfigValueContainer.h"
#include "TclThreadManager.h"
#include "util/String.h"

namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(fallback)()
        {
            return std::list<std::pair<std::string, std::string> >();
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalueclasses)()
        {
            std::list<std::pair<std::string, std::string> > classlist;

            for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getIdentifierMapBegin(); it != Identifier::getIdentifierMapEnd(); ++it)
                if ((*it).second->hasConfigValues())
                    classlist.push_back(std::pair<std::string, std::string>(getLowercase((*it).first), (*it).second->getName()));

            return classlist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalues)(const std::string& classname)
        {
            std::list<std::pair<std::string, std::string> > configvalues;
            std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getIdentifierMap().find(classname);

            if (identifier != Identifier::getIdentifierMapEnd() && (*identifier).second->hasConfigValues())
            {
                for (std::map<std::string, ConfigValueContainer*>::const_iterator it = (*identifier).second->getConfigValueMapBegin(); it != (*identifier).second->getConfigValueMapEnd(); ++it)
                    configvalues.push_back(std::pair<std::string, std::string>(getLowercase((*it).first), (*it).second->getName()));
            }

            return configvalues;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalue)(const std::string& varname, const std::string& classname)
        {
            std::list<std::pair<std::string, std::string> > oldvalue;
            std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getLowercaseIdentifierMap().find(getLowercase(classname));
            if (identifier != Identifier::getLowercaseIdentifierMapEnd())
            {
                std::map<std::string, ConfigValueContainer*>::const_iterator variable = (*identifier).second->getLowercaseConfigValueMap().find(getLowercase(varname));
                if (variable != (*identifier).second->getLowercaseConfigValueMapEnd())
                {
                    std::string valuestring = (*variable).second->toString();
                    oldvalue.push_back(std::pair<std::string, std::string>(valuestring, valuestring));
                }
            }
            return oldvalue;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(tclthreads)()
        {
            return TclThreadManager::getInstance().getThreadList();
        }
    }
}
