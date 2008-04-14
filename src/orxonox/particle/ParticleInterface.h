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

#ifndef _ParticleInterface_H__
#define _ParticleInterface_H__

#include "OrxonoxPrereqs.h"

#include <string>

// #include "ParticleInterface.h"
// #include <Ogre.h>
// #include <OIS/OIS.h>
// #include <CEGUI/CEGUI.h>
// #include <CEGUIRenderer.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>

#include "util/Math.h"


namespace orxonox
{

  class _OrxonoxExport ParticleInterface
  {
  public:

    ParticleInterface( Ogre::SceneManager *sceneManager, std::string name, std::string templateName );
    ~ParticleInterface( void );

    inline void addToSceneNode( Ogre::SceneNode* sceneNode )
        { sceneNode_ = sceneNode; sceneNode_->attachObject(particleSystem_);};
    inline void detachFromSceneNode( void )
        { sceneNode_->detachObject(particleSystem_); sceneNode_ = NULL;};

    Ogre::ParticleEmitter* getEmitter ( int emitterNr );
    void newEmitter ( void );

    Vector3 getPositionOfEmitter ( int emitterNr );
    inline void setPositionOfEmitter ( int emitterNr, Vector3 position )
        { particleSystem_->getEmitter(emitterNr)->setPosition(position); };

    inline Vector3 getDirection ( void )
        { return particleSystem_->getEmitter(0)->getDirection(); };
    void setDirection ( Vector3 direction );

    inline Real getVelocity()
        {return velocity_; };
    void setVelocity( Real v );

    inline float getRate()
      { return rate_; };
    void setRate( float r );

    inline Real getDistance()
        { return distance_; };
    void setDistance( Real d );

    inline ColourValue getColour( void )
        {return colour_;};
    void setColour( ColourValue colour );

    void switchEnable();

    inline Ogre::ParticleSystem* getParticleSystem()
        { return this->particleSystem_; };

  private:
    Ogre::SceneNode *sceneNode_;
    Ogre::SceneManager *sceneManager_;
    Ogre::ParticleSystem *particleSystem_;
    Real distance_;
    Real velocity_;
    float rate_;
    ColourValue colour_;
    int numberOfEmitters_;

    void standardizeEmitters();
  };

}

#endif /* _ParticleInterface_H__ */
