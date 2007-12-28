#ifndef _ParticleInterface_H__
#define _ParticleInterface_H__

#include <string>

// #include "ParticleInterface.h"
// #include <Ogre.h>
// #include <OIS/OIS.h>
// #include <CEGUI/CEGUI.h>
// #include <CEGUIRenderer.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>

#include "../OrxonoxPrereqs.h"

#include "misc/Math.h"


namespace orxonox
{

  class ParticleInterface
  {
  public:

    ParticleInterface( Ogre::SceneManager *sceneManager, std::string name, std::string templateName );
    ~ParticleInterface( void );

    void inline addToSceneNode( Ogre::SceneNode* sceneNode ) { sceneNode_ = sceneNode; sceneNode_->attachObject(particleSystem_);};
    void inline detachFromSceneNode( void ) { sceneNode_->detachObject(particleSystem_); sceneNode_ = NULL;};

    Ogre::ParticleEmitter* getEmitter ( int emitterNr );
    void newEmitter ( void );

    Vector3 getPositionOfEmitter ( int emitterNr );
    inline void setPositionOfEmitter ( int emitterNr, Vector3 position ) { particleSystem_->getEmitter(emitterNr)->setPosition(position); };

    inline Vector3 getDirection ( void ) { return particleSystem_->getEmitter(0)->getDirection(); };
    void setDirection ( Vector3 direction );

    inline Real getVelocity() {return velocity_; };
    void setVelocity( Real v );

    inline int getRate() { return rate_; };
    void setRate( int r );

    inline Real getDistance() { return distance_; };
    void setDistance( Real d );

    inline ColourValue getColour( void ) {return colour_;};
    void setColour( ColourValue colour );

    void switchEnable();

    inline Ogre::ParticleSystem* getParticleSystem() { return this->particleSystem_; };

  private:
    Ogre::SceneNode *sceneNode_;
    Ogre::SceneManager *sceneManager_;
    Ogre::ParticleSystem *particleSystem_;
    Real distance_;
    Real velocity_;
    int rate_;
    ColourValue colour_;
    int numberOfEmitters_;

    void standardizeEmitters();
  };

}

#endif /* _ParticleInterface_H__ */
