#ifndef _OGREODECOLLISION_H_
#define _OGREODECOLLISION_H_

#include "OgreOdePreReqs.h"
#include <OgreVector3.h>

namespace OgreOde 
{
	class _OgreOdeExport CollisionListener
	{
	public:
		CollisionListener();
		virtual ~CollisionListener();

		virtual bool collision(Contact* contact) = 0;
	};


	/** Using the default collisionCallbacks provided here will use the internal collisions setting of 
		of the space. 
		The space collide function now takes a bool useInternalCollisionFlag if you want internal collisions.
	*/
	class _OgreOdeExport CollisionCallback : public CollisionListener
	{
	public:
	
		friend class Space;
		friend class Geometry;
	
		CollisionCallback();
		virtual ~CollisionCallback();
		
		/** handles the ode callback and passes on to pure virtual methods that inherited classes implement
		*/
		static void collisionCallback(void *data, dGeomID geom_a, dGeomID geom_b);

		/** user impliemented
		*/
		virtual void collisionCallback(OgreOde::Space* spaceFirst, OgreOde::Space* spaceSecond);
		
		virtual void collisionCallback(OgreOde::Space* space, OgreOde::Geometry* geometry);
		
		virtual void collisionCallback(OgreOde::Space* space);
		
		// This is down to a AABB level of the geometry, call collide() in here to go down further.
		virtual void collisionCallback(OgreOde::Geometry* geometryFirst, OgreOde::Geometry* geometrySecond);
	};


	class _OgreOdeExport Contact
	{
		friend class Geometry;
		friend class ContactMapCollisionListener;
	
	public:
		enum Flag
		{
			Flag_UseAdditionalFriction			= dContactMu2,
			Flag_UseFirstFrictionDirection		= dContactFDir1,
			Flag_SurfaceIsBouncy				= dContactBounce,
			Flag_UseERP							= dContactSoftERP,
			Flag_UseCFM							= dContactSoftCFM,
			Flag_IndependentMotion				= dContactMotion1, 
			Flag_AdditionalIndependentMotion	= dContactMotion2,
			Flag_UseFDS							= dContactSlip1,
			Flag_UseAdditionalFDS				= dContactSlip2,
			Flag_FrictionPyramid				= dContactApprox1_1,
			Flag_AdditionalFrictionPyramid		= dContactApprox1_2,
			Flag_BothFrictionPyramids			= dContactApprox1 
		};

	public:
		Contact();
		~Contact();

		const Ogre::Vector3& getPosition();
		const Ogre::Vector3& getNormal();
		Ogre::Real getPenetrationDepth();

        int getFirstSide();
        int getSecondSide();

		Geometry* getFirstGeometry();
		Geometry* getSecondGeometry();

		void setFirstFrictionDirection(const Ogre::Vector3& vector);
		void setFrictionMode(Contact::Flag flag);

		void setCoulombFriction(Ogre::Real mu, Ogre::Real additional_mu = -1.0);
		void setBouncyness(Ogre::Real bouncyness, Ogre::Real velocity_threshold = -1.0);
		void setSoftness(Ogre::Real ERP, Ogre::Real CFM);
		void setIndependentMotion(Ogre::Real velocity, Ogre::Real additional_velocity = -1.0);
		void setForceDependentSlip(Ogre::Real FDS);
		void setAdditionalFDS(Ogre::Real FDS);

		inline void setContact(dContact* contact)
		{
			_contact = contact;
			_contact->surface.mode = 0;
		}
	protected:
		inline Contact& operator=(dContact* contact)
		{
			_contact = contact;
			_contact->surface.mode = 0;
			return *this;
		}

	protected:
		dContact* _contact;
		Ogre::Vector3 _position,_normal;
	};

	class _OgreOdeExport ContactMapCollisionListener:public CollisionListener
	{
	public:
		ContactMapCollisionListener();
		virtual ~ContactMapCollisionListener();

		virtual bool collision(Contact* contact);

		void createContact(MaterialID materialA,MaterialID materialB); 
		Contact *getContactPtr(MaterialID materialA,MaterialID materialB); 

	protected:
		std::map<MaterialID,MaterialMap* > _map;
	};
}

#endif
