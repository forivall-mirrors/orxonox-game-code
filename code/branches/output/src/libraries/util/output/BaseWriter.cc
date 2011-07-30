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

#include "BaseWriter.h"

#include "OutputManager.h"

namespace orxonox
{
    BaseWriter::BaseWriter(const std::string& name)
    {
        this->name_ = name;

        this->configurableMaxLevel_ = level::none;
        this->configurableContextsMaxLevel_ = level::verbose;
    }

    BaseWriter::~BaseWriter()
    {
    }

    void BaseWriter::output(OutputLevel level, OutputContext context, const std::vector<std::string>& lines)
    {
        if (level <= this->configurableMaxLevel_ || (level <= this->configurableContextsMaxLevel_ && this->isAdditionalContext(context)))
        {
            const std::string& prefix = OutputManager::getInstance().getDefaultPrefix(level, context);
            std::string blanks(prefix.length(), ' ');

            for (size_t i = 0; i < lines.size(); ++i)
                this->printLine((i == 0 ? prefix : blanks) + lines[i], level);
        }
    }

    void BaseWriter::setLevelMax(OutputLevel max)
    {
        this->configurableMaxLevel_ = max;
        this->changedConfigurableLevels();
    }

    void BaseWriter::changedConfigurableLevels()
    {
        OutputLevel max_level = std::max(this->configurableMaxLevel_, this->configurableContextsMaxLevel_);
        OutputListener::setLevelMax(max_level);
    }

    void BaseWriter::changedConfigurableContexts()
    {
        this->configurableContextsSet_.clear();
        for (size_t i = 0; i < this->configurableContexts_.size(); ++i)
            this->configurableContextsSet_.insert(this->configurableContexts_[i]);
    }

    bool BaseWriter::isAdditionalContext(OutputContext context) const
    {
        const std::string& name = OutputManager::getInstance().getContextName(context);
        std::set<std::string>::const_iterator it = this->configurableContextsSet_.find(name);
        return (it != this->configurableContextsSet_.end());
    }
}
