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
#include "core/class/OrxonoxInterface.h"
#include "core/object/SmartPtr.h"

namespace orxonox
{
    class BaseObject;

    /**
    @brief Interface for receiving window events.
    */
    class _OrxonoxExport RadarViewable : virtual public OrxonoxInterface
    {
    public:
        enum Shape
        {
            Square,
            Dot,
            Triangle
        };


    public:
        RadarViewable(BaseObject* creator, const WorldEntity* wePtr);
        virtual ~RadarViewable();

        virtual void setRadarName(const std::string& name)
            {
            // The following if-clause hides objects with radarname "HIDDEN"
                if (name == "HIDDEN")
                {
                    this->bVisibility_ = 0;
                    this->settingsChanged();
                }
                if (this->radarName_ != name)
                {
                    this->radarName_ = name;
                    this->settingsChanged();
                }
            }
        const std::string& getRadarName() const
            { return this->radarName_; }

        inline void setRadarObjectCamouflage(float camouflage)
            {
                if( this->radarObjectCamouflage_ != camouflage )
                {
                    this->radarObjectCamouflage_ = camouflage;
                    this->settingsChanged();
                }
            }
        inline float getRadarObjectCamouflage() const
            { return this->radarObjectCamouflage_; }

        inline void setRadarObjectColour(const ColourValue& colour)
            {
                if(this->radarObjectColour_ != colour)
                {
                    this->radarObjectColour_ = colour;
                    this->settingsChanged();
                }
            }
        inline const ColourValue& getRadarObjectColour() const
            { return this->radarObjectColour_; }

//         void setRadarObjectDescription(const std::string& str);
//         inline const std::string& getRadarObjectDescription() const
//             { return this->radarObjectDescription_; }

        inline void setRadarVisibility(bool b)
            {
                if(b!=this->bVisibility_)
                {
                    this->bVisibility_ = b;
                    this->settingsChanged();
                }
            }
        inline bool getRadarVisibility() const
            { return this->bVisibility_; }

        virtual const WorldEntity* getWorldEntity() const{ return this->wePtr_; }

        const Vector3& getRVWorldPosition() const;
        Vector3 getRVOrientedVelocity() const;
        Vector3 getRVVelocity() const;

        inline void setRadarObjectShape(Shape shape)
            {
                if( this->radarObjectShape_ != shape )
                {
                    this->radarObjectShape_ = shape;
                    this->settingsChanged();
                }
            }
        inline Shape getRadarObjectShape() const
            { return this->radarObjectShape_; }

        inline void setRadarObjectScale(float scale)
            {
                if(this->scale_ != scale)
                {
                    this->scale_ = scale;
                    this->settingsChanged();
                }
            }
        inline float getRadarObjectScale() const
            { return this->scale_; }

        void settingsChanged();


        bool isHumanShip_;
        inline const std::string& getUniqueId()
        {
            return this->uniqueId_;
        }
        //friend class Map;

    private:
        void validate(const WorldEntity* object) const;
        bool bVisibility_;
        bool bInitialized_;
        //Map
        std::string uniqueId_;


        //Radar
        const WorldEntity* wePtr_;
        SmartPtr<Radar> radar_;
        float radarObjectCamouflage_;
        Shape radarObjectShape_;
        std::string radarObjectDescription_;
        ColourValue radarObjectColour_;
        float scale_;
        std::string radarName_;
    };
}

#endif /* _RadarViewable_H__ */
