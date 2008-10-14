#ifndef __OgreOdeTriangleMeshDataManager_H__
#define __OgreOdeTriangleMeshDataManager_H__

#include <Ogre.h>

#include "OgreOdePreReqs.h"
#include "OgreOdeTriangleMeshData.h"

using namespace std;

namespace OgreOde
{

	/** Each time a new TriangleMeshData is created it is added to this mananger, in TriangleMeshData's
		destructor it is removed from the manager
	*/
	class TriangleMeshDataManager : public Ogre::Singleton<TriangleMeshDataManager>
	{
	
	protected:

		struct vectorLess {
			bool operator()(const Ogre::Vector3& a, const Ogre::Vector3& b) const
			{
				if (a.x < b.x) return true;
				if (a.x > b.x) return false;
				if (a.y < b.y) return true;
				if (a.y > b.y) return false;
				return a.z < b.z;
			}
		};
		
	public:
	
		typedef map<Ogre::String, map<Ogre::Vector3, TriangleMeshDataPtr, vectorLess > > TriangleMeshDataPtrMap;

		/** 
		*/
		TriangleMeshDataManager();

		/**
		*/
		virtual ~TriangleMeshDataManager();
		
		/**
		*/
		static TriangleMeshDataManager* getSingletonPtr();
		
		/**
		*/
		static TriangleMeshDataManager& getSingleton();
		
		/** 
		*/
		void addTriangleMeshData(const Ogre::String& name, TriangleMeshDataPtr shape, Ogre::Vector3 scale);
		
		/** 
		*/
		void removeTriangleMeshData(const Ogre::String& name, Ogre::Vector3 scale);
		
		/** 
		*/
		TriangleMeshDataPtr getTriangleMeshData(const Ogre::String& name, Ogre::Vector3 scale);
		
		/** 
		*/
		const TriangleMeshDataPtrMap getTriangleMeshDataPtrMap(){ return mTriangleMeshDataMap; }
		
	protected:

		// mesh name, scale, TriangleMeshDataPtr
		TriangleMeshDataPtrMap mTriangleMeshDataMap;

	};
}

#endif //__OgreOdeTriangleMeshDataManager_H__


