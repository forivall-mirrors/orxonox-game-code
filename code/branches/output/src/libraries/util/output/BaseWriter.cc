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
    @brief Implementation of the BaseWriter class.
*/

#include "BaseWriter.h"

#include "OutputManager.h"

namespace orxonox
{
    /**
        @brief Constructor: Initializes the config-values.
    */
    BaseWriter::BaseWriter(const std::string& name, bool bRegister) : SubcontextOutputListener(bRegister)
    {
        this->name_ = name;

        this->configurableMaxLevel_ = level::none;
        this->configurableAdditionalContextsMaxLevel_ = level::verbose;
        this->configurableAdditionalContexts_.push_back("example");

        this->changedConfigurableLevel();
        this->changedConfigurableAdditionalContextsLevel();
        this->changedConfigurableAdditionalContexts();
    }

    /**
        @brief Destructor.
    */
    BaseWriter::~BaseWriter()
    {
    }

    /**
        @brief This function is inherited from OutputListener, each message is split into lines and sent to printLine().
    */
    void BaseWriter::output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
    {
        const std::string& prefix = OutputManager::getInstance().getDefaultPrefix(level, context);
        std::string blanks(prefix.length(), ' ');

        for (size_t i = 0; i < lines.size(); ++i)
            this->printLine((i == 0 ? prefix : blanks) + lines[i], level);
    }

    /**
        @brief Overwritten implementation of the function inherited from OutputListener, sets also the corresponding config-value.
    */
    void BaseWriter::setLevelMax(OutputLevel max)
    {
        this->configurableMaxLevel_ = max;
        this->changedConfigurableLevel();
    }

    /**
        @brief Overwritten implementation of the function inherited from OutputListener, sets also the corresponding config-value.
    */
    void BaseWriter::setAdditionalContextsLevelMax(OutputLevel max)
    {
        this->configurableAdditionalContextsMaxLevel_ = max;
        this->changedConfigurableAdditionalContextsLevel();
    }

    /**
        @brief Called if the config value has changed, updates the corresponding mask in OutputListener.
    */
    void BaseWriter::changedConfigurableLevel()
    {
        OutputListener::setLevelMax(static_cast<OutputLevel>(this->configurableMaxLevel_));
    }

    /**
        @brief Called if the config value has changed, updates the corresponding mask in OutputListener.
    */
    void BaseWriter::changedConfigurableAdditionalContextsLevel()
    {
        OutputListener::setAdditionalContextsLevelMax(static_cast<OutputLevel>(this->configurableAdditionalContextsMaxLevel_));
    }

    /**
        @brief Called if the config-vector of accepted contexts has changed, updates the masks in SubcontextOutputListener.
    */
    void BaseWriter::changedConfigurableAdditionalContexts()
    {
        OutputContextMask main_contexts = context::none;
        std::set<const OutputContextContainer*> sub_contexts;

        // iterate over all strings in the config-vector
        for (size_t i = 0; i < this->configurableAdditionalContexts_.size(); ++i)
        {
            const std::string& full_name = this->configurableAdditionalContexts_[i];

            // split the name into main-name and sub-name (if given; otherwise sub-name remains empty). both names are separated by ::
            std::string name = full_name;
            std::string subname;

            size_t pos = full_name.find("::");
            if (pos != std::string::npos)
            {
                name = full_name.substr(0, pos);
                subname = full_name.substr(pos + 2);
            }

            // get the context defined by this name
            const OutputContextContainer& context = OutputManager::getInstance().registerContext(name, subname);

            // if the context is a sub-context, insert it to the set of sub-contexts. Otherwise add it's mask to the mask of main-contexts.
            if (context.sub_id == context::no_subcontext)
                main_contexts |= context.mask;
            else
                sub_contexts.insert(&context);
        }

        // pass main-contexts and sub-contexts to SubcontextOutputListener
        this->setAdditionalContextsMask(main_contexts);
        this->setAdditionalSubcontexts(sub_contexts);
    }
}
