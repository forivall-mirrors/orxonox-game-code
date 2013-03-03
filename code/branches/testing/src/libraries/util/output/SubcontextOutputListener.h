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
    /**
        @brief This class extends the basic OutputListener interface and adds the ability to filter sub-contexts.

        Defining additional contexts with setAdditionalContextsMask() enables
        all sub-contexts of these additional contexts. To accept only some
        particular sub-contexts, setAdditionalSubcontexts() has to be used.
        Note that this requires a set, since a mask is not possible with
        sub-contexts.

        The "final" context mask which will be seen by OutputManager is the
        combination of all regular contexts plus the masks of all sub-contexts.

        @remark
        It would have been possible to implement filtering of sub-contexts
        directly in OutputListener and even to make OutputManager aware of
        sub-contexts. This would reduce the amount of unnecessarily generated
        output, but also increase the complexity of the checks whether some
        output is needed.
        On the other hand, filtering of sub-contexts makes the whole concept
        more complicated, as it adds another mask and a set. So to keep it
        clean and simple I figured it's best to put sub-context filtering into
        a seaparate class.
    */
    class _UtilExport SubcontextOutputListener : public OutputListener
    {
        public:
            SubcontextOutputListener(bool bRegister = true);
            virtual ~SubcontextOutputListener();

            virtual void setAdditionalContextsMask(OutputContextMask mask);
            void setAdditionalSubcontexts(const std::set<const OutputContextContainer*>& subcontexts);

            virtual bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const;

        private:
            OutputContextMask subcontextsCheckMask_;    ///< All contexts defined by this mask need to be checked whether they are accepted by the set of sub-contexts
            OutputContextMask subcontextsNoCheckMask_;  ///< All contexts defined by this mask don't need to be checked since we accept all sub-contexts
            std::set<OutputContextSubID> subcontexts_;  ///< The set of accepted sub-contexts
    };
}

#endif /* _SubcontextOutputListener_H__ */
