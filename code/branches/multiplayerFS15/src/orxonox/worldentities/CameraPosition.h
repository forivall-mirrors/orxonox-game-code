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

#ifndef _CameraPosition_H__
#define _CameraPosition_H__

#include "OrxonoxPrereqs.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport CameraPosition : public StaticEntity
    {
        public:
            CameraPosition(Context* context);
            virtual ~CameraPosition();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setDrag(bool bDrag)
                { this->bDrag_ = bDrag; }
            inline bool getDrag() const
                { return this->bDrag_; }

            inline void setAllowMouseLook(bool bAllow)
                { this->bAllowMouseLook_ = bAllow; }
            inline bool getAllowMouseLook() const
                { return this->bAllowMouseLook_; }

            inline void setIsAbsolute(bool bAbsolute)
                { this->bAbsolute_ = bAbsolute; }
            inline bool getIsAbsolute() const
                { return this->bAbsolute_; }

            inline void setRenderCamera(bool bRenderCamera)
                { this->bRenderCamera_ = bRenderCamera; }
            inline bool getRenderCamera() const
                { return this->bRenderCamera_; }

            void attachCamera(Camera* camera);
            void attachCamera(Ogre::Camera* camera);

        private:
            bool bDrag_;
            bool bAllowMouseLook_;
            bool bAbsolute_;
            bool bRenderCamera_;
    };
}

#endif /* _CameraPosition_H__ */
