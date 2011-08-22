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

#include "Loader.h"

#include <sstream>
#include <tinyxml/ticpp.h>
#include <boost/scoped_ptr.hpp>

#include "util/Output.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "BaseObject.h"
#include "Iterator.h"
#include "ObjectList.h"
#include "LuaState.h"
#include "Namespace.h"
#include "Resource.h"
#include "XMLFile.h"

namespace orxonox
{
    std::vector<std::pair<const XMLFile*, ClassTreeMask> > Loader::files_s;
    ClassTreeMask Loader::currentMask_s;

    bool Loader::open(const XMLFile* file, const ClassTreeMask& mask, bool bVerbose)
    {
        Loader::add(file, mask);
        return Loader::load(file, mask, bVerbose);
    }

    void Loader::close()
    {
        Loader::unload();
        Loader::files_s.clear();
    }

    void Loader::close(const XMLFile* file)
    {
        Loader::unload(file);
        Loader::remove(file);
    }

    void Loader::add(const XMLFile* file, const ClassTreeMask& mask)
    {
        if (!file)
            return;
        Loader::files_s.insert(Loader::files_s.end(), std::pair<const XMLFile*, ClassTreeMask>(file, mask));
    }

    void Loader::remove(const XMLFile* file)
    {
        if (!file)
            return;
        for (std::vector<std::pair<const XMLFile*, ClassTreeMask> >::iterator it = Loader::files_s.begin(); it != Loader::files_s.end(); ++it)
        {
            if (it->first == file)
            {
                Loader::files_s.erase(it);
                break;
            }
        }
    }

    /**
    @brief
        Loads all opened files, while conforming to the restrictions given by the input ClassTreeMask.
    @param mask
        A ClassTreeMask, which defines which types of classes are loaded and which aren't.
    @param bVerbose
        Whether the loader is verbose (prints its progress in a low output level) or not.
    @return
        Returns true if successful.
    */
    bool Loader::load(const ClassTreeMask& mask, bool bVerbose)
    {
        bool success = true;
        for (std::vector<std::pair<const XMLFile*, ClassTreeMask> >::iterator it = Loader::files_s.begin(); it != Loader::files_s.end(); ++it)
            if (!Loader::load(it->first, it->second * mask, bVerbose))
                success = false;

        return success;
    }

    void Loader::unload(const ClassTreeMask& mask)
    {
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); )
        {
            if (mask.isIncluded(it->getIdentifier()))
                (it++)->destroy();
            else
                ++it;
        }
    }

    /**
    @brief
        Reloads all opened files, while conforming to the restrictions given by the input ClassTreeMask.
    @param mask
        A ClassTreeMask, which defines which types of classes are reloaded and which aren't.
    @param bVerbose
        Whether the loader is verbose (prints its progress in a low output level) or not.
    @return
        Returns true if successful.
    */
    bool Loader::reload(const ClassTreeMask& mask, bool bVerbose)
    {
        Loader::unload(mask);
        return Loader::load(mask, bVerbose);
    }

    /**
    @brief
        Loads the input file, while conforming to the restrictions given by the input ClassTreeMask.
    @param file
        The file to be loaded.
    @param mask
        A ClassTreeMask, which defines which types of classes are loaded and which aren't.
    @param bVerbose
        Whether the loader is verbose (prints its progress in a low output level) or not.
    @param bRemoveLuaTags
        If true lua tags are just ignored and removed. The default is false.
    @return
        Returns true if successful.
    */
    bool Loader::load(const XMLFile* file, const ClassTreeMask& mask, bool bVerbose, bool bRemoveLuaTags)
    {
        if (!file)
            return false;

        Loader::currentMask_s = file->getMask() * mask;

        std::string xmlInput;
        if (file->getLuaSupport() && !bRemoveLuaTags)
        {
            // Use the LuaState to replace the XML tags (calls our function)
            scoped_ptr<LuaState> luaState(new LuaState());
            luaState->setIncludeParser(&Loader::replaceLuaTags);
            luaState->includeFile(file->getFilename());
            xmlInput = luaState->getOutput().str();
        }
        else
        {
            shared_ptr<ResourceInfo> info = Resource::getInfo(file->getFilename());
            if (info == NULL)
            {
                orxout(user_error, context::loader) << "Could not find XML file '" << file->getFilename() << "'." << endl;
                return false;
            }
            xmlInput = Resource::open(file->getFilename())->getAsString();

            if (bRemoveLuaTags)
            {
                // Remove all Lua code.
                // Note: we only need this to speed up parsing of level files at the
                // start of the program.
                // Assumption: the LevelInfo tag does not use Lua scripting
                xmlInput = removeLuaTags(xmlInput);
            }
        }

        try
        {
            if(bVerbose)
            {
                orxout(user_info) << "Start loading " << file->getFilename() << "..." << endl;
                orxout(internal_info, context::loader) << "Mask: " << Loader::currentMask_s << endl;
            }
            else
            {
                orxout(verbose, context::loader) << "Start loading " << file->getFilename() << "..." << endl;
                orxout(verbose_more, context::loader) << "Mask: " << Loader::currentMask_s << endl;
            }

            ticpp::Document xmlfile(file->getFilename());
            xmlfile.Parse(xmlInput, true);

            ticpp::Element rootElement;
            rootElement.SetAttribute("name", "root");
            rootElement.SetAttribute("bAutogenerated", true);

            for (ticpp::Iterator<ticpp::Element> child = xmlfile.FirstChildElement(false); child != child.end(); child++)
                rootElement.InsertEndChild(*child);

            orxout(verbose, context::loader) << "  creating root-namespace..." << endl;
            Namespace* rootNamespace = new Namespace(0);
            rootNamespace->setLoaderIndentation("    ");
            rootNamespace->setFile(file);
            rootNamespace->setNamespace(rootNamespace);
            rootNamespace->setRoot(true);
            rootNamespace->XMLPort(rootElement, XMLPort::LoadObject);

            if(bVerbose)
                orxout(user_info) << "Finished loading " << file->getFilename() << '.' << endl;
            else
                orxout(verbose, context::loader) << "Finished loading " << file->getFilename() << '.' << endl;

            orxout(verbose, context::loader) << "Namespace-tree:" << '\n' << rootNamespace->toString("  ") << endl;

            return true;
        }
        catch (ticpp::Exception& ex)
        {
            orxout(user_error, context::loader) << endl;
            orxout(user_error, context::loader) << "An XML-error occurred in Loader.cc while loading " << file->getFilename() << ':' << endl;
            orxout(user_error, context::loader) << ex.what() << endl;
            orxout(user_error, context::loader) << "Loading aborted." << endl;
            return false;
        }
        catch (Exception& ex)
        {
            orxout(user_error, context::loader) << endl;
            orxout(user_error, context::loader) << "A loading-error occurred in Loader.cc while loading " << file->getFilename() << ':' << endl;
            orxout(user_error, context::loader) << ex.what() << endl;
            orxout(user_error, context::loader) << "Loading aborted." << endl;
            return false;
        }
        catch (...)
        {
            orxout(user_error, context::loader) << endl;
            orxout(user_error, context::loader) << "An error occurred in Loader.cc while loading " << file->getFilename() << ':' << endl;
            orxout(user_error, context::loader) << Exception::handleMessage() << endl;
            orxout(user_error, context::loader) << "Loading aborted." << endl;
            return false;
        }
    }

    void Loader::unload(const XMLFile* file, const ClassTreeMask& mask)
    {
        if (!file)
            return;
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it; )
        {
            if ((it->getFile() == file) && mask.isIncluded(it->getIdentifier()))
                (it++)->destroy();
            else
                ++it;
        }
    }

    /**
    @brief
        Reloads the input file, while conforming to the restrictions given by the input ClassTreeMask.
    @param file
        The file to be reloaded.
    @param mask
        A ClassTreeMask, which defines which types of classes are reloaded and which aren't.
    @param bVerbose
        Whether the loader is verbose (prints its progress in a low output level) or not.
    @return
        Returns true if successful.
    */
    bool Loader::reload(const XMLFile* file, const ClassTreeMask& mask, bool bVerbose)
    {
        Loader::unload(file, mask);
        return Loader::load(file, mask, bVerbose);
    }

    bool Loader::getLuaTags(const std::string& text, std::map<size_t, bool>& luaTags)
    {
        // fill map with all Lua tags
        {
            size_t pos = 0;
            while ((pos = text.find("<?lua", pos)) != std::string::npos)
                luaTags[pos++] = true;
        }
        {
            size_t pos = 0;
            while ((pos = text.find("?>", pos)) != std::string::npos)
                luaTags[pos++] = false;
        }

        // erase all tags from the map that are between two quotes
        {
            std::map<size_t, bool>::iterator it = luaTags.begin();
            std::map<size_t, bool>::iterator it2 = it;
            bool bBetweenQuotes = false;
            size_t pos = 0;
            while ((pos = getNextQuote(text, pos)) != std::string::npos)
            {
                while ((it != luaTags.end()) && (it->first < pos))
                {
                    if (bBetweenQuotes)
                    {
                        it2++;
                        if (it->second && !(it2->second) && it2->first < pos)
                            it = ++it2;
                        else
                            luaTags.erase(it++);
                    }
                    else
                        ++it;
                }
                bBetweenQuotes = !bBetweenQuotes;
                pos++;
            }
        }

        // check whether on every opening <?lua tag a closing ?> tag follows
        {
            bool expectedValue = true;
            for (std::map<size_t, bool>::iterator it = luaTags.begin(); it != luaTags.end(); ++it)
            {
                if (it->second == expectedValue)
                    expectedValue = !expectedValue;
                else
                {
                    expectedValue = false;
                    break;
                }
            }
            if (!expectedValue)
            {
                orxout(internal_error, context::loader) << "Error in level file" << endl;
                // TODO: error handling
                return false;
            }
        }

        return true;
    }

    std::string Loader::replaceLuaTags(const std::string& text)
    {
        // create a map with all lua tags
        std::map<size_t, bool> luaTags;
        if (!getLuaTags(text, luaTags))
            return "";

        // Use a stringstream object to speed up the parsing
        std::ostringstream output;

        // cut the original string into pieces and put them together with print() instead of lua tags
        {
            std::map<size_t, bool>::iterator it = luaTags.begin();
            bool bInPrintFunction = true;
            size_t start = 0;
            size_t end = 0;

            do
            {
                if (it != luaTags.end())
                    end = (it++)->first;
                else
                    end = std::string::npos;

                unsigned int equalSignCounter = 0;

                if (bInPrintFunction)
                {
                    // count ['='[ and ]'='] and replace tags with print([[ and ]])
                    const std::string& temp = text.substr(start, end - start);
                    {
                    size_t pos = 0;
                    while ((pos = temp.find('[', pos)) != std::string::npos)
                    {
                        unsigned int tempCounter = 1;
                        size_t tempPos = pos++;
                        while (temp[++tempPos] == '=')
                        {
                            tempCounter++;
                        }
                        if (temp[tempPos] != '[')
                        {
                            tempCounter = 0;
                        }
                        else if (tempCounter == 0)
                        {
                            tempCounter = 1;
                        }
                        if (tempCounter > equalSignCounter)
                            equalSignCounter = tempCounter;
                        }
                    }
                    {
                        size_t pos = 0;
                        while ((pos = temp.find(']', pos)) != std::string::npos)
                        {
                            unsigned int tempCounter = 1;
                            size_t tempPos = pos++;
                            while (temp[++tempPos] == '=')
                            {
                                tempCounter++;
                            }
                            if (temp[tempPos] != ']')
                            {
                                tempCounter = 0;
                            }
                            else if (tempCounter == 0)
                            {
                                tempCounter = 1;
                            }
                            if (tempCounter > equalSignCounter)
                                equalSignCounter = tempCounter;
                        }
                    }
                    std::string equalSigns;
                    for (unsigned int i = 0; i < equalSignCounter; i++)
                    {
                        equalSigns += '=';
                    }
                    output << "print([" + equalSigns + '[' + temp + ']' + equalSigns +"])";
                    start = end + 5;
                }
                else
                {
                    output << text.substr(start, end - start);
                    start = end + 2;
                }

                bInPrintFunction = !bInPrintFunction;
            }
            while (end != std::string::npos);
        }

        return output.str();
    }

    std::string Loader::removeLuaTags(const std::string& text)
    {
        // create a map with all lua tags
        std::map<size_t, bool> luaTags;
        if (!getLuaTags(text, luaTags))
            return "";

        // Use a stringstream object to speed up the concatenation
        std::ostringstream output;

        // cut the original string into pieces and only write the non Lua parts
        std::map<size_t, bool>::iterator it = luaTags.begin();
        bool bLuaCode = false;
        size_t start = 0;
        size_t end = 0;

        do
        {
            if (it != luaTags.end())
                end = (it++)->first;
            else
                end = std::string::npos;

            if (!bLuaCode)
            {
                output << text.substr(start, end - start);
                start = end + 5;
            }
            else
                start = end + 2;

            bLuaCode = !bLuaCode;
        }
        while (end != std::string::npos);

        return output.str();
    }
}
