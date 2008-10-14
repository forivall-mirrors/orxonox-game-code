
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeUtility.h"

using namespace OgreOde;
using namespace Ogre;

const Ogre::Real Utility::Infinity = dInfinity;

Real Utility::randomReal()
{
	return (Real)dRandReal();
}

/**
According to the ODE docs;
	
By adjusting the values of ERP and CFM, you can achieve various effects. 
For example you can simulate springy constraints, where the two bodies oscillate 
as though connected by springs. Or you can simulate more spongy constraints, without 
the oscillation. In fact, ERP and CFM can be selected to have the same effect as any 
desired spring and damper constants. If you have a spring constant kp and damping constant kd, 
then the corresponding ODE constants are:
	
ERP = h kp / (h kp + kd)
CFM = 1 / (h kp + kd)
	
where h is the stepsize. These values will give the same effect as a spring-and-damper 
system simulated with implicit first order integration.
*/
//-----------------------------------------------------------------------
Real Utility::getCFM(Real spring, Real dampening, Real timeStep)
{
	return 1 / ((timeStep * spring) + dampening);
}

//-----------------------------------------------------------------------
Real Utility::getERP(Real spring, Real dampening, Real timeStep)
{
	return  (timeStep * spring) / ((timeStep * spring) + dampening);
}
	
//-----------------------------------------------------------------------
void Utility::getSpringConstants(Real CFM, Real ERP, Real timeStep, Real &spring, Real &dampening)
{
	spring = (ERP / CFM) / timeStep;
	dampening = (1 / CFM) - timeStep * spring;
}
