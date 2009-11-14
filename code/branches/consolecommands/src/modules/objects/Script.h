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

#ifndef _Script_H__
#define _Script_H__

#include "objects/ObjectsPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{
    class _ObjectsExport Script : public BaseObject
    {
    public:
        Script(BaseObject* creator);
        ~Script();
        void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        void execute();

        void setCode(const std::string& code) { code_ = code; }
        const std::string& getCode() const { return code_; }

    private:
        std::string code_;
        LuaState* luaState_;
    };
}

#endif /* _Script_H__ */
