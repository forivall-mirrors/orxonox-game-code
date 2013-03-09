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
    @brief Implementation of the OutputManager singleton.
*/

#include "OutputManager.h"

#include <iostream>

#include "MemoryWriter.h"
#include "ConsoleWriter.h"
#include "LogWriter.h"
#include "util/Output.h"
#include "util/StringUtils.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes all values.
    */
    OutputManager::OutputManager()
    {
        this->combinedLevelMask_ = level::none;
        this->combinedAdditionalContextsLevelMask_ = level::none;
        this->combinedAdditionalContextsMask_ = context::none;

        this->subcontextCounter_ = 0;
    }

    /**
        @brief Destructor.
    */
    OutputManager::~OutputManager()
    {
    }

    /*static*/ SharedPtr<OutputManager>& OutputManager::Testing::getInstancePointer()
    {
        static SharedPtr<OutputManager> instance(new OutputManager());
        return instance;
    }

    /**
        @brief Returns the only existing instance of the OutputManager singleton.
    */
    /*static*/ OutputManager& OutputManager::getInstance()
    {
        return *OutputManager::Testing::getInstancePointer();
    }

    /**
        @brief Returns the only existing instance of the OutputManager singleton
        and ensures that the most important output listeners exist.

        You should use this function if you send output to OutputManager and want
        to be sure that the most important output listeners exist. Don't use it
        elsewhere inside the output system to avoid circular calls.
    */
    /*static*/ OutputManager& OutputManager::getInstanceAndCreateListeners()
    {
        static OutputManager& instance = OutputManager::getInstance();

        static MemoryWriter& memoryWriterInstance = OutputManager::getInstance().getMemoryWriter(); (void)memoryWriterInstance;
        static ConsoleWriter& consoleWriterInstance = OutputManager::getInstance().getConsoleWriter(); (void)consoleWriterInstance;
        static LogWriter& logWriterInstance = OutputManager::getInstance().getLogWriter(); (void)logWriterInstance;

        return instance;
    }

    /**
     * @brief Returns the main instance of MemoryWriter which is managed by the OutputManager singleton.
     * @note If OutputManager is ever un-singletonized, this instance must not remain static.
     */
    MemoryWriter& OutputManager::getMemoryWriter()
    {
        static MemoryWriter instance;
        return instance;
    }

    /**
     * @brief Returns the main instance of ConsoleWriter which is managed by the OutputManager singleton.
     * @note If OutputManager is ever un-singletonized, this instance must not remain static.
     */
    ConsoleWriter& OutputManager::getConsoleWriter()
    {
        static ConsoleWriter instance(std::cout);
        return instance;
    }

    /**
     * @brief Returns the main instance of LogWriter which is managed by the OutputManager singleton.
     * @note If OutputManager is ever un-singletonized, this instance must not remain static.
     */
    LogWriter& OutputManager::getLogWriter()
    {
        static LogWriter instance;
        return instance;
    }

    /**
        @brief Sends an output message to all output listeners.
        @param level The level of the message
        @param context The context of the message
        @param message The output message (may contain '\\n')

        This function splits the message into lines (if it contains '\\n') and
        sends it to the output listeners. They may ignore the message if it
        doesn't match their level- and context-masks.
    */
    void OutputManager::pushMessage(OutputLevel level, const OutputContextContainer& context, const std::string& message)
    {
        std::vector<std::string> lines;
        vectorize(message, '\n', &lines);

        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->listeners_[i]->unfilteredOutput(level, context, lines);
    }

    /**
        @brief Adds an output listener to the list of listeners.
    */
    void OutputManager::registerListener(OutputListener* listener)
    {
        listener->registerListener(this);
        this->listeners_.push_back(listener);
        this->updateMasks();
    }

    /**
        @brief Removes an output listener from the list of listeners.
    */
    void OutputManager::unregisterListener(OutputListener* listener)
    {
        listener->unregisterListener(this);
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

    /**
        @brief Updates all three combined level- and context-masks.
    */
    void OutputManager::updateMasks()
    {
        this->updateCombinedLevelMask();
        this->updateCombinedAdditionalContextsLevelMask();
        this->updateCombinedAdditionalContextsMask();
    }

    /**
        @brief Updates the combined level mask. The masks of all listeners are ORed to form the combined mask.
    */
    void OutputManager::updateCombinedLevelMask()
    {
        int mask = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            mask |= this->listeners_[i]->getLevelMask();
        this->combinedLevelMask_ = static_cast<OutputLevel>(mask);
    }

    /**
        @brief Updates the combined additional contexts level mask. The masks of all listeners are ORed to form the combined mask.
    */
    void OutputManager::updateCombinedAdditionalContextsLevelMask()
    {
        int mask = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            mask |= this->listeners_[i]->getAdditionalContextsLevelMask();
        this->combinedAdditionalContextsLevelMask_ = static_cast<OutputLevel>(mask);
    }

    /**
        @brief Updates the combined additional contexts mask. The masks of all listeners are ORed to form the combined mask.
    */
    void OutputManager::updateCombinedAdditionalContextsMask()
    {
        this->combinedAdditionalContextsMask_ = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->combinedAdditionalContextsMask_ |= this->listeners_[i]->getAdditionalContextsMask();
    }

    /**
        @brief Registers a context (or sub-context) and returns the container which identifies the context.
        @param name The name of the context
        @param subname The name of the sub-context (or "" if it is not a sub-context)

        If the context doesn't exist, it gets created. Otherwise the existing instance is returned.
    */
    const OutputContextContainer& OutputManager::registerContext(const std::string& name, const std::string& subname)
    {
        // the full name of a context is a combination of name and subname with "::" in between
        std::string full_name = name;
        if (subname != "")
            full_name += "::" + subname;

        // check if the context already exists (and return it if it does)
        std::map<std::string, OutputContextContainer>::iterator it_container = this->contextContainers_.find(full_name);
        if (it_container != this->contextContainers_.end())
            return it_container->second;

        // create a new context container
        OutputContextContainer container;
        container.name = full_name;

        // check if the mask of the main-context already exists
        std::map<std::string, OutputContextMask>::iterator it_mask = this->contextMasks_.find(name);
        if (it_mask != this->contextMasks_.end())
        {
            // the mask exists, assign it to the container
            container.mask = it_mask->second;
        }
        else
        {
            // the mask doesn't exist, create it. It's a binary mask. The n-th main-context is defined by the n-th bit in the mask.
            container.mask = static_cast<OutputContextMask>(0x1) << this->contextMasks_.size();
            this->contextMasks_[name] = container.mask;

            if (container.mask == 0)
                orxout(internal_warning) << "More than " << sizeof(OutputContextMask) * 8 << " output contexts defined. Context '" << name << "' might not get filtered correctly" << endl;
        }

        // if the context is a sub-context, assign a unique ID.
        if (subname == "")
            container.sub_id = context::no_subcontext;
        else
            container.sub_id = ++this->subcontextCounter_; // start with 1

        // add the new context to the map and return it
        return (this->contextContainers_[full_name] = container);
    }

    /**
        @brief Static function, shortcut to OutputManager::registerContext().
        The function is declared in OutputDefinitions.h.
    */
    const OutputContextContainer& registerContext(const std::string& name, const std::string& subname)
    {
        return OutputManager::getInstance().registerContext(name, subname);
    }

    /**
        @brief Returns a human readable string for each output level.
    */
    const std::string& OutputManager::getLevelName(OutputLevel level) const
    {
        switch (level)
        {
            // using static cache variables for speed
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

    /**
        @brief Returns a string containing the name of the level and the context (if any) which
        can be prepended to an output message if it is written to the console or the log file.
    */
    std::string OutputManager::getDefaultPrefix(OutputLevel level, const OutputContextContainer& context) const
    {
        // "undefined" context is ignored because it's used implicitly if no explicit context is defined
        OutputContextMask undefined_mask = context::undefined().mask;

        std::string prefix = this->getLevelName(level) + ": ";
        if (context.mask != undefined_mask)
            prefix += "[" + context.name + "] ";

        return prefix;
    }
}
