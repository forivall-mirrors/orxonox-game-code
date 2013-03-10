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
    @ingroup Output
    @brief Declaration of the OutputManager class which receives output from orxonox::OutputStream
    and distributes it to all instances of orxonox::OutputListener.
*/

#ifndef _OutputManager_H__
#define _OutputManager_H__

#include "util/UtilPrereqs.h"

#include <vector>
#include <map>

#include "OutputDefinitions.h"
#include "AdditionalContextListener.h"

namespace orxonox
{
    /**
        @brief OutputManager acts as the center of the output system and is implemented as a singleton.

        All instances of OutputStream (and hence also the orxout() function)
        send their buffered output to OutputManager. OutputManager then
        distributes this output to all registered instances of OutputListener.

        For each listener OutputManager checks if it wants to receive output
        with the given level and context. OutputManager itself also maintains
        masks that define the accepted levels and concept. They are a
        combination of the masks of all output listeners. See the description
        of OutputListener for a more conclusive description of these masks.

        Additionally OutputManager is used to register output contexts.
    */
    class _UtilExport OutputManager : public AdditionalContextListener
    {
        public:
            OutputManager();
            OutputManager(const OutputManager&);
            virtual ~OutputManager();

            static OutputManager& getInstance();
            static OutputManager& getInstanceAndCreateListeners();

            inline MemoryWriter& getMemoryWriter()   { return *this->memoryWriterInstance_; }
            inline ConsoleWriter& getConsoleWriter() { return *this->consoleWriterInstance_; }
            inline LogWriter& getLogWriter()         { return *this->logWriterInstance_; }

            virtual void pushMessage(OutputLevel level, const OutputContextContainer& context, const std::string& message);

            virtual void registerListener(OutputListener* listener);
            virtual void unregisterListener(OutputListener* listener);

            virtual void updatedLevelMask(const OutputListener* listener)
                { this->updateCombinedLevelMask(); }
            virtual void updatedAdditionalContextsLevelMask(const OutputListener* listener)
                { this->updateCombinedAdditionalContextsLevelMask(); }
            virtual void updatedAdditionalContextsMask(const OutputListener* listener)
                { this->updateCombinedAdditionalContextsMask(); }

            /**
                @brief Returns true if at least one of the output listeners will accept output with the given level and context.

                For the sake of performance, output messages with levels or
                contexts that are not accepted should be ignored or, even
                better, not generated at all.
            */
            inline bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const
            {
                return (this->combinedLevelMask_ & level) ||
                       ((this->combinedAdditionalContextsLevelMask_ & level) && (this->combinedAdditionalContextsMask_ & context.mask));
            }

            const OutputContextContainer& registerContext(const std::string& name, const std::string& subname = "");

            const std::string& getLevelName(OutputLevel level) const;
            std::string getDefaultPrefix(OutputLevel level, const OutputContextContainer& context) const;

            inline const std::vector<OutputListener*>& getListeners() const
                { return this->listeners_; }

        private:
            void updateMasks();
            void updateCombinedLevelMask();
            void updateCombinedAdditionalContextsLevelMask();
            void updateCombinedAdditionalContextsMask();

            std::vector<OutputListener*> listeners_;                            ///< List of all registered output listeners

            OutputLevel       combinedLevelMask_;                               ///< The combined mask of accepted levels of all listeners
            OutputLevel       combinedAdditionalContextsLevelMask_;             ///< The combined mask of accepted additional contexts levels of all listeners
            OutputContextMask combinedAdditionalContextsMask_;                  ///< The combined mask of accepted additional contexts of all listeners

            std::map<std::string, OutputContextMask> contextMasks_;             ///< Contains all main-contexts and their masks
            std::map<std::string, OutputContextContainer> contextContainers_;   ///< Contains all contexts including sub-contexts and their containers
            OutputContextSubID subcontextCounter_;                              ///< Counts the number of sub-contexts (and generates their IDs)

            bool isInitialized_;                                                ///< Becomes true once the following instances were created
            MemoryWriter*  memoryWriterInstance_;                               ///< The main instance of MemoryWriter, managed by OutputManager
            ConsoleWriter* consoleWriterInstance_;                              ///< The main instance of ConsoleWriter, managed by OutputManager
            LogWriter*     logWriterInstance_;                                  ///< The main instance of LogWriter, managed by OutputManager

        public:
            struct _UtilExport Testing
            {
                static SharedPtr<OutputManager>& getInstancePointer();
            };
    };
}

#endif /* _OutputManager_H__ */
