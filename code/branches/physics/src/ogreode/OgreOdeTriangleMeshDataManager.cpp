#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeTriangleMeshDataManager.h"

using namespace Ogre;
using namespace OgreOde;

namespace Ogre
{
	template<> TriangleMeshDataManager *Ogre::Singleton<TriangleMeshDataManager>::ms_Singleton = 0;
}

namespace OgreOde
{

	
	//-----------------------------------------------------------------------
	TriangleMeshDataManager::TriangleMeshDataManager()
	{
	}

	//-----------------------------------------------------------------------
	TriangleMeshDataManager::~TriangleMeshDataManager()
	{
		//mTriangleMeshDataMap.clear();
	}
	
	//-----------------------------------------------------------------------
	TriangleMeshDataManager *TriangleMeshDataManager::getSingletonPtr () 
	{
		return ms_Singleton;
	}

	//-----------------------------------------------------------------------
	TriangleMeshDataManager &TriangleMeshDataManager::getSingleton ()
	{  
		assert (ms_Singleton);  
		return (*ms_Singleton);
	}


	//-----------------------------------------------------------------------
	void TriangleMeshDataManager::addTriangleMeshData(const String& name, TriangleMeshDataPtr shape, Vector3 scale)
	{

		mTriangleMeshDataMap[name][scale] = shape;
		
		// reduce count so when Ode::Geometry changes its dataPtr or is deleted we arn't
		// left with this one on a referance count of 1
		unsigned int* count = shape.useCountPointer();
		(*count)--;
		
	}


	//-----------------------------------------------------------------------
	void TriangleMeshDataManager::removeTriangleMeshData(const String& name, Vector3 scale)
	{
		
		TriangleMeshDataPtrMap::iterator itr = mTriangleMeshDataMap.find(name);
		if(itr != mTriangleMeshDataMap.end())
		{
		
			std::map<Ogre::Vector3, TriangleMeshDataPtr, vectorLess>::iterator itr2;
			
			itr2 = itr->second.find(scale);
			
			if ( itr2 != itr->second.end() )
			{
				//printf ("no TriangleMeshData for scale so erase it\n");
				itr->second.erase(itr2);
				itr2->second.setNull();
			}
			
			if (itr->second.size() == 0) 
			{
				//printf ("have no TriangleMeshData for this mesh so erase it from map\n");
				mTriangleMeshDataMap.erase(itr); 
			}
		}
			
	}
	
	
	/** looks for mesh name then size or returns a NULL TriangleMeshDataPtr
	*/
	//-----------------------------------------------------------------------
	TriangleMeshDataPtr TriangleMeshDataManager::getTriangleMeshData(const String& name, Vector3 scale)
	{
	
		TriangleMeshDataPtrMap::iterator itr = mTriangleMeshDataMap.find(name);
		if(itr != mTriangleMeshDataMap.end())
		{
			std::map<Ogre::Vector3, TriangleMeshDataPtr, vectorLess>::iterator itr2;
			
			itr2 = itr->second.find(scale);
			
			if ( itr2 != itr->second.end() )
			{
				return itr2->second;
			}
		}
		
		//printf ("TriangleMeshDataManager::getTriangleMeshData not found\n");
		
		return NULL;
	
	}

}
