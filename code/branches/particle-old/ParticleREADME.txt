=== ParticleEffects ===

== Files ==
Benötigte Files: ParticleInterface.h, ParticleInterface.cc
Benötigte Resourcen: alle particle-Scripts und das material-Script

== Code ==

	#include "ParticleInterface.h"

	//Definiere SceneNode particleNode mit einer geeigneten Entity z.B. Schiff

	//Effekt w für die Laser
	particle::ParticleInterface *w = new particle::ParticleInterface(mSceneMgr,"schuss","Orxonox/schuss");;

	//Effekt e für die Triebwerke
	particle::ParticleInterface *e = new particle::ParticleInterface(mSceneMgr,"engine","Orxonox/treibwerk");;

	//Einstellen der Parameter

	//Macht, dass sich die Parameter mit der SceneNode mitbewegen
	w->particleSystem_->setParameter("local_space","true");
	e->particleSystem_->setParameter("local_space","true");

	//Fügt einen weiteren Laser hinzu und setzt die beiden an 
	//zwei verschiedene Positionen und setzt die Richtung richtig
	w->newEmitter();
	w->setDirection(Vector3(0,0,1));
	w->setPositionOfEmitter(0, Vector3(10,10,0));
	w->setPositionOfEmitter(1, Vector3(-10,10,0));

	//Platziert das Triebwerk und gibt die Ausstossrichtung an
	e->setPositionOfEmitter(0, Vector3(0,0,-10));
	e->setDirection(Vector3(0,0,-1));


	//Anfügen der PartikelEffekte an die SceneNode
	w->addToSceneNode(particleNode);
	e->addToSceneNode(particleNode);


== Dynamische Änderungen ==

	//Verändert die Geschwindigkeit der Partikel 
	w->setVelocity(w->getVelocity()+0.05);
	w->setVelocity(w->getVelocity()-0.05);

	//Verändert die Distanz, bis wo die Partikel zu sehen sind
	w->setDistance(w->getDistance()+0.1);
	w->setDistance(w->getDistance()-0.1);

	//Ändert von Partikel erzeugen auf gestoppt oder umgekehrt
	w->switchEnable();
