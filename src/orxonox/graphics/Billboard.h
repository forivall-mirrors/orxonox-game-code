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
 *      Maurus Kaufmann
 *
 */

#ifndef _Billboard_H__
#define _Billboard_H__

#include "OrxonoxPrereqs.h"

#include "OgreBillboardSet.h"

#include "util/Math.h"
#include "tools/BillboardSet.h"
#include "interfaces/TeamColourable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Billboard : public StaticEntity, public TeamColourable
    {
        public:
            Billboard(Context* context);
            virtual ~Billboard();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedVisibility();

            inline const BillboardSet& getBillboardSet() const
                { return this->billboard_; }

            inline void setMaterial(const std::string& material)
                { this->material_ = material; this->changedMaterial(); }
            inline const std::string& getMaterial() const
                { return this->material_; }

            inline void setColour(const ColourValue& colour)
                { this->colour_ = colour; this->changedColour(); }
            inline const ColourValue& getColour() const
                { return this->colour_; }


            inline void setRotation(const Radian& rotation)
                { this->rotation_ = rotation; this->changedRotation(); }
            inline const Radian& getRotation() const
                { return this->rotation_; }


            virtual void setTeamColour(const ColourValue& colour)
                { this->setColour(colour); }
                
            void setBillboardType(Ogre::BillboardType bbt);
            
            void setCommonDirection(Vector3 vec); //!< normalised Vector vec as argument
            
            void setCommonUpVector(Vector3 vec); //!< normalised Vector vec as argument
            
            void setDefaultDimensions(float width, float height);


        protected:
            inline BillboardSet& getBillboardSet()
                { return this->billboard_; }

            virtual void changedColour();

        private:
            void registerVariables();
            void changedMaterial();
            void changedRotation();

            BillboardSet billboard_;
            std::string material_;
            ColourValue colour_;
            Radian rotation_;
    };
}

#endif /* _Billboard_H__ */
