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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#ifndef _GUIOverlay_H__
#define _GUIOverlay_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "OrxonoxOverlay.h"

namespace orxonox
{
    class _OrxonoxExport GUIOverlay : public OrxonoxOverlay
    {
        public:

            GUIOverlay(BaseObject* creator);
            virtual ~GUIOverlay();

            virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

            inline void setGUIName(const std::string& name) { this->guiName_ = name; }
            inline const std::string& getGUIName() const { return this->guiName_; }

            virtual void changedVisibility();

        private:
            std::string guiName_;
    };
}

#endif
