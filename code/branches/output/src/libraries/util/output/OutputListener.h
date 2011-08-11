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

#ifndef _OutputListener_H__
#define _OutputListener_H__

#include "util/UtilPrereqs.h"

#include <vector>

#include "OutputDefinitions.h"

namespace orxonox
{
    class _UtilExport OutputListener
    {
        public:
            OutputListener(bool bRegister = true);
            virtual ~OutputListener();

            void setLevelMax(OutputLevel max);
            void setLevelRange(OutputLevel min, OutputLevel max);
            void setLevelMask(OutputLevel mask);

            void setAdditionalContextsLevelMax(OutputLevel max);
            void setAdditionalContextsLevelRange(OutputLevel min, OutputLevel max);
            void setAdditionalContextsLevelMask(OutputLevel mask);

            void setAdditionalContextsMask(OutputContextMask mask);

            inline OutputLevel getLevelMask() const
                { return this->levelMask_; }
            inline OutputLevel getAdditionalContextsLevelMask() const
                { return this->additionalContextsLevelMask_; }
            inline OutputContextMask getAdditionalContextsMask() const
                { return this->additionalContextsMask_; }

            inline bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const
            {
                return (this->levelMask_ & level) ||
                       ((this->additionalContextsLevelMask_ & level) && (this->additionalContextsMask_ & context.mask)); }

            inline void unfilteredOutput(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
                { if (this->acceptsOutput(level, context)) this->output(level, context, lines); }

        protected:
            virtual void output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines) = 0;

        private:
            OutputLevel       levelMask_;
            OutputLevel       additionalContextsLevelMask_;
            OutputContextMask additionalContextsMask_;
    };
}

#endif /* _OutputListener_H__ */
