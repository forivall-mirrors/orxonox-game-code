#ifndef MODULE_ParticleInterface_H 
#define MODULE_ParticleInterface_H

#include "ParticleInterface.h"
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUIRenderer.h>

using namespace Ogre;

namespace particle
{

class ParticleInterface
{
 public:
	ParticleSystem *particleSystem_;

	ParticleInterface( SceneManager *sceneManager, String name, String templateName );
	~ParticleInterface( void );
	void addToSceneNode( SceneNode* sceneNode );
	void dettachFromSceneNode( void );
	ParticleEmitter* getEmitter ( int emitterNr );
	void newEmitter ( void );
	void setPositionOfEmitter ( int emitterNr, Vector3 position );
	Vector3 getPositionOfEmitter ( int emitterNr );
	void setDirection ( Vector3 direction );
	Vector3 getDirection ( void );
	Real getVelocity();
	void setVelocity( Real v );
	int getRate();
	void setRate( int r );
	Real getDistance();
	void setDistance( Real d );
	ColourValue getColour( void );
	void setColour( ColourValue colour );
	void switchEnable( void );

 private:
	SceneNode *sceneNode_;
	SceneManager *sceneManager_;
	Real distance_;
	Real velocity_;
	int rate_;
	ColourValue colour_;
	int numberOfEmitters_;

	void standardizeEmitters();
};


}
#endif
