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

#include "SubcontextOutputListener.h"

namespace orxonox
{
    SubcontextOutputListener::SubcontextOutputListener(bool bRegister) : OutputListener(bRegister)
    {
        this->subcontextsCheckMask_ = context::none;
        this->subcontextsNoCheckMask_ = context::none;
    }

    SubcontextOutputListener::~SubcontextOutputListener()
    {
    }

    void SubcontextOutputListener::setAdditionalContextsMask(OutputContextMask mask)
    {
        this->subcontextsNoCheckMask_ = mask;

        OutputListener::setAdditionalContextsMask(this->subcontextsCheckMask_ | this->subcontextsNoCheckMask_);
    }

    void SubcontextOutputListener::setAdditionalSubcontexts(const std::set<const OutputContextContainer*>& subcontexts)
    {
        this->subcontextsCheckMask_ = context::none;
        this->subcontexts_.clear();

        for (std::set<const OutputContextContainer*>::const_iterator it = subcontexts.begin(); it != subcontexts.end(); ++it)
        {
            this->subcontextsCheckMask_ |= (*it)->mask;
            this->subcontexts_.insert((*it)->sub_id);
        }

        OutputListener::setAdditionalContextsMask(this->subcontextsCheckMask_ | this->subcontextsNoCheckMask_);
    }

    /**
        @brief Returns true if this listener accepts output of the given level and context, based on the levels, contexts masks, and sub-contexts.
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
