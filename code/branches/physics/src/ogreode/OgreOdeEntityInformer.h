#ifndef _OGREODEENTITYINFORMER_H_
#define _OGREODEENTITYINFORMER_H_

#include "OgreOdePreReqs.h"

#include <OgreMesh.h>
#include "OgreOdeTriangleMeshData.h"

namespace OgreOde 
{
    class _OgreOdeExport EntityInformer
    {
	public:
		EntityInformer(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		EntityInformer();
		~EntityInformer();

		void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		// Cannot be animated. 
		void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);

		Ogre::Real getRadius();
		Ogre::Vector3 getSize();
		Ogre::Vector3 getCenter();

		Body* createSingleDynamicSphere(Ogre::Real mass, World *world, Space* space = 0);
		Body* createSingleDynamicBox(Ogre::Real mass, World *world, Space* space = 0);

		TriangleMeshGeometry* createStaticTriangleMesh(World *world, Space* space = 0);
		BoxGeometry* createSingleStaticBox(World *world, Space* space = 0);

		CapsuleGeometry* createOrientedCapsule(unsigned char bone, World *world, Space* space = 0);
		BoxGeometry* createOrientedBox(unsigned char bone, World *world, Space* space = 0);
		BoxGeometry* createAlignedBox(unsigned char bone, World *world, Space* space = 0);

		const Ogre::Vector3* getVertices();
		unsigned int getVertexCount();
		const TriangleIndex* getIndices();
		unsigned int getIndexCount();

	protected:
        void addVertexData(const Ogre::VertexData *vertex_data, 
            const Ogre::VertexData *blended_data = 0, 
            const Ogre::Mesh::IndexMap *indexMap = 0);
		void addIndexData(Ogre::IndexData *data, const unsigned int offset = 0);
		bool getBoneVertices(unsigned char bone,unsigned int &vertex_count, Ogre::Vector3* &vertices);

		Ogre::Entity*		_entity;
		Ogre::SceneNode*	_node;
		Ogre::Matrix4		_transform;

		Ogre::Real			_radius;
		Ogre::Vector3		_size;
		Ogre::Vector3		_center;

        Ogre::Vector3*		_vertices;
		TriangleIndex*		_indices; 
		unsigned int		_vertex_count;
		unsigned int		_index_count;

		BoneMapping *_bone_mapping;
	};

	/** Class may create a TriangleMeshDataPtr, store and reuses it with TriangleMeshDataManager.
		This can limit the number of instances of the mesh data at a given scale to one.
		Since TriangleMeshGeometry is created using TriangleMeshDataPtr, have the advantage that 
		the TriangleMeshDataPtr can be changed without having to recreate the TriangleMeshGeometry see 
		TriangleMeshGeometry::changeTriangleMeshData().
		When TriangleMeshGeometry is created it is created at the origin with an identity orientation.
	*/
	class _OgreOdeExport EntityInformerReuse : public EntityInformer
	{

	public:
	
		/** forceRecreate will force the creation of a TriangleMeshDataPtr, use only when the mesh has undergone some change
		*/
		EntityInformerReuse(Ogre::Entity *entity, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE, bool forceRecreate = false);
		EntityInformerReuse(const Ogre::MeshPtr &mesh, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE, bool forceRecreate = false);
		
		virtual ~EntityInformerReuse();
		
		/** Creates TriangleMeshGeometry using TriangleMeshDataPtr 
		*/
		TriangleMeshGeometry* createStaticTriangleMesh(World *world, Space* space = 0);
		
		/** Recreates any TriangleMeshGeometry with the TriangleMeshDataPtr
		*/
		TriangleMeshGeometry* recreateStaticTriangleMesh(TriangleMeshGeometry* geom);
		
		/** Shouldn't be adding Entity or Mesh, one TriangleMeshData per Mesh/Entity for any scale.
		*/
		virtual void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY){}
		virtual void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY){}
		
		TriangleMeshDataPtr getTriangleMeshDataPtr(){ return _dataPtr; }
		
	protected:
	
		TriangleMeshDataPtr _dataPtr;
		
	};

}

#endif


