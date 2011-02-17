/*
===========================================================================
Copyright (C) 2008 Daniel Örstadius
Copyright (C) 2009 Jared Prince

This file is part of bsp-renderer source code.

bsp-renderer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bsp-renderer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bsp-renderer.  If not, see <http://www.gnu.org/licenses/>.

*/

// Q3Map.h -- handles the map data

#ifndef _Q3MAP_H
#define _Q3MAP_H

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "Q3Map_Bezier.h"

#include <string>
#include <vector>

#include "Q3Map_misc.h"

//#include "JarDebug.h" //!! just for debugging, remove from final build

#define MEMADD	65536 // memory is grown in 66k blocks
#define MAX_TOKENSIZE 1024 // make sure tokens in texts aren't ridiculously large



#define MINMAXLIMIT 10000000.0

#define BRIGHTNESSTWEAK	0.5f //1.25f				// tweak for spotlight brightness

#define ADDTEXTUREUNIQUE_FAIL		-1
#define ADDTEXLAMP_FAIL					-2

#define ADDSPOTLIGHTTEXTURE_FAIL	-1

////////////////////////////////////////////////////

#define AXIS_X	0
#define AXIS_Y	1
#define AXIS_Z	2

////////////////////////////////////////////////////
/*
#define FACESORT_FACE_LIMIT			16777216
#define FACESORT_FACE_SHIFT			0
#define FACESORT_FACE_MASK			0x0000000000FFFFFF
#define FACESORT_TEXTURE_LIMIT	4096
#define FACESORT_TEXTURE_SHIFT	24
#define FACESORT_TEXTURE_MASK		0x0000000FFF000000
#define FACESORT_SUBZONE_LIMIT			4096 // also see MAX_ZONE
#define FACESORT_SUBZONE_SHIFT			36
#define FACESORT_SUBZONE_MASK			0x0000FFF000000000
*/

#define FACESORT_FACE_LIMIT			16777216
#define FACESORT_FACE_SHIFT			0
#define FACESORT_FACE_MASK			0x0000000000FFFFFF
#define FACESORT_GROUP_LIMIT		65536	// also see MAX_TRANS
#define FACESORT_GROUP_SHIFT		24
#define FACESORT_GROUP_MASK			0x000000FFFF000000
#define FACESORT_TEXTURE_LIMIT	4096
#define FACESORT_TEXTURE_SHIFT	40
#define FACESORT_TEXTURE_MASK		0x000FFF0000000000
#define FACESORT_SUBZONE_LIMIT	4096 // also see MAX_ZONE
#define FACESORT_SUBZONE_SHIFT	52
#define FACESORT_SUBZONE_MASK		0xFFF0000000000000

#define GROUPSORT_FACE_LIMIT		16777216
#define GROUPSORT_FACE_SHIFT		0
#define GROUPSORT_FACE_MASK			0x0000000000FFFFFF
#define GROUPSORT_GROUP_LIMIT		16777216
#define GROUPSORT_GROUP_SHIFT		24
#define GROUPSORT_GROUP_MASK		0x0000FFFFFF000000
#define GROUPSORT_SUBZONE_LIMIT	4096 // also see MAX_ZONE
#define GROUPSORT_SUBZONE_SHIFT	52
#define GROUPSORT_SUBZONE_MASK	0xFFF0000000000000



////////////////////////////////////////////////////

#define MAX_ZONE	4096							// max subzones and max zones. Also see FACESORT_SUBZONE_LIMIT
#define MAX_SUBZONEPERZONE	16			// the most subzones that can make up a zone, further subzones ignored. INDEX_SUBZONECOUNT must be same as this
#define INDEX_SUBZONECOUNT	16			// helps subzone counting in m_nZone.  See MAX_SUBZONEPERZONE

#define MAX_PORTAL	32768						// max subzoneportals in a map.
#define MAX_PORTALPERZONE 64				// the most portals that a zone can have, further portals ignored.  INDEX_PORTALCOUNT must be same as this
#define MAX_ZONEPERPORTAL	8					// the most zones a portal can connect, further zones ignored.  INDEX_PORTALZONECOUNT must be the same as this
#define MAX_LIGHTPERPORTAL 16				// the most lights that can touch a portal, further lights ignored. INDEX_PORTALLIGHTCOUNT must be the same as this
#define INDEX_PORTALCOUNT	64				// helps portal counting in m_nZoneTouchesPortal. See MAX_PORTALSPERZONE
#define INDEX_PORTALZONECOUNT 8			// helps portalconnect counting in m_nPortalTouchesZone. See MAX_ZONEPERPORTAL
#define INDEX_PORTALLIGHTCOUNT 16		// helps portallight counting in m_PortalConnectsLight.	See MAX_LIGHTPERPORTAL

#define MAX_ZONEPERZONE		64				// the most zones that a zone can connect to.
#define INDEX_ZONEPERZONECOUNT	64  // helps zone per zone counting.

#define MAX_LIGHT		32768						// max lights in a map, both from the map and other things (flashlight, missiles, etc)
#define MAX_LIGHTPERZONE 64					// the most lights that a zone can have, further lights ignored.  INDEX_LIGHTCOUNT must be same as this
#define MAX_ZONEPERLIGHT	8					// the most zones a light can touch, further zones ignored.  INDEX_LIGHTZONECOUNT must be the same as this
#define INDEX_LIGHTCOUNT	64				// helps light counting in m_nZoneContainsLight.  see MAX_LIGHTPERZONE
#define INDEX_LIGHTZONECOUNT	8			// helps light counting in m_nLightContainedByZone. See MAX_ZONEPERLIGHT

#define MAX_SUBLIGHT	262144				// must be MAX_LIGHT * MAX_ZONEPERLIGHT
#define MAX_TRANS			65536					// most transparent objects per zone (objects may be defined in various ways).  see FACESORT_TRANS_LIMIT



////////////////////////////////////////////////////

#define SUBZONE_EPSILON	0.01f	// small buffer of extra size around subzones so that we don't accidentally exclude a point due to float accuracy errors.
#define FRUSTUM_EPSILON 0.0001f // once frustum is this tiny consider it dead
#define VERYSMALL 0.00001			// small number

#define PORTAL_UNCHECKED		0
#define PORTAL_VISCHECK			1
#define PORTAL_VISIBLE			2

#define PORTALSTATE_OPENCHECK	0
#define PORTALSTATE_CLOSED			1
#define PORTALSTATE_OPEN				2

#define ZONE_UNCHECKED			0
#define ZONE_VISIBLE				1

#define LIGHT_UNCHECKED				0
#define LIGHT_OUTOFFRUSTUM		1
#define LIGHT_INSIDEFRUSTUM		2
#define LIGHT_NOTVISIBLE			4
#define LIGHT_VISIBLE					8
#define LIGHT_CHECKED					16

///////////////////////////////////////////////////

#define MAX_PROJECTORTEX	16		// most types of projector textures we can have per map.


///////////////////////////////////////////////////

// NOERROR is already defined in a windows file, it is 0
#define ERROR_ParseMap						1
#define ERROR_ParseEntities				2
#define ERROR_AllocateVertex			3
#define ERROR_AllocateTriangle		4
#define ERROR_InitializeFaces			5
#define ERROR_ConvertFaces				6
#define ERROR_ConvertPatches			7
#define ERROR_ConvertLamps				8
#define ERROR_ConvertLampGlow			9
#define ERROR_ConvertLightGlow		10
#define ERROR_AssignTriangles			11
#define ERROR_SortTriangles				12
#define ERROR_ConvertTexLamp			13
#define ERROR_SetupTransTextures	14
#define ERROR_SortGroups					15


///////////////////////////////////////////////////
// error values for parsing key/values
#define KEY_OK										0
#define KEY_NONE									1
#define KEY_ERROR									2

//////////////////////////////////////////////////
// entity types
#define ENTITY_ERROR							0
#define ENTITY_UNKNOWN						1
#define ENTITY_WORLDSPAWN					2
#define ENTITY_LIGHT							3

///////////////////////////////////////////////////



// The lump idexes that make up a bsp file.
// Bzn bsp lump data is almost identical to q3a bsp lump data,  except that the
// Effects, Lightmaps and VisData lumps are removed and there are two new lumps, one
// describing the subzones and one describing the portals. The other difference is the lighting
// entities (which are stored as normal entities in lump 0) since q3a bsps don't usually
// store the light entities at all, and BZN has it's own unique set of lighting keys.

const int Textures = 1;
const int Planes = 2;
const int Nodes = 3;
const int Leafs = 4;
const int LeafFaces = 5;
const int LeafBrushes = 6;
const int Brushes = 8;
const int BrushSides = 9;
const int Vertices = 10;
const int MeshVerts = 11;
const int Effects = 12;				// removed from bzn
const int Faces = 13;
const int LightMaps = 14;			// removed from bzn
const int VisData = 16;				// removed from bzn
const int SubZoneData = 17 ;	// added to bzn: overlapping subzones form zones. Zones are like groups of axial boxes.
const int PortalData = 18 ;		// added to bzn: portals overlapping different zones allow them to see each other.
const int MAX_LUMP = 19 ;

const int MAX_LUMP_SIZE = 100000000 ; // this value is very large and arbitrary, 100 megabytes.  Just make sure MAX_LUMP_SIZE * MAX_LUMP is less than the max size for a size_t or parseMap error checking won't work.

enum {POLYGON = 1, PATCH, MESH, BILLBOARD};

typedef struct{
  int iOffset;
  int iLength;
} direntry_t;

#define Q3NAMESIZE	64
typedef struct {
  char name[Q3NAMESIZE];
  int flags;
  int contents;
} Q3BspTexture;

typedef struct{
  unsigned char magic[4];
  int version;
  direntry_t Lumps[MAX_LUMP];
} Q3BspHeader_t;

typedef struct{
  int texture;
  int effect;
  int type;
  int vertex;
  int n_vertexes;
  int meshvert;
  int n_meshverts;
  int lm_index;
  int lm_start[2];
  int lm_size[2];
  float lm_origin[3];
  float lm_vecs[2][3];
  float normal[3];
  int size[2];
} Q3BspFace_t;

typedef struct{
  float position[3];
  float texcoord[2][2];
  float normal[3];
  unsigned char color[4];
} Q3BspVertex;

typedef struct {
  int cluster;
  int area;
  int mins[3];
  int maxs[3];
  int leafface;
  int n_leaffaces;
  int leafbrush;
  int n_leafbrushes;
} Q3BspLeaf;

typedef struct {
  float normal[3];
  float dist;
} Q3BspPlane;

typedef struct {
  int plane;
  int children[2];
  int mins[3];
  int maxs[3];
} Q3BspNode;

typedef struct {
  int brushside;
  int n_brushsides;
  int texture;
} Q3BspBrush;

typedef struct {
  int plane;
  int texture;
} Q3BspBrushSide;

typedef struct {
  unsigned char lightmap[128][128][3];
} Q3BspLightMap;

typedef struct {
  int n_vecs;
  int sz_vecs;
  unsigned char *vecs;
} Q3BspVisData;

typedef struct {
  int size;
  Bezier *bezier;
} Q3BspPatch;


// this struct must be the same as in BZNq3map2
typedef struct
{
	int Zone ;
	float Min[3] ;
	float Max[3] ;
}
BZN_SubZone_t;

// this struct must be the same as in BZNq3map2
typedef struct
{
	float Min[3] ;
	float Max[3] ;
}
BZN_Portal_t;


typedef struct{
	int texture;    
	int type;
	int vertex;
	int n_vertexes;
	int meshvert;
	int n_meshverts;
	int n_triangles;
	int lm_index;    
	float normal[3]; 
	int zone ; // added by Jared 
	Q3BspPatch *patch;
} Q3BspFaceRenderer;

//////////////////////////////////////
typedef struct {
	int Zone ; // this gets set to -1 for triangles in no zone, and they don't get added to the manualobjects
	int Texture ;
	//int Lightmap ; // bzn doesn't use lightmaps
	int VIndex[3] ;
	int Lamp ; // which lamp this triangle came from, -1 for most triangles.

	int Group ; // triangles from the same face are the same group, triangles from the same patch are the same group.  Most stuff is group 0 unless it needs special attention, such as transparent stuff
} triangle_t;

typedef struct {
	unsigned int Flags ;
	float	Position[3] ;
	float Direction[3] ;
	float Colour[3] ;
	float Min[3] ;
	float Max[3] ;
	float	Cutoff ;
	float	Angle ;
	float Brightness ;
	int Texture ; // texture index of light
	unsigned int ZoneCount ; // how many zones this light touches
	unsigned int CentreZone ; // which zone contains the centre of this light.  even if on a boundary, the light will only choose one zone as its centre.
	short SubLightStart ; // where this light's sublights start in the sublight list.
} light_t ;

typedef struct {
	unsigned int Flags ;
	float	Position[3] ;
	float Colour[3] ;
	float Min[3] ;
	float Max[3] ;
	float Brightness ;
	int Texture ; // texture index of light
	int LightNode ; // -1 if not a node, 0 - 3 if a node.  LightNodes are used as the data for TexLamps, freeform geometry that gets converted to deferred shading lamp triangles.
	unsigned short Zone[MAX_ZONEPERLIGHT+1] ; // the zones this lamp touches
} lamp_t ;

typedef struct
{
	float Min[3] ;
	float Max[3] ;
}
minmax_t;

// this used for the light culling algo, where each cutup light is treated as a subzone.
typedef struct
{
	unsigned short Light ;
	unsigned short Zone ;
	float Min[3] ;
	float Max[3] ;
}
sublight_t;


class Q3Map
{
public:
  Q3Map();
  ~Q3Map();

	int m_nNewCount ;

	int m_nDebugA ;

//	CJarDebug Q3Bug ; //!! just for debugging, remove from final build
	char m_chBug[10240] ;

	int ParseAndTriangulateMap(const char* pData, size_t Size) ;
	void FreeParseMem(void) ;

	int parseMap(const char* pMem, size_t Size);

  int findVisibleFaces(const QVECTOR *camPos, int *facesToRender);
  Q3BspFace_t *getFaces(void);
  Q3BspPatch *handlePatch(int faceIndex);

	char m_chSpotlightTexture[MAX_PROJECTORTEX][Q3NAMESIZE] ;
	int m_nMaxSpotlightTexture ;
	int AddSpolightTexture(char TEXNAME[]) ;

private:

  
  
  

  	

  int findLeaf(const QVECTOR *camPos) const;
  bool isClusterVisible(int visCluster, int testCluster) const;	

  int *mVisibleFaces;

  std::vector<Q3BspVisData> *patches;

public:
  // map data

  Q3BspHeader_t m_BspHeader;

  char *m_pEntities;

  int m_iNumTexs;
  Q3BspTexture *m_pTexturesOrig;

  int m_iNumFaces;
  Q3BspFace_t *m_pFaces; 

  int m_iNumVertices;
  Q3BspVertex *m_pVertices;

  int m_iNumMeshVerts;	
  int *m_pMeshVerts;

  int m_iNumLeafs;
  Q3BspLeaf *m_pLeafs;

  int m_iNumLeafFaces;
  int *m_pLeafFaces;

  int m_iNumPlanes;
  Q3BspPlane *m_pPlanes;

  int m_iNumNodes;
  Q3BspNode *m_pNodes;

  int m_iNumLeafBrushes;
  int *m_pLeafBrushes;

  int m_iNumBrushes;
  Q3BspBrush *m_pBrushes;

  int m_iNumBrushSides;
  Q3BspBrushSide *m_pBrushSides;

  int m_iNumLightMaps;
  Q3BspLightMap *m_pLightMaps;

  Q3BspVisData *m_VisData;

	int m_iNumSubZones ;
  BZN_SubZone_t *m_pSubZones;

	int m_iNumPortals ;
  BZN_Portal_t *m_pPortals;




  int m_ClusterCount ;

	void swizzleCoords(void);
  void swizzleFloat3(float t[3]);
  void swizzleInt3(int t[3]);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// memory management for the triangles
	int AllocateTriangleMemory(void) ;
	void FreeTriangleMemory(void) ;
	int ExpandTriangleMemory(void) ;
	int AddTriangle(triangle_t Triangle) ;
	int m_nTriangleSize ; // starting memory size
	void* m_pTriangleMem ; // allocate starting memory space
	triangle_t* m_pTriangle ; // a pointer to the memory cast as a triangle_t
	int m_nTriangleMax ;
	int m_nTriangleLimit ;

	// memory management for the vertices
	int AllocateVertexMemory(int nVertNum) ;
	void FreeVertexMemory(void) ;
	int ExpandVertexMemory(void) ;
	int AddVertex(Q3BspVertex Vertex) ;
	int m_nVertexSize ; // starting memory size
	void* m_pVertexMem ; // allocate starting memory space
	Q3BspVertex* m_pVertex ; // a pointer to the memory cast as a Q3BspVertex
	int m_nVertexMax ;
	int m_nVertexLimit ;

	// memory management for the lights
	int AllocateLightMemory(void) ;
	void FreeLightMemory(void) ;
	int ExpandLightMemory(void) ;
	int AddLight(light_t Light) ;
	int m_nLightSize ; // starting memory size
	void* m_pLightMem ; // allocate starting memory space
	light_t* m_pLight ; // a pointer to the memory cast as a light_t
	int m_nLightMax ;
	int m_nLightLimit ;

	// memory management for the lamps (a lamp is a deferred shading non-shadowing point light)
	int AllocateLampMemory(void) ;
	void FreeLampMemory(void) ;
	int ExpandLampMemory(void) ;
	int AddLamp(lamp_t Lamp) ;
	int m_nLampSize ; // starting memory size
	void* m_pLampMem ; // allocate starting memory space
	lamp_t* m_pLamp ; // a pointer to the memory cast as a lamp_t
	int m_nLampMax ;
	int m_nLampLimit ;

	// memory management for the textures
	int AllocateTextureMemory(void) ;
	void FreeTextureMemory(void) ;
	int ExpandTextureMemory(void) ;
	int AddTexture(Q3BspTexture Texture) ;
	int AddTextureUnique(Q3BspTexture Texture) ; // special version of the Add function, will not add if the texture name already exist.  returns texture index, or -1 on fail
	int m_nTextureSize ; // starting memory size
	void* m_pTextureMem ; // allocate starting memory space
	Q3BspTexture* m_pTexture ; // a pointer to the memory cast as a Texture_t
	int m_nTextureMax ;
	int m_nTextureLimit ;

	// memory management for the TexLamp Triangles
	int AllocateTexLampMemory(void) ;
	void FreeTexLampMemory(void) ;
	int ExpandTexLampMemory(void) ;
	int AddTexLamp(int TexLamp) ;
	int m_nTexLampSize ; // starting memory size
	void* m_pTexLampMem ; // allocate starting memory space
	int* m_pTexLamp ; // a pointer to the memory cast as an int
	int m_nTexLampMax ;
	int m_nTexLampLimit ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int ParseEntities(void) ;
	int NextEntity(int* pPos, int nMaxPos) ;
	int GetEntityType(int nPos, int nMaxPos) ;
	int GetEntityKeyAndValue(int* pPos, int nMaxPos, char* pKey, char* pValue) ;
	int GetNumbersFromValue(char* pValue, float *pNumber, int nNumberSize) ;
	int GetNumbersFromValue(char* pValue, int *pNumber, int nNumberSize) ;
	int ParseAndAddLight(int* pPos, int nMaxPos) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Q3BspFaceRenderer *m_BspFaces;	
	int m_NumBspFaces;
	void DestroyBspFacesMemory(void) ;
	int initFaces(void) ;

	// we keep track of common texture indexes to avoid slow calls to AddTextureUnique when we add default light texture indexes
	int m_nDefaultTextureIndexLamp ;
	int m_nDefaultTextureIndexLamp2Pass ;
	int m_nDefaultTextureIndexGlowLamp ;
	int m_nDefaultTextureIndexGlowLight ;
	int m_nDefaultTextureIndexSpotlight ;

	void GetTexLampTextureNumbers() ;
	int m_nBZN_LightNode0 ;
	int m_nBZN_LightNode1 ;
	int m_nBZN_LightNode2 ;
	int m_nBZN_LightNode3 ;

	int m_nGroup ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Much easier to keep track of how zones/subzones/portals/lights connect to each other using hardwired arrays
	// than dynamically allocated memory, even if it does waste a little space and impose some design limits.
	// It's fast, easy to manage, and the total memory used is reasonably small.  
	// The design limits are generous enough that it's unlikely maps will get anywhere near hitting them.
	//
	// Memory Footprints:
	//
	// m_nZone:											 4096 * (16+1) * sizeof(unsigned short) =	  139,264
	// m_ZoneBoundary								 4096 *					 sizeof(minmax_t)24			=	   98,304
	// m_nZoneTouchesPortal					 4096 * (64+1) * sizeof(unsigned short) =	  532,480
	// m_nPortalTouchesZone					32768 * ( 8+1) * sizeof(unsigned short) =	  589,824
	//
	// m_nZoneContainsLightCentre		 4096 * (64+1) * sizeof(unsigned short) =	  532,480 
	// m_nLightTouchesZone					32768 * ( 8+1) * sizeof(unsigned short) =	  589,824
	// m_nZoneTouchesSubLight				 4096 * (64+1) * sizeof(unsigned short) =	  532,480
	//
	// m_nMultiZoneLight						32768 *					 sizeof(unsigned short)	=		 65,536	
	// m_SubLight										32768 * 8			 * sizeof(sublight_t)28		=	6,815,744
	// m_nZoneTouchesZone						 4096 * (64+1) * sizeof(unsigned short) =	  532,480
	//
	//																																TOTAL = 10,428,416 (9.9 meg)

	unsigned short m_nZone[MAX_ZONE][MAX_SUBZONEPERZONE+1] ;									// last index is used as a counter, INDEX_SUBZONECOUNT
	minmax_t m_ZoneBoundary[MAX_ZONE] ; // min and max of a zone.  Since zones might not be square, NEVER use this as the real zone boundary, work it out using the subzones instead.  This is just for cutting up lights.
	
	unsigned short m_nZoneTouchesPortal[MAX_ZONE][MAX_PORTALPERZONE+1] ;			// last index is used as a counter, INDEX_PORTALCOUNT
	unsigned short m_nPortalTouchesZone[MAX_PORTAL][MAX_ZONEPERPORTAL+1] ;		// last index is used as a counter, INDEX_PORTALZONECOUNT
	
	// PortalTouchesLight is true if the light AABB touches the portal.
	unsigned short m_nPortalTouchesLight[MAX_PORTAL][MAX_LIGHTPERPORTAL+1] ;	// last index is used as a counter, INDEX_PORTALLIGHTCOUNT

	unsigned short m_nZoneContainsLightCentre[MAX_ZONE][MAX_LIGHTPERZONE+1] ;				// last index is used as a counter, INDEX_LIGHTCOUNT
	unsigned short m_nLightTouchesZone[MAX_LIGHT][MAX_ZONEPERLIGHT+1] ;				// last index is used as a counter, INDEX_LIGHTZONECOUNT
	unsigned short m_nZoneTouchesSubLight[MAX_ZONE][MAX_LIGHTPERZONE+1] ;				// last index is used as a counter, INDEX_LIGHTCOUNT

	unsigned short m_nZoneTouchesZone[MAX_ZONE][MAX_ZONEPERZONE+1] ;				// last index is used as a counter, INDEX_ZONEPERZONECOUNT

	// list of lights that touche more than one zone.  These require more complex visibility checks
	unsigned short m_nMultiZoneLight[MAX_LIGHT] ; 
	int m_nMaxMultiZoneLight ;
	
	sublight_t m_SubLight[MAX_SUBLIGHT] ;
	int m_nSubLightMax ;
	 

	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int m_nMaxZone ;

	int ConvertFacesToTriangles(void) ;
	int ConvertPatchesToTriangles(void) ;
	int ConvertTexLampsToLampTriangles(void) ;
	int ConvertLampsToTriangles(void) ;
	int ConvertLampsToGlowTriangles(void) ;
	int ConvertLightsToGlowTriangles(void) ;


	void SetVertex(Q3BspVertex *pVert, float flXPos, float flYPos, float flZPos) ;
	bool VectorsAreEqual(Q3BspVertex* pVecA, Q3BspVertex* pVecB) ;
	Q3BspVertex NormalizedCrossProduct(Q3BspVertex VertA, Q3BspVertex VertB, Q3BspVertex VertC) ;
	float VertexDistance(Q3BspVertex* VertA, Q3BspVertex* VertB) ;
	void VertexScale(Q3BspVertex* pVert, float flScale) ;
	Q3BspVertex GetNormalised(Q3BspVertex* pVector) ;
	Q3BspVertex VectorAdd(Q3BspVertex* pVecA, Q3BspVertex* pVecB) ;
	Q3BspVertex VectorSubtract(Q3BspVertex* pVecA, Q3BspVertex* pVecB) ;
	Q3BspVertex VectorMultiply(Q3BspVertex* pVecA, Q3BspVertex* pVecB) ;

	void SetupZones(void) ;
	int GetNextSubZone(float *flPoint, int nStart, int nMax) ;

	bool PointInSubZone(float *flPoint, int nSubZone) ;
	bool PointInZone(float *flPos, int nZone) ;

	bool AABBTouchesSubZone(float *flPointMin, float *flPointMax, int nSubZone) ;
	bool AABBTouchesZone(float *flPosMin, float *flPosMax, int nZone) ;

	int AssignTrianglesToZones(void) ;
	int FindTriangleZone(int nTriangle) ;
	int SetupTriangleZone(int nTriangle) ;
	int SplitTriangle(int nTriangle, int nAxis, float flCutPos) ; // only does axial cuts.  returns false on failure, probably due to lack of memory.
	void CreateTweenVert(Q3BspVertex* pVertA, Q3BspVertex* pVertB, float flPercent0, Q3BspVertex* pVertAB) ;
	

	int SortTrianglesIntoGroups(void) ;
	static int compareGroups( const void *arg1, const void *arg2 ) ;

	int SortTrianglesIntoBatches(void) ;
	static int compareTriangles( const void *arg1, const void *arg2 ) ;

	int SetupTransTextures(void) ;
	int* m_pTransTexture ;


	int AssignPortalsToZones(void) ;
	int AssignLightsToPortals(void) ;
	int AssignLightsToZones(void) ;
	int AssignZonesToZones(void) ;

};

#endif /* _Q3MAP_H */
