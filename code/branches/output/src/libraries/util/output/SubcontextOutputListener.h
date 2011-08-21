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
    @ingroup Output
    @brief Declaration of the SubcontextOutputListener interface which adds the ability to filter sub-contexts to OutputListener.
*/

#ifndef _SubcontextOutputListener_H__
#define _SubcontextOutputListener_H__

#include "util/UtilPrereqs.h"

#include <set>

#include "OutputListener.h"

namespace orxonox
{
    class _UtilExport SubcontextOutputListener : public OutputListener
    {
        public:
            SubcontextOutputListener(bool bRegister = true);
            virtual ~SubcontextOutputListener();

            void setAdditionalContextsMask(OutputContextMask mask);
            void setAdditionalSubcontexts(const std::set<const OutputContextContainer*>& subcontexts);

            virtual bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const;

        private:
            OutputContextMask subcontextsCheckMask_;
            OutputContextMask subcontextsNoCheckMask_;
            std::set<OutputContextSubID> subcontexts_;
    };
}

#endif /* _SubcontextOutputListener_H__ */
