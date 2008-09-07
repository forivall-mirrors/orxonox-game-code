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

#ifndef _Ambient_H__
#define _Ambient_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "core/BaseObject.h"
#include "network/Synchronisable.h"

namespace orxonox
{
    class _OrxonoxExport Ambient : public BaseObject, public network::Synchronisable
    {
        public:
            Ambient();
            virtual ~Ambient();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setAmbientLight(const ColourValue& colour);
            virtual bool create();
            void registerAllVariables();

            static void setAmbientLightTest(const ColourValue& colour)
                { Ambient::instance_s->setAmbientLight(colour); }

        private:
            static Ambient* instance_s;
            ColourValue ambientLight_;

    };
}

#endif /* _Ambient_H__ */
