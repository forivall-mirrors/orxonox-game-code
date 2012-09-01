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
    @brief Implementation of all argument completion functions
*/

#include "ArgumentCompletionFunctions.h"

#include <map>
#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "core/Identifier.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueContainer.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "TclThreadManager.h"

// Differentiate Boost Filesystem v2 and v3
#if (BOOST_FILESYSTEM_VERSION < 3)
#  define BF_LEAF leaf
#  define BF_GENERIC_STRING string
#  define BF_DICTIONARY_ENTRY_NAME string
#else
#  define BF_LEAF path().filename().string
#  define BF_GENERIC_STRING generic_string
#  define BF_DICTIONARY_ENTRY_NAME path().string
#endif

namespace orxonox
{
    namespace autocompletion
    {
        /**
            @brief Fallback implementation, returns an empty list.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(fallback)()
        {
            return ArgumentCompletionList();
        }

        namespace detail
        {
            /**
                @brief Returns true if a group of console commands is visible (i.e. if at least one command in this group is visible).
            */
            bool groupIsVisible(const std::map<std::string, ConsoleCommand*>& group, bool bOnlyShowHidden)
            {
                for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = group.begin(); it_command != group.end(); ++it_command)
                    if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden)
                        return true;

                return false;
            }

            /**
                @brief Returns a list of all console command groups AND all console command shortcuts.
                @param fragment The last argument
                @param bOnlyShowHidden If true, only hidden groups and commands are returned
            */
            ArgumentCompletionList _groupsandcommands(const std::string& fragment, bool bOnlyShowHidden)
            {
                // note: this function returns only arguments that begin with "fragment", which wouldn't be necessary for the
                //       auto-completion, but it's necessary to place the line-break "\n" between groups and commands
                //       only if both groups AND commands are in the list.

                ArgumentCompletionList groupList;
                std::string fragmentLC = getLowercase(fragment);

                // get all the groups that are visible (except the shortcut group "")
                const std::map<std::string, std::map<std::string, ConsoleCommand*> >& commands = ConsoleCommand::getCommands();
                for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = commands.begin(); it_group != commands.end(); ++it_group)
                    if (groupIsVisible(it_group->second, bOnlyShowHidden) && it_group->first != "" && (fragmentLC == "" || getLowercase(it_group->first).find(fragmentLC) == 0))
                        groupList.push_back(ArgumentCompletionListElement(it_group->first, getLowercase(it_group->first)));

                // now add all shortcuts (in group "")
                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = commands.find("");
                if (it_group != commands.end())
                {
                    // add a line-break if the list isn't empty
                    if (!groupList.empty())
                        groupList.push_back(ArgumentCompletionListElement("", "", "\n"));

                    // add the shortcuts
                    for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = it_group->second.begin(); it_command != it_group->second.end(); ++it_command)
                        if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden && (fragmentLC == "" || getLowercase(it_command->first).find(fragmentLC) == 0))
                            groupList.push_back(ArgumentCompletionListElement(it_command->first, getLowercase(it_command->first)));
                }

                // if no shortcut was added, remove the line-break again
                if (!groupList.empty() && groupList.back().getDisplay() == "\n")
                    groupList.pop_back();

                return groupList;
            }

            /**
                @brief Returns a list of all console commands in a given group.
                @param fragment The last argument
                @param group The group's name
                @param bOnlyShowHidden If true, only hidden console commands are returned
            */
            ArgumentCompletionList _subcommands(const std::string& fragment, const std::string& group, bool bOnlyShowHidden)
            {
                ArgumentCompletionList commandList;

                std::string groupLC = getLowercase(group);

                // find the iterator of the given group
                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommands().begin();
                for ( ; it_group != ConsoleCommand::getCommands().end(); ++it_group)
                    if (getLowercase(it_group->first) == groupLC)
                        break;

                // add all commands in the group to the list
                if (it_group != ConsoleCommand::getCommands().end())
                {
                    for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = it_group->second.begin(); it_command != it_group->second.end(); ++it_command)
                        if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden)
                            commandList.push_back(ArgumentCompletionListElement(it_command->first, getLowercase(it_command->first)));
                }

                return commandList;
            }
        }

        /**
            @brief Returns a list of all console command groups AND all console command shortcuts.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(groupsandcommands)(const std::string& fragment)
        {
            return detail::_groupsandcommands(fragment, false);
        }

        /**
            @brief Returns a list of all console commands in a given group.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(subcommands)(const std::string& fragment, const std::string& group)
        {
            return detail::_subcommands(fragment, group, false);
        }

        /**
            @brief Returns a list of commands and groups and also supports auto-completion of the arguments of these commands.

            This is a multi-word function, because commands are composed of 1-2 words and additional arguments.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(command)(const std::string& fragment)
        {
            CommandEvaluation evaluation = CommandExecutor::evaluate(fragment);
            const std::string& hint = evaluation.hint();

            if (evaluation.getPossibleArguments().size() > 0 && evaluation.getPossibleArgumentsSize() > 0)
            {
                return evaluation.getPossibleArguments();
            }
            else
            {
                ArgumentCompletionList list;
                list.push_back(ArgumentCompletionListElement("", "", hint));
                return list;
            }
        }

        /**
            @brief Returns a list of hidden commands and groups and also supports auto-completion of the arguments of these commands.

            This is a multi-word function, because commands are composed of 1-2 words and additional arguments.

            This function makes commands visible that would usually be hidden.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(hiddencommand)(const std::string& fragment)
        {
            SubString tokens(fragment, " ", SubString::WhiteSpaces, false, '\\', true, '"', true, '{', '}', true, '\0');

            if (tokens.size() == 0)
                return detail::_groupsandcommands(fragment, true);

            if (ConsoleCommand::getCommandLC(getLowercase(tokens[0])))
                return ARGUMENT_COMPLETION_FUNCTION_CALL(command)(fragment);

            if (tokens.size() == 1)
            {
                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommands().find(tokens[0]);
                if (it_group != ConsoleCommand::getCommands().end())
                    return detail::_subcommands(fragment, tokens[0], true);
                else
                    return detail::_groupsandcommands(fragment, true);
            }

            if (ConsoleCommand::getCommandLC(getLowercase(tokens[0]), getLowercase(tokens[1])))
                return ARGUMENT_COMPLETION_FUNCTION_CALL(command)(fragment);

            return ArgumentCompletionList();
        }

        /**
            @brief Returns possible files and directories and also supports files in arbitrary deeply nested subdirectories.

            This function returns files and directories in the given path. This allows to
            navigate iteratively through the file system. The first argument @a fragment
            is used to get the current path.
        */
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
                    const std::string& dir = startdirectory.BF_GENERIC_STRING();
                    if (dir.size() > 0 && dir[dir.size() - 1] == ':')
                        startdirectory = dir + '/';
                }
#endif

                boost::filesystem::directory_iterator file(startdirectory);
                boost::filesystem::directory_iterator end;

                while (file != end)
                {
                    if (boost::filesystem::is_directory(*file))
                        dirlist.push_back(ArgumentCompletionListElement(file->BF_DICTIONARY_ENTRY_NAME() + '/', getLowercase(file->BF_DICTIONARY_ENTRY_NAME()) + '/', file->BF_LEAF() + '/'));
                    else
                        filelist.push_back(ArgumentCompletionListElement(file->BF_DICTIONARY_ENTRY_NAME(), getLowercase(file->BF_DICTIONARY_ENTRY_NAME()), file->BF_LEAF()));
                    ++file;
                }
            }
            catch (...) {}

            filelist.insert(filelist.begin(), dirlist.begin(), dirlist.end());
            return filelist;
        }

        /**
            @brief Returns the sections of the config file.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingssections)()
        {
            ArgumentCompletionList sectionList;

            const std::set<std::string>& names = SettingsConfigFile::getInstance().getSectionNames();
            for (std::set<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
                sectionList.push_back(ArgumentCompletionListElement(*it, getLowercase(*it)));

            return sectionList;
        }

        /**
            @brief Returns the entries in a given section of the config file.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingsentries)(const std::string& fragment, const std::string& section)
        {
            ArgumentCompletionList entryList;
            SettingsConfigFile& settings = SettingsConfigFile::getInstance();
            const std::string& sectionLC = getLowercase(section);

            SettingsConfigFile::ContainerMap::const_iterator upper = settings.getContainerUpperBound(sectionLC);
            for (SettingsConfigFile::ContainerMap::const_iterator it = settings.getContainerLowerBound(sectionLC); it != upper; ++it)
                entryList.push_back(ArgumentCompletionListElement(it->second.second->getName(), it->second.first));

            return entryList;
        }

        /**
            @brief Returns the current value of a given value in a given section of the config file.
        */
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingsvalue)(const std::string& fragment, const std::string& entry, const std::string& section)
        {
            ArgumentCompletionList oldValue;
            SettingsConfigFile& settings = SettingsConfigFile::getInstance();
            const std::string& sectionLC = getLowercase(section);
            const std::string& entryLC = getLowercase(entry);

            SettingsConfigFile::ContainerMap::const_iterator upper = settings.getContainerUpperBound(sectionLC);
            for (SettingsConfigFile::ContainerMap::const_iterator it = settings.getContainerLowerBound(sectionLC); it != upper; ++it)
            {
                if (it->second.first == entryLC)
                {
                    const std::string& valuestring = it->second.second->toString();
                    oldValue.push_back(ArgumentCompletionListElement(valuestring, getLowercase(valuestring), "Old value: " + valuestring));
                }
            }

            return oldValue;
        }

        /**
            @brief Returns a list of indexes of the available Tcl threads (see TclThreadManager).
        */
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
