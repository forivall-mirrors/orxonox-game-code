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

#include "ArgumentCompletionFunctions.h"

#include <map>
#include <boost/version.hpp>
#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "Identifier.h"
#include "ConfigValueContainer.h"
#include "TclThreadManager.h"

// Boost 1.36 has some issues with deprecated functions that have been omitted
#if (BOOST_VERSION == 103600)
#  define BOOST_LEAF_FUNCTION filename
#else
#  define BOOST_LEAF_FUNCTION leaf
#endif

namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(fallback)()
        {
            return ArgumentCompletionList();
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(files)(const std::string& fragment)
        {
            ArgumentCompletionList dirlist;
            ArgumentCompletionList filelist;

            try
            {
                boost::filesystem::path input(fragment);
                boost::filesystem::path startdirectory(input.branch_path());

                if (!boost::filesystem::exists(startdirectory))
                {
                    startdirectory = ".";
                }
#ifdef ORXONOX_PLATFORM_WINDOWS
                else
                {
                    const std::string& dir = startdirectory.string();
                    if (dir.size() > 0 && dir[dir.size() - 1] == ':')
                        startdirectory = dir + '/';
                }
#endif

                boost::filesystem::directory_iterator file(startdirectory);
                boost::filesystem::directory_iterator end;

                while (file != end)
                {
                    if (boost::filesystem::is_directory(*file))
                        dirlist.push_back(ArgumentCompletionListElement(file->string() + '/', getLowercase(file->string()) + '/', file->BOOST_LEAF_FUNCTION() + '/'));
                    else
                        filelist.push_back(ArgumentCompletionListElement(file->string(), getLowercase(file->string()), file->BOOST_LEAF_FUNCTION()));
                    ++file;
                }
            }
            catch (...) {}

            filelist.insert(filelist.begin(), dirlist.begin(), dirlist.end());
            return filelist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalueclasses)()
        {
            ArgumentCompletionList classlist;

            for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getStringIdentifierMapBegin(); it != Identifier::getStringIdentifierMapEnd(); ++it)
                if (it->second->hasConfigValues())
                    classlist.push_back(ArgumentCompletionListElement(it->first, getLowercase(it->first)));

            return classlist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalues)(const std::string& fragment, const std::string& classname)
        {
            ArgumentCompletionList configvalues;
            std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getLowercaseStringIdentifierMap().find(getLowercase(classname));

            if (identifier != Identifier::getLowercaseStringIdentifierMapEnd() && identifier->second->hasConfigValues())
            {
                for (std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->second->getConfigValueMapBegin(); it != identifier->second->getConfigValueMapEnd(); ++it)
                    configvalues.push_back(ArgumentCompletionListElement(it->first, getLowercase(it->first)));
            }

            return configvalues;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalue)(const std::string& fragment, const std::string& varname, const std::string& classname)
        {
            ArgumentCompletionList oldvalue;
            std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getLowercaseStringIdentifierMap().find(getLowercase(classname));
            if (identifier != Identifier::getLowercaseStringIdentifierMapEnd())
            {
                std::map<std::string, ConfigValueContainer*>::const_iterator variable = identifier->second->getLowercaseConfigValueMap().find(getLowercase(varname));
                if (variable != identifier->second->getLowercaseConfigValueMapEnd())
                {
                    const std::string& valuestring = variable->second->toString();
                    oldvalue.push_back(ArgumentCompletionListElement(valuestring, getLowercase(valuestring), "Old value: " + valuestring));
                }
            }
            return oldvalue;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(tclthreads)()
        {
            std::list<unsigned int> threadnumbers = TclThreadManager::getInstance().getThreadList();
            ArgumentCompletionList threads;

            for (std::list<unsigned int>::const_iterator it = threadnumbers.begin(); it != threadnumbers.end(); ++it)
                threads.push_back(ArgumentCompletionListElement(multi_cast<std::string>(*it)));

            return threads;
        }
    }
}
