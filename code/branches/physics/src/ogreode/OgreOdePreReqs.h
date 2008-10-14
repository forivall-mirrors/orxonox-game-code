#ifndef _OGREODEPREREQS_H_
#define _OGREODEPREREQS_H_

#include "Ogre.h"
#include "OgreNoMemoryMacros.h"
#include "ode/ode.h"
#include "OgreMemoryMacros.h"


namespace OgreOde 
{
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #   if defined( OGREODE_EXPORTS )
    #       define _OgreOdeExport __declspec( dllexport )
    #   else
    #       if defined( __MINGW32__ )
    #           define _OgreOdeExport
    #       else
    #           define _OgreOdeExport __declspec(dllimport)
    #       endif
    #   endif
    #elif defined ( OGRE_GCC_VISIBILITY )
    #   define _OgreOdeExport  __attribute__ ((visibility("default")))
    #else
    #   define _OgreOdeExport
    #endif

// determine ODE version. In 0.10 and following, ODE uses odeconfig.h in the headers and
// out-"sourced" config.h to hide it from the include folder. So there is a new macro for the
// header: ODECONFIG_H instead of _ODE_CONFIG_H_
#ifdef ODECONFIG_H
#  define ODE_VERSION_MINOR 10 // or larger
#else
#  define ODE_VERSION_MINOR 9 // or smaller
#endif


#if ODE_VERSION_MINOR < 10
#  ifndef dTriIndex
#    if dTRIMESH_16BIT_INDICES
#      if dTRIMESH_GIMPACT
	     typedef uint32 dTriIndex;
#      else // dTRIMESH_GIMPACT
	     typedef uint16 dTriIndex;
#      endif // dTRIMESH_GIMPACT
#    else // dTRIMESH_16BIT_INDICES
	   typedef int dTriIndex;
#    endif // dTRIMESH_16BIT_INDICES
#  endif
#endif

	typedef dTriIndex TriangleIndex;


	class World;
	class Space;

	class Body;
	class Mass;

	class JointGroup;
	class Joint;

    class Geometry;
    typedef std::list<Geometry*>   GeometryList;
    typedef std::vector<Geometry*> GeometryArray;

    class RayGeometry;
    class CylinderGeometry;
    class SphereGeometry;
    class InfinitePlaneGeometry;
    class TransformGeometry;
	
	class TriangleMeshData;
	class TriangleMeshDataPtr;
	class TriangleMeshGeometry;
	class BoxGeometry;
    class CapsuleGeometry;


	class Contact;

    class StepListener;

    class StepHandler;
    class BasicStepper;
    class QuickStepper;
    class FastStepper;

    class StepMode;
    class BasicStepMode;
    class QuickStepMode;
    class FastStepMode;



	class CollisionListener;
	class CollisionCallback;
	class MaintainedItem;


	class EntityInformer;
    typedef std::vector<Ogre::Vector3> Vector3Array;
	typedef std::map<unsigned char,Vector3Array* > BoneMapping;
	typedef std::pair<unsigned short,Vector3Array* > BoneMappingKey;

	typedef size_t MaterialID;
	typedef std::map<MaterialID,Contact*> MaterialMap;
    typedef std::pair<MaterialID,MaterialMap* > MaterialMapPair;

    class DebugObject;
    class DebugContact;
    class DebugContactText;
    class DebugNormal;
}


#endif

