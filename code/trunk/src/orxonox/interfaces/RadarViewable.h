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

#ifndef _RadarViewable_H__
#define _RadarViewable_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <cassert>

#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief Interface for receiving window events.
    */
    class _OrxonoxExport RadarViewable : virtual public OrxonoxClass
    {
    public:
        enum Shape
        {
            Square,
            Dot,
            Triangle
        };


    public:
        RadarViewable();
        virtual ~RadarViewable();

        inline void setRadarObjectCamouflage(float camouflage)
            { this->radarObjectCamouflage_ = camouflage; }
        inline float getRadarObjectCamouflage() const
            { return this->radarObjectCamouflage_; }

        inline void setRadarObjectColour(const ColourValue& colour)
            { this->radarObjectColour_ = colour; }
        inline const ColourValue& getRadarObjectColour() const
            { return this->radarObjectColour_; }

        void setRadarObjectDescription(const std::string& str);
        inline const std::string& getRadarObjectDescription() const
            { return this->radarObjectDescription_; }

        inline void setRadarVisibility(bool b)
            { this->bVisibility_ = b; }
        inline bool getRadarVisibility() const
            { return this->bVisibility_; }

        virtual const WorldEntity* getWorldEntity() const = 0;

        const Vector3& getRVWorldPosition() const;
        Vector3 getRVOrientedVelocity() const;

        inline void setRadarObjectShape(Shape shape)
            { this->radarObjectShape_ = shape; }
        inline Shape getRadarObjectShape() const
            { return this->radarObjectShape_; }

/*
        inline void setMapNode(Ogre::SceneNode * node)
            { this->MapNode_ = node; }
        inline Ogre::SceneNode * getMapNode() const
            { return this->MapNode_; }
        inline void setMapEntity(Ogre::Entity * ent)
            { this->MapEntity_ = ent; }
        inline Ogre::Entity * getMapEntity() const
            { return this->MapEntity_; }
*/
        //Used for Map
        Ogre::SceneNode * MapNode_;
        Ogre::Entity * MapEntity_;
        Ogre::DynamicLines* line_;
        Ogre::SceneNode * LineNode_;
        void addMapEntity();
        void updateMapPosition();
        bool isHumanShip_;
        inline const std::string& getUniqueId()
        {
            return this->uniqueId_;
        }
        //friend class Map;

    private:
        void validate(const WorldEntity* object) const;
        bool bVisibility_;
        //Map
        std::string uniqueId_;


        //Radar
        float radarObjectCamouflage_;
        Shape radarObjectShape_;
        std::string radarObjectDescription_;
        ColourValue radarObjectColour_;

    };
}

#endif /* _RadarViewable_H__ */
