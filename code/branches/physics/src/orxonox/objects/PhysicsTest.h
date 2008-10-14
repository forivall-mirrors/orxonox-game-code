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

#ifndef _PhysicsTest_H__
#define _PhysicsTest_H__

#include "OrxonoxPrereqs.h"

#include "ogreode/OgreOdePreReqs.h"
#include "ogreode/OgreOdeMass.h"
#include "core/BaseObject.h"
#include "Tickable.h"

namespace orxonox
{
    class _OrxonoxExport PhysicsTest : public BaseObject, public Tickable
    {
    public:
        PhysicsTest();
        virtual ~PhysicsTest();
        void setConfigValues();

        virtual void tick(float dt);

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

    private:
        OgreOde::World*                 odeWorld_;
        OgreOde::Space*                 odeSpace_;
        OgreOde::StepHandler*           odeStepper_;

        OgreOde::InfinitePlaneGeometry* odeGround_;    
        OgreOde::Body*                  odeBody_;
        OgreOde::Geometry*              odeGeom_;
        OgreOde::BoxMass                odeMass_;
        Ogre::SceneNode*                sceneNode_;
        Ogre::Entity*                   entity_;

        bool                            bRunning_;
    };
}

#endif /* _PhysicsTest_H__ */
