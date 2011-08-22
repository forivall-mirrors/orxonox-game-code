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
    @brief Implementation of the SubcontextOutputListener interface.
*/

#include "SubcontextOutputListener.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes the context masks.
    */
    SubcontextOutputListener::SubcontextOutputListener(bool bRegister) : OutputListener(bRegister)
    {
        this->subcontextsCheckMask_ = context::none;
        this->subcontextsNoCheckMask_ = context::none;
    }

    /**
        @brief Destructor.
    */
    SubcontextOutputListener::~SubcontextOutputListener()
    {
    }

    /**
        @brief Overwritten implementation of the function defined by OutputListener.

        Contexts defined with this function are accepted independent of the
        sub-context. The "final" mask of additional contexts is defined by the
        combination of this mask and the masks of all accepted sub-contexts.
    */
    void SubcontextOutputListener::setAdditionalContextsMask(OutputContextMask mask)
    {
        this->subcontextsNoCheckMask_ = mask;

        OutputListener::setAdditionalContextsMask(this->subcontextsCheckMask_ | this->subcontextsNoCheckMask_);
    }

    /**
        @brief Defines the set of accepted sub-contexts.

        The masks of sub-contexts in this set are added to the mask of
        additional contexts, but output is only accepted if the exact
        sub-context exists in this set.
    */
    void SubcontextOutputListener::setAdditionalSubcontexts(const std::set<const OutputContextContainer*>& subcontexts)
    {
        this->subcontextsCheckMask_ = context::none;
        this->subcontexts_.clear();

        // compose the mask of subcontexts and build the set of sub-context-IDs
        for (std::set<const OutputContextContainer*>::const_iterator it = subcontexts.begin(); it != subcontexts.end(); ++it)
        {
            this->subcontextsCheckMask_ |= (*it)->mask;
            this->subcontexts_.insert((*it)->sub_id);
        }

        OutputListener::setAdditionalContextsMask(this->subcontextsCheckMask_ | this->subcontextsNoCheckMask_);
    }

    /**
        @brief Returns true if this listener accepts output of the given level and context, based on the levels and contexts masks, as well as the set of accepted sub-contexts.
    */
    bool SubcontextOutputListener::acceptsOutput(OutputLevel level, const OutputContextContainer& context) const
    {
        // check if the output level is accepted by the level mask (independent of the context)
        if (this->getLevelMask() & level)
            return true;

        // check if the output level is accepted by the additional contexts level mask
        if (this->getAdditionalContextsLevelMask() & level)
        {
            // check if the output context is accepted by the "no check" mask
            if (this->subcontextsNoCheckMask_ & context.mask)
                return true;

            // check if the output context is accepted by the "check" mask
            if (this->subcontextsCheckMask_ & context.mask)
            {
                // check if the output's subcontext is in the set of accepted sub-context
                if (this->subcontexts_.find(context.sub_id) != this->subcontexts_.end())
                    return true;
            }
        }

        // otherwise we don't accept the output
        return false;
    }
}
