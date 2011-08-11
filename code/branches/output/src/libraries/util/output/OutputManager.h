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

#ifndef _OutputManager_H__
#define _OutputManager_H__

#include "util/UtilPrereqs.h"

#include <vector>
#include <map>

#include "OutputDefinitions.h"

namespace orxonox
{
    class _UtilExport OutputManager
    {
        public:
            static OutputManager& getInstance();
            static OutputManager& getInstanceAndCreateListeners();

            void pushMessage(OutputLevel level, const OutputContextContainer& context, const std::string& message);

            void registerListener(OutputListener* listener);
            void unregisterListener(OutputListener* listener);

            void updateMasks();
            void updateCombinedLevelMask();
            void updateCombinedAdditionalContextsLevelMask();
            void updateCombinedAdditionalContextsMask();

            inline bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const
            {
                return (this->combinedLevelMask_ & level) ||
                       ((this->combinedAdditionalContextsLevelMask_ & level) && (this->combinedAdditionalContextsMask_ & context.mask));
            }

            const OutputContextContainer& registerContext(const std::string& name, const std::string& subname = "");

            const std::string& getLevelName(OutputLevel level) const;
            std::string getDefaultPrefix(OutputLevel level, const OutputContextContainer& context) const;

        private:
            OutputManager();
            OutputManager(const OutputManager&);
            ~OutputManager();

            std::vector<OutputListener*> listeners_;

            OutputLevel       combinedLevelMask_;
            OutputLevel       combinedAdditionalContextsLevelMask_;
            OutputContextMask combinedAdditionalContextsMask_;

            std::map<std::string, OutputContextMask> contextMasks_;
            std::map<std::string, OutputContextContainer> contextContainers_;
            OutputContextSubID subcontextCounter_;
    };
}

#endif /* _OutputManager_H__ */
