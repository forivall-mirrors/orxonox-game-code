/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#ifndef _Namespace_H__
#define _Namespace_H__

#include <list>

#include "BaseObject.h"
#include "util/XMLIncludes.h"

#include "CorePrereqs.h"

namespace orxonox
{
    class Namespace : public BaseObject
    {
        public:
            Namespace();
            virtual ~Namespace();

            virtual void XMLPort(Element& xmlelement, bool loading);

            void loadObjects(BaseObject* object);
            const BaseObject* saveObjects(unsigned int index) const;

        private:
            std::list<Namespace*> namespaces_;
    };
}

#endif /* _Namespace_H__ */
