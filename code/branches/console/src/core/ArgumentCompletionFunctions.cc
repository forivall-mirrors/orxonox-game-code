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

//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <dirent.h>

#include "ArgumentCompletionFunctions.h"
#include "CoreIncludes.h"
#include "Identifier.h"
#include "ConfigValueContainer.h"
#include "TclThreadManager.h"
#include "util/String.h"
#include "util/SubString.h"

namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(fallback)()
        {
            return std::list<std::pair<std::string, std::string> >();
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(files)(const std::string& fragment)
        {
            std::list<std::pair<std::string, std::string> > dirlist;
            std::list<std::pair<std::string, std::string> > filelist;

            SubString tokens(fragment, "/", "", false, '\0', false, '\0', false, '\0', '\0', false, '\0');

            std::string startdirectory = ".";
            if (fragment.size() > 0 && fragment[fragment.size() - 1] == '/' && tokens.size() > 0)
                startdirectory = tokens.subSet(0, tokens.size()).join("/");
            else if (tokens.size() > 1)
                startdirectory = tokens.subSet(0, tokens.size() - 1).join("/");

std::cout << "startdir: " << startdirectory << std::endl;

            struct stat fileInfo;
            struct dirent *currentFile;
            DIR *handler = 0;

            handler = opendir(startdirectory.c_str());
            if (handler)
            {
                while ((currentFile = readdir(handler)) != 0)
                {
                    if (strcmp(currentFile->d_name, ".") && strcmp(currentFile->d_name, ".."))
                    {
                        std::string path = startdirectory + "/" + currentFile->d_name;
                        if (stat(path.c_str(), &fileInfo) == -1)
                        {
                            closedir(handler);
                            break;
                        }

                        if (S_ISREG(fileInfo.st_mode)) // normal file
                            filelist.push_back(std::pair<std::string, std::string>(getLowercase(path), path));
                        else if (S_ISDIR(fileInfo.st_mode)) // directory
                            dirlist.push_back(std::pair<std::string, std::string>(getLowercase(path) + "/", path + "/"));
                        else // special file
                            filelist.push_back(std::pair<std::string, std::string>(getLowercase(path), path));
                    }
                }

                closedir(handler);
            }

            filelist.insert(filelist.begin(), dirlist.begin(), dirlist.end());
            return filelist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalueclasses)()
        {
            std::list<std::pair<std::string, std::string> > classlist;

            for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getIdentifierMapBegin(); it != Identifier::getIdentifierMapEnd(); ++it)
                if ((*it).second->hasConfigValues())
                    classlist.push_back(std::pair<std::string, std::string>(getLowercase((*it).first), (*it).second->getName()));

            return classlist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalues)(const std::string& fragment, const std::string& classname)
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

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(configvalue)(const std::string& fragment, const std::string& varname, const std::string& classname)
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
