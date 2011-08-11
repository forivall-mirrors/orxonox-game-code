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

#include "OutputManager.h"

#include "MemoryWriter.h"
#include "ConsoleWriter.h"
#include "LogWriter.h"
#include "util/Output.h"
#include "util/StringUtils.h"

namespace orxonox
{
    OutputManager::OutputManager()
    {
        this->combinedLevelMask_ = level::none;
        this->combinedAdditionalContextsLevelMask_ = level::none;
        this->combinedAdditionalContextsMask_ = context::none;

        this->subcontextCounter_ = 0;
    }

    OutputManager::~OutputManager()
    {
    }

    /*static*/ OutputManager& OutputManager::getInstance()
    {
        static OutputManager instance;
        return instance;
    }

    /*static*/ OutputManager& OutputManager::getInstanceAndCreateListeners()
    {
        static OutputManager& instance = OutputManager::getInstance();

        static MemoryWriter& memoryWriterInstance = MemoryWriter::getInstance(); (void)memoryWriterInstance;
        static ConsoleWriter& consoleWriterInstance = ConsoleWriter::getInstance(); (void)consoleWriterInstance;
        static LogWriter& logWriterInstance = LogWriter::getInstance(); (void)logWriterInstance;

        return instance;
    }

    void OutputManager::pushMessage(OutputLevel level, const OutputContextContainer& context, const std::string& message)
    {
        std::vector<std::string> lines;
        vectorize(message, '\n', &lines);

        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->listeners_[i]->unfilteredOutput(level, context, lines);
    }

    void OutputManager::registerListener(OutputListener* listener)
    {
        this->listeners_.push_back(listener);
        this->updateMasks();
    }

    void OutputManager::unregisterListener(OutputListener* listener)
    {
        for (std::vector<OutputListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if (*it == listener)
            {
                this->listeners_.erase(it);
                break;
            }
        }
        this->updateMasks();
    }

    void OutputManager::updateMasks()
    {
        this->updateCombinedLevelMask();
        this->updateCombinedAdditionalContextsLevelMask();
        this->updateCombinedAdditionalContextsMask();
    }

    void OutputManager::updateCombinedLevelMask()
    {
        int mask = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            mask |= this->listeners_[i]->getLevelMask();
        this->combinedLevelMask_ = static_cast<OutputLevel>(mask);
    }

    void OutputManager::updateCombinedAdditionalContextsLevelMask()
    {
        int mask = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            mask |= this->listeners_[i]->getAdditionalContextsLevelMask();
        this->combinedAdditionalContextsLevelMask_ = static_cast<OutputLevel>(mask);
    }

    void OutputManager::updateCombinedAdditionalContextsMask()
    {
        this->combinedAdditionalContextsMask_ = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->combinedAdditionalContextsMask_ |= this->listeners_[i]->getAdditionalContextsMask();
    }

    const OutputContextContainer& OutputManager::registerContext(const std::string& name, const std::string& subname)
    {
        std::string full_name = name;
        if (subname != "")
            full_name += "::" + subname;

        std::map<std::string, OutputContextContainer>::iterator it_container = this->contextContainers_.find(full_name);
        if (it_container != this->contextContainers_.end())
            return it_container->second;

        OutputContextContainer container;
        container.name = full_name;

        std::map<std::string, OutputContextMask>::iterator it_mask = this->contextMasks_.find(name);
        if (it_mask != this->contextMasks_.end())
        {
            container.mask = it_mask->second;
        }
        else
        {
            container.mask = static_cast<OutputContextMask>(0x1) << this->contextMasks_.size();
            this->contextMasks_[name] = container.mask;

            if (container.mask == 0)
                orxout(internal_warning) << "More than " << sizeof(OutputContextMask) * 8 << " output contexts defined. Context '" << name << "' might not get filtered correctly" << endl;
        }

        if (subname == "")
            container.sub_id = context::no_subcontext;
        else
            container.sub_id = ++this->subcontextCounter_; // start with 1

        return (this->contextContainers_[full_name] = container);
    }

    const OutputContextContainer& registerContext(const std::string& name, const std::string& subname)
    {
        return OutputManager::getInstance().registerContext(name, subname);
    }

    const std::string& OutputManager::getLevelName(OutputLevel level) const
    {
        switch (level)
        {
            case level::none:               { static std::string name = "None"; return name; }
            case level::message:            { static std::string name = "Message"; return name; }
            case level::debug_output:       { static std::string name = "Debug"; return name; }
            case level::user_error:         { static std::string name = "Error"; return name; }
            case level::user_warning:       { static std::string name = "Warning"; return name; }
            case level::user_status:        { static std::string name = "Status"; return name; }
            case level::user_info:          { static std::string name = "Info"; return name; }
            case level::internal_error:     { static std::string name = "Error (internal)"; return name; }
            case level::internal_warning:   { static std::string name = "Warning (internal)"; return name; }
            case level::internal_status:    { static std::string name = "Status (internal)"; return name; }
            case level::internal_info:      { static std::string name = "Info (internal)"; return name; }
            case level::verbose:            { static std::string name = "Verbose"; return name; }
            case level::verbose_more:       { static std::string name = "Verbose (more)"; return name; }
            case level::verbose_ultra:      { static std::string name = "Verbose (ultra)"; return name; }
            default:                        { static std::string name = ""; return name; }
        }
    }

    std::string OutputManager::getDefaultPrefix(OutputLevel level, const OutputContextContainer& context) const
    {
        static OutputContextMask undefined_mask = context::undefined().mask;

        std::string prefix = this->getLevelName(level) + ": ";
        if (context.mask != undefined_mask)
            prefix += "[" + context.name + "] ";

        return prefix;
    }
}
