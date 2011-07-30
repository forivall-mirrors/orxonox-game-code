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

#ifndef _BaseWriter_H__
#define _BaseWriter_H__

#include "util/UtilPrereqs.h"

#include <set>
#include <vector>

#include "OutputListener.h"

namespace orxonox
{
    class _UtilExport BaseWriter : public OutputListener
    {
        public:
            BaseWriter(const std::string& name);
            virtual ~BaseWriter();

            const std::string& getName() const
                { return this->name_; }

            void setLevelMax(OutputLevel max);

            OutputLevel configurableMaxLevel_;
            inline std::string getConfigurableMaxLevelName() const
                { return "outputLevel" + this->name_; }

            OutputLevel configurableContextsMaxLevel_;
            inline std::string getConfigurableContextsMaxLevelName() const
                { return "outputContextsLevel" + this->name_; }

            std::vector<std::string> configurableContexts_;
            inline std::string getConfigurableContextsName() const
                { return "outputContexts" + this->name_; }

            void changedConfigurableLevels();
            void changedConfigurableContexts();

            static inline std::string getConfigurableSectionName()
                { return "Output"; }

        protected:
            virtual void output(OutputLevel level, OutputContext context, const std::vector<std::string>& lines);

        private:
            virtual void printLine(const std::string& line, OutputLevel level) = 0;

            void setLevelRange(OutputLevel min, OutputLevel max);
            void setLevelMask(OutputLevel mask);

            bool isAdditionalContext(OutputContext context) const;

            std::string name_;
            std::set<std::string> configurableContextsSet_;
    };
}

#endif /* _BaseWriter_H__ */
