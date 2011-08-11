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
        this->configurableAdditionalContextsMaxLevel_ = level::verbose;
        this->configurableAdditionalContexts_.push_back("example");

        this->subcontextsCheckMask_ = context::none;

        this->changedConfigurableLevel();
        this->changedConfigurableAdditionalContextsLevel();
        this->changedConfigurableAdditionalContexts();
    }

    BaseWriter::~BaseWriter()
    {
    }

    void BaseWriter::output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
    {
        if (((this->subcontextsCheckMask_ & context.mask) == 0) || (this->subcontexts_.find(context.sub_id) != this->subcontexts_.end()))
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
        this->changedConfigurableLevel();
    }

    void BaseWriter::setAdditionalContextsLevelMax(OutputLevel max)
    {
        this->configurableAdditionalContextsMaxLevel_ = max;
        this->changedConfigurableAdditionalContextsLevel();
    }

    void BaseWriter::changedConfigurableLevel()
    {
        OutputListener::setLevelMax(static_cast<OutputLevel>(this->configurableMaxLevel_));
    }

    void BaseWriter::changedConfigurableAdditionalContextsLevel()
    {
        OutputListener::setAdditionalContextsLevelMax(static_cast<OutputLevel>(this->configurableAdditionalContextsMaxLevel_));
    }

    void BaseWriter::changedConfigurableAdditionalContexts()
    {
        OutputContextMask context_mask = context::none;
        this->subcontextsCheckMask_ = context::none;

        this->subcontexts_.clear();
        this->subcontexts_.insert(context::no_subcontext);

        for (size_t i = 0; i < this->configurableAdditionalContexts_.size(); ++i)
        {
            const std::string& full_name = this->configurableAdditionalContexts_[i];

            std::string name = full_name;
            std::string subname;

            size_t pos = full_name.find("::");
            if (pos != std::string::npos)
            {
                name = full_name.substr(0, pos);
                subname = full_name.substr(pos + 2);
            }

            const OutputContextContainer& container = OutputManager::getInstance().registerContext(name, subname);

            context_mask |= container.mask;

            if (container.sub_id != context::no_subcontext)
            {
                this->subcontexts_.insert(container.sub_id);
                this->subcontextsCheckMask_ |= container.mask;
            }
        }

        this->setAdditionalContextsMask(context_mask);
    }
}
