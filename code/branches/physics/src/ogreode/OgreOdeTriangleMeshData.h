#ifndef __OgreOdeTriangleMeshData_H__
#define __OgreOdeTriangleMeshData_H__

#include <Ogre.h>

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

using namespace std;

namespace OgreOde
{
	/** Stores triangle mesh data once and shares it between all geoms that require it
	*/
	class _OgreOdeExport TriangleMeshData
	{
	public:

		TriangleMeshData(Ogre::String name, Ogre::Vector3 scale, const Ogre::Vector3* vertices, unsigned int vertex_count, const TriangleIndex* indices, unsigned int index_count);

		virtual ~TriangleMeshData();
		
		Ogre::String getMeshName(){ return _name; }
		
		Ogre::Vector3 getScale(){ return _scale; }

		dTriMeshDataID getData(){ return _data; }

		dVector3* getVertices(){ return _vertices; }

		TriangleIndex* getIndices(){ return _indices; }
	
		unsigned int getVertexCount(){ return _vertex_count; }

		unsigned int getIndexCount(){ return _index_count; }		
		
	protected:
	
		Ogre::String _name;
		Ogre::Vector3 _scale;
		dTriMeshDataID _data;
		dVector3* _vertices;
		TriangleIndex* _indices;
		unsigned int _vertex_count;
        unsigned int _index_count;
	};

	/** SharedPtr of TriangleMeshData
	*/
	class _OgreOdeExport TriangleMeshDataPtr : public Ogre::SharedPtr<TriangleMeshData> 
	{
	public:
		TriangleMeshDataPtr() : Ogre::SharedPtr<TriangleMeshData>() {}
		TriangleMeshDataPtr(TriangleMeshData* rep) : Ogre::SharedPtr<TriangleMeshData>(rep) {}
		TriangleMeshDataPtr(const TriangleMeshDataPtr& r) : Ogre::SharedPtr<TriangleMeshData>(r) {} 
	};
	
}

#endif //__OgreOdeTriangleMeshData_H__


