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
    @brief Implementation of the OutputListener class.
*/

#include "OutputListener.h"

#include "OutputManager.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes the values and registers the instance at OutputManager if requested.
        @param bRegister If \c true, the instance is automatically registered at OutputManager.
                         Should be \c false if the constructor of the derived class generates output.
    */
    OutputListener::OutputListener(bool bRegister)
    {
        this->levelMask_ = level::none;
        this->additionalContextsLevelMask_ = level::none;
        this->additionalContextsMask_ = context::none;

        if (bRegister)
            OutputManager::getInstance().registerListener(this);
    }

    /**
        @brief Destructor, unregisters the instance from OutputManager.
    */
    OutputListener::~OutputListener()
    {
        OutputManager::getInstance().unregisterListener(this);
    }

    /**
        @brief Defines the level mask in a way which accepts all output up to the level \c max.
    */
    void OutputListener::setLevelMax(OutputLevel max)
    {
        this->setLevelRange(static_cast<OutputLevel>(0x1), max);
    }

    /**
        @brief Defines the level mask in a way which accepts all output between the levels \c min and \c max.
    */
    void OutputListener::setLevelRange(OutputLevel min, OutputLevel max)
    {
        int mask = 0;
        for (int level = min; level <= max; level = level << 1)
            mask |= level;

        this->setLevelMask(static_cast<OutputLevel>(mask));
    }

    /**
        @brief Defines the level mask.
    */
    void OutputListener::setLevelMask(OutputLevel mask)
    {
        this->levelMask_ = mask;

        OutputManager::getInstance().updateCombinedLevelMask();
    }

    /**
        @brief Defines the level mask of additional contexts in a way which accepts all output up to the level \c max.
    */
    void OutputListener::setAdditionalContextsLevelMax(OutputLevel max)
    {
        this->setAdditionalContextsLevelRange(static_cast<OutputLevel>(0x1), max);
    }

    /**
        @brief Defines the level mask of additional contexts in a way which accepts all output between the levels \c min and \c max.
    */
    void OutputListener::setAdditionalContextsLevelRange(OutputLevel min, OutputLevel max)
    {
        int mask = 0;
        for (int level = min; level <= max; level = level << 1)
            mask |= level;

        this->setAdditionalContextsLevelMask(static_cast<OutputLevel>(mask));
    }

    /**
        @brief Defines the level mask of additional contexts.
    */
    void OutputListener::setAdditionalContextsLevelMask(OutputLevel mask)
    {
        this->additionalContextsLevelMask_ = mask;

        OutputManager::getInstance().updateCombinedAdditionalContextsLevelMask();
    }

    /**
        @brief Defines the mask of additional contexts.
    */
    void OutputListener::setAdditionalContextsMask(OutputContextMask mask)
    {
        this->additionalContextsMask_ = mask;

        OutputManager::getInstance().updateCombinedAdditionalContextsMask();
    }

    /**
        @brief Returns true if this listener accepts output of the given level and context, based on the levels and contexts masks.
    */
    bool OutputListener::acceptsOutput(OutputLevel level, const OutputContextContainer& context) const
    {
        // check if the output level is accepted by the level mask (independent of the context)
        if (this->levelMask_ & level)
            return true;

        // check if the output context is accepted by the additional context mask and if the level matches as well
        if ((this->additionalContextsMask_ & context.mask) && (this->additionalContextsLevelMask_ & level))
            return true;

        // otherwise we don't accept the output
        return false;
    }
}
