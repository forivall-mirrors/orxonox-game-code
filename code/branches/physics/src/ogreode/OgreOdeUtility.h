#ifndef _OGREODEUTILITY_H_
#define _OGREODEUTILITY_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
	class _OgreOdeExport Utility
	{
	public:
		Utility(){}
		~Utility(){}

		static Ogre::Real randomReal();

		static const Ogre::Real Infinity;

		/** returns a CFM value for use on joints or contacts
			@param timeStep is the the stepping time for the world, see Steppers
		*/
		static Ogre::Real getCFM(Ogre::Real spring, Ogre::Real dampening, Ogre::Real timeStep);

		/** returns a ERP value for use on joints or contacts
		*/
		static Ogre::Real getERP(Ogre::Real spring, Ogre::Real dampening, Ogre::Real timeStep);
		
		/** can be used to get spring constants out of a ERP and CFM value
		*/
		static void getSpringConstants(Ogre::Real CFM, Ogre::Real ERP, Ogre::Real timeStep, Ogre::Real &spring, Ogre::Real &dampening);
	};
}

#endif

