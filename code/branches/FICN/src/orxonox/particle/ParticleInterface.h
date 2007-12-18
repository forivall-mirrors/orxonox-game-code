#ifndef MODULE_ParticleInterface_H
#define MODULE_ParticleInterface_H

// #include "ParticleInterface.h"
#include <Ogre.h>
#include <OIS/OIS.h>
// #include <CEGUI/CEGUI.h>
// #include <CEGUIRenderer.h>


namespace particle
{

class ParticleInterface
{
 public:
	Ogre::ParticleSystem *particleSystem_;

	ParticleInterface( Ogre::SceneManager *sceneManager, Ogre::String name, Ogre::String templateName );
	~ParticleInterface( void );
	void addToSceneNode( Ogre::SceneNode* sceneNode );
	void dettachFromSceneNode( void );
	Ogre::ParticleEmitter* getEmitter ( int emitterNr );
	void newEmitter ( void );
	void setPositionOfEmitter ( int emitterNr, Ogre::Vector3 position );
	Ogre::Vector3 getPositionOfEmitter ( int emitterNr );
	void setDirection ( Ogre::Vector3 direction );
	Ogre::Vector3 getDirection ( void );
	Ogre::Real getVelocity();
	void setVelocity( Ogre::Real v );
	int getRate();
	void setRate( int r );
	Ogre::Real getDistance();
	void setDistance( Ogre::Real d );
	Ogre::ColourValue getColour( void );
	void setColour( Ogre::ColourValue colour );
	void switchEnable( void );

 private:
	Ogre::SceneNode *sceneNode_;
	Ogre::SceneManager *sceneManager_;
	Ogre::Real distance_;
	Ogre::Real velocity_;
	int rate_;
	Ogre::ColourValue colour_;
	int numberOfEmitters_;

	void standardizeEmitters();
};


}
#endif
