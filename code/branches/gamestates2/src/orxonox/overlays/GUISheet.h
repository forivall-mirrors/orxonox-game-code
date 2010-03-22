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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _GUISheet_H__
#define _GUISheet_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{
    //! Simple XML loadable interface to the otherwise Lua coded GUI framework.
    class _OrxonoxExport GUISheet : public BaseObject
    {
    public:
        GUISheet(BaseObject* creator);
        ~GUISheet();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        void show();
        void hide();

        void setScript(const std::string& filename);
        inline const std::string& getScript() const
            { return this->script_; }

        inline void setShowOnLoad(bool bShow)
            { this->bShowOnLoad_ = bShow; }
        inline bool getShowOnLoad() const
            { return this->bShowOnLoad_; }

        void setCursorVisibility(bool bShow);
        inline bool getCursorVisibility() const
            { return this->bShowCursor_; }

        void setPreviousHiding(bool bHide);
        inline bool getPreviousHiding() const
            { return this->bHidePrevious_; }

    private:
        std::string script_;
        bool bShowOnLoad_;
        bool bShowCursor_;
        bool bHidePrevious_;
  };
}

#endif /* _GUISheet_H__ */
