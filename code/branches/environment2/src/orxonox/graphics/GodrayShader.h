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
 *      Markus 'atok' Wegmann
 *   Co-authors:
 *      ...
 *
 */


/**
 @file
 
 @brief Header of the GodrayShader class.
*/


#ifndef _GodrayShader_H__
#define _GodrayShader_H__

#include "OrxonoxPrereqs.h"

#include <OgreCompositorInstance.h>

#include "tools/interfaces/Tickable.h"
#include "core/BaseObject.h"
#include "util/Math.h"
#include "WorldEntity.h"
#include "GlobalShader.h"


namespace orxonox
{
    /**
     @brief 
     
    */
    class _OrxonoxExport GodrayShader : public BaseObject, public Tickable
    {
        public:
            GodrayShader(BaseObject* creator);
            virtual ~GodrayShader();
            
            virtual void tick(float dt);
        
            virtual void updateParameters();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        
            virtual void changedVisibility();
        
            void setWorldEntity(WorldEntity* worldEntity);
            
            inline const GlobalShader& getGlobalShader() const
            { return this->globalShader_; }
        
            void setSunPosition(const Vector3& position);
            void setSunColor(const ColourValue& color);
            void setSkyColor(const ColourValue& color);
            const Vector3& getSunPosition() const;
            const ColourValue& getSunColor() const;
            const ColourValue& getSkyColor() const;
        
        private:
        
            WorldEntity* worldEntity_;
            GlobalShader globalShader_;
            Ogre::CompositorInstance* compositor_;
            Vector3 sunPosition_;
            ColourValue sunColor_;
            ColourValue skyColor_;
            float exposure_;
            float decay_;
            float density_;
    };
}

#endif /* _GodrayShader_H__ */
