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

#include <boost/bimap.hpp>

#include "OutputDefinitions.h"

namespace orxonox
{
namespace test
{
    class OutputListener;

    class _UtilExport OutputManager
    {
        friend class OutputListener;

        public:
            static OutputManager& getInstance();

            void pushMessage(OutputLevel level, OutputContext context, const std::string& message);

            void registerListener(OutputListener* listener);
            void unregisterListener(OutputListener* listener);

            void updateMasks();
            void updateCombinedLevelMask();
            void updateCombinedContextMask();

            inline OutputLevel getCombinedLevelMask() const
                { return this->combinedLevelMask_; }
            inline OutputContext getCombinedContextMask() const
                { return this->combinedContextMask_; }

            inline bool acceptsOutput(OutputLevel level, OutputContext context) const
                { return ((this->combinedLevelMask_ & level) && (this->combinedContextMask_ & context)); }

            OutputContext registerContext(const std::string& name);

            const std::string& getLevelName(OutputLevel level) const;
            const std::string& getContextName(OutputContext context) const;
            std::string getComposedContextName(OutputContext context) const;

        private:
            OutputManager();
            OutputManager(const OutputManager&);
            ~OutputManager();

            static OutputManager& getInstanceInternal();

            std::vector<OutputListener*> listeners_;

            OutputLevel   combinedLevelMask_;
            OutputContext combinedContextMask_;

            boost::bimap<OutputContext, std::string> contexts_;
    };
}
}

#endif /* _OutputManager_H__ */
