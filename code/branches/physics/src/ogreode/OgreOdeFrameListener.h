/*
    PhysicsFrameListener.h

	A very simple wrapper class for ODE
	primarily for the example car application
*/
#ifndef _PHYSICSFRAMELISTENER_H_
#define _PHYSICSFRAMELISTENER_H_

#include "OgreOdePreReqs.h"
#include <OgreSceneQuery.h>
#include <OgreFrameListener.h>

// Uncomment this if you're planning to use the Plane2D joint
// But you'll have to have modified ODE first!
// #define INCLUDE_PLANE2D_JOINT

/*
A physical thing
*/
class PhysicalThing : public Ogre::RaySceneQueryListener
{
public:
	// Doesn't actually do anything
	PhysicalThing(dSpace *space);
	~PhysicalThing(void);

	// Just create geoms
    void makePlane(Ogre::Real a, Ogre::Real b, Ogre::Real c, Ogre::Real d);				// Plane
    void makeBox(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real lx, Ogre::Real ly, Ogre::Real lz);	// Box
    void makeSphere(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real r);				// Sphere

	// Create bodies and geoms
    void makePlane(dWorld *world, Ogre::SceneNode *node);
	void makeBox(dWorld *world, Ogre::SceneNode *node, Ogre::Real mass = 1, Ogre::Vector3 const &offs = Ogre::Vector3(0,0,0));
    void makeSphere(dWorld *world, Ogre::SceneNode *node, Ogre::Real mass = 1);
	void makeTerrainTriMesh(dWorld *world, Ogre::SceneManager *mgr, Ogre::Real grid_size, Ogre::Real grid_spacing);

	// Callback functions for the ray scene query functionality
	virtual bool queryResult(Ogre::SceneQuery::WorldFragment *fragment, Ogre::Real distance);
	virtual bool queryResult(Ogre::MovableObject *obj, Ogre::Real distance);

	// Update the position and orientation of the scene node we're controlling
	void update(void);

	// Utility functions to get at internal stuff
	Ogre::SceneNode *getNode(void);
	dBodyID getBodyID(void);
	dSpaceID getSpaceID(void);

protected:
	dGeom		*_geom;
	dBody		*_body;
	dSpace		*_space;
	Ogre::SceneNode	*_node;

	std::list<Ogre::Vector3> _frag;

	dVector3 *_vertices;
	int *_indices;
};

/*
A Frame listener that implements the physics
*/
class PhysicsFrameListener : public Ogre::FrameListener
{
protected:
	// All the stuff we need for ODE
    dWorld          *world;
    dHashSpace      *space;
    dJointGroup     *contactgroup;
    dSpace          *current_space;
	bool			paused;

	// Keep track of anything we've created
    std::list<PhysicalThing*>   things;
    std::list<dSimpleSpace*>    spaces;
	std::list<dJoint*>			joints;
    
	// Adjustable variables
    Ogre::Real total_time,step_size;
	Ogre::Real k_spring,k_damper,k_tyre,k_mu,k_erp,k_cfm;

	// Utility function to find the physical thing associated with an Ogre scene node
	dBodyID getBodyID(Ogre::SceneNode *node);

public:
	// Standard constructor/destructor
    PhysicsFrameListener(void);
    ~PhysicsFrameListener(void);

	// Tell us when every frame's about to be started
    bool frameStarted(const Ogre::FrameEvent& evt);
	void pause(bool p = true);

	// Mainly so we can access internal stuff from the collision callback
    dWorld *getWorld(void){return world;};
    dSpace *getSpace(void){return space;};
    dJointGroup *getContactGroup(void){return contactgroup;};

	// Create things manually, no physical bodies 
	// so they won't move, but things will collide with them
    PhysicalThing *createPlane(Ogre::Real a,Ogre::Real b,Ogre::Real c,Ogre::Real d);
    PhysicalThing *createBox(Ogre::Real x,Ogre::Real y,Ogre::Real z,Ogre::Real lx,Ogre::Real ly,Ogre::Real lz);
    PhysicalThing *createSphere(Ogre::Real x,Ogre::Real y,Ogre::Real z,Ogre::Real r);

	// Create objects based on Ogre nodes, these nodes will then
	// come under control of the physics simulation and will move about
	// as if by magic!
	// Except for the plane, which can't actually be a physical body
	// and the trimesh which we're not going to allow to be a physical body
    PhysicalThing *createPlane(Ogre::SceneNode *node);
	PhysicalThing *createBox(Ogre::SceneNode *node,Ogre::Real mass = 1,Ogre::Vector3 const &offs = Ogre::Vector3(0,0,0));
    PhysicalThing *createSphere(Ogre::SceneNode *node,Ogre::Real mass = 1);
	PhysicalThing *createTerrainTriMesh(Ogre::SceneManager *mgr,Ogre::Real grid_size,Ogre::Real grid_spacing);

	// Create and activate a new space inside the standard hash space
	// Things won't collide within a space so (for example) the objects
	// that make up a car won't collide with each other
    dSimpleSpace *createSpace(void);

	// Create joints between the nodes specified
	// The order of n1/n2 does matter; n1 is what you want attaching
	// and n2 is what you want it attaching to
	dJoint *createHinge2(Ogre::SceneNode *n1,Ogre::SceneNode *n2,const Ogre::Vector3 &suspension = Ogre::Vector3(0,1,0),const Ogre::Vector3 &axle = Ogre::Vector3(1,0,0));
	dJoint *createHinge(Ogre::SceneNode *n1,Ogre::SceneNode *n2,const Ogre::Vector3 &axis = Ogre::Vector3(1,0,0));
	dJoint *createSlider(Ogre::SceneNode *n1,Ogre::SceneNode *n2,const Ogre::Vector3 &axis = Ogre::Vector3(0,1,0));
	dJoint *createBall(Ogre::SceneNode *n1,Ogre::SceneNode *n2);
#ifdef INCLUDE_PLANE2D_JOINT
	dJoint *createPlane2D(Ogre::SceneNode *n);
#endif

	// Apply forces to a body under our control
	void addTorque(Ogre::SceneNode *n,const Ogre::Vector3 &force);

	// Set the parameters used for the CFM/ERP joint parameters
	void setSuspension(Ogre::Real spring,Ogre::Real damper);

	// Set the acceleration due to gravity
	void setGravity(Ogre::Real g);

	// Get our constants
	Ogre::Real getTyre(void){return k_tyre;};
	Ogre::Real getMu(void){return k_mu;};
	Ogre::Real getCFM(void){return k_cfm;};
	Ogre::Real getERP(void){return k_erp;};

	// Static function for ODE to call when collisions (potentially) occur
    static void collisionCallback(void *data,dGeomID o1,dGeomID o2);
};

#endif


