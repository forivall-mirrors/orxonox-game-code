#ifndef MODULE_ParticleInterface_H
#define MODULE_ParticleInterface_H

// #include "ParticleInterface.h"
// #include <Ogre.h>
// #include <OIS/OIS.h>
// #include <CEGUI/CEGUI.h>
// #include <CEGUIRenderer.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>


namespace particle
{

class ParticleInterface
{
 public:

	ParticleInterface( Ogre::SceneManager *sceneManager, Ogre::String name, Ogre::String templateName );
	~ParticleInterface( void );

  void inline addToSceneNode( Ogre::SceneNode* sceneNode ) { sceneNode_ = sceneNode; sceneNode_->attachObject(particleSystem_);};
  void inline detachFromSceneNode( void ) { sceneNode_->detachObject(particleSystem_); sceneNode_ = NULL;};

	Ogre::ParticleEmitter* getEmitter ( int emitterNr );
	void newEmitter ( void );

  Ogre::Vector3 getPositionOfEmitter ( int emitterNr );
  inline void setPositionOfEmitter ( int emitterNr, Ogre::Vector3 position ) { particleSystem_->getEmitter(emitterNr)->setPosition(position); };

  inline Ogre::Vector3 getDirection ( void ) { return particleSystem_->getEmitter(0)->getDirection(); };
	void setDirection ( Ogre::Vector3 direction );

	inline Ogre::Real getVelocity() {return velocity_; };
	void setVelocity( Ogre::Real v );

	inline int getRate() { return rate_; };
	void setRate( int r );

	inline Ogre::Real getDistance() { return distance_; };
	void setDistance( Ogre::Real d );

	inline Ogre::ColourValue getColour( void ) {return colour_;};
	void setColour( Ogre::ColourValue colour );

	void switchEnable( void );

  inline Ogre::ParticleSystem* getParticleSystem() {return this->particleSystem_; };

 private:
	Ogre::SceneNode *sceneNode_;
	Ogre::SceneManager *sceneManager_;
  Ogre::ParticleSystem *particleSystem_;
	Ogre::Real distance_;
	Ogre::Real velocity_;
	int rate_;
	Ogre::ColourValue colour_;
	int numberOfEmitters_;

	void standardizeEmitters();
};


}
#endif
