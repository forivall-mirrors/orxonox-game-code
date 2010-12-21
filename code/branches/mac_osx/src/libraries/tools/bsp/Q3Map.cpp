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

// Q3Map.cpp -- handles the map data

#include <string>
#include <math.h>

#include "Q3Map.h"
#include "Q3Map_misc.h"


//temp
//#include <io.h>
//#include <fcntl.h>      /* Needed only for _O_RDWR definition */
//#include <sys/stat.h>

Q3Map::Q3Map()
{
	m_nDebugA=0 ;

	m_nNewCount=0 ;
	
  m_pTexturesOrig=NULL ;
  m_pFaces=NULL ;
  m_pVertices=NULL ;
  m_pMeshVerts=NULL ;
  m_pLeafs=NULL ;
  m_pLeafFaces=NULL ;
  m_pPlanes=NULL ;
  m_pNodes=NULL ;

	//m_VisData->vecs=NULL ; // can't NULL this because m_VisData doesn't exist yet!
	m_VisData=NULL ;
   
  m_pBrushes=NULL ;
  m_pLeafBrushes=NULL ;
  m_pBrushSides=NULL ;
  m_pLightMaps=NULL ;
  m_pEntities=NULL ;

	m_pSubZones=NULL ;
	m_pPortals=NULL ;
	//////////////////////////////////////////////////////

	// initiallize our triangle memory management
	m_nTriangleSize=0 ;
	m_pTriangleMem=NULL ;
	m_pTriangle=NULL ;
	m_nTriangleMax=0 ;
	m_nTriangleLimit=0 ;

	m_nVertexSize=0 ;
	m_pVertexMem=NULL ;
	m_pVertex=NULL ;
	m_nVertexMax=0 ;
	m_nVertexLimit=0 ;

	m_nLightSize=0 ;
	m_pLightMem=NULL ;
	m_pLight=NULL ;
	m_nLightMax=0 ;
	m_nLightLimit=0 ;
	m_nMaxMultiZoneLight=0 ;

	m_nLampSize=0 ;
	m_pLampMem=NULL ;
	m_pLamp=NULL ;
	m_nLampMax=0 ;
	m_nLampLimit=0 ;

	m_nTextureSize=0 ;
	m_pTextureMem=NULL ;
	m_pTexture=NULL ;
	m_nTextureMax=0 ;
	m_nTextureLimit=0 ;

	m_nTexLampSize=0 ;
	m_pTexLampMem=NULL ;
	m_pTexLamp=NULL ;
	m_nTexLampMax=0 ;
	m_nTexLampLimit=0 ;

	m_BspFaces=NULL;

	m_pTransTexture=NULL ;


//	Q3Bug.LogInit() ;

}

Q3Map::~Q3Map()
{
//	Q3Bug.LogSave("Q3Bug.log") ;

	DELETE_ARRAY(m_pTransTexture) ;
	//DELETE_ARRAY(m_pSubZones) ;
	DELETE_ARRAY(mVisibleFaces) ;

	FreeLightMemory() ;
	FreeLampMemory() ;
	FreeVertexMemory() ;
	FreeTriangleMemory() ;
	FreeTextureMemory() ;
	FreeTexLampMemory() ;
	DestroyBspFacesMemory() ;	// free up face and patch data.  Normally already done if we're just loading a new level or quitting, but we might also be here due to a loading fail or something.

	// no need to delete any of the pointers to the lumps since their memory belongs to m_pRawBspFile over in OgreFramework.

	
}

void Q3Map::DestroyBspFacesMemory(void)
{
	if(m_BspFaces) 
	{
		// clean up any memory declared for patches
		for (int i=0; i < m_NumBspFaces; i++)
      if ((m_BspFaces[i].type == PATCH) && (m_BspFaces[i].patch != NULL))
			{
				DELETE_ARRAY( m_BspFaces[i].patch->bezier ) ;
				DELETE_POINTER( m_BspFaces[i].patch ) ;
			}

		// delete the faces memory
		DELETE_ARRAY( m_BspFaces ) ;
	}
}



int Q3Map::parseMap(const char* pMem, size_t Size)
{	
	// we check all lump info to make sure it isn't trying to go out of bounds,
	// in case some mangled bsp is trying to do something devious or is just corrupted
	
	
	


	m_BspHeader=*((Q3BspHeader_t*)pMem) ; // pointer to the header

	// run a check that the total size of all the iLengths plus the header isn't too large
	size_t TotalLength=sizeof(Q3BspHeader_t) ; // initialize to the size of the header
	int nLump=0 ;
	int nOther=0 ;
	int nLumpMin=0 ;
	int nLumpMax=0 ;
	int nOtherMin=0 ;
	int nOtherMax=0 ;
	for(nLump=0 ; nLump<MAX_LUMP ; nLump++)
	{
		if(m_BspHeader.Lumps[nLump].iLength<0) return -1 ; // lumps shouldn't have a negative size. FAIL!
		if(m_BspHeader.Lumps[nLump].iLength>MAX_LUMP_SIZE) return -2 ; // no lump has a right to be this big... FAIL!
		
		
		if( (m_BspHeader.Lumps[nLump].iLength>0) && (m_BspHeader.Lumps[nLump].iOffset<sizeof(Q3BspHeader_t)) )
			return -3 ; // lump overlaps header, FAIL!

		if((m_BspHeader.Lumps[nLump].iLength==0) && (m_BspHeader.Lumps[nLump].iOffset!=0))
			return -4 ; // lump size is zero and yet offset is not zero???  FAIL!


		TotalLength+=m_BspHeader.Lumps[nLump].iLength ;
		if(TotalLength>Size) return -5 ;// this file is messed up, the lumps add up to more than the file size.  FAIL!

		// make sure this lump doesn't overlap any other lumps
		nLumpMin=m_BspHeader.Lumps[nLump].iOffset ;
		nLumpMax=nLumpMin+m_BspHeader.Lumps[nLump].iLength-1 ;

		for(nOther=nLump+1 ; nOther<MAX_LUMP ; nOther++)
			if((m_BspHeader.Lumps[nLump].iLength>0) && (m_BspHeader.Lumps[nOther].iLength>0)) // don't check zero sized lumps
			{
				nOtherMin=m_BspHeader.Lumps[nOther].iOffset ;
				nOtherMax=nOtherMin+m_BspHeader.Lumps[nOther].iLength-1 ;

				if((nLumpMax>=nOtherMin) && (nLumpMin<=nOtherMax)) 
				return -6 ; // lump overlaps another lump, FAIL!
			}
	}

	


	// setup pointers to the lumps

	if((m_BspHeader.Lumps[0].iOffset<0) || (m_BspHeader.Lumps[0].iOffset>=Size)) return -7 ; // fail if out of memory bounds
	m_pEntities=(char*)(pMem+m_BspHeader.Lumps[0].iOffset) ;

	if((m_BspHeader.Lumps[Faces].iOffset<0) || (m_BspHeader.Lumps[Faces].iOffset>=Size)) return -8 ; // out of bounds
	if(m_BspHeader.Lumps[Faces].iOffset+m_BspHeader.Lumps[Faces].iLength>Size) return -9 ; // out of bounds
  m_iNumFaces = m_BspHeader.Lumps[Faces].iLength / sizeof(Q3BspFace_t);
	m_pFaces=(Q3BspFace_t*)(pMem+m_BspHeader.Lumps[Faces].iOffset) ;

	if((m_BspHeader.Lumps[Vertices].iOffset<0) || (m_BspHeader.Lumps[Vertices].iOffset>=Size)) return -10 ; // out of bounds
	if(m_BspHeader.Lumps[Vertices].iOffset+m_BspHeader.Lumps[Vertices].iLength>Size) return -11 ; // out of bounds
  m_iNumVertices = m_BspHeader.Lumps[Vertices].iLength / sizeof(Q3BspVertex);
	m_pVertices=(Q3BspVertex*)(pMem+m_BspHeader.Lumps[Vertices].iOffset) ;

	if((m_BspHeader.Lumps[MeshVerts].iOffset<0) || (m_BspHeader.Lumps[MeshVerts].iOffset>=Size)) return -12 ; // out of bounds
	if(m_BspHeader.Lumps[MeshVerts].iOffset+m_BspHeader.Lumps[MeshVerts].iLength>Size) return -13 ; // out of bounds
  m_iNumMeshVerts = m_BspHeader.Lumps[MeshVerts].iLength / sizeof(int);
	m_pMeshVerts=(int*)(pMem+m_BspHeader.Lumps[MeshVerts].iOffset) ;

	if((m_BspHeader.Lumps[Leafs].iOffset<0) || (m_BspHeader.Lumps[Leafs].iOffset>=Size)) return -14 ; // out of bounds
	if(m_BspHeader.Lumps[Leafs].iOffset+m_BspHeader.Lumps[Leafs].iLength>Size) return -15 ; // out of bounds
  m_iNumLeafs = m_BspHeader.Lumps[Leafs].iLength / sizeof(Q3BspLeaf);
	m_pLeafs=(Q3BspLeaf*)(pMem+m_BspHeader.Lumps[Leafs].iOffset) ;

	if((m_BspHeader.Lumps[LeafFaces].iOffset<0) || (m_BspHeader.Lumps[LeafFaces].iOffset>=Size)) return -16 ; // out of bounds
	if(m_BspHeader.Lumps[LeafFaces].iOffset+m_BspHeader.Lumps[LeafFaces].iLength>Size) return -17 ; // out of bounds
  m_iNumLeafFaces = m_BspHeader.Lumps[LeafFaces].iLength / sizeof(int);
	m_pLeafFaces=(int*)(pMem+m_BspHeader.Lumps[LeafFaces].iOffset) ;

	if((m_BspHeader.Lumps[LeafBrushes].iOffset<0) || (m_BspHeader.Lumps[LeafBrushes].iOffset>=Size)) return -18 ; // out of bounds
	if(m_BspHeader.Lumps[LeafBrushes].iOffset+m_BspHeader.Lumps[LeafBrushes].iLength>Size) return -19 ; // out of bounds
  m_iNumLeafBrushes = m_BspHeader.Lumps[LeafBrushes].iLength / sizeof(int);
	m_pLeafBrushes=(int*)(pMem+m_BspHeader.Lumps[LeafBrushes].iOffset) ;

	if((m_BspHeader.Lumps[Textures].iOffset<0) || (m_BspHeader.Lumps[Textures].iOffset>=Size)) return -20 ; // out of bounds
	if(m_BspHeader.Lumps[Textures].iOffset+m_BspHeader.Lumps[Textures].iLength>Size) return -21 ; // out of bounds
  m_iNumTexs = m_BspHeader.Lumps[Textures].iLength / sizeof(Q3BspTexture);
	m_pTexturesOrig=(Q3BspTexture*)(pMem+m_BspHeader.Lumps[Textures].iOffset) ;

	if((m_BspHeader.Lumps[Planes].iOffset<0) || (m_BspHeader.Lumps[Planes].iOffset>=Size)) return -22 ; // out of bounds
	if(m_BspHeader.Lumps[Planes].iOffset+m_BspHeader.Lumps[Planes].iLength>Size) return -23 ; // out of bounds
  m_iNumPlanes = m_BspHeader.Lumps[Planes].iLength / sizeof(Q3BspPlane);
	m_pPlanes=(Q3BspPlane*)(pMem+m_BspHeader.Lumps[Planes].iOffset) ;

	if((m_BspHeader.Lumps[Nodes].iOffset<0) || (m_BspHeader.Lumps[Nodes].iOffset>=Size)) return -24 ; // out of bounds
	if(m_BspHeader.Lumps[Nodes].iOffset+m_BspHeader.Lumps[Nodes].iLength>Size) return -25 ; // out of bounds
  m_iNumNodes = m_BspHeader.Lumps[Nodes].iLength / sizeof(Q3BspNode);
	m_pNodes=(Q3BspNode*)(pMem+m_BspHeader.Lumps[Nodes].iOffset) ;

  //m_iNumModels = m_BspHeader.Lumps[Models].iLength / sizeof(Q3BspModel);
  //m_pModels = new Q3BspModel[m_iNumModels];

	// bzn doesn't use lightmaps
  //m_iNumLightMaps = m_BspHeader.Lumps[LightMaps].iLength / sizeof(Q3BspLightMap);
	//m_pLightMaps=(Q3BspLightMap*)(pMem+m_BspHeader.Lumps[LightMaps].iOffset) ;

	if((m_BspHeader.Lumps[Brushes].iOffset<0) || (m_BspHeader.Lumps[Brushes].iOffset>=Size)) return -26 ; // out of bounds
	if(m_BspHeader.Lumps[Brushes].iOffset+m_BspHeader.Lumps[Brushes].iLength>Size) return -27 ; // out of bounds
  m_iNumBrushes = m_BspHeader.Lumps[Brushes].iLength / sizeof(Q3BspBrush);
	m_pBrushes=(Q3BspBrush*)(pMem+m_BspHeader.Lumps[Brushes].iOffset) ;


	if((m_BspHeader.Lumps[BrushSides].iOffset<0) || (m_BspHeader.Lumps[BrushSides].iOffset>=Size)) return -28 ; // out of bounds
	if(m_BspHeader.Lumps[BrushSides].iOffset+m_BspHeader.Lumps[BrushSides].iLength>Size) return -29 ; // out of bounds
  m_iNumBrushSides = m_BspHeader.Lumps[BrushSides].iLength / sizeof(Q3BspBrushSide);
	m_pBrushSides=(Q3BspBrushSide*)(pMem+m_BspHeader.Lumps[BrushSides].iOffset) ;

  //m_iNumEffects = m_BspHeader.Lumps[Effects].iLength / sizeof(Q3BspEffect);
  //m_pEffects = new Q3BspEffect[m_iNumEffects];
  //
  //m_pImages = new BDTexture[m_iNumTexs];

	// bzn doesn't use visdata
	//m_VisData=(Q3BspVisData*)(pMem+m_BspHeader.Lumps[VisData].iOffset) ;
	//m_VisData->vecs=(unsigned char*)(pMem+m_BspHeader.Lumps[VisData].iOffset + 2*sizeof(int)) ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// bzn specific data
	if((m_BspHeader.Lumps[SubZoneData].iOffset<0) || (m_BspHeader.Lumps[SubZoneData].iOffset>=Size)) return -30 ; // out of bounds
	if(m_BspHeader.Lumps[SubZoneData].iOffset+m_BspHeader.Lumps[SubZoneData].iLength>Size) return -31 ; // out of bounds
	m_iNumSubZones = m_BspHeader.Lumps[SubZoneData].iLength / sizeof(BZN_SubZone_t);
  m_pSubZones=(BZN_SubZone_t*)(pMem+m_BspHeader.Lumps[SubZoneData].iOffset) ;

	if((m_BspHeader.Lumps[PortalData].iOffset<0) || (m_BspHeader.Lumps[PortalData].iOffset>=Size)) return -32 ; // out of bounds
	if(m_BspHeader.Lumps[PortalData].iOffset+m_BspHeader.Lumps[PortalData].iLength>Size) return -33 ; // out of bounds
	m_iNumPortals = m_BspHeader.Lumps[PortalData].iLength / sizeof(BZN_Portal_t);
	m_pPortals=(BZN_Portal_t*)(pMem+m_BspHeader.Lumps[PortalData].iOffset) ;

	




	// fix coords and setup face memory
	swizzleCoords();
  mVisibleFaces = new int[m_iNumFaces];


	// we need a new version of the textures, because when we parse the lights they will have textures to add to it,
	// and we can't expand the texture lump because it's in the middle of a block of memory containing all the lumps.

	// copy the texture lump
	int nTexture=0 ;
	for(nTexture=0 ; nTexture<m_iNumTexs ; nTexture++)
	{
		if(!AddTexture(m_pTexturesOrig[nTexture])) return -34 ; // failed to add texture, probably out of memory
	}
	
	return 0 ;

}


void Q3Map::swizzleCoords(void)
{
  //DEBUG_OUTPUT("swizziling data...");
  // vertices
  for (int i=0; i < m_iNumVertices; i++)
  {
    swizzleFloat3(m_pVertices[i].position);
    swizzleFloat3(m_pVertices[i].normal);
    //m_pVertices[i].texcoord[0][0] = 1.0f - m_pVertices[i].texcoord[0][0];		
  }

  // leafs
  for (int i=0; i < m_iNumLeafs; i++)
  {
    swizzleInt3(m_pLeafs[i].maxs);
    swizzleInt3(m_pLeafs[i].mins);
  }

  // faces, do lightmaps later...
  for (int i=0; i < m_iNumFaces; i++)
  {
    swizzleFloat3(m_pFaces[i].normal);
  }

  // planes
  for (int i=0; i < m_iNumPlanes; i++)
  {
    swizzleFloat3(m_pPlanes[i].normal);		
  }

  // nodes
  for (int i=0; i < m_iNumNodes; i++)
  {
    swizzleInt3(m_pNodes[i].maxs);
    swizzleInt3(m_pNodes[i].mins);
  }

	
	// subzones
	float flTemp=0.0f ;
	for (int i=0; i < m_iNumSubZones; i++)
  {
		swizzleFloat3(m_pSubZones[i].Max);		
		swizzleFloat3(m_pSubZones[i].Min);

		// swizzling will mix up z max and min due to the sign change, so swap them
		flTemp=m_pSubZones[i].Max[2] ;
		m_pSubZones[i].Max[2]=m_pSubZones[i].Min[2] ;
		m_pSubZones[i].Min[2]=flTemp ;

  }

	// portals
	for (int i=0; i < m_iNumPortals; i++)
  {
		swizzleFloat3(m_pPortals[i].Max);		
		swizzleFloat3(m_pPortals[i].Min);

		// swizzling will mix up z max and min due to the sign change, so swap them
		flTemp=m_pPortals[i].Max[2] ;
		m_pPortals[i].Max[2]=m_pPortals[i].Min[2] ;
		m_pPortals[i].Min[2]=flTemp ;

  }

	
	

}

void Q3Map::swizzleFloat3(float t[3])
{	
  float temp;
  temp = t[1];
  t[1] = t[2];
  t[2] = -temp;
}

void Q3Map::swizzleInt3(int t[3])
{	
  int temp;
  temp = t[1];
  t[1] = t[2];
  t[2] = -temp;
}



Q3BspPatch *Q3Map::handlePatch(int faceIndex)
{
  Q3BspPatch *q3patch;
  q3patch = new Q3BspPatch;

  int patch_size_x = (m_pFaces[faceIndex].size[0] - 1) / 2;
  int patch_size_y = (m_pFaces[faceIndex].size[1] - 1) / 2;
  int num_bezier_patches = patch_size_y * patch_size_x;

  q3patch->size = num_bezier_patches;
  q3patch->bezier = new Bezier[q3patch->size];

  int patchIndex =  0;
  int ii, n, j, nn;
  for (ii = 0, n = 0; n < patch_size_x; n++, ii = 2*n)
  {				
    for (j=0, nn=0; nn < patch_size_y; nn++, j = 2*nn)
    {
      int index = 0;
      for (int ctr = 0; ctr < 3; ctr++)
      { 
        int pos = ctr * m_pFaces[faceIndex].size[0];

        q3patch->bezier[patchIndex].mControls[index++] = 
          BspVertex(
          // position
          m_pVertices[m_pFaces[faceIndex].vertex + 
          ii + 
          m_pFaces[faceIndex].size[0] * j + 
          pos].position,
          // texture coordinates
          m_pVertices[m_pFaces[faceIndex].vertex + 
          ii + 
          m_pFaces[faceIndex].size[0] * j +
          pos].texcoord,
          // normal
          m_pVertices[m_pFaces[faceIndex].vertex +
          ii +
          m_pFaces[faceIndex].size[0] * j +
          pos].normal);

        q3patch->bezier[patchIndex].mControls[index++] = 
                BspVertex(
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 1].position,
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 1].texcoord,
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 1].normal);

        q3patch->bezier[patchIndex].mControls[index++] = 
                BspVertex(
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 2].position,
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 2].texcoord,
                  m_pVertices[m_pFaces[faceIndex].vertex + ii + m_pFaces[faceIndex].size[0] * j + pos + 2].normal);						
      }      
      q3patch->bezier[patchIndex].tessellate(5);
      patchIndex++;
    }
  }

  return q3patch;
}


int Q3Map::findVisibleFaces(const QVECTOR *camPos, int *facesToRender)
{
  int leaf;
  int visCluster;	

  leaf = findLeaf(camPos);	

  visCluster = m_pLeafs[leaf].cluster;

  memset(mVisibleFaces, 0, sizeof(int) * m_iNumFaces);	

  int faceindex;
  int renderindex=0;
  m_ClusterCount=0 ;
  
  for (int i=0; i < m_iNumLeafs; i++)
  {
    if (isClusterVisible(visCluster, m_pLeafs[i].cluster))
    {				
			m_ClusterCount++ ;
      bool vis=true ;//bool vis = mViewFrustum->checkIfBoxInside(m_pLeafs[i].mins, m_pLeafs[i].maxs);						
      
	  if (vis)
      {
        for (int k=0; k < m_pLeafs[i].n_leaffaces; k++)
        {					
          faceindex =	m_pLeafFaces[m_pLeafs[i].leafface + k];				
          if (mVisibleFaces[faceindex] == 0)
          {
            mVisibleFaces[faceindex] = 1;						
            facesToRender[renderindex++] = faceindex;
          }
        }
      }			
    }
  }
  

  facesToRender[renderindex] = -1;	

  return renderindex;
}


int Q3Map::findLeaf(const QVECTOR *camPos) const
{
  int index = 0;

  while (index >= 0)
  {
    const Q3BspNode *node = &m_pNodes[index];
    const Q3BspPlane *plane = &m_pPlanes[node->plane];

    // distance from point to plane
    //QVECTOR normal = QVECTOR(plane->normal);		 
    QVECTOR normal ;
	normal[0]=plane->normal[0] ;
	normal[1]=plane->normal[1] ;
	normal[2]=plane->normal[2] ;
	
	
	//const float distance = D3DXVec3Dot(&normal,camPos) - plane->dist;

	const float distance=(normal[0]* *camPos[0] + normal[1]* *camPos[1] + normal[2]* *camPos[2]) - plane->dist ;

    if(distance >= 0)
      index = node->children[0];
    else
      index = node->children[1];
  }

  return -index - 1;
}

bool Q3Map::isClusterVisible(int visCluster, int testCluster) const
{
  if (m_VisData == NULL)
    return true;

  if ((m_VisData->vecs == NULL) || (visCluster < 0)) 	
    return true;    

  int i = (visCluster * m_VisData->sz_vecs) + (testCluster >> 3);
  unsigned char visSet = m_VisData->vecs[i];

  return (visSet & (1 << (testCluster & 7))) != 0;
}

Q3BspFace_t *Q3Map::getFaces(void)
{
  return m_pFaces;
}





 /***********************************************************************************************************\
                                               
                                   New Parsing and Triangulation Functions 
                                                
 \***********************************************************************************************************/




// This routine is basically an overview of the entire process that converts the BSP
// into something our Ogre code can use to construct the map's mesh and level data.
// In essence, it converts the map geometry into a list of triangles sorted by zone and material,
// as well as extracting other map info like zone and portal bounding boxes, lights, entities etc.

int Q3Map::ParseAndTriangulateMap(const char* pData, size_t Size)
{
	
//	char chMessage[1024] ;
	int nError=0 ;

		// setup pointers to the various lumps and get their quantities
		nError=parseMap( pData, Size ) ;
		if(nError<0)
		{
			//sprintf(chMessage, "Parse Map Error: %i", nError) ; 
			//Q3Bug.LogAddCR(chMessage) ;
			return ERROR_ParseMap ;
		}

		// extract entities such as lights, monsters, etc
		if(!ParseEntities())										return ERROR_ParseEntities ;

		// initial memory allocation for triangles
		m_nVertexMax=0 ;
		if(!AllocateVertexMemory(m_iNumVertices))	return ERROR_AllocateVertex ;
		if(!AllocateTriangleMemory())						return ERROR_AllocateTriangle ;
		if(!initFaces())												return ERROR_InitializeFaces ;

		// no new map textures should be added after here, or else SetupTransTextures won't work
		if(!SetupTransTextures())								return ERROR_SetupTransTextures ;

		// work out the zones
		SetupZones() ;

		// convert faces to triangles
		if(!ConvertFacesToTriangles())					return ERROR_ConvertFaces ;

		if(!ConvertPatchesToTriangles())				return ERROR_ConvertPatches ;

		if(!ConvertLampsToTriangles())					return ERROR_ConvertLamps ;

		if(!ConvertLampsToGlowTriangles())			return ERROR_ConvertLampGlow ;

		if(!ConvertLightsToGlowTriangles())			return ERROR_ConvertLightGlow ;

		GetTexLampTextureNumbers() ; // find out which textures, if any, are textures/common/bzn_lightnode0 to textures/common/bzn_lightnode3

		// assign triangles to zones, splitting them where necessary
		if(!AssignTrianglesToZones())						return ERROR_AssignTriangles ;

		if(!ConvertTexLampsToLampTriangles())		return ERROR_ConvertTexLamp ;

		// sort by group and re-range the group numbers
		if(!SortTrianglesIntoGroups())					return ERROR_SortGroups ;

		// sort the triangles in order of zone and texture.  This will also get rid of any unsubzoned triangles.
		if(!SortTrianglesIntoBatches())					return ERROR_SortTriangles ;

		


		// Setup the portals, lights and various bits of map connectivity
		AssignPortalsToZones() ;		// what portals each zone touches
		AssignLightsToZones() ;			// what lights each zone touches
		AssignLightsToPortals() ;		// what lights each portal touches
		AssignZonesToZones() ;			// what zones each zone touches


		return NOERROR ;
}

void Q3Map::FreeParseMem(void)
{
	FreeVertexMemory() ;
	FreeTriangleMemory() ;
	DestroyBspFacesMemory() ;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory management

int Q3Map::AllocateTriangleMemory(void)
{
	// memory for the Triangle
	m_nTriangleSize=MEMADD ; // starting memory size
	m_pTriangleMem=malloc(m_nTriangleSize) ; // allocate starting memory space
	m_pTriangle=(triangle_t*)m_pTriangleMem ; // a pointer to the memory cast as a triangle_t
	m_nTriangleMax=0 ;
	m_nTriangleLimit=m_nTriangleSize/sizeof(triangle_t) ; // if pos reaches this memory must expand

	if(m_pTriangleMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeTriangleMemory(void)
{
	if(m_pTriangleMem) free(m_pTriangleMem) ;
	m_pTriangleMem=NULL ;
	m_pTriangle=NULL ;
	m_nTriangleMax=0 ;
	m_nTriangleLimit=0 ;
}

// increase size of Triangle memory, return 0 if failed
int Q3Map::ExpandTriangleMemory(void)
{
	m_nTriangleSize+=MEMADD ; // increase size
	m_pTriangleMem=realloc(m_pTriangleMem, m_nTriangleSize) ; // reallocate the memory
	if(m_pTriangleMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pTriangle=(triangle_t*)m_pTriangleMem ; // pointer to the memory cast as a triangle_t
	m_nTriangleLimit=m_nTriangleSize/sizeof(triangle_t) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddTriangle(triangle_t Triangle)
{
	if(m_nTriangleMax>=m_nTriangleLimit)
		if( !ExpandTriangleMemory() )
			return 0 ;

	m_pTriangle[m_nTriangleMax++]=Triangle ;

	return 1 ;
}

int Q3Map::AllocateVertexMemory(int nVertNum)
{
	// memory for the Vertex
	m_nVertexSize=nVertNum*sizeof(Q3BspVertex)+MEMADD ; // starting memory size
	m_pVertexMem=malloc(m_nVertexSize) ; // allocate starting memory space
	m_pVertex=(Q3BspVertex*)m_pVertexMem ; // a pointer to the memory cast as a triangle_t
	m_nVertexLimit=m_nVertexSize/sizeof(Q3BspVertex) ; // if pos reaches this memory must expand

	if(m_pVertexMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeVertexMemory(void)
{
	if(m_pVertexMem) free(m_pVertexMem) ;
	m_pVertexMem=NULL ;
	m_pVertex=NULL ;
	m_nVertexMax=0 ;
	m_nVertexLimit=0 ;
}

// increase size of Vertex memory, return 0 if failed
int Q3Map::ExpandVertexMemory(void)
{
	m_nVertexSize+=MEMADD ; // increase size
	m_pVertexMem=realloc(m_pVertexMem, m_nVertexSize) ; // reallocate the memory
	if(m_pVertexMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pVertex=(Q3BspVertex*)m_pVertexMem ; // pointer to the memory cast as a triangle_t
	m_nVertexLimit=m_nVertexSize/sizeof(Q3BspVertex) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddVertex(Q3BspVertex Vertex)
{

	if(m_nVertexMax>=m_nVertexLimit)
		if( !ExpandVertexMemory() )
			return 0 ;

	m_pVertex[m_nVertexMax++]=Vertex ;

	return 1 ;
}

int Q3Map::AllocateLightMemory(void)
{
	// memory for the Light
	m_nLightSize=MEMADD ; // starting memory size
	m_pLightMem=malloc(m_nLightSize) ; // allocate starting memory space
	m_pLight=(light_t*)m_pLightMem ; // a pointer to the memory cast as a light_t
	m_nLightMax=0 ;
	m_nLightLimit=m_nLightSize/sizeof(light_t) ; // if pos reaches this memory must expand

	if(m_pLightMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeLightMemory(void)
{
	if(m_pLightMem) free(m_pLightMem) ;
	m_pLightMem=NULL ;
	m_pLight=NULL ;
	m_nLightMax=0 ;
	m_nLightLimit=0 ;
}

// increase size of Light memory, return 0 if failed
int Q3Map::ExpandLightMemory(void)
{
	m_nLightSize+=MEMADD ; // increase size
	m_pLightMem=realloc(m_pLightMem, m_nLightSize) ; // reallocate the memory
	if(m_pLightMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pLight=(light_t*)m_pLightMem ; // pointer to the memory cast as a light_t
	m_nLightLimit=m_nLightSize/sizeof(light_t) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddLight(light_t Light)
{
	if(m_nLightLimit==0) // light memory hasn't been allocated yet
	{
		if( !AllocateLightMemory() )
			return 0 ;
	}
	else
		if(m_nLightMax>=m_nLightLimit)
			if( !ExpandLightMemory() )
				return 0 ;

	m_pLight[m_nLightMax++]=Light ;

	return 1 ;
}

// lamps are deferred shading, non-shadowing point lights
int Q3Map::AllocateLampMemory(void)
{
	// memory for the Lamp
	m_nLampSize=MEMADD ; // starting memory size
	m_pLampMem=malloc(m_nLampSize) ; // allocate starting memory space
	m_pLamp=(lamp_t*)m_pLampMem ; // a pointer to the memory cast as a lamp_t
	m_nLampMax=0 ;
	m_nLampLimit=m_nLampSize/sizeof(lamp_t) ; // if pos reaches this memory must expand

	if(m_pLampMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeLampMemory(void)
{
	if(m_pLampMem) free(m_pLampMem) ;
	m_pLampMem=NULL ;
	m_pLamp=NULL ;
	m_nLampMax=0 ;
	m_nLampLimit=0 ;
}

// increase size of Lamp memory, return 0 if failed
int Q3Map::ExpandLampMemory(void)
{
	m_nLampSize+=MEMADD ; // increase size
	m_pLampMem=realloc(m_pLampMem, m_nLampSize) ; // reallocate the memory
	if(m_pLampMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pLamp=(lamp_t*)m_pLampMem ; // pointer to the memory cast as a lamp_t
	m_nLampLimit=m_nLampSize/sizeof(lamp_t) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddLamp(lamp_t Lamp)
{
	if(m_nLampLimit==0) // Lamp memory hasn't been allocated yet
	{
		if( !AllocateLampMemory() )
			return 0 ;
	}
	else
		if(m_nLampMax>=m_nLampLimit)
			if( !ExpandLampMemory() )
				return 0 ;

	m_pLamp[m_nLampMax++]=Lamp ;

	return 1 ;
}

//////////////
// Q3BspTexture textures.  We duplicate the loaded texture mem and then add lighting textures to it.

int Q3Map::AllocateTextureMemory(void)
{
	// memory for the Texture
	m_nTextureSize=MEMADD ; // starting memory size
	m_pTextureMem=malloc(m_nTextureSize) ; // allocate starting memory space
	m_pTexture=(Q3BspTexture*)m_pTextureMem ; // a pointer to the memory cast as a Q3BspTexture
	m_nTextureMax=0 ;
	m_nTextureLimit=m_nTextureSize/sizeof(Q3BspTexture) ; // if pos reaches this memory must expand

	if(m_pTextureMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeTextureMemory(void)
{
	if(m_pTextureMem) free(m_pTextureMem) ;
	m_pTextureMem=NULL ;
	m_pTexture=NULL ;
	m_nTextureMax=0 ;
	m_nTextureLimit=0 ;
}

// increase size of Texture memory, return 0 if failed
int Q3Map::ExpandTextureMemory(void)
{
	m_nTextureSize+=MEMADD ; // increase size
	m_pTextureMem=realloc(m_pTextureMem, m_nTextureSize) ; // reallocate the memory
	if(m_pTextureMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pTexture=(Q3BspTexture*)m_pTextureMem ; // pointer to the memory cast as a Q3BspTexture
	m_nTextureLimit=m_nTextureSize/sizeof(Q3BspTexture) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddTexture(Q3BspTexture Texture)
{
	if(m_nTextureLimit==0) // Texture memory hasn't been allocated yet
	{
		if( !AllocateTextureMemory() )
			return 0 ;
	}
	else
		if(m_nTextureMax>=m_nTextureLimit)
			if( !ExpandTextureMemory() )
				return 0 ;

	m_pTexture[m_nTextureMax++]=Texture ;

	return 1 ;
}

// special version of the Add function, will not add if the texture name already exist.  
// Will succeed even if the texture is already on the list, but fails if it can't add a new texture
// returns texture index, or -1 on fail
// Q3 texture names can be tricky, I think I've had cases where they ended in spaces instead of nulls,
// and they might go all the way to the end without either.

int Q3Map::AddTextureUnique(Q3BspTexture Texture)
{
	if(m_nTextureLimit==0) // Texture memory hasn't been allocated yet
		if( !AllocateTextureMemory() )
			return ADDTEXTUREUNIQUE_FAIL ; // fail


	// scan through all the newly added textures so far and see if this one already exists.
	int nTexture=0 ;
	int nPos=0 ;

	bool bMatch=false ;

	for(nTexture=0 ; nTexture<m_nTextureMax ; nTexture++)
	{
		bMatch=true ;
		// scan through the characters of the texture names, comparing them.  We start after the original textures
		//for(nPos=m_iNumTexs ; nPos<Q3NAMESIZE ; nPos++)
		for(nPos=0 ; nPos<Q3NAMESIZE ; nPos++)
		{
			// is it the end of the texture name?  
			if(
						((Texture.name[nPos]							==0)	|| (Texture.name[nPos]							==' ')) // Texture    name end
					&&((m_pTexture[nTexture].name[nPos] ==0)	|| (m_pTexture[nTexture].name[nPos]	==' ')) // m_pTexture name end
				)
				break ;

			// do the two textures have a difference in the name at this position?
			if(Texture.name[nPos]!=m_pTexture[nTexture].name[nPos])
			{
				bMatch=false ;
				break ;
			}
		
		}// end scanning name

		if(bMatch) // found a match, so return ok but don't add a texture
		{
			return nTexture ;  // we don't add any new texture, return this texture's index
		}
	}

	// if we got this far, we must have a unique texture

	// add the texture, it is unique
	if(m_nTextureMax>=m_nTextureLimit)
		if( !ExpandTextureMemory() )
			return ADDTEXTUREUNIQUE_FAIL ; // fail

	m_pTexture[m_nTextureMax++]=Texture ;

	return m_nTextureMax-1 ; // return this new texture's index

}


////////////////////////////////////////

int Q3Map::AllocateTexLampMemory(void)
{
	// memory for the TexLamp
	m_nTexLampSize=MEMADD ; // starting memory size
	m_pTexLampMem=malloc(m_nTexLampSize) ; // allocate starting memory space
	m_pTexLamp=(int*)m_pTexLampMem ; // a pointer to the memory cast as an int
	m_nTexLampMax=0 ;
	m_nTexLampLimit=m_nTexLampSize/sizeof(int) ; // if pos reaches this memory must expand

	if(m_pTexLampMem==NULL)
		return 0 ;

	return 1 ;
}

void Q3Map::FreeTexLampMemory(void)
{
	if(m_pTexLampMem) free(m_pTexLampMem) ;
	m_pTexLampMem=NULL ;
	m_pTexLamp=NULL ;
	m_nTexLampMax=0 ;
	m_nTexLampLimit=0 ;
}

// increase size of TexLamp memory, return 0 if failed
int Q3Map::ExpandTexLampMemory(void)
{
	m_nTexLampSize+=MEMADD ; // increase size
	m_pTexLampMem=realloc(m_pTexLampMem, m_nTexLampSize) ; // reallocate the memory
	if(m_pTexLampMem==NULL) return 0 ; // failed to allocate memory, get out and return false

	// if here then memory allocation succeeded
	m_pTexLamp=(int*)m_pTexLampMem ; // pointer to the memory cast as an int
	m_nTexLampLimit=m_nTexLampSize/sizeof(int) ; // if pos reaches this memory must expand
	return 1 ; // ok
}

int Q3Map::AddTexLamp(int TexLamp)
{
	if(m_nTexLampMax>=m_nTexLampLimit)
		if( !ExpandTexLampMemory() )
			return 0 ;

	m_pTexLamp[m_nTexLampMax++]=TexLamp ;

	return 1 ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// extract entities from bsp entities lump
int Q3Map::ParseEntities(void)
{
//	char chKey[MAX_TOKENSIZE+1] ;   // +1 to leave room for null terminator
//	char chValue[MAX_TOKENSIZE+1] ; // +1 to leave room for null terminator
	int nPos=0 ;
	int nMaxPos=m_BspHeader.Lumps[0].iLength ;
	int nEntityType=0 ;


	// reset the spotlight textures
	m_nMaxSpotlightTexture=0 ;
	for(nPos=0 ; nPos<MAX_PROJECTORTEX ; nPos++)
		m_chSpotlightTexture[nPos][0]='\0' ;

	strcpy(m_chSpotlightTexture[m_nMaxSpotlightTexture], "spotlight.dds") ; // the default spotlight texture
	m_nMaxSpotlightTexture++ ;

	////////////////////////////////////////////////////////////////////////////
	// before adding all the entities we need to add some default textures
	Q3BspTexture Texture ;
	Texture.contents=0 ;
	Texture.flags=0 ;

	// default lamp texture
	strcpy(Texture.name, "lights/lamp_default") ;
	m_nDefaultTextureIndexLamp=AddTextureUnique(Texture) ;
	if(m_nDefaultTextureIndexLamp==ADDTEXTUREUNIQUE_FAIL) return 0 ;

	// default lamp2Pass texture
	strcpy(Texture.name, "lights/lamp2pass_default") ;
	m_nDefaultTextureIndexLamp2Pass=AddTextureUnique(Texture) ;
	if(m_nDefaultTextureIndexLamp2Pass==ADDTEXTUREUNIQUE_FAIL) return 0 ;

	// default glow texture
	strcpy(Texture.name, "GLOW_lamp") ;
	m_nDefaultTextureIndexGlowLamp=AddTextureUnique(Texture) ;
	if(m_nDefaultTextureIndexGlowLamp==ADDTEXTUREUNIQUE_FAIL) return 0 ;

	// default glow texture
	strcpy(Texture.name, "GLOW_light") ;
	m_nDefaultTextureIndexGlowLight=AddTextureUnique(Texture) ;
	if(m_nDefaultTextureIndexGlowLight==ADDTEXTUREUNIQUE_FAIL) return 0 ;


	// default spotlight texture
	//strcpy(Texture.name, "lights/light_default") ;
	//m_nDefaultTextureIndexSpotlight=AddTextureUnique(Texture) ;
	//if(m_nDefaultTextureIndexSpotlight==ADDTEXTUREUNIQUE_FAIL) return 0 ;

	//
	///////////////////////////////////////////////////////////////////////////


	nPos=-1 ;
	while(NextEntity(&nPos, nMaxPos))
	{
		
		nEntityType=GetEntityType(nPos, nMaxPos) ; // what type of entity is it?


		switch(nEntityType)
		{
			case ENTITY_ERROR: return 0 ; // something is wrong with the entity data

			case ENTITY_LIGHT: 
				if(!ParseAndAddLight(&nPos, nMaxPos)) 
					return 0 ; // something went wrong with parsing the light
				break ;
		}// end switch entity type


	}// end get next entity



	// everything is ok.
	return 1 ;
}

// move to the beginning of the next entity.
// fail if there are no more.
int Q3Map::NextEntity(int* pPos, int nMaxPos)
{
	while((++*pPos<nMaxPos) && (m_pEntities[*pPos]!='{')) ;

	if(*pPos==nMaxPos) 
		return 0 ;

	return 1 ;
}

// find out what type of entity this is.
// Since the classname might not be at the beginning we have to scan through the whole entity
// This function also doesn't update the position like the others do,
// since scanning of further key/values will have to start at the beginning of the entity again.
int Q3Map::GetEntityType(int nPos, int nMaxPos)
{
	char chKey[MAX_TOKENSIZE+1] ;   // +1 to leave room for null terminator
	char chValue[MAX_TOKENSIZE+1] ; // +1 to leave room for null terminator

	while(nPos<nMaxPos)
	{
		if(GetEntityKeyAndValue(&nPos, nMaxPos, chKey, chValue)!=KEY_OK) return ENTITY_ERROR ; // something went wrong, couldn't find any good keys
		
		if(strcmp(chKey, "classname")==0) // found the classname key
		{
			if(strcmp(chValue, "worldspawn")==0)			return ENTITY_WORLDSPAWN ;
			if(strcmp(chValue, "light")==0)						return ENTITY_LIGHT ;

			// wasn't any entity we recognize
			return ENTITY_UNKNOWN ;

		}// end if got entity classname

	}// end while nPos

	// didn't find any classname
	return ENTITY_ERROR ;
}

// get next entity key and value
int Q3Map::GetEntityKeyAndValue(int* pPos, int nMaxPos, char* pKey, char* pValue)
{

	char* pEntText=m_pEntities ;
	int nEntPos=*pPos ;
	int nKeyPos=-1 ;
	int nValuePos=-1 ;

	// clear key and value strings
	pKey[0]='\0' ;
	pValue[0]='\0' ;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Key

	// find the next "
	while((++nEntPos<nMaxPos) && (pEntText[nEntPos]!='}') && (pEntText[nEntPos]!='"')) ;

	// didn't find key, get out
	if((nEntPos==nMaxPos) || (pEntText[nEntPos]=='}'))
		return KEY_NONE ;


	// copy key
	while((++nEntPos<nMaxPos) && (nKeyPos<MAX_TOKENSIZE) && (pEntText[nEntPos]!='}') && (pEntText[nEntPos]!='"')) 
		pKey[++nKeyPos]=pEntText[nEntPos] ;

	if((nEntPos==nMaxPos) || (nKeyPos==MAX_TOKENSIZE) || (pEntText[nEntPos]=='}'))
		return KEY_ERROR ; // entity was incomplete or too big

	pKey[++nKeyPos]='\0' ;

	/////////////////////////////////////////////////////////////////////////////////////////
	// value

	// find the next "
	while((++nEntPos<nMaxPos) && (pEntText[nEntPos]!='}') && (pEntText[nEntPos]!='"')) ;

	// didn't find value, get out
	if((nEntPos==nMaxPos) || (pEntText[nEntPos]=='}'))
		return KEY_ERROR ; 

	// copy value
	while((++nEntPos<nMaxPos) && (nValuePos<MAX_TOKENSIZE) && (pEntText[nEntPos]!='}') && (pEntText[nEntPos]!='"')) 
		pValue[++nValuePos]=pEntText[nEntPos] ;

	if((nEntPos==nMaxPos) || (nValuePos==MAX_TOKENSIZE) || (pEntText[nEntPos]=='}'))
		return KEY_ERROR ; // entity was incomplete or too big

	pValue[++nValuePos]='\0' ;

	/////////////////////////////////////////////////////////////////////////////////////////

	*pPos=nEntPos+1 ;

	return KEY_OK ;
}


// fills array pNumber with the values extracted from pValue, returns how many numbers it got.
// float version
int Q3Map::GetNumbersFromValue(char* pValue, float *pNumber, int nNumberSize)
{
	int nLength=strlen(pValue) ;
	if(nLength<1) return 0 ;

	int nPos=-1 ;
	int nCount=0 ;
	char chTemp[MAX_TOKENSIZE+1] ;
	int nTempPos=0 ;

	do
	{

		nPos++ ;
		
		if(
				((pValue[nPos]>='0') && (pValue[nPos]<='9')) // found another digit
				||
				(pValue[nPos]=='.')
				||
				(pValue[nPos]=='-')
			)
		{
			chTemp[nTempPos++]=pValue[nPos] ;
			if(nTempPos==MAX_TOKENSIZE) return 0 ; // number too big
		}
		else // anything else means the end of the number
		{
			
			chTemp[nTempPos]='\0' ;
			pNumber[nCount++]=atof(chTemp) ;
			nTempPos=0 ;
		}
		

	}while((nPos<nLength) && (nCount<nNumberSize) && (pValue[nPos]!='\0')) ;


	return nCount ;
}

// integer version
int Q3Map::GetNumbersFromValue(char* pValue, int *pNumber, int nNumberSize)
{
	int nLength=strlen(pValue) ;
	if(nLength<1) return 0 ;

	int nPos=-1 ;
	int nCount=0 ;
	char chTemp[MAX_TOKENSIZE+1] ;
	int nTempPos=0 ;

	do
	{

		nPos++ ;
		
		if(
				((pValue[nPos]>='0') && (pValue[nPos]<='9')) // found another digit
				||
				(pValue[nPos]=='.')
				||
				(pValue[nPos]=='-')
			)
		{
			chTemp[nTempPos++]=pValue[nPos] ;
			if(nTempPos==MAX_TOKENSIZE) return 0 ; // number too big
		}
		else // anything else means the end of the number
		{
			
			chTemp[nTempPos]='\0' ;
			pNumber[nCount++]=atoi(chTemp) ;
			nTempPos=0 ;
		}
		

	}while((nPos<nLength) && (nCount<nNumberSize) && (pValue[nPos]!='\0')) ;


	return nCount ;
}

//''
// extracts data for either forward rendered shadow casting spotlights or deferred shading non-shadowing point lights
// the point lights ("lamps") will later be changed into map triangles.
int Q3Map::ParseAndAddLight(int* pPos, int nMaxPos)
{
//	char chMessage[1024] ;


	char chKey[MAX_TOKENSIZE+1] ;   // +1 to leave room for null terminator
	char chValue[MAX_TOKENSIZE+1] ; // +1 to leave room for null terminator
	float flOrigin[3]={0.0f, 0.0f, 0.0f} ;
	float flColour[3]={1.0f, 1.0f, 1.0f} ;
	float flCentre[3]={0.0f, 0.0f, 0.0f} ;
	float flAimvec[3]={0.0f, 0.0f, 0.0f} ;
	float flRadius[3]={0.0f, 0.0f, 0.0f} ;
	
	float flTemp[3]={0.0f, 0.0f, 0.0f} ;
	int nTemp[3] ;

	float flAngle=0.0f ;
	float flCutoff=0.0f ;
	float flLength=0.0f ;
	float flBrightness=0.0f ;
	int nLightNode=-1 ;

	bool bSpotLight=false ;

	Q3BspTexture Q3Texture ;
	Q3Texture.flags=0 ;
	Q3Texture.contents=0 ;
	Q3Texture.name[0]=0 ;


	light_t NewLight ; // forward rendered shadow casting spotlight
	ZeroMemory((void*)&NewLight, sizeof(light_t)) ;

	lamp_t NewLamp ; // deferred shading non-shadowing point light
	ZeroMemory((void*)&NewLamp, sizeof(lamp_t)) ;

	int nKeyReturn=KEY_NONE ; 

	do
	{
		nKeyReturn=GetEntityKeyAndValue(pPos, nMaxPos, chKey, chValue) ;
	
		if(nKeyReturn==KEY_OK) // found a key
		{

			if(strcmp(chKey, "origin")==0)
			{
				if(GetNumbersFromValue(chValue, flOrigin, 3)!=3) return 0 ; // extract the numbers
				swizzleFloat3(flOrigin) ; // fix coordinates
			}
			else
			if(strcmp(chKey, "_color")==0)
			{
				if(GetNumbersFromValue(chValue, flColour, 3)!=3) return 0 ; // extract the numbers
			}
			else
			if(strcmp(chKey, "light_center")==0)
			{
				if(GetNumbersFromValue(chValue, flCentre, 3)!=3) return 0 ; // extract the numbers		
				swizzleFloat3(flCentre) ; // fix coordinates
			}
			else
			if(strcmp(chKey, "light_target")==0)
			{
				if(GetNumbersFromValue(chValue, flAimvec, 3)!=3) return 0 ; // extract the numbers		
				swizzleFloat3(flAimvec) ; // fix coordinates
				bSpotLight=true ; // if there's a target key, then this must be a spotlight
			}
			else
			if(strcmp(chKey, "light_radius")==0)
			{
				if(GetNumbersFromValue(chValue, flRadius, 3)!=3) return 0 ; // extract the numbers
				swizzleFloat3(flRadius) ; // fix coordinates
				// make sure all values are positive
				flRadius[0]=fabs(flRadius[0]) ;
				flRadius[1]=fabs(flRadius[1]) ;
				flRadius[2]=fabs(flRadius[2]) ;
			}
			else
			if(strcmp(chKey, "light_abc")==0)
			{
				if(GetNumbersFromValue(chValue, flTemp, 3)!=3) return 0 ; // extract the numbers
				flAngle				= flTemp[0] ;
				flBrightness	= flTemp[1] ;
				flCutoff			= flTemp[2] ;
			}
			else
			if(strcmp(chKey, "texture")==0)
			{
				strcpy(Q3Texture.name, chValue) ;
			}
			if(strcmp(chKey, "light_node")==0)
			{
				if(GetNumbersFromValue(chValue, nTemp, 1)!=1) return 0 ; // extract the number
				nLightNode				= nTemp[0] ;	
				if((nLightNode<0) || (nLightNode>3)) return 0 ; // something dodgy about the lightnode number
			}

		}// end if key ok

		

	}while(nKeyReturn==KEY_OK) ;  // end do looping through keys


	// return a fail if there was a problem with the keys
	if(nKeyReturn==KEY_ERROR) return 0 ; 


	if(bSpotLight)// found a light_target so this must be a spotlight
	{
		// light settings.
		NewLight.Position[0]=flOrigin[0]+flCentre[0] ;
		NewLight.Position[1]=flOrigin[1]+flCentre[1] ;
		NewLight.Position[2]=flOrigin[2]+flCentre[2] ;

		NewLight.Min[0]=flOrigin[0]-flRadius[0] ;
		NewLight.Min[1]=flOrigin[1]-flRadius[1] ;
		NewLight.Min[2]=flOrigin[2]-flRadius[2] ;

		NewLight.Max[0]=flOrigin[0]+flRadius[0] ;
		NewLight.Max[1]=flOrigin[1]+flRadius[1] ;
		NewLight.Max[2]=flOrigin[2]+flRadius[2] ;

		NewLight.Colour[0]=flColour[0] ;
		NewLight.Colour[1]=flColour[1] ;
		NewLight.Colour[2]=flColour[2] ;

		NewLight.Angle=flAngle ;
		NewLight.Cutoff=flCutoff ;
		NewLight.Brightness=flBrightness * BRIGHTNESSTWEAK ;


		// direction light points, as a normal
		flLength=sqrt( flAimvec[0]*flAimvec[0] + flAimvec[1]*flAimvec[1] + flAimvec[2]*flAimvec[2] ) ;
		if(flLength>0.0f)
		{
			NewLight.Direction[0]=flAimvec[0]/flLength ;
			NewLight.Direction[1]=flAimvec[1]/flLength ;
			NewLight.Direction[2]=flAimvec[2]/flLength ;
		}
		else
		{ // default to pointing down
			NewLight.Direction[0]=0.0f ;
			NewLight.Direction[1]=1.0f ;
			NewLight.Direction[2]=0.0f ;
		}

		NewLight.ZoneCount=0 ;
		NewLight.CentreZone=0 ;
	
		
		if(Q3Texture.name[0]==0)	
			strcpy(Q3Texture.name, "spotlight.dds") ;
		
		NewLight.Texture=AddSpolightTexture(Q3Texture.name) ;


		if(NewLight.Texture==ADDSPOTLIGHTTEXTURE_FAIL)
			return 0 ; // failure	
	
		//AddTextureUnique(Q3Texture) ;
		/*
		// add the light's texture index
		if(Q3Texture.name[0]==0)
			NewLight.Texture=m_nDefaultTextureIndexSpotlight ;
		else
		{
			NewLight.Texture=AddTextureUnique(Q3Texture) ; // this will add the texture name to the list if it is unique, as well as setting the index
			if(NewLight.Texture==ADDTEXTUREUNIQUE_FAIL) 
				return 0 ;
		}
		*/

		return AddLight(NewLight) ;
	}
	else // add a non-shadowing deferred shading point light
	{
		// light settings.
		NewLamp.Position[0]=flOrigin[0]+flCentre[0] ;
		NewLamp.Position[1]=flOrigin[1]+flCentre[1] ;
		NewLamp.Position[2]=flOrigin[2]+flCentre[2] ;

		NewLamp.Min[0]=flOrigin[0]-flRadius[0] ;
		NewLamp.Min[1]=flOrigin[1]-flRadius[1] ;
		NewLamp.Min[2]=flOrigin[2]-flRadius[2] ;

		NewLamp.Max[0]=flOrigin[0]+flRadius[0] ;
		NewLamp.Max[1]=flOrigin[1]+flRadius[1] ;
		NewLamp.Max[2]=flOrigin[2]+flRadius[2] ;

		NewLamp.Colour[0]=flColour[0] ;
		NewLamp.Colour[1]=flColour[1] ;
		NewLamp.Colour[2]=flColour[2] ;

		NewLamp.Brightness=flBrightness * BRIGHTNESSTWEAK ;

		NewLamp.LightNode=nLightNode ; // typically -1, but may be 0 to 3 if this lamp is the node for some TexLamp freeform deferred shading geometry.


		// Note that m_pLamp.Zone will be set after we first convert lamps into triangles and then 
		// assign those triangles to zones.  At that point, if the assigned triangle is also flagged as
		// coming from a lamp, then the lamp's list of zones will be updated.

				// add the light's texture index
		if(Q3Texture.name[0]==0)
		{
			if(nLightNode==-1)
				NewLamp.Texture=m_nDefaultTextureIndexLamp ; // normal 1 pass deferred shading
			else
				NewLamp.Texture=m_nDefaultTextureIndexLamp2Pass ; // special 2 pass deferred shading to texlamps
		}
		else
		{
			NewLamp.Texture=AddTextureUnique(Q3Texture) ; // this will add the texture name to the list if it is unique, as well as setting the index
			if(NewLamp.Texture==ADDTEXTUREUNIQUE_FAIL) 
				return 0 ;
		}	

		return AddLamp(NewLamp) ;
	}

}

// adds a spotlight texture name if it is unique, returns the index to that texture name either way.
// returns ADDSPOTLIGHTTEXTURE_FAIL on a fail

int Q3Map::AddSpolightTexture(char TEXNAME[])
{
	if((strlen(TEXNAME)>Q3NAMESIZE) || (m_nMaxSpotlightTexture>=MAX_PROJECTORTEX))
		return ADDSPOTLIGHTTEXTURE_FAIL ;


	// scan through all the newly added textures so far and see if this one already exists.
	int nTexture=0 ;
	int nPos=0 ;

	bool bMatch ;

	for(nTexture=0 ; nTexture<m_nMaxSpotlightTexture ; nTexture++)
	{
		bMatch=true ;
		// scan through the characters of the texture names, comparing them. 
		for(nPos=0 ; nPos<Q3NAMESIZE ; nPos++)
		{
			// do the two textures have a difference in the name at this position?
			if(m_chSpotlightTexture[nTexture][nPos]!=TEXNAME[nPos])
			{
				bMatch=false ;
				break ;
			}

			// is it the end of the texture name?  
			if(TEXNAME[nPos]=='\0') // end of texture
				break ;
		
		}// end scanning name

		if(bMatch) // found a match, so return ok but don't add a texture
			return nTexture ;  // we don't add any new texture, return this texture's index
	}

	// if we got this far, we must have a unique texture
	strcpy(m_chSpotlightTexture[m_nMaxSpotlightTexture], TEXNAME) ;
	m_nMaxSpotlightTexture++ ;

	return m_nMaxSpotlightTexture-1 ; // return this new texture's index

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Q3Map::initFaces(void)
{

	m_nGroup=0 ; // reset the group counter, used to work out transparent groupings for triangles.
	
  Q3BspFace_t *faces = getFaces();

  m_BspFaces = new Q3BspFaceRenderer[m_iNumFaces];
	if(m_BspFaces==NULL) return 0 ; // fail, out of memory.
  m_NumBspFaces = m_iNumFaces;

	
  for (int i=0; i < m_NumBspFaces; i++)
  {					
    m_BspFaces[i].lm_index = faces[i].lm_index;
    m_BspFaces[i].meshvert = faces[i].meshvert;
    m_BspFaces[i].n_meshverts = faces[i].n_meshverts;
    m_BspFaces[i].n_vertexes = faces[i].n_vertexes;
    for (int j=0; j<3; j++)
      m_BspFaces[i].normal[j] = faces[i].normal[j];
    m_BspFaces[i].texture = faces[i].texture;
    m_BspFaces[i].type = faces[i].type;
    m_BspFaces[i].vertex = faces[i].vertex;

    m_BspFaces[i].n_triangles = m_BspFaces[i].n_meshverts / 3;

    if (m_BspFaces[i].type == PATCH)
    {
      m_BspFaces[i].patch = handlePatch(i);
    }
    else
    {
      m_BspFaces[i].patch = NULL;
    }


  }


	// check patches aren't degenerate
	int numIndex = 0;
  int numVertex = 0;


    for (int i=0; i < m_NumBspFaces; i++)
    {		

			numIndex = 0;
			numVertex = 0;

      if ((m_BspFaces[i].type == PATCH) && (m_BspFaces[i].patch != NULL))
			{
          for (int j=0; j < m_BspFaces[i].patch->size; j++)
          {
            numIndex += m_BspFaces[i].patch->bezier[j].mNumIndex;
            numVertex += m_BspFaces[i].patch->bezier[j].mNumVertex;
          }

					if((numIndex==0) || (numVertex==0))
					{
						DELETE_ARRAY( m_BspFaces[i].patch->bezier ) ;
						DELETE_POINTER( m_BspFaces[i].patch ) ;
					}

      }// end if patch


    }// end for 



	// copy the vertices over.  
	// We need to work on a copy because we need to create new verts when splitting triangles that cross subzones, and for patches
	for(int i=0 ; i<m_iNumVertices ; i++)
	{
		m_pVertex[i]=m_pVertices[i] ;
		m_nVertexMax++ ; // need to update this manually since we aren't adding new verts, but filling in the original mem.
	}


	return 1 ;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// stuff for assigning triangles to subzones, splitting them where necessary.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Q3Map::SetupZones(void)
{
	int nSubZone=0 ;
	int nZone=0 ;
	int nPos=0 ;

//	float flMinX=0.0f ;
//	float flMinY=0.0f ;
//	float flMinZ=0.0f ;
//	float flMaxX=0.0f ;
//	float flMaxY=0.0f ;
//	float flMaxZ=0.0f ;

	m_nMaxZone=0 ;
	
	for(nZone=0 ; nZone<MAX_ZONE ; nZone++)
	{
		m_nZone[nZone][INDEX_SUBZONECOUNT]=0 ;
		m_ZoneBoundary[nZone].Min[0]=MINMAXLIMIT ;
		m_ZoneBoundary[nZone].Min[1]=MINMAXLIMIT ;
		m_ZoneBoundary[nZone].Min[2]=MINMAXLIMIT ;
		m_ZoneBoundary[nZone].Max[0]=-MINMAXLIMIT ;
		m_ZoneBoundary[nZone].Max[1]=-MINMAXLIMIT ;
		m_ZoneBoundary[nZone].Max[2]=-MINMAXLIMIT ;
	}


	// find the highest used zone number, then +1 to get our limit.
	for(nSubZone=0 ; nSubZone<m_iNumSubZones ; nSubZone++)
		if(m_pSubZones[nSubZone].Zone>m_nMaxZone)
			m_nMaxZone=m_pSubZones[nSubZone].Zone ;

	m_nMaxZone++ ; // our limit

	
	//char chMessage[1024] ;


	// fill in what subzones are in each zone
	for(nSubZone=0 ; nSubZone<m_iNumSubZones ; nSubZone++)
	{
		

		nZone=m_pSubZones[nSubZone].Zone ;

		// find next empty slot in this zone
		nPos=0 ;
		while((nPos<MAX_SUBZONEPERZONE) && (nPos<m_nZone[nZone][INDEX_SUBZONECOUNT]))
			nPos++ ;

		// if there's room, add the subzone to this zone
		if(nPos<MAX_SUBZONEPERZONE) 
		{
			m_nZone[nZone][nPos]=nSubZone ;

			// limits used for cutting up lights (not useful for other boundary stuff, check subzones instead)
			if(m_pSubZones[nSubZone].Min[0]<m_ZoneBoundary[nZone].Min[0]) 
				m_ZoneBoundary[nZone].Min[0]= m_pSubZones[nSubZone].Min[0] ;
			if(m_pSubZones[nSubZone].Min[1]<m_ZoneBoundary[nZone].Min[1]) 
				m_ZoneBoundary[nZone].Min[1]= m_pSubZones[nSubZone].Min[1] ;
			if(m_pSubZones[nSubZone].Min[2]<m_ZoneBoundary[nZone].Min[2]) 
				m_ZoneBoundary[nZone].Min[2]= m_pSubZones[nSubZone].Min[2] ;

			if(m_pSubZones[nSubZone].Max[0]>m_ZoneBoundary[nZone].Max[0]) 
				m_ZoneBoundary[nZone].Max[0]= m_pSubZones[nSubZone].Max[0] ;
			if(m_pSubZones[nSubZone].Max[1]>m_ZoneBoundary[nZone].Max[1]) 
				m_ZoneBoundary[nZone].Max[1]= m_pSubZones[nSubZone].Max[1] ;
			if(m_pSubZones[nSubZone].Max[2]>m_ZoneBoundary[nZone].Max[2]) 
				m_ZoneBoundary[nZone].Max[2]= m_pSubZones[nSubZone].Max[2] ;

			m_nZone[nZone][INDEX_SUBZONECOUNT]++ ;
		}


	}


}


// work out what zone each triangle is in.
// if it is in more than one, cut it up into smaller triangles that are only in one zone each.
int Q3Map::AssignTrianglesToZones(void)
{
	int nCurrentTriangle=0 ;
//	int nZone=0 ;


	/*
	char chMessage[1024] ;
	float flPos[3] ;
	int nTri=0 ;
	float flVert[3][3] ;
	

	for(nTri=0 ; nTri<m_nTriangleMax ; nTri++)
	{
		flVert[0][0]=m_pVertex[  m_pTriangle[nTri].VIndex[0]  ].position[0] ;
		flVert[0][1]=m_pVertex[  m_pTriangle[nTri].VIndex[0]  ].position[1] ;
		flVert[0][2]=m_pVertex[  m_pTriangle[nTri].VIndex[0]  ].position[2] ;

		flVert[1][0]=m_pVertex[  m_pTriangle[nTri].VIndex[1]  ].position[0] ;
		flVert[1][1]=m_pVertex[  m_pTriangle[nTri].VIndex[1]  ].position[1] ;
		flVert[1][2]=m_pVertex[  m_pTriangle[nTri].VIndex[1]  ].position[2] ;

		flVert[2][0]=m_pVertex[  m_pTriangle[nTri].VIndex[2]  ].position[0] ;
		flVert[2][1]=m_pVertex[  m_pTriangle[nTri].VIndex[2]  ].position[1] ;
		flVert[2][2]=m_pVertex[  m_pTriangle[nTri].VIndex[2]  ].position[2] ;


		flPos[0]=(flVert[0][0]+flVert[1][0]+flVert[2][0])/3.0f ;
		flPos[1]=(flVert[0][1]+flVert[1][1]+flVert[2][1])/3.0f ;
		flPos[2]=(flVert[0][2]+flVert[1][2]+flVert[2][2])/3.0f ;

		nZone=0 ;
		while((nZone<m_iNumSubZones) && !PointInZone(flPos, nZone))
			nZone++ ;




	}
	*/



	for(nCurrentTriangle=0 ; nCurrentTriangle<m_nTriangleMax ; nCurrentTriangle++)
	{
		if(!SetupTriangleZone(nCurrentTriangle)) return 0 ; // what zone completely contains this triangle, if any

	}// end for current triangle

	return 1 ;
}

// return the zone this triangle is in, or -1 if it is not entirely contained by any zone
// this is also a convienient spot for us to update Lamp zones when we discover what zones its triangles are in,
// and also a handy place to note if the triangle is a TexLamp or not 
// (TexLamp is free form geometry that is bound to a lamp and gets converted to deferred shading lights, allowing us to do fake shadowing)

int Q3Map::FindTriangleZone(int nTriangle)
{
	int nZone=0 ;
//	int nSubZone=0 ;
	int nPos=0 ;
	bool bVertInSubZone=false ;
	int nVert=0 ;
	bool bTriangleInZone=false ;
//	int nMaxSubZone=m_iNumSubZones ;
	float flVert[6][3] ; // verts 0, 1, 2 are the original triangle corners, verts 3, 4, 5 are interpolated edge points.
	// we need the edge points, since it's possible for all the 
	// triangle verts to be in one L shaped zone but the triangle they form not be in that zone. 
	// (such as if there's a vert at the corner and two ends of the "L")


	flVert[0][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[0] ;
	flVert[0][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[1] ;
	flVert[0][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[2] ;

	flVert[1][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[0] ;
	flVert[1][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[1] ;
	flVert[1][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[2] ;

	flVert[2][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[0] ;
	flVert[2][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[1] ;
	flVert[2][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[2] ;

	flVert[3][0]=(flVert[0][0]+flVert[1][0])/2.0f ;
	flVert[3][1]=(flVert[0][1]+flVert[1][1])/2.0f ;
	flVert[3][2]=(flVert[0][2]+flVert[1][2])/2.0f ;

	flVert[4][0]=(flVert[1][0]+flVert[2][0])/2.0f ;
	flVert[4][1]=(flVert[1][1]+flVert[2][1])/2.0f ;
	flVert[4][2]=(flVert[1][2]+flVert[2][2])/2.0f ;

	flVert[5][0]=(flVert[2][0]+flVert[0][0])/2.0f ;
	flVert[5][1]=(flVert[2][1]+flVert[0][1])/2.0f ;
	flVert[5][2]=(flVert[2][2]+flVert[0][2])/2.0f ;


	// scan through the zones until:
	// a) we find a zone that completely contains the six vertices
	// b) we run out of zones.


	do
	{
		nVert=0 ;
		bTriangleInZone=false ;

		// scan through the verts until:
		// a) we find a vert that isn't in this zone
		// b) we run out of verts

		do
		{

			// scan through the zone's subzones until: 
			// a) we find one that contains this vert, 
			// b) we hit the subzoneperzone limit, 
			// c) this zone runs out of subzones

			nPos=0 ;
			do
			{
				bVertInSubZone=PointInSubZone(flVert[nVert], m_nZone[nZone][nPos]) ;
			}while( !bVertInSubZone && (++nPos<MAX_SUBZONEPERZONE) && (nPos<m_nZone[nZone][INDEX_SUBZONECOUNT])) ;

			// if bVertInSubZone is false, we found a vert that isn't in this zone.

		}while(bVertInSubZone && (++nVert<6)) ;

		if(bVertInSubZone) bTriangleInZone=true ;

	}while(!bTriangleInZone && (++nZone<m_nMaxZone)) ;


	


	if(!bTriangleInZone)
		return -1 ;
	else
	{

		// if this triangle came from a lamp entity, we note in that lamp entity that it touches this zone
		int nLamp=m_pTriangle[nTriangle].Lamp ;
		if(nLamp>-1) // if we have a lamp
		{


			int nSlot=m_pLamp[nLamp].Zone[MAX_ZONEPERLIGHT] ;

			if(nSlot<MAX_ZONEPERLIGHT) // if the lamp isn't maxed out on zones
			{
				// check if we already have this zone recorded
				int nCheckSlot=0 ;
				int nFoundDuplicate=0 ;
				for(nCheckSlot=0 ; nCheckSlot<nSlot ; nCheckSlot++)
					if(m_pLamp[nLamp].Zone[nCheckSlot]==nZone)
					{
						nFoundDuplicate=1 ;
						break ;
					}

				if(!nFoundDuplicate)
				{
					m_pLamp[nLamp].Zone[nSlot]=nZone ;  // write the zone into this slot
					m_pLamp[nLamp].Zone[MAX_ZONEPERLIGHT]++ ; // note that we have one more zone
				}

			}
		}


		// if this triangle is a TexLamp triangle (free form deferred lighting shapes)
		// then we remember this, in order to make assigning it to a lamp faster
		int nTexture=m_pTriangle[nTriangle].Texture ;
		if((nTexture==m_nBZN_LightNode0) || (nTexture==m_nBZN_LightNode1) || (nTexture==m_nBZN_LightNode2) || (nTexture==m_nBZN_LightNode3))
			if(!AddTexLamp(nTriangle)) 
				return ADDTEXLAMP_FAIL ; //if we failed to note the texlamp (probably out of memory), the whole level load will fail.


		// finally, return the zone the triangle is in.
		return nZone ;
	}
}



// work out what zone a triangle is in, cut it up if it's in more than one.
int Q3Map::SetupTriangleZone(int nTriangle)
{


	int nZone=0 ;

	nZone=FindTriangleZone(nTriangle) ;

	if(nZone==ADDTEXLAMP_FAIL)
		return 0 ; 


	if(nZone!=-1) // triangle was completely in a zone
		m_pTriangle[nTriangle].Zone=nZone ;
	else
	{
		// This triangle is in more than one zone.  (Or no subzone at all)
		// we chop it up along the edges of every subzone it is in (regardless of whether they are a common zone or not)
		// so that the resulting triangles will all be in just one subzone, and therefore in only one zone.
		// this might produce a few extra triangles more than we strictly need, since some will both be in the same zone,
		// but it is simple and the extra triangles are trivial in number.

		// As we go to each new cut plane, it must be applied progressively to all newly created triangles too.

		int nInitialTrianglePos=m_nTriangleMax ; // where we start adding new triangles

		int nTriLoop=0 ;
		int nTriangleToCut=0 ;
		int nMaxTriangle=0 ;
		int nSide=0 ;
		int nAxis=0 ;
		float flCutPos=0.0f ;
		int nSubZone=0 ;
		int nMaxSubZone=m_iNumSubZones ;
		int nVert=0 ;
		float flVert[6][3] ; // verts 0, 1, 2 are the original triangle corners, verts 3, 4, 5 are interpolated edge points.
		// we need the edge points, since it's possible for all the 
		// triangle verts to be in one L shaped zone but the triangle they form not be in that zone. 
		// (such as if there's a vert at the corner and two ends of the "L")


		flVert[0][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[0] ;
		flVert[0][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[1] ;
		flVert[0][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[2] ;

		flVert[1][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[0] ;
		flVert[1][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[1] ;
		flVert[1][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[2] ;

		flVert[2][0]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[0] ;
		flVert[2][1]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[1] ;
		flVert[2][2]=m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[2] ;

		flVert[3][0]=(flVert[0][0]+flVert[1][0])/2.0f ;
		flVert[3][1]=(flVert[0][1]+flVert[1][1])/2.0f ;
		flVert[3][2]=(flVert[0][2]+flVert[1][2])/2.0f ;

		flVert[4][0]=(flVert[1][0]+flVert[2][0])/2.0f ;
		flVert[4][1]=(flVert[1][1]+flVert[2][1])/2.0f ;
		flVert[4][2]=(flVert[1][2]+flVert[2][2])/2.0f ;

		flVert[5][0]=(flVert[2][0]+flVert[0][0])/2.0f ;
		flVert[5][1]=(flVert[2][1]+flVert[0][1])/2.0f ;
		flVert[5][2]=(flVert[2][2]+flVert[0][2])/2.0f ;
	



		for(nSubZone=0 ; nSubZone<nMaxSubZone ; nSubZone++)
		{
			// are any of our original verts in this subzone?

			nVert=0 ;
			while( !PointInSubZone(flVert[nVert], nSubZone) && (++nVert<6)) ;

			if(nVert<6) // one of the verts must have been in this subzone.
			{
				// cutup all triangles by the sides of this subzone.
				// we'll need to cut more and more triangles as this progresses. (or at least test if they need to be cut)
				for(nSide=0 ; nSide<6 ; nSide++)
				{
					switch(nSide)
					{
						case 0: nAxis=AXIS_X ; flCutPos=m_pSubZones[nSubZone].Min[0] ; break ;
						case 1: nAxis=AXIS_X ; flCutPos=m_pSubZones[nSubZone].Max[0] ; break ;
						case 2: nAxis=AXIS_Y ; flCutPos=m_pSubZones[nSubZone].Min[1] ; break ;
						case 3: nAxis=AXIS_Y ; flCutPos=m_pSubZones[nSubZone].Max[1] ; break ;
						case 4: nAxis=AXIS_Z ; flCutPos=m_pSubZones[nSubZone].Min[2] ; break ;
						case 5: nAxis=AXIS_Z ; flCutPos=m_pSubZones[nSubZone].Max[2] ; break ;
					}


					nMaxTriangle=m_nTriangleMax-nInitialTrianglePos ;  // how may new triangles have been created since we first started cutting the original.

					for(nTriLoop=-1 ; nTriLoop<nMaxTriangle ; nTriLoop++)
					{
						// work out if we are cutting up the original triangle or one of the newly created ones.
						if(nTriLoop==-1)
							nTriangleToCut=nTriangle ; // the original triangle, perhaps heavily cutup by now.
						else
							nTriangleToCut=nInitialTrianglePos+nTriLoop ;  // one of the newly created triangles.

						if(!SplitTriangle(nTriangleToCut, nAxis, flCutPos)) return 0 ; // cut up the triangle, fail if we're out of memory or whatever.


					}// end for nTriLoop

				}// end cutting by each side of the subzone

			}// end if one of the verts was in this subzone

		}// end going through all subzones


		// now that the triangle is well and truly chopped up, assign it a zone.
		// Even though it should be entirely in a subzone by now, there's still the chance that
		// it might not be inside any subzone at all.  If so, it will be assigned -1 zone and 
		// exluded from the manualobjects
		
		m_pTriangle[nTriangle].Zone=FindTriangleZone(nTriangle) ;

		// we don't have to worry about assigning zones to the newly created triangles, 
		// they'll get theirs when the AssignTrianglesToZones loop reaches them at the end.

	}// end if triangle was in more than one subzone (or no subzone at all)

	return 1 ;
}


// cut a triangle along some axial plane, turning into 2 or 3 triangles.
// If the plane doesn't go through the triangle then nothing will happen.
int Q3Map::SplitTriangle(int nTriangle, int nAxis, float flCutPos)
{

	triangle_t NewTri ;

	// these will stay -1 if no vert is created, else will be index of the new vert
	int nABNum=-1 ;
	int nBCNum=-1 ;
	int nCANum=-1 ;

	Q3BspVertex	VertA = m_pVertex[  m_pTriangle[ nTriangle ].VIndex[0]  ] ;
	Q3BspVertex	VertB = m_pVertex[  m_pTriangle[ nTriangle ].VIndex[1]  ] ;
	Q3BspVertex	VertC = m_pVertex[  m_pTriangle[ nTriangle ].VIndex[2]  ] ;
	Q3BspVertex	VertexAB ;
	Q3BspVertex	VertexBC ;
	Q3BspVertex	VertexCA ;

	float flSpan=0.0f ;
	float flCutSpan=0.0f ;
	float flPercent=0.0f ;

	switch(nAxis)
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case AXIS_X:

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertA is on one side of the cut and VertB is on the other, create VertexAB on the cutline.
			if( 
					((VertA.position[0]<flCutPos-SUBZONE_EPSILON) && (VertB.position[0]>flCutPos+SUBZONE_EPSILON))
					||
					((VertA.position[0]>flCutPos+SUBZONE_EPSILON) && (VertB.position[0]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertA.position[0]<flCutPos)
				{
					flSpan=VertB.position[0]-VertA.position[0] ;
					flCutSpan=flCutPos-VertA.position[0] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertA.position[0]-VertB.position[0] ;
					flCutSpan=flCutPos-VertB.position[0] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertA, &VertB, flPercent, &VertexAB) ;
				nABNum=m_nVertexMax ;
				if(!AddVertex(VertexAB)) 
					return 0 ;
			}// end if need create VertexAB

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertB is on one side of the cut and VertC is on the other, create VertexBC on the cutline.
			if( 
					((VertB.position[0]<flCutPos-SUBZONE_EPSILON) && (VertC.position[0]>flCutPos+SUBZONE_EPSILON))
					||
					((VertB.position[0]>flCutPos+SUBZONE_EPSILON) && (VertC.position[0]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertB.position[0]<flCutPos)
				{
					flSpan=VertC.position[0]-VertB.position[0] ;
					flCutSpan=flCutPos-VertB.position[0] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertB.position[0]-VertC.position[0] ;
					flCutSpan=flCutPos-VertC.position[0] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertB, &VertC, flPercent, &VertexBC) ;
				nBCNum=m_nVertexMax ;
				if(!AddVertex(VertexBC)) 
					return 0 ;

			}// end if need create VertexBC

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertC is on one side of the cut and VertA is on the other, create VertexCA on the cutline.
			if( 
					((VertC.position[0]<flCutPos) && (VertA.position[0]>flCutPos))
					||
					((VertC.position[0]>flCutPos) && (VertA.position[0]<flCutPos))
				)
			{
				// work out the span and percentage
				if(VertC.position[0]<flCutPos)
				{
					flSpan=VertA.position[0]-VertC.position[0] ;
					flCutSpan=flCutPos-VertC.position[0] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertC.position[0]-VertA.position[0] ;
					flCutSpan=flCutPos-VertA.position[0] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertC, &VertA, flPercent, &VertexCA) ;
				nCANum=m_nVertexMax ;
				if(!AddVertex(VertexCA)) 
					return 0 ;

			}// end if need create VertexCA
			break ;

		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case AXIS_Y:

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertA is on one side of the cut and VertB is on the other, create VertexAB on the cutline.
			if( 
					((VertA.position[1]<flCutPos-SUBZONE_EPSILON) && (VertB.position[1]>flCutPos+SUBZONE_EPSILON))
					||
					((VertA.position[1]>flCutPos+SUBZONE_EPSILON) && (VertB.position[1]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertA.position[1]<flCutPos)
				{
					flSpan=VertB.position[1]-VertA.position[1] ;
					flCutSpan=flCutPos-VertA.position[1] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertA.position[1]-VertB.position[1] ;
					flCutSpan=flCutPos-VertB.position[1] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertA, &VertB, flPercent, &VertexAB) ;
				nABNum=m_nVertexMax ;
				if(!AddVertex(VertexAB)) 
					return 0 ;
			}// end if need create VertexAB

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertB is on one side of the cut and VertC is on the other, create VertexBC on the cutline.
			if( 
					((VertB.position[1]<flCutPos-SUBZONE_EPSILON) && (VertC.position[1]>flCutPos+SUBZONE_EPSILON))
					||
					((VertB.position[1]>flCutPos+SUBZONE_EPSILON) && (VertC.position[1]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertB.position[1]<flCutPos)
				{
					flSpan=VertC.position[1]-VertB.position[1] ;
					flCutSpan=flCutPos-VertB.position[1] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertB.position[1]-VertC.position[1] ;
					flCutSpan=flCutPos-VertC.position[1] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertB, &VertC, flPercent, &VertexBC) ;
				nBCNum=m_nVertexMax ;
				if(!AddVertex(VertexBC)) 
					return 0 ;

			}// end if need create VertexBC

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertC is on one side of the cut and VertA is on the other, create VertexCA on the cutline.
			if( 
					((VertC.position[1]<flCutPos) && (VertA.position[1]>flCutPos))
					||
					((VertC.position[1]>flCutPos) && (VertA.position[1]<flCutPos))
				)
			{
				// work out the span and percentage
				if(VertC.position[1]<flCutPos)
				{
					flSpan=VertA.position[1]-VertC.position[1] ;
					flCutSpan=flCutPos-VertC.position[1] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertC.position[1]-VertA.position[1] ;
					flCutSpan=flCutPos-VertA.position[1] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertC, &VertA, flPercent, &VertexCA) ;
				nCANum=m_nVertexMax ;
				if(!AddVertex(VertexCA)) 
					return 0 ;

			}// end if need create VertexCA
			break ;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case AXIS_Z:

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertA is on one side of the cut and VertB is on the other, create VertexAB on the cutline.
			if( 
					((VertA.position[2]<flCutPos-SUBZONE_EPSILON) && (VertB.position[2]>flCutPos+SUBZONE_EPSILON))
					||
					((VertA.position[2]>flCutPos+SUBZONE_EPSILON) && (VertB.position[2]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertA.position[2]<flCutPos)
				{
					flSpan=VertB.position[2]-VertA.position[2] ;
					flCutSpan=flCutPos-VertA.position[2] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertA.position[2]-VertB.position[2] ;
					flCutSpan=flCutPos-VertB.position[2] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertA, &VertB, flPercent, &VertexAB) ;
				nABNum=m_nVertexMax ;
				if(!AddVertex(VertexAB)) 
					return 0 ;
			}// end if need create VertexAB

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertB is on one side of the cut and VertC is on the other, create VertexBC on the cutline.
			if( 
					((VertB.position[2]<flCutPos-SUBZONE_EPSILON) && (VertC.position[2]>flCutPos+SUBZONE_EPSILON))
					||
					((VertB.position[2]>flCutPos+SUBZONE_EPSILON) && (VertC.position[2]<flCutPos-SUBZONE_EPSILON))
				)
			{
				// work out the span and percentage
				if(VertB.position[2]<flCutPos)
				{
					flSpan=VertC.position[2]-VertB.position[2] ;
					flCutSpan=flCutPos-VertB.position[2] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertB.position[2]-VertC.position[2] ;
					flCutSpan=flCutPos-VertC.position[2] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertB, &VertC, flPercent, &VertexBC) ;
				nBCNum=m_nVertexMax ;
				if(!AddVertex(VertexBC)) 
					return 0 ;

			}// end if need create VertexBC

			////////////////////////////////////////////////////////////////////////////////////////////////
			// if VertC is on one side of the cut and VertA is on the other, create VertexCA on the cutline.
			if( 
					((VertC.position[2]<flCutPos) && (VertA.position[2]>flCutPos))
					||
					((VertC.position[2]>flCutPos) && (VertA.position[2]<flCutPos))
				)
			{
				// work out the span and percentage
				if(VertC.position[2]<flCutPos)
				{
					flSpan=VertA.position[2]-VertC.position[2] ;
					flCutSpan=flCutPos-VertC.position[2] ;
					flPercent=1.0f-flCutSpan/flSpan ;
				}
				else
				{
					flSpan=VertC.position[2]-VertA.position[2] ;
					flCutSpan=flCutPos-VertA.position[2] ;
					flPercent=flCutSpan/flSpan ;
				}

				CreateTweenVert(&VertC, &VertA, flPercent, &VertexCA) ;
				nCANum=m_nVertexMax ;
				if(!AddVertex(VertexCA)) 
					return 0 ;

			}// end if need create VertexCA
			break ;


	}

//	int nInitialTrianglePos=m_nTriangleMax ; // debugging

	// default parameters for all new triangles
	NewTri.Texture	=	m_pTriangle[ nTriangle ].Texture ;
	//NewTri.Lightmap	=	m_pTriangle[ nTriangle ].Lightmap ; // bzn doesn't use lightmaps
	NewTri.Lamp			= m_pTriangle[ nTriangle ].Lamp ; 
	NewTri.Group = m_pTriangle[ nTriangle ].Group ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// now we create new triangles depending on the verts we created.
	if((nABNum!=-1) && (nCANum!=-1))
	{
		// add (AB, B, C)
		NewTri.VIndex[0]=nABNum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[1] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[2] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// add (C, CA, AB)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[2] ;
		NewTri.VIndex[1]=nCANum ;
		NewTri.VIndex[2]=nABNum ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (A, AB, CA)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[0] ;
		NewTri.VIndex[1]=nABNum ;
		NewTri.VIndex[2]=nCANum ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}
	else
	if((nABNum!=-1) && (nBCNum!=-1))
	{
		// add (BC, C, A)
		NewTri.VIndex[0]=nBCNum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[2] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[0] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// add (A, AB, BC)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[0] ;
		NewTri.VIndex[1]=nABNum ;
		NewTri.VIndex[2]=nBCNum ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (B, BC, AB)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[1] ;
		NewTri.VIndex[1]=nBCNum ;
		NewTri.VIndex[2]=nABNum ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}
	else
	if((nBCNum!=-1) && (nCANum!=-1))
	{
		// add (CA, A, B)
		NewTri.VIndex[0]=nCANum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[0] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[1] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// add (B, BC, CA)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[1] ;
		NewTri.VIndex[1]=nBCNum ;
		NewTri.VIndex[2]=nCANum ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (C, CA, BC)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[2] ;
		NewTri.VIndex[1]=nCANum ;
		NewTri.VIndex[2]=nBCNum ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}
	else
	if(nABNum!=-1)
	{
		// add (AB, B, C)
		NewTri.VIndex[0]=nABNum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[1] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[2] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (A, AB, C)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[0] ;
		NewTri.VIndex[1]=nABNum ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[2] ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}
	else
	if(nBCNum!=-1)
	{
		// add (BC, C, A)
		NewTri.VIndex[0]=nBCNum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[2] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[0] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (B, BC, A)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[1] ;
		NewTri.VIndex[1]=nBCNum ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[0] ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}
	else
	if(nCANum!=-1)
	{
		// add (CA, A, B)
		NewTri.VIndex[0]=nCANum ;
		NewTri.VIndex[1]=m_pTriangle[ nTriangle ].VIndex[0] ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[1] ;
		if(!AddTriangle(NewTri)) return 0 ;

		// overwrite the original triangle with (C, CA, B)
		NewTri.VIndex[0]=m_pTriangle[ nTriangle ].VIndex[2] ;
		NewTri.VIndex[1]=nCANum ;
		NewTri.VIndex[2]=m_pTriangle[ nTriangle ].VIndex[1] ;
		m_pTriangle[ nTriangle ]=NewTri ;
	}

	return 1 ;
}

void Q3Map::CreateTweenVert(Q3BspVertex* pVertA, Q3BspVertex* pVertB, float flPercent0, Q3BspVertex* pVertexAB)
{
	float flPercent1=1.0f-flPercent0 ;

	pVertexAB->color[0]=(unsigned char)(flPercent0*pVertA->color[0] + flPercent1*pVertB->color[0]) ;
	pVertexAB->color[1]=(unsigned char)(flPercent0*pVertA->color[1] + flPercent1*pVertB->color[1]) ;
	pVertexAB->color[2]=(unsigned char)(flPercent0*pVertA->color[2] + flPercent1*pVertB->color[2]) ;
	pVertexAB->color[3]=(unsigned char)(flPercent0*pVertA->color[3] + flPercent1*pVertB->color[3]) ;

	pVertexAB->position[0]=flPercent0*pVertA->position[0] + flPercent1*pVertB->position[0] ;
	pVertexAB->position[1]=flPercent0*pVertA->position[1] + flPercent1*pVertB->position[1] ;
	pVertexAB->position[2]=flPercent0*pVertA->position[2] + flPercent1*pVertB->position[2] ;
				
	pVertexAB->texcoord[0][0]=flPercent0*pVertA->texcoord[0][0] + flPercent1*pVertB->texcoord[0][0] ;
	pVertexAB->texcoord[0][1]=flPercent0*pVertA->texcoord[0][1] + flPercent1*pVertB->texcoord[0][1] ;
	pVertexAB->texcoord[1][0]=flPercent0*pVertA->texcoord[1][0] + flPercent1*pVertB->texcoord[1][0] ;
	pVertexAB->texcoord[1][1]=flPercent0*pVertA->texcoord[1][1] + flPercent1*pVertB->texcoord[1][1] ;

	pVertexAB->normal[0]=flPercent0*pVertA->normal[0] + flPercent1*pVertB->normal[0] ;
	pVertexAB->normal[1]=flPercent0*pVertA->normal[1] + flPercent1*pVertB->normal[1] ;
	pVertexAB->normal[2]=flPercent0*pVertA->normal[2] + flPercent1*pVertB->normal[2] ;
	// normalize
	float flLen=sqrt(pVertexAB->normal[0]*pVertexAB->normal[0] + pVertexAB->normal[1]*pVertexAB->normal[1] + pVertexAB->normal[2]*pVertexAB->normal[2]) ;
	if(flLen!=0.0f) // don't divide by zero... but normal is messed up.
	{
		pVertexAB->normal[0]/=flLen ;
		pVertexAB->normal[1]/=flLen ;
		pVertexAB->normal[2]/=flLen ;
	}
	else
	{
		// default a messed up normal to point upward
		pVertexAB->normal[0]=0.0f ;
		pVertexAB->normal[1]=1.0f ;
		pVertexAB->normal[2]=0.0f ;
	}
}

// returns the next subzone a point is in after the start subzone, or -1 if there are no more subzones
int Q3Map::GetNextSubZone(float *flPoint, int nStart, int nMax)
{
	while(++nStart<nMax)
		if(PointInSubZone(flPoint, nStart))
			break ;

	if(nStart==nMax)
		return -1 ;
	else
		return nStart ;
}

// returns true if a point is in a subzone.
bool Q3Map::PointInSubZone(float *flPoint, int nSubZone)
{
	if(
				(flPoint[0]>=m_pSubZones[nSubZone].Min[0]-SUBZONE_EPSILON) && (flPoint[0]<=m_pSubZones[nSubZone].Max[0]+SUBZONE_EPSILON)
			&&(flPoint[1]>=m_pSubZones[nSubZone].Min[1]-SUBZONE_EPSILON) && (flPoint[1]<=m_pSubZones[nSubZone].Max[1]+SUBZONE_EPSILON)
			&&(flPoint[2]>=m_pSubZones[nSubZone].Min[2]-SUBZONE_EPSILON) && (flPoint[2]<=m_pSubZones[nSubZone].Max[2]+SUBZONE_EPSILON)
		)
		return true ;

	return false ;
}

// returns true if a point is in a zone.
bool Q3Map::PointInZone(float *flPos, int nZone)
{
	int nMaxSubZone=m_nZone[nZone][INDEX_SUBZONECOUNT] ;

	for(int nSubZoneIndex=0 ; nSubZoneIndex<nMaxSubZone ; nSubZoneIndex++)
		if(PointInSubZone(flPos, m_nZone[nZone][nSubZoneIndex]))
			return true ;
		
	return false ;
}

// returns true if an axis aligned bounding box touches a subzone.
bool Q3Map::AABBTouchesSubZone(float *flPointMin, float *flPointMax, int nSubZone)
{
	// if test AABB overlaps the subzone AABB
	if(
				 (m_pSubZones[nSubZone].Min[0]<flPointMax[0]) && (m_pSubZones[nSubZone].Max[0]>flPointMin[0])
			&& (m_pSubZones[nSubZone].Min[1]<flPointMax[1]) && (m_pSubZones[nSubZone].Max[1]>flPointMin[1])
			&& (m_pSubZones[nSubZone].Min[2]<flPointMax[2]) && (m_pSubZones[nSubZone].Max[2]>flPointMin[2])
		)
		return true ;

	return false ;
}

// returns true if an axis aligned bounding box touches a zone.
bool Q3Map::AABBTouchesZone(float *flPosMin, float *flPosMax, int nZone)
{
	int nMaxSubZone=m_nZone[nZone][INDEX_SUBZONECOUNT] ;

	for(int nSubZoneIndex=0 ; nSubZoneIndex<nMaxSubZone ; nSubZoneIndex++)
		if(AABBTouchesSubZone(flPosMin, flPosMax, m_nZone[nZone][nSubZoneIndex]))
			return true ;
		
	return false ;
}

// take the faces from the BSP and add them to our list of triangles.
int Q3Map::ConvertFacesToTriangles(void)
{
	int nFaceIndex = 0 ;
	
//	int nVertex=0 ;
//	int nVertexMax=0 ;
	int nTriangle=0 ;
	int nTriangleMax=0 ;

//	float flPosX=0.0f ;
//	float flPosY=0.0f ;
//	float flPosZ=0.0f ;
//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;
	int nMeshVert=0 ;
//	int nMeshVertA=0 ;
//	int nMeshVertB=0 ;
//	int nMeshVertC=0 ;

//	Q3BspVertex *vertices = m_pVertices ;
	int *meshverts = m_pMeshVerts ;
	
	QVECTOR junk ;
	junk[0]=0.0f ;
	junk[1]=0.0f ;
	junk[2]=0.0f ;


	nFaceIndex = 0 ;	
	while(nFaceIndex<m_iNumFaces)
	{

		switch (m_BspFaces[nFaceIndex].type)
		{
			case MESH:    
			case POLYGON:	

				nTriangleMax=m_BspFaces[nFaceIndex].n_triangles ;
				nMeshVert=m_BspFaces[nFaceIndex].meshvert ;
				
				for(nTriangle=0 ; nTriangle<nTriangleMax ; nTriangle++)
				{

					// if we are out of memory, grow it.  If we can't grow it, fail
					if(m_nTriangleMax>=m_nTriangleLimit)
						if(!ExpandTriangleMemory())
							return 0 ;

					

					m_pTriangle[ m_nTriangleMax ].Texture=		m_BspFaces[nFaceIndex].texture ;
					//m_pTriangle[ m_nTriangleMax ].Lightmap=		m_BspFaces[nFaceIndex].lm_index ; // bzn doesn't use lightmaps
					m_pTriangle[ m_nTriangleMax ].VIndex[0]=	meshverts[ nMeshVert++ ]+m_BspFaces[nFaceIndex].vertex ;
					m_pTriangle[ m_nTriangleMax ].VIndex[1]=	meshverts[ nMeshVert++ ]+m_BspFaces[nFaceIndex].vertex ;
					m_pTriangle[ m_nTriangleMax ].VIndex[2]=	meshverts[ nMeshVert++ ]+m_BspFaces[nFaceIndex].vertex ;

					m_pTriangle[ m_nTriangleMax ].Lamp=-1 ; // assume it didn't come from a lamp, this will be updated later
					
					m_pTriangle[ m_nTriangleMax ].Group=m_nGroup ; // increment group number.  

					m_nTriangleMax++ ;
				}// end for nTriangle
				m_nGroup++ ; // increment group.  Every face is a new group.
				
			break ;

		}// end switch
		
		nFaceIndex++;
	} // end while	

	return 1 ;

}


// convert the patch info from the BSP into bezier curved triangle meshes and add to our triangle list.
int Q3Map::ConvertPatchesToTriangles(void)
{

//	float flPosX=0.0f ;
//	float flPosY=0.0f ;
//	float flPosZ=0.0f ;
//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;
//	int nMeshVert=0 ;
	int nMeshVertA=0 ;
	int nMeshVertB=0 ;
	int nMeshVertC=0 ;


	int nTriPerRow=0 ;
	int nRow=0 ;

	int nFirstVertex=m_nVertexMax ;
	
	int nVertCount=nFirstVertex ;
//	int nPatchCount=0 ;

	int* pIndexBuffer=NULL ;

	Q3BspVertex NewVert ;
		



	int nCount=0 ;
	int nCountB=0 ;



		
    int indexBufferindex = 0;
	
    int vertexBufferindex = 0;

    for (int faceIndex=0; faceIndex < m_iNumFaces; faceIndex++)
    {		

			nCount++ ;
			if(nCount==1)
			{
				nCountB+=nCount ;
				nCount=0 ;
			}


      if (m_BspFaces[faceIndex].type == PATCH)
      {
        Q3BspPatch *patch = m_BspFaces[faceIndex].patch;

        if (patch != NULL)
        {
					
          for (int bezierIndex=0; bezierIndex < patch->size; bezierIndex++)
          {
						indexBufferindex = 0;
						pIndexBuffer = new int[patch->bezier[bezierIndex].mNumIndex] ;
						if(pIndexBuffer==NULL) return 0 ; // ran out of memory


            for (int index=0; index < patch->bezier[bezierIndex].mNumIndex; index++)
            {	
              pIndexBuffer[indexBufferindex] = patch->bezier[bezierIndex].mIndex[index];
              indexBufferindex++;
            }

            for (int vertex=0; vertex < patch->bezier[bezierIndex].mNumVertex; vertex++)
            {

              BspVertex *bspVertex = &patch->bezier[bezierIndex].mVertex[vertex];

							NewVert.position[0]=bspVertex->mPosition[0] ;
							NewVert.position[1]=bspVertex->mPosition[1] ;
							NewVert.position[2]=bspVertex->mPosition[2] ;
							NewVert.normal[0]=bspVertex->mNormal[0] ;
							NewVert.normal[1]=bspVertex->mNormal[1] ;
							NewVert.normal[2]=bspVertex->mNormal[2] ;
							NewVert.texcoord[0][0]=bspVertex->mTexcoord[0][0] ;
							NewVert.texcoord[0][1]=bspVertex->mTexcoord[0][1] ;

							

							// if we are out of memory, grow it.  If we can't grow it, fail
							if(m_nVertexMax>=m_nVertexLimit)
								if(!ExpandVertexMemory())
								{
									if(pIndexBuffer) DELETE_ARRAY( pIndexBuffer ) ;
									return 0 ;
								}


							if(!AddVertex(NewVert)) 
							{
								if(pIndexBuffer) DELETE_ARRAY( pIndexBuffer ) ;
								return 0 ;
							}

							nVertCount++ ;
              vertexBufferindex++;

            }// end for vertex


						for (int j=0; j < 5; j++)
						{
							nRow=m_BspFaces[faceIndex].patch->bezier[bezierIndex].mRowIndex[j] ;
							nTriPerRow=m_BspFaces[faceIndex].patch->bezier[bezierIndex].mTrianglesPerRow[j] ;

							nMeshVertA=pIndexBuffer[nRow+0]+nFirstVertex ;
							nMeshVertB=pIndexBuffer[nRow+1]+nFirstVertex ;
							
							for(int nVert=2 ; nVert<nTriPerRow ; nVert++)
							{
								// if we are out of memory, grow it.  If we can't grow it, fail
								if(m_nTriangleMax>=m_nTriangleLimit)
									if(!ExpandTriangleMemory())
									{
										if(pIndexBuffer) DELETE_ARRAY( pIndexBuffer ) ;
										return 0 ;
									}

								m_pTriangle[ m_nTriangleMax ].Texture=		m_BspFaces[faceIndex].texture ;
								//m_pTriangle[ m_nTriangleMax ].Lightmap=		m_BspFaces[faceIndex].lm_index ; // bzn doesn't use lightmaps

								
								
								nMeshVertC=pIndexBuffer[nRow+nVert]+nFirstVertex ;


								if(nVert&1)
								{
									m_pTriangle[ m_nTriangleMax ].VIndex[0]=	nMeshVertB ;
									m_pTriangle[ m_nTriangleMax ].VIndex[1]=	nMeshVertA ;
									m_pTriangle[ m_nTriangleMax ].VIndex[2]=	nMeshVertC ;
								}
								else
								{
									m_pTriangle[ m_nTriangleMax ].VIndex[0]=	nMeshVertA ;
									m_pTriangle[ m_nTriangleMax ].VIndex[1]=	nMeshVertB ;
									m_pTriangle[ m_nTriangleMax ].VIndex[2]=	nMeshVertC ;
								}

								m_pTriangle[ m_nTriangleMax ].Lamp=-1 ; // assume it didn't come from a lamp, this will be updated later


								m_pTriangle[ m_nTriangleMax ].Group=m_nGroup ;


								m_nTriangleMax++ ;

								nMeshVertA=nMeshVertB ;
								nMeshVertB=nMeshVertC ;
							}
				

						}
						

						// finished with the index buffer
						if(pIndexBuffer) 
							DELETE_ARRAY( pIndexBuffer ) ; 

						nFirstVertex=nVertCount ;
          }// end for bezier index


					m_nGroup++ ; // increment the group number.  Each patch is treated as a single group.

        }// end if patch not null
      }// end if patch
    }// end for faceIndex

	return 1 ;

}

//''
// some triangles might be designed to be deferred shading shapes inside of Lamp entities.
// If so, change their material to the deferred shading configuration for that lamp entity.
int Q3Map::ConvertTexLampsToLampTriangles(void)
{

	float flCentreX=0.0f ;
	float flCentreY=0.0f ;
	float flCentreZ=0.0f ;

//	float flMinX=0.0f ;
//	float flMinY=0.0f ;
//	float flMinZ=0.0f ;
//	float flMaxX=0.0f ;
//	float flMaxY=0.0f ;
//	float flMaxZ=0.0f ;

//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;

	float flColR=0.0f ;
	float flColG=0.0f ;
	float flColB=0.0f ;

	float flBrightness=0.0f ;

	int nLamp=0 ;
	int nTriangle=0 ;
	int nTexLampListPos=0 ;
	int nLightNode=0 ;
	int nTexture=0 ;
	int nZone=0 ;
//	int nLampZone=0 ;
//	int nMaxLampZone=0 ;
//	int nZoneMatch=0 ;


	for(nTexLampListPos=0 ; nTexLampListPos<m_nTexLampMax ; nTexLampListPos++)
	{
		nTriangle=m_pTexLamp[nTexLampListPos] ;

		nZone=m_pTriangle[nTriangle].Zone ;

		nTexture=m_pTriangle[nTriangle].Texture ;

		if(nTexture==m_nBZN_LightNode0)
			nLightNode=0 ;
		else
		if(nTexture==m_nBZN_LightNode1)
			nLightNode=1 ;
		else
		if(nTexture==m_nBZN_LightNode2)
			nLightNode=2 ;
		else
		if(nTexture==m_nBZN_LightNode3)
			nLightNode=3 ;
		else
			return 0 ; // didn't match any lightnode, something went wrong.


		// scan through all the lamps, finding the ones that touch the same zone as this triangle and that have the same lightnode number
		for(nLamp=0 ; nLamp<m_nLampMax ; nLamp++)
		{
			// skip if the lightnode doesn't match
			if(m_pLamp[nLamp].LightNode!=nLightNode)
				continue ;


			/*
			// lightnode matches, check if lamp touches the same zone (lamps can touch multiple zones so we have to check them all)
			nZoneMatch=0 ;
			nMaxLampZone=m_pLamp[nLamp].Zone[MAX_ZONEPERLIGHT] ;
			for(nLampZone=0 ; nLampZone<nMaxLampZone ; nLampZone++)
			{
				if(m_pLamp[nLamp].Zone[nLampZone]==nZone)
				{
					nZoneMatch=1 ;
					break ;
				}
			}
			
			
			// if the zone didn't match, continue
			if(!nZoneMatch) continue ;

			*/



			// check if all three vertices of this triangle fall within the bounds of this lamp
			if(
						// first vert
						(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[0]>=m_pLamp[nLamp].Min[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[0]<=m_pLamp[nLamp].Max[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[1]>=m_pLamp[nLamp].Min[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[1]<=m_pLamp[nLamp].Max[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[2]>=m_pLamp[nLamp].Min[2])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].position[2]<=m_pLamp[nLamp].Max[2])	
					&&
						// second vert
						(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[0]>=m_pLamp[nLamp].Min[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[0]<=m_pLamp[nLamp].Max[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[1]>=m_pLamp[nLamp].Min[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[1]<=m_pLamp[nLamp].Max[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[2]>=m_pLamp[nLamp].Min[2])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].position[2]<=m_pLamp[nLamp].Max[2])	
					&&
						// third vert
						(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[0]>=m_pLamp[nLamp].Min[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[0]<=m_pLamp[nLamp].Max[0])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[1]>=m_pLamp[nLamp].Min[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[1]<=m_pLamp[nLamp].Max[1])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[2]>=m_pLamp[nLamp].Min[2])
					&&(m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].position[2]<=m_pLamp[nLamp].Max[2])	
	
					)
			{


				m_pTriangle[nTriangle].Texture=m_pLamp[nLamp].Texture ;
				m_pTriangle[nTriangle].Lamp=nLamp ;

				flCentreX =		m_pLamp[nLamp].Position[0] ;
				flCentreY =		m_pLamp[nLamp].Position[1] ;
				flCentreZ =		m_pLamp[nLamp].Position[2] ;
				flColR =			m_pLamp[nLamp].Colour[0]*255.0f ;
				flColG =			m_pLamp[nLamp].Colour[1]*255.0f ;
				flColB =			m_pLamp[nLamp].Colour[2]*255.0f ;
				flBrightness=	m_pLamp[nLamp].Brightness ; 

				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].texcoord[0][0]=flCentreX ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].texcoord[0][1]=flCentreY ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].texcoord[1][0]=flCentreZ ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].texcoord[1][1]=flBrightness ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].color[0]=(unsigned char)flColR ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].color[1]=(unsigned char)flColG ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[0]  ].color[2]=(unsigned char)flColB ;

				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].texcoord[0][0]=flCentreX ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].texcoord[0][1]=flCentreY ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].texcoord[1][0]=flCentreZ ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].texcoord[1][1]=flBrightness ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].color[0]=(unsigned char)flColR ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].color[1]=(unsigned char)flColG ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[1]  ].color[2]=(unsigned char)flColB ;

				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].texcoord[0][0]=flCentreX ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].texcoord[0][1]=flCentreY ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].texcoord[1][0]=flCentreZ ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].texcoord[1][1]=flBrightness ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].color[0]=(unsigned char)flColR ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].color[1]=(unsigned char)flColG ;
				m_pVertex[  m_pTriangle[nTriangle].VIndex[2]  ].color[2]=(unsigned char)flColB ;

				
				m_pTriangle[ m_nTriangleMax ].Group=m_nGroup ;


			}// end triangle is in bounds of lamp


			m_nGroup++ ; // increment group number.  Every texture lamp is a group.


		}// end for nLamp


	}



	return 1 ;
}


// convert lamps into boxes.  Lamps are deferred shading non-shadowing lights, and are rendered as triangles
// The texture coords are actually the light centre point, the spot the shaders calculate as the source of the lighting, 
// and also the brightness
// The triangles created will remember the lamp they came from, via Triangle.Lamp

int Q3Map::ConvertLampsToTriangles(void)
{
	float flCentreX=0.0f ;
	float flCentreY=0.0f ;
	float flCentreZ=0.0f ;

	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;

//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;

	float flColR=0.0f ;
	float flColG=0.0f ;
	float flColB=0.0f ;

	float flBrightness=0.0f ;

	int nLamp=0 ;

	// lower case = min, upper case = max
	Q3BspVertex Vert_xyz ;
	Q3BspVertex Vert_Xyz ;
	Q3BspVertex Vert_xYz ;
	Q3BspVertex Vert_XYz ;
	Q3BspVertex Vert_xyZ ;
	Q3BspVertex Vert_XyZ ;
	Q3BspVertex Vert_xYZ ;
	Q3BspVertex Vert_XYZ ;

	int n_xyz=0 ;
	int n_Xyz=0 ;
	int n_xYz=0 ;
	int n_XYz=0 ;
	int n_xyZ=0 ;
	int n_XyZ=0 ;
	int n_xYZ=0 ;
	int n_XYZ=0 ;

	int nFirstVertex=0 ;

	triangle_t Triangle ;
	ZeroMemory((void*)&Triangle, sizeof(triangle_t)) ;
	
	for(nLamp=0 ; nLamp<m_nLampMax ; nLamp++)
	{
		if(m_pLamp[nLamp].LightNode>-1) continue ; // lamps that are lightnodes don't add their own triangles.  They just exist as information for TexLamps.


		flCentreX =		m_pLamp[nLamp].Position[0] ;
		flCentreY =		m_pLamp[nLamp].Position[1] ;
		flCentreZ =		m_pLamp[nLamp].Position[2] ;

		flMinX =			m_pLamp[nLamp].Min[0] ;
		flMinY =			m_pLamp[nLamp].Min[1] ;
		flMinZ =			m_pLamp[nLamp].Min[2] ;

		flMaxX =			m_pLamp[nLamp].Max[0] ;
		flMaxY =			m_pLamp[nLamp].Max[1] ;
		flMaxZ =			m_pLamp[nLamp].Max[2] ;

		flColR =			m_pLamp[nLamp].Colour[0]*255.0f ;
		flColG =			m_pLamp[nLamp].Colour[1]*255.0f ;
		flColB =			m_pLamp[nLamp].Colour[2]*255.0f ;
		
		flBrightness=	m_pLamp[nLamp].Brightness ; 
		
		//////////////////////////////////////
		// setup our 8 vertices.  Normal isn't that important, I just approximate regardless of actual box shape


		nFirstVertex=m_nVertexMax ; // we need to remember which vertex is which for defining the triangles

		// vertex numbers
		n_xyz=nFirstVertex+0 ;
		n_Xyz=nFirstVertex+1 ;
		n_xYz=nFirstVertex+2 ;
		n_XYz=nFirstVertex+3 ;
		n_xyZ=nFirstVertex+4 ;
		n_XyZ=nFirstVertex+5 ;
		n_xYZ=nFirstVertex+6 ;
		n_XYZ=nFirstVertex+7 ;


		Vert_xyz.position[0]=flMinX ;
		Vert_xyz.position[1]=flMinY ;
		Vert_xyz.position[2]=flMinZ ;
		Vert_xyz.normal[0]=-0.5773502691896 ;
		Vert_xyz.normal[1]=-0.5773502691896 ;
		Vert_xyz.normal[2]=-0.5773502691896 ;
		Vert_xyz.texcoord[0][0]=flCentreX ;
		Vert_xyz.texcoord[0][1]=flCentreY ;
		Vert_xyz.texcoord[1][0]=flCentreZ ;
		Vert_xyz.texcoord[1][1]=flBrightness ;
		Vert_xyz.color[0]=(unsigned char)flColR ;
		Vert_xyz.color[1]=(unsigned char)flColG ;
		Vert_xyz.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_xyz)) return 0 ;
			

		Vert_Xyz.position[0]=flMaxX ;
		Vert_Xyz.position[1]=flMinY ;
		Vert_Xyz.position[2]=flMinZ ;
		Vert_Xyz.normal[0]= 0.5773502691896 ;
		Vert_Xyz.normal[1]=-0.5773502691896 ;
		Vert_Xyz.normal[2]=-0.5773502691896 ;
		Vert_Xyz.texcoord[0][0]=flCentreX ;
		Vert_Xyz.texcoord[0][1]=flCentreY ;
		Vert_Xyz.texcoord[1][0]=flCentreZ ;
		Vert_Xyz.texcoord[1][1]=flBrightness ;
		Vert_Xyz.color[0]=(unsigned char)flColR ;
		Vert_Xyz.color[1]=(unsigned char)flColG ;
		Vert_Xyz.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_Xyz)) return 0 ;

		Vert_xYz.position[0]=flMinX ;
		Vert_xYz.position[1]=flMaxY ;
		Vert_xYz.position[2]=flMinZ ;
		Vert_xYz.normal[0]=-0.5773502691896 ;
		Vert_xYz.normal[1]= 0.5773502691896 ;
		Vert_xYz.normal[2]=-0.5773502691896 ;
		Vert_xYz.texcoord[0][0]=flCentreX ;
		Vert_xYz.texcoord[0][1]=flCentreY ;
		Vert_xYz.texcoord[1][0]=flCentreZ ;
		Vert_xYz.texcoord[1][1]=flBrightness ;
		Vert_xYz.color[0]=(unsigned char)flColR ;
		Vert_xYz.color[1]=(unsigned char)flColG ;
		Vert_xYz.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_xYz)) return 0 ;

		Vert_XYz.position[0]=flMaxX ;
		Vert_XYz.position[1]=flMaxY ;
		Vert_XYz.position[2]=flMinZ ;
		Vert_XYz.normal[0]= 0.5773502691896 ;
		Vert_XYz.normal[1]= 0.5773502691896 ;
		Vert_XYz.normal[2]=-0.5773502691896 ;
		Vert_XYz.texcoord[0][0]=flCentreX ;
		Vert_XYz.texcoord[0][1]=flCentreY ;
		Vert_XYz.texcoord[1][0]=flCentreZ ;
		Vert_XYz.texcoord[1][1]=flBrightness ;
		Vert_XYz.color[0]=(unsigned char)flColR ;
		Vert_XYz.color[1]=(unsigned char)flColG ;
		Vert_XYz.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_XYz)) return 0 ;

		//////////////////////////////////////

		Vert_xyZ.position[0]=flMinX ;
		Vert_xyZ.position[1]=flMinY ;
		Vert_xyZ.position[2]=flMaxZ ;
		Vert_xyZ.normal[0]=-0.5773502691896 ;
		Vert_xyZ.normal[1]=-0.5773502691896 ;
		Vert_xyZ.normal[2]= 0.5773502691896 ;
		Vert_xyZ.texcoord[0][0]=flCentreX ;
		Vert_xyZ.texcoord[0][1]=flCentreY ;
		Vert_xyZ.texcoord[1][0]=flCentreZ ;
		Vert_xyZ.texcoord[1][1]=flBrightness ;
		Vert_xyZ.color[0]=(unsigned char)flColR ;
		Vert_xyZ.color[1]=(unsigned char)flColG ;
		Vert_xyZ.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_xyZ)) return 0 ;

		Vert_XyZ.position[0]=flMaxX ;
		Vert_XyZ.position[1]=flMinY ;
		Vert_XyZ.position[2]=flMaxZ ;
		Vert_XyZ.normal[0]= 0.5773502691896 ;
		Vert_XyZ.normal[1]=-0.5773502691896 ;
		Vert_XyZ.normal[2]= 0.5773502691896 ;
		Vert_XyZ.texcoord[0][0]=flCentreX ;
		Vert_XyZ.texcoord[0][1]=flCentreY ;
		Vert_XyZ.texcoord[1][0]=flCentreZ ;
		Vert_XyZ.texcoord[1][1]=flBrightness ;
		Vert_XyZ.color[0]=(unsigned char)flColR ;
		Vert_XyZ.color[1]=(unsigned char)flColG ;
		Vert_XyZ.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_XyZ)) return 0 ;

		Vert_xYZ.position[0]=flMinX ;
		Vert_xYZ.position[1]=flMaxY ;
		Vert_xYZ.position[2]=flMaxZ ;
		Vert_xYZ.normal[0]=-0.5773502691896 ;
		Vert_xYZ.normal[1]= 0.5773502691896 ;
		Vert_xYZ.normal[2]= 0.5773502691896 ;
		Vert_xYZ.texcoord[0][0]=flCentreX ;
		Vert_xYZ.texcoord[0][1]=flCentreY ;
		Vert_xYZ.texcoord[1][0]=flCentreZ ;
		Vert_xYZ.texcoord[1][1]=flBrightness ;
		Vert_xYZ.color[0]=(unsigned char)flColR ;
		Vert_xYZ.color[1]=(unsigned char)flColG ;
		Vert_xYZ.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_xYZ)) return 0 ;

		Vert_XYZ.position[0]=flMaxX ;
		Vert_XYZ.position[1]=flMaxY ;
		Vert_XYZ.position[2]=flMaxZ ;
		Vert_XYZ.normal[0]= 0.5773502691896 ;
		Vert_XYZ.normal[1]= 0.5773502691896 ;
		Vert_XYZ.normal[2]= 0.5773502691896 ;
		Vert_XYZ.texcoord[0][0]=flCentreX ;
		Vert_XYZ.texcoord[0][1]=flCentreY ;
		Vert_XYZ.texcoord[1][0]=flCentreZ ;
		Vert_XYZ.texcoord[1][1]=flBrightness ;
		Vert_XYZ.color[0]=(unsigned char)flColR ;
		Vert_XYZ.color[1]=(unsigned char)flColG ;
		Vert_XYZ.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_XYZ)) return 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Triangle.Texture=m_pLamp[nLamp].Texture ;
		Triangle.Lamp=nLamp ;
		Triangle.Group=m_nGroup ;

		/////////////////////////////////////

		Triangle.VIndex[2]=n_xyz ;
		Triangle.VIndex[1]=n_xyZ ;
		Triangle.VIndex[0]=n_xYZ ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[2]=n_xYZ ;
		Triangle.VIndex[1]=n_xYz ;
		Triangle.VIndex[0]=n_xyz ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		/////////////////////////////////////

		Triangle.VIndex[0]=n_Xyz ;
		Triangle.VIndex[1]=n_XyZ ;
		Triangle.VIndex[2]=n_XYZ ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_XYZ ;
		Triangle.VIndex[1]=n_XYz ;
		Triangle.VIndex[2]=n_Xyz ;
		if(!AddTriangle(Triangle)) return 0 ;

		/////////////////////////////////////

		Triangle.VIndex[2]=n_xyz ;
		Triangle.VIndex[1]=n_xYz ;
		Triangle.VIndex[0]=n_XYz ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[2]=n_XYz ;
		Triangle.VIndex[1]=n_Xyz ;
		Triangle.VIndex[0]=n_xyz ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		/////////////////////////////////////

		Triangle.VIndex[0]=n_xyZ ;
		Triangle.VIndex[1]=n_xYZ ;
		Triangle.VIndex[2]=n_XYZ ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_XYZ ;
		Triangle.VIndex[1]=n_XyZ ;
		Triangle.VIndex[2]=n_xyZ ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		/////////////////////////////////////

		Triangle.VIndex[0]=n_xyz ;
		Triangle.VIndex[1]=n_xyZ ;
		Triangle.VIndex[2]=n_XyZ ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_XyZ ;
		Triangle.VIndex[1]=n_Xyz ;
		Triangle.VIndex[2]=n_xyz ;
		if(!AddTriangle(Triangle)) return 0 ;

		/////////////////////////////////////

		Triangle.VIndex[2]=n_xYz ;
		Triangle.VIndex[1]=n_xYZ ;
		Triangle.VIndex[0]=n_XYZ ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[2]=n_XYZ ;
		Triangle.VIndex[1]=n_XYz ;
		Triangle.VIndex[0]=n_xYz ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		m_nGroup++ ; // increment group once for every lamp
	}
	


	return 1 ;
}


int Q3Map::ConvertLampsToGlowTriangles(void)
{
	float flCentreX=0.0f ;
	float flCentreY=0.0f ;
	float flCentreZ=0.0f ;

	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;

//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;

	float flColR=0.0f ;
	float flColG=0.0f ;
	float flColB=0.0f ;

	float flBrightness=0.0f ;

	int nLamp=0 ;


	Q3BspVertex Vert_L ;
	Q3BspVertex Vert_R ;
	Q3BspVertex Vert_F ;
	Q3BspVertex Vert_B ;
	Q3BspVertex Vert_U ;
	Q3BspVertex Vert_D ;


	int n_L=0 ;
	int n_R=0 ;
	int n_F=0 ;
	int n_B=0 ;
	int n_U=0 ;
	int n_D=0 ;
	
	int nFirstVertex=0 ;

	float flBaseGlowSize=0.15f ;//0.2f;//0.001f ;
	float flGlowSize=0.0f ;



	triangle_t Triangle ;
	ZeroMemory((void*)&Triangle, sizeof(triangle_t)) ;
	
	for(nLamp=0 ; nLamp<m_nLampMax ; nLamp++)
	{

		flGlowSize=flBaseGlowSize*m_pLamp[nLamp].Brightness ;

		flCentreX =		m_pLamp[nLamp].Position[0] ;
		flCentreY =		m_pLamp[nLamp].Position[1] ;
		flCentreZ =		m_pLamp[nLamp].Position[2] ;

		flMinX =			flCentreX-flGlowSize ;
		flMinY =			flCentreY-flGlowSize ;
		flMinZ =			flCentreZ-flGlowSize ;

		flMaxX =			flCentreX+flGlowSize ;
		flMaxY =			flCentreY+flGlowSize ;
		flMaxZ =			flCentreZ+flGlowSize ;

		flColR =			m_pLamp[nLamp].Colour[0]*255.0f ;
		flColG =			m_pLamp[nLamp].Colour[1]*255.0f ;
		flColB =			m_pLamp[nLamp].Colour[2]*255.0f ;
		
		flBrightness=	m_pLamp[nLamp].Brightness*4.0f ; 
		
		//////////////////////////////////////
		// setup our 8 vertices.  Normal isn't that important, I just approximate regardless of actual box shape


		nFirstVertex=m_nVertexMax ; // we need to remember which vertex is which for defining the triangles

		// vertex numbers
		n_L=nFirstVertex+0 ;
		n_R=nFirstVertex+1 ;
		n_F=nFirstVertex+2 ;
		n_B=nFirstVertex+3 ;
		n_U=nFirstVertex+4 ;
		n_D=nFirstVertex+5 ;



		Vert_L.position[0]=flMinX ;
		Vert_L.position[1]=flCentreY ;
		Vert_L.position[2]=flCentreZ ;
		Vert_L.normal[0]=-1.0 ;
		Vert_L.normal[1]=0.0 ;
		Vert_L.normal[2]=0.0 ;
		Vert_L.texcoord[0][0]=flCentreX ;
		Vert_L.texcoord[0][1]=flCentreY ;
		Vert_L.texcoord[1][0]=flCentreZ ;
		Vert_L.texcoord[1][1]=flBrightness ;
		Vert_L.color[0]=(unsigned char)flColR ;
		Vert_L.color[1]=(unsigned char)flColG ;
		Vert_L.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_L)) return 0 ;

		Vert_R.position[0]=flMaxX ;
		Vert_R.position[1]=flCentreY ;
		Vert_R.position[2]=flCentreZ ;
		Vert_R.normal[0]=1.0 ;
		Vert_R.normal[1]=0.0 ;
		Vert_R.normal[2]=0.0 ;
		Vert_R.texcoord[0][0]=flCentreX ;
		Vert_R.texcoord[0][1]=flCentreY ;
		Vert_R.texcoord[1][0]=flCentreZ ;
		Vert_R.texcoord[1][1]=flBrightness ;
		Vert_R.color[0]=(unsigned char)flColR ;
		Vert_R.color[1]=(unsigned char)flColG ;
		Vert_R.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_R)) return 0 ;

		Vert_F.position[0]=flCentreX ;
		Vert_F.position[1]=flCentreY ;
		Vert_F.position[2]=flMinZ ;
		Vert_F.normal[0]=0.0 ;
		Vert_F.normal[1]=0.0 ;
		Vert_F.normal[2]=-1.0 ;
		Vert_F.texcoord[0][0]=flCentreX ;
		Vert_F.texcoord[0][1]=flCentreY ;
		Vert_F.texcoord[1][0]=flCentreZ ;
		Vert_F.texcoord[1][1]=flBrightness ;
		Vert_F.color[0]=(unsigned char)flColR ;
		Vert_F.color[1]=(unsigned char)flColG ;
		Vert_F.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_F)) return 0 ;
		
		Vert_B.position[0]=flCentreX ;
		Vert_B.position[1]=flCentreY ;
		Vert_B.position[2]=flMaxZ ;
		Vert_B.normal[0]=0.0 ;
		Vert_B.normal[1]=0.0 ;
		Vert_B.normal[2]=1.0 ;
		Vert_B.texcoord[0][0]=flCentreX ;
		Vert_B.texcoord[0][1]=flCentreY ;
		Vert_B.texcoord[1][0]=flCentreZ ;
		Vert_B.texcoord[1][1]=flBrightness ;
		Vert_B.color[0]=(unsigned char)flColR ;
		Vert_B.color[1]=(unsigned char)flColG ;
		Vert_B.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_B)) return 0 ;

		Vert_U.position[0]=flCentreX ;
		Vert_U.position[1]=flMaxY ;
		Vert_U.position[2]=flCentreZ ;
		Vert_U.normal[0]=0.0 ;
		Vert_U.normal[1]=1.0 ;
		Vert_U.normal[2]=0.0 ;
		Vert_U.texcoord[0][0]=flCentreX ;
		Vert_U.texcoord[0][1]=flCentreY ;
		Vert_U.texcoord[1][0]=flCentreZ ;
		Vert_U.texcoord[1][1]=flBrightness ;
		Vert_U.color[0]=(unsigned char)flColR ;
		Vert_U.color[1]=(unsigned char)flColG ;
		Vert_U.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_U)) return 0 ;

		Vert_D.position[0]=flCentreX ;
		Vert_D.position[1]=flMinY ;
		Vert_D.position[2]=flCentreZ ;
		Vert_D.normal[0]=0.0 ;
		Vert_D.normal[1]=-1.0 ;
		Vert_D.normal[2]=0.0 ;
		Vert_D.texcoord[0][0]=flCentreX ;
		Vert_D.texcoord[0][1]=flCentreY ;
		Vert_D.texcoord[1][0]=flCentreZ ;
		Vert_D.texcoord[1][1]=flBrightness ;
		Vert_D.color[0]=(unsigned char)flColR ;
		Vert_D.color[1]=(unsigned char)flColG ;
		Vert_D.color[2]=(unsigned char)flColB ;
		if(!AddVertex(Vert_D)) return 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Triangle.Texture=m_nDefaultTextureIndexGlowLamp ;
		Triangle.Lamp=-1 ;
		Triangle.Group=m_nGroup ;

		/////////////////////////////////////

		Triangle.VIndex[0]=n_U ;
		Triangle.VIndex[1]=n_L ;
		Triangle.VIndex[2]=n_F ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_U ;
		Triangle.VIndex[1]=n_F ;
		Triangle.VIndex[2]=n_R ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		Triangle.VIndex[0]=n_U ;
		Triangle.VIndex[1]=n_R ;
		Triangle.VIndex[2]=n_B ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_U ;
		Triangle.VIndex[1]=n_B ;
		Triangle.VIndex[2]=n_L ;
		if(!AddTriangle(Triangle)) return 0 ;

		//////////////////////////////////////

		Triangle.VIndex[2]=n_D ;
		Triangle.VIndex[1]=n_L ;
		Triangle.VIndex[0]=n_F ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[2]=n_D ;
		Triangle.VIndex[1]=n_F ;
		Triangle.VIndex[0]=n_R ;
		if(!AddTriangle(Triangle)) return 0 ;
	
		Triangle.VIndex[2]=n_D ;
		Triangle.VIndex[1]=n_R ;
		Triangle.VIndex[0]=n_B ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[2]=n_D ;
		Triangle.VIndex[1]=n_B ;
		Triangle.VIndex[0]=n_L ;
		if(!AddTriangle(Triangle)) return 0 ;

		m_nGroup++ ; // increment group once for each glow
		
	}
	


	return 1 ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


int Q3Map::ConvertLightsToGlowTriangles(void)
{
	float flCentreX=0.0f ;
	float flCentreY=0.0f ;
	float flCentreZ=0.0f ;

	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;

//	float	flNormX=0.0f ;
//	float	flNormY=0.0f ;
//	float	flNormZ=0.0f ;
//	float flTexU=0.0f ;
//	float flTexV=0.0f ;

	float flColR=0.0f ;
	float flColG=0.0f ;
	float flColB=0.0f ;

	float flBrightness=0.0f ;

	int nLight=0 ;

	Q3BspVertex Vert_Or ;
	Q3BspVertex Vert_A0 ;
	Q3BspVertex Vert_A1 ;
	Q3BspVertex Vert_B0 ;
	Q3BspVertex Vert_B1 ;


	int n_Or=0 ;
	int n_A0=0 ;
	int n_A1=0 ;
	int n_B0=0 ;
	int n_B1=0 ;
	
	int nFirstVertex=0 ;

	float flBaseGlowSize=0.2f ;//0.001f ;
	float flGlowSize=0.0f ;

//	char chMessage[1024] ;



	triangle_t Triangle ;
	ZeroMemory((void*)&Triangle, sizeof(triangle_t)) ;
	
	for(nLight=0 ; nLight<m_nLightMax ; nLight++)
	//for(nLight=0 ; nLight<1 ; nLight++)
	{
		//if(m_pLight[nLight].LightNode>-1) continue ; // Lights that are lightnodes don't add their own triangles.  They just exist as information for TexLights.

		flGlowSize=flBaseGlowSize*m_pLight[nLight].Brightness ;

		flCentreX =		m_pLight[nLight].Position[0] ;
		flCentreY =		m_pLight[nLight].Position[1] ;
		flCentreZ =		m_pLight[nLight].Position[2] ;

		flMinX =			flCentreX-flGlowSize ;
		flMinY =			flCentreY-flGlowSize ;
		flMinZ =			flCentreZ-flGlowSize ;

		flMaxX =			flCentreX+flGlowSize ;
		flMaxY =			flCentreY+flGlowSize ;
		flMaxZ =			flCentreZ+flGlowSize ;

		flColR =			m_pLight[nLight].Colour[0]*255.0f ;
		flColG =			m_pLight[nLight].Colour[1]*255.0f ;
		flColB =			m_pLight[nLight].Colour[2]*255.0f ;
		
		flBrightness=	m_pLight[nLight].Brightness * 0.75 ;//*2.0 ; 
		
		//////////////////////////////////////
		// setup our 5 vertices.


		nFirstVertex=m_nVertexMax ; // we need to remember which vertex is which for defining the triangles

		// vertex numbers
		n_Or=nFirstVertex+0 ;
		n_A0=nFirstVertex+1 ;
		n_A1=nFirstVertex+2 ;
		n_B0=nFirstVertex+3 ;
		n_B1=nFirstVertex+4 ;
















	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		
	
	float flHALFPI =	1.5707963 ;
	float flTWOPI =		6.2831853 ;
	float flPI =			3.1415926 ;
	float flAngle=(m_pLight[nLight].Angle/360.0f*flTWOPI)/2.0 ;
	float flCutoff=32.0f ;//m_pLight[nLight].Brightness ;
	
	Q3BspVertex color ;

	Q3BspVertex NormalVert ;
	SetVertex(&NormalVert, m_pLight[nLight].Direction[0], m_pLight[nLight].Direction[1], m_pLight[nLight].Direction[2]) ;
	NormalVert=GetNormalised(&NormalVert) ;

	Q3BspVertex normal ;
	SetVertex(&normal, m_pLight[nLight].Direction[0]*flCutoff, m_pLight[nLight].Direction[1]*flCutoff, m_pLight[nLight].Direction[2]*flCutoff) ;

	Q3BspVertex adjust=normal ;
	VertexScale(&adjust, 0.5) ;

	Q3BspVertex start ;		SetVertex(&start, flCentreX, flCentreY, flCentreZ) ;
	Q3BspVertex end ;			SetVertex(&end, flCentreX+normal.position[0], flCentreY+normal.position[1], flCentreZ+normal.position[2]) ;

	Q3BspVertex xaxis ; SetVertex(&xaxis, 1,0,0) ;
	Q3BspVertex xaxisneg ; SetVertex(&xaxisneg, -1,0,0) ;
	Q3BspVertex yaxis ; SetVertex(&yaxis, 0,1,0) ;
	Q3BspVertex Origin ; SetVertex(&Origin, 0,0,0) ;
	Q3BspVertex tangentA ;
	Q3BspVertex tangentB ;
	Q3BspVertex tangentStart ;
	Q3BspVertex tangentEnd ;
	Q3BspVertex tangentEndA0 ;
	Q3BspVertex tangentEndA1 ;
	Q3BspVertex tangentEndB0 ;
	Q3BspVertex tangentEndB1 ;

	Q3BspVertex tangentANorm ;
	Q3BspVertex tangentBNorm ;

	Q3BspVertex raytangent ;

	Q3BspVertex ray ;
	float theta=0.0f ;
	float adjacent=0.0f ;
	SetVertex(&color, 0.66, 0.66, 0.66) ;

	if(flAngle<0.0001) return 0 ;

	if(flAngle>flPI-0.01) // near on 180 degrees
		flAngle=flPI-0.01 ;


	Q3BspVertex backshift ;
	backshift=normal ;
	VertexScale(&backshift, 0.95) ;
	

	if( !VectorsAreEqual(&NormalVert, &xaxis) && !VectorsAreEqual(&NormalVert, &xaxisneg) )
		tangentA=NormalizedCrossProduct(Origin, normal, xaxis) ;
	else
		tangentA=NormalizedCrossProduct(Origin, normal, yaxis) ;
	
	tangentB=NormalizedCrossProduct(Origin, normal, tangentA) ;

	tangentANorm=tangentA ;
	tangentBNorm=tangentB ;


	
	theta=flHALFPI-flAngle ; // angle between adjacent and hypotenuse (the normal is the "opposite" side, and we know there's a right angle)
	adjacent=VertexDistance(&Origin, &normal)/tan(theta) ; 

	//////////////////////////////////////////////////////////////////////
	
	Vert_Or.position[0]=end.position[0]-backshift.position[0] ;
	Vert_Or.position[1]=end.position[1]-backshift.position[1] ;
	Vert_Or.position[2]=end.position[2]-backshift.position[2] ;
	Vert_Or.normal[0]= NormalVert.position[0] ;
	Vert_Or.normal[1]= NormalVert.position[1] ;
	Vert_Or.normal[2]= NormalVert.position[2] ;
	Vert_Or.texcoord[0][0]=flCentreX ;
	Vert_Or.texcoord[0][1]=flCentreY ;
	Vert_Or.texcoord[1][0]=flCentreZ ;
	Vert_Or.texcoord[1][1]=flBrightness ;
	Vert_Or.color[0]=(unsigned char)flColR ;
	Vert_Or.color[1]=(unsigned char)flColG ;
	Vert_Or.color[2]=(unsigned char)flColB ;
	if(!AddVertex(Vert_Or)) return 0 ;




	//////////////////////////////////////////////////////////////////////

	flColR=0.0f ;
	flColG=0.0f ;
	flColB=0.0f ;

	tangentA=GetNormalised(&tangentA) ;
	VertexScale(&tangentA,  adjacent) ; 

	tangentStart=start ;
	tangentEnd=VectorSubtract(&end, &tangentA) ;
	ray=VectorSubtract(&tangentEnd, &tangentStart) ;
	ray=GetNormalised(&ray) ;
	VertexScale(&ray, flCutoff) ;
	tangentStart=start ;
	tangentEndA0=VectorAdd(&start, &ray) ;

	raytangent=VectorSubtract(&end, &tangentEndA0) ;
	raytangent=VectorAdd(&raytangent, &adjust) ;
	raytangent=GetNormalised(&raytangent) ;



	tangentEndA0=VectorSubtract(&tangentEndA0, &backshift) ;
	Vert_A0.position[0]=tangentEndA0.position[0] ;
	Vert_A0.position[1]=tangentEndA0.position[1] ;
	Vert_A0.position[2]=tangentEndA0.position[2] ;
	Vert_A0.normal[0]= -raytangent.position[0] ;
	Vert_A0.normal[1]= -raytangent.position[1] ;
	Vert_A0.normal[2]= -raytangent.position[2] ;
	Vert_A0.texcoord[0][0]=flCentreX ;
	Vert_A0.texcoord[0][1]=flCentreY ;
	Vert_A0.texcoord[1][0]=flCentreZ ;
	Vert_A0.texcoord[1][1]=flBrightness ;
	Vert_A0.color[0]=(unsigned char)flColR ;//abs(Vert_A0.normal[0])*255 ;//0.0f ;
	Vert_A0.color[1]=(unsigned char)flColG ;//abs(Vert_A0.normal[1])*255 ;//0.0f ;
	Vert_A0.color[2]=(unsigned char)flColB ;//abs(Vert_A0.normal[2])*255 ;//0.0f ;
	if(!AddVertex(Vert_A0)) return 0 ;


	tangentStart=start ;
	tangentEnd=VectorAdd(&end, &tangentA) ;
	ray=VectorSubtract(&tangentEnd, &tangentStart) ;
	ray=GetNormalised(&ray) ;
	VertexScale(&ray, flCutoff) ; //ray.getScaledBy(cutoff) ;
	tangentStart=start ;
	tangentEndA1=VectorAdd(&start, &ray) ;

	raytangent=VectorSubtract(&end, &tangentEndA1) ;
	raytangent=VectorAdd(&raytangent, &adjust) ;
	raytangent=GetNormalised(&raytangent) ;

	tangentEndA1=VectorSubtract(&tangentEndA1, &backshift) ;
	Vert_A1.position[0]=tangentEndA1.position[0] ;
	Vert_A1.position[1]=tangentEndA1.position[1] ;
	Vert_A1.position[2]=tangentEndA1.position[2] ;
	Vert_A1.normal[0]= -raytangent.position[0] ;
	Vert_A1.normal[1]= -raytangent.position[1] ;
	Vert_A1.normal[2]= -raytangent.position[2] ;
	Vert_A1.texcoord[0][0]=flCentreX ;
	Vert_A1.texcoord[0][1]=flCentreY ;
	Vert_A1.texcoord[1][0]=flCentreZ ;
	Vert_A1.texcoord[1][1]=flBrightness ;
	Vert_A1.color[0]=(unsigned char)flColR ;//abs(Vert_A1.normal[0])*255 ;//0.0f ;
	Vert_A1.color[1]=(unsigned char)flColG ;//abs(Vert_A1.normal[1])*255 ;//0.0f ;
	Vert_A1.color[2]=(unsigned char)flColB ;//abs(Vert_A1.normal[2])*255 ;//0.0f ;
	if(!AddVertex(Vert_A1)) return 0 ;

	//////////////////////////////////////////////////////////////////////

	tangentB=GetNormalised(&tangentB) ;
	VertexScale(&tangentB, adjacent) ; //tangentB.getScaledBy(adjacent) ;

	tangentStart=start ;
	tangentEnd=VectorSubtract(&end, &tangentB) ;
	ray=VectorSubtract(&tangentEnd, &tangentStart) ;
	ray=GetNormalised(&ray) ;
	VertexScale(&ray, flCutoff) ; //ray.getScaledBy(cutoff) ;
	tangentStart=start ;
	tangentEndB0=VectorAdd(&start, &ray) ;

	raytangent=VectorSubtract(&end, &tangentEndB0) ;
	raytangent=VectorAdd(&raytangent, &adjust) ;
	raytangent=GetNormalised(&raytangent) ;

	tangentEndB0=VectorSubtract(&tangentEndB0, &backshift) ;
	Vert_B0.position[0]=tangentEndB0.position[0] ;
	Vert_B0.position[1]=tangentEndB0.position[1] ;
	Vert_B0.position[2]=tangentEndB0.position[2] ;
	Vert_B0.normal[0]= -raytangent.position[0] ;
	Vert_B0.normal[1]= -raytangent.position[1] ;
	Vert_B0.normal[2]= -raytangent.position[2] ;
	Vert_B0.texcoord[0][0]=flCentreX ;
	Vert_B0.texcoord[0][1]=flCentreY ;
	Vert_B0.texcoord[1][0]=flCentreZ ;
	Vert_B0.texcoord[1][1]=flBrightness ;
	Vert_B0.color[0]=(unsigned char)flColR ;//abs(Vert_B0.normal[0])*255 ;//0.0f ;
	Vert_B0.color[1]=(unsigned char)flColG ;//abs(Vert_B0.normal[1])*255 ;//0.0f ;
	Vert_B0.color[2]=(unsigned char)flColB ;//abs(Vert_B0.normal[2])*255 ;//0.0f ;
	if(!AddVertex(Vert_B0)) return 0 ;

	tangentStart=start ;
	tangentEnd=VectorAdd(&end, &tangentB) ;
	ray=VectorSubtract(&tangentEnd, &tangentStart) ;
	ray=GetNormalised(&ray) ;
	VertexScale(&ray, flCutoff) ; //ray.getScaledBy(cutoff) ;
	tangentStart=start ;
	tangentEndB1=VectorAdd(&start, &ray) ;

	raytangent=VectorSubtract(&end, &tangentEndB1) ;
	raytangent=VectorAdd(&raytangent, &adjust) ;
	raytangent=GetNormalised(&raytangent) ;

	tangentEndB1=VectorSubtract(&tangentEndB1, &backshift) ;
	Vert_B1.position[0]=tangentEndB1.position[0] ;
	Vert_B1.position[1]=tangentEndB1.position[1] ;
	Vert_B1.position[2]=tangentEndB1.position[2] ;
	Vert_B1.normal[0]= -raytangent.position[0] ;
	Vert_B1.normal[1]= -raytangent.position[1] ;
	Vert_B1.normal[2]= -raytangent.position[2] ;
	Vert_B1.texcoord[0][0]=flCentreX ;
	Vert_B1.texcoord[0][1]=flCentreY ;
	Vert_B1.texcoord[1][0]=flCentreZ ;
	Vert_B1.texcoord[1][1]=flBrightness ;
	Vert_B1.color[0]=(unsigned char)flColR ;//abs(Vert_B1.normal[0])*255 ;//0.0f ;
	Vert_B1.color[1]=(unsigned char)flColG ;//abs(Vert_B1.normal[1])*255 ;//0.0f ;
	Vert_B1.color[2]=(unsigned char)flColB ;//abs(Vert_B1.normal[2])*255 ;//0.0f ;
	if(!AddVertex(Vert_B1)) return 0 ;

	/////////////////////////////////////////////////////////////////////




	// the four verts are position correctly to make the large end of a cone (or rather, pyramid) that would be consistent
	// with the angle and their distance from the origin.  However we cheat and move them back on top of the origin
	// so that map lights come out looking better.
	
	/*
	SetVertex(&normal, m_pLight[nLight].Direction[0]*flCutoff, m_pLight[nLight].Direction[1]*flCutoff, m_pLight[nLight].Direction[2]*flCutoff) ;

	tangentEndA0=VectorSubtract(&tangentEndA0, &normal) ;
	Vert_A0.position[0]=tangentEndA0.position[0] ;
	Vert_A0.position[1]=tangentEndA0.position[1] ;
	Vert_A0.position[2]=tangentEndA0.position[2] ;

	tangentEndA1=VectorSubtract(&tangentEndA1, &normal) ;
	Vert_A1.position[0]=tangentEndA1.position[0] ;
	Vert_A1.position[1]=tangentEndA1.position[1] ;
	Vert_A1.position[2]=tangentEndA1.position[2] ;

	tangentEndB0=VectorSubtract(&tangentEndB0, &normal) ;
	Vert_A0.position[0]=tangentEndB0.position[0] ;
	Vert_A0.position[1]=tangentEndB0.position[1] ;
	Vert_A0.position[2]=tangentEndB0.position[2] ;

	tangentEndB1=VectorSubtract(&tangentEndB1, &normal) ;
	Vert_A1.position[0]=tangentEndB1.position[0] ;
	Vert_A1.position[1]=tangentEndB1.position[1] ;
	Vert_A1.position[2]=tangentEndB1.position[2] ;
	*/


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Triangle.Texture=m_nDefaultTextureIndexGlowLight ;
		Triangle.Lamp=-1 ;
		Triangle.Group=m_nGroup ;

		/////////////////////////////////////

		Triangle.VIndex[0]=n_Or ;
		Triangle.VIndex[2]=n_A0 ;
		Triangle.VIndex[1]=n_B1 ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_Or ;
		Triangle.VIndex[2]=n_B1 ;
		Triangle.VIndex[1]=n_A1 ;
		if(!AddTriangle(Triangle)) return 0 ;

		Triangle.VIndex[0]=n_Or ;
		Triangle.VIndex[2]=n_A1 ;
		Triangle.VIndex[1]=n_B0 ;
		if(!AddTriangle(Triangle)) return 0 ;
		
		Triangle.VIndex[0]=n_Or ;
		Triangle.VIndex[2]=n_B0 ;
		Triangle.VIndex[1]=n_A0 ;
		if(!AddTriangle(Triangle)) return 0 ;

		m_nGroup++ ; // increment group once for each glow
		
	}
	
	return 1 ;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////
// some vertex manipulation functions used for setting up the light disk.
// They only work on the position, so other parts of the struct need to be setup seperately


void Q3Map::SetVertex(Q3BspVertex *pVert, float flXPos, float flYPos, float flZPos)
{
	pVert->position[0]=flXPos ;
	pVert->position[1]=flYPos ;
	pVert->position[2]=flZPos ;
}

// this doesn't test exact equivalence, but rather within the range of an epsilon VERYSMALL
bool Q3Map::VectorsAreEqual(Q3BspVertex* pVecA, Q3BspVertex* pVecB)
{
	if( fabs(pVecA->position[0] - pVecB->position[0])>VERYSMALL ) return false ;
	if( fabs(pVecA->position[1] - pVecB->position[1])>VERYSMALL ) return false ;
	if( fabs(pVecA->position[2] - pVecB->position[2])>VERYSMALL ) return false ;

	return true ;
}

// VertA is the origin of VertB and VertC
Q3BspVertex Q3Map::NormalizedCrossProduct(Q3BspVertex VertA, Q3BspVertex VertB, Q3BspVertex VertC)
{


	Q3BspVertex Cross ;

	// edge vectors
	float flVecXA_CP = VertA.position[0] - VertB.position[0] ;
	float flVecYA_CP = VertA.position[1] - VertB.position[1] ;
	float flVecZA_CP = VertA.position[2] - VertB.position[2] ;
	float flVecXB_CP = VertA.position[0] - VertC.position[0] ;
	float flVecYB_CP = VertA.position[1] - VertC.position[1] ;
	float flVecZB_CP = VertA.position[2] - VertC.position[2] ;

	// cross product
	float flCpx_CP = (flVecZA_CP * flVecYB_CP) - (flVecYA_CP * flVecZB_CP);
	float flCpy_CP = (flVecXA_CP * flVecZB_CP) - (flVecZA_CP * flVecXB_CP);
	float flCpz_CP = (flVecYA_CP * flVecXB_CP) - (flVecXA_CP * flVecYB_CP);

	// Normalize 
	float flR_CP = sqrt(flCpx_CP * flCpx_CP + flCpy_CP * flCpy_CP + flCpz_CP * flCpz_CP);

	Cross.position[0] = flCpx_CP / flR_CP;
	Cross.position[1] = flCpy_CP / flR_CP;
	Cross.position[2] = flCpz_CP / flR_CP;

	return Cross ;
}

float Q3Map::VertexDistance(Q3BspVertex* VertA, Q3BspVertex* VertB)
{
	float flXDis=VertA->position[0]-VertB->position[0] ;
	float flYDis=VertA->position[1]-VertB->position[1] ;
	float flZDis=VertA->position[2]-VertB->position[2] ;

	return sqrt(flXDis*flXDis+flYDis*flYDis+flZDis*flZDis) ;
}

void Q3Map::VertexScale(Q3BspVertex* pVert, float flScale)
{
	pVert->position[0]*=flScale ;
	pVert->position[1]*=flScale ;
	pVert->position[2]*=flScale ;
}

Q3BspVertex Q3Map::GetNormalised(Q3BspVertex* pVector)
{
	float flLength=sqrt((pVector->position[0]*pVector->position[0])+(pVector->position[1]*pVector->position[1])+(pVector->position[2]*pVector->position[2])) ;

	Q3BspVertex Vector=*pVector ;

	Vector.position[0]/=flLength ;
	Vector.position[1]/=flLength ;
	Vector.position[2]/=flLength ;

	return Vector ;
}

Q3BspVertex Q3Map::VectorAdd(Q3BspVertex* pVecA, Q3BspVertex* pVecB)
{
	Q3BspVertex Vector ;

	Vector.position[0]=pVecA->position[0] + pVecB->position[0] ;
	Vector.position[1]=pVecA->position[1] + pVecB->position[1] ;
	Vector.position[2]=pVecA->position[2] + pVecB->position[2] ;

	return Vector ;
}

Q3BspVertex Q3Map::VectorSubtract(Q3BspVertex* pVecA, Q3BspVertex* pVecB)
{
	Q3BspVertex Vector ;

	Vector.position[0]=pVecA->position[0] - pVecB->position[0] ;
	Vector.position[1]=pVecA->position[1] - pVecB->position[1] ;
	Vector.position[2]=pVecA->position[2] - pVecB->position[2] ;

	return Vector ;
}

Q3BspVertex Q3Map::VectorMultiply(Q3BspVertex* pVecA, Q3BspVertex* pVecB)
{
	Q3BspVertex Vector ;

	Vector.position[0]=pVecA->position[0] * pVecB->position[0] ;
	Vector.position[1]=pVecA->position[1] * pVecB->position[1] ;
	Vector.position[2]=pVecA->position[2] * pVecB->position[2] ;

	return Vector ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void Q3Map::GetTexLampTextureNumbers()
{
	m_nBZN_LightNode0=-1 ;
	m_nBZN_LightNode1=-1 ;
	m_nBZN_LightNode2=-1 ;
	m_nBZN_LightNode3=-1 ;

	int nTex=0;
	char chTexName[1024] ;
	int nLen=0 ;
	int nPos=0 ;

	int nMatch=0 ;

	strcpy(chTexName, "textures/common/bzn_lightnode") ;
	nLen=strlen(chTexName) ;

	for(nTex=0 ; nTex<m_nTextureMax ; nTex++)
	{
		nMatch=1 ;
		for(nPos=nLen-1 ; nPos>=0 ; nPos--) // faster to check backwards since lots of textures will start with "textures/common/"
			if(m_pTexture[nTex].name[nPos]!=chTexName[nPos])
			{
				nMatch=0 ;
				break ;
			}

		if(nMatch)
		{
			// what is the last character?
			if(m_pTexture[nTex].name[nLen]=='0')
				m_nBZN_LightNode0=nTex ;
			else
			if(m_pTexture[nTex].name[nLen]=='1')
				m_nBZN_LightNode1=nTex ;
			else
			if(m_pTexture[nTex].name[nLen]=='2')
				m_nBZN_LightNode2=nTex ;
			else
			if(m_pTexture[nTex].name[nLen]=='3')
				m_nBZN_LightNode3=nTex ;
			
		}// end if match


	}// end for nTex

}

int Q3Map::SortTrianglesIntoGroups(void)
{


	int nNewSize=0 ; // we will drop non-subzoned (-1 zone) triangles here, so new number of triangles may be less than old number.

	ULONGLONG *pFaceOrder = new ULONGLONG[m_nTriangleMax] ;
	if(pFaceOrder==NULL) return 0 ; // out of memory.

	// temporary copy of m_pTriangles to make sorting easier
	triangle_t *TempTriangle = new triangle_t[m_nTriangleMax];
	if(TempTriangle==NULL) { DELETE_ARRAY( pFaceOrder ) ; return 0 ; } // out of memory
	memcpy((void*)TempTriangle, (void*)m_pTriangle, m_nTriangleMax*sizeof(triangle_t)) ;

	// create the initial face "value" by setting the most significant bits to it's criteria number
	for (int i=0; i < m_nTriangleMax; i++)
		if(m_pTriangle[i].Zone!=-1) // drop unsubzoned triangles
			pFaceOrder[nNewSize++] = ( ((ULONGLONG)m_pTriangle[i].Zone)<<GROUPSORT_SUBZONE_SHIFT ) + ( ((ULONGLONG)m_pTriangle[i].Texture)<<GROUPSORT_GROUP_SHIFT ) + i ;

	// sort the ULONGLONGs, this will order things according to our criteria
	// sorting is based firstly on subzone, secondly on texture.  
	// So all triangles in a subzone will be grouped together, 
	// and within that group all triangles with the same material will be grouped together.  

	qsort( (void *)pFaceOrder, nNewSize, sizeof(ULONGLONG), &Q3Map::compareGroups);

	// copy the sorted faces back to the original
	for (int i=0; i < nNewSize; i++)
		m_pTriangle[i]=TempTriangle[ (pFaceOrder[i]&FACESORT_FACE_MASK) ] ;

	// update m_nTriangleMax to show that there may now be less triangles
	m_nTriangleMax=nNewSize ;

	// clean up the memory we used
	DELETE_ARRAY( pFaceOrder ) ;
	DELETE_ARRAY( TempTriangle ) ;


	// now that the triangles are sorting according to zone and group, 
	// re-assign the group numbers per zone, from 0 to FACESORT_GROUP_LIMIT-1
	// All non-trans triangles will be group 0, trans triangles will be 1 to FACESORT_GROUP_LIMIT-1

	int nTri=0 ;
	int nZone=-1 ;
	int nGroup=-1 ;
	int nNewGroup=0 ;

	//char chMessage[1024] ;

	for(nTri=0 ; nTri<m_nTriangleMax ; nTri++)
	{
		// reset the newgroup if we've got a new zone
		if(m_pTriangle[nTri].Zone!=nZone) 
		{
			nZone=m_pTriangle[nTri].Zone ;
			nGroup=-1 ;
			nNewGroup=0 ;
		}	

		// if we have a new group, increment the newgroup number, fail if we have too many
		if(m_pTriangle[nTri].Group!=nGroup) 
		{
			// if this is a trans triangle, inc the newgroup
			if(m_pTransTexture[ m_pTriangle[nTri].Texture ])
			{
				nNewGroup++ ; // will always be at least 1
				if(nNewGroup>=FACESORT_GROUP_LIMIT) return 0 ; // too many groups in a zone.
			}

			nGroup=m_pTriangle[nTri].Group ;
		}

		if(m_pTransTexture[ m_pTriangle[nTri].Texture ]==0)
			m_pTriangle[nTri].Group=0 ;
		else
			m_pTriangle[nTri].Group=nNewGroup ;



	}// end for tri

	return 1 ;
}


// static function for sorting groups, required by qsort
int Q3Map::compareGroups( const void *arg1, const void *arg2 )
{
	ULONGLONG FaceA= *(ULONGLONG*)arg1 ;
	ULONGLONG FaceB= *(ULONGLONG*)arg2 ;

	if(FaceA < FaceB)
		return -1 ;
	else
		if(FaceA > FaceB)
			return 1 ;
		
	return 0 ;
}



// sort faces so that we can batch effectively when constructing our manualobjects
// Currently grouped according to zone and texture.
// Triangles not in any subzone will be dropped at this stage.
int Q3Map::SortTrianglesIntoBatches(void)
{


	int nNewSize=0 ; // we will drop non-subzoned (-1 zone) triangles here, so new number of triangles may be less than old number.

	ULONGLONG *pFaceOrder = new ULONGLONG[m_nTriangleMax] ;
	if(pFaceOrder==NULL) return 0 ; // out of memory.

	// temporary copy of m_pTriangles to make sorting easier
	triangle_t *TempTriangle = new triangle_t[m_nTriangleMax];
	if(TempTriangle==NULL) { DELETE_ARRAY( pFaceOrder ) ; return 0 ; } // out of memory
	memcpy((void*)TempTriangle, (void*)m_pTriangle, m_nTriangleMax*sizeof(triangle_t)) ;

	// create the initial face "value" by setting the most significant bits to it's criteria number
	for (int i=0; i < m_nTriangleMax; i++)
		if(m_pTriangle[i].Zone!=-1) // drop unsubzoned triangles
			pFaceOrder[nNewSize++] = ( ((ULONGLONG)m_pTriangle[i].Zone)<<FACESORT_SUBZONE_SHIFT ) + ( ((ULONGLONG)m_pTriangle[i].Texture)<<FACESORT_TEXTURE_SHIFT ) + ( ((ULONGLONG)m_pTriangle[i].Group)<<FACESORT_GROUP_SHIFT ) + i ;

	// sort the ULONGLONGs, this will order things according to our criteria
	// sorting is based firstly on subzone, secondly on texture.  
	// So all triangles in a subzone will be grouped together, 
	// and within that group all triangles with the same material will be grouped together.  

	qsort( (void *)pFaceOrder, nNewSize, sizeof(ULONGLONG), &Q3Map::compareTriangles);

	// copy the sorted faces back to the original
	for (int i=0; i < nNewSize; i++)
		m_pTriangle[i]=TempTriangle[ (pFaceOrder[i]&FACESORT_FACE_MASK) ] ;

	// update m_nTriangleMax to show that there may now be less triangles
	m_nTriangleMax=nNewSize ;

	// clean up the memory we used
	DELETE_ARRAY( pFaceOrder ) ;
	DELETE_ARRAY( TempTriangle ) ;

	return 1 ;
}

// static function for sorting triangles, required by qsort
int Q3Map::compareTriangles( const void *arg1, const void *arg2 )
{
	ULONGLONG FaceA= *(ULONGLONG*)arg1 ;
	ULONGLONG FaceB= *(ULONGLONG*)arg2 ;

	if(FaceA < FaceB)
		return -1 ;
	else
		if(FaceA > FaceB)
			return 1 ;
		
	return 0 ;
}




// note which texture numbers correspond to textures that have transparency.
// needed for when we work out transparency groups and related stuff
int Q3Map::SetupTransTextures(void)
{
	int nMaterial=0 ;
	int nPos=0 ;
	char chMaterial[1024] ;

	// create the memory for the transtextures
	m_pTransTexture = new int[m_nTextureMax] ;
	if(m_pTransTexture==NULL) return 0 ; // out of memory.

	for(nMaterial=0 ; nMaterial<m_nTextureMax ; nMaterial++)
	{
		// copy the material name.
		// q3 texture names are a max of 64 characters ( Q3NAMESIZE ) and may not be null terminated.  They have no extension either.
		nPos=-1 ;
		while((++nPos<Q3NAMESIZE) && (m_pTexture[nMaterial].name[nPos]!=' ') && (m_pTexture[nMaterial].name[nPos]!='\0'))
			chMaterial[nPos]=m_pTexture[nMaterial].name[nPos] ;

		// make sure name is null terminated
		chMaterial[nPos]='\0' ;


		// is the texture a type we need to flag as transparent?
		if(strstr(chMaterial, "GEL")!=NULL)
			m_pTransTexture[nMaterial]=1 ;
		else
			m_pTransTexture[nMaterial]=0 ;

	}// end for nMaterial

	return 1 ;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Q3Map::AssignPortalsToZones(void)
{
	int nZone=0 ;
	float flMinX=0.0f ;
	float flMaxX=0.0f ;
	float flMinY=0.0f ;
	float flMaxY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxZ=0.0f ;
	int nMaxSubZone=m_iNumSubZones ;
	int nSubZone=0 ;
	int nMaxPortal=m_iNumPortals ;
	int nPortal=0 ;
	int nIndex=0 ;
	int nPos=0 ;



	// clear the portal settings.
	for(nZone=0 ; nZone<m_nMaxZone ; nZone++)
		m_nZoneTouchesPortal[nZone][INDEX_PORTALCOUNT]=0 ;

	for(nPortal=0 ; nPortal<nMaxPortal ; nPortal++)
		m_nPortalTouchesZone[nPortal][INDEX_PORTALZONECOUNT]=0 ;



	// now test each subzone against each portal to see what zones each portal contains.
	for(nSubZone=0 ; nSubZone<nMaxSubZone ; nSubZone++)
	{
		nZone=m_pSubZones[nSubZone].Zone ;
		flMinX=m_pSubZones[nSubZone].Min[0] ;
		flMaxX=m_pSubZones[nSubZone].Max[0] ;
		flMinY=m_pSubZones[nSubZone].Min[1] ;
		flMaxY=m_pSubZones[nSubZone].Max[1] ;
		flMinZ=m_pSubZones[nSubZone].Min[2] ;
		flMaxZ=m_pSubZones[nSubZone].Max[2] ;

		// test all the portals to see if any overlap this subzone
		for(nPortal=0 ; nPortal<nMaxPortal ; nPortal++)
		{

			// if AABB overlap
			if(
						 (flMinX<m_pPortals[nPortal].Max[0]) && (flMaxX>m_pPortals[nPortal].Min[0])
					&& (flMinY<m_pPortals[nPortal].Max[1]) && (flMaxY>m_pPortals[nPortal].Min[1])
					&& (flMinZ<m_pPortals[nPortal].Max[2]) && (flMaxZ>m_pPortals[nPortal].Min[2])
				)
			{

				// add this portal to the zone's portal list
				nIndex=m_nZoneTouchesPortal[nZone][INDEX_PORTALCOUNT] ;
				if(nIndex<MAX_PORTALPERZONE) // only add if we aren't already maxed out.
				{
					// we need to check this portal isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nZoneTouchesPortal[nZone][nPos]!=nPortal)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nPortal in the list
					{
						m_nZoneTouchesPortal[nZone][nIndex]=nPortal ;
						m_nZoneTouchesPortal[nZone][INDEX_PORTALCOUNT]++ ;
					}
				}

				// add this zone to the portal's list
				nIndex=m_nPortalTouchesZone[nPortal][INDEX_PORTALZONECOUNT] ;
				if(nIndex<MAX_ZONEPERPORTAL)
				{
					// we need to check this zone isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nPortalTouchesZone[nPortal][nPos]!=nZone)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nZone in the list
					{
						m_nPortalTouchesZone[nPortal][nIndex]=nZone ;
						m_nPortalTouchesZone[nPortal][INDEX_PORTALZONECOUNT]++ ;
					}
				}

			}// end if portal overlaps subzone

		}// end for portal

	}// end for subzone

	return 1 ;
}

int Q3Map::AssignLightsToZones(void)
{
	int nZone=0 ;
	float flMinX=0.0f ;
	float flMaxX=0.0f ;
	float flMinY=0.0f ;
	float flMaxY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxZ=0.0f ;
	int nMaxSubZone=m_iNumSubZones ;
	int nSubZone=0 ;
	int nMaxLight=m_nLightMax ;
	int nLight=0 ;
	int nIndex=0 ;
	int nPos=0 ;



	// clear the light settings.
	for(nZone=0 ; nZone<m_nMaxZone ; nZone++)
		m_nZoneContainsLightCentre[nZone][INDEX_LIGHTCOUNT]=m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT]=0 ;

	for(nLight=0 ; nLight<nMaxLight ; nLight++)
		m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT]=0 ;
	



	// now test each subzone against each light what contains/touches what
	for(nSubZone=0 ; nSubZone<nMaxSubZone ; nSubZone++)
	{
		nZone=m_pSubZones[nSubZone].Zone ;
		flMinX=m_pSubZones[nSubZone].Min[0] ;
		flMaxX=m_pSubZones[nSubZone].Max[0] ;
		flMinY=m_pSubZones[nSubZone].Min[1] ;
		flMaxY=m_pSubZones[nSubZone].Max[1] ;
		flMinZ=m_pSubZones[nSubZone].Min[2] ;
		flMaxZ=m_pSubZones[nSubZone].Max[2] ;

		// test all the lights to see if any have centers inside this subzone
		for(nLight=0 ; nLight<nMaxLight ; nLight++)
		{

			/////////////////////////////////////////////////////////////////////////////////////

			// if light center is in this subzone
			if(
						 (m_pLight[nLight].Position[0]>=flMinX) && (m_pLight[nLight].Position[0]<=flMaxX)
					&& (m_pLight[nLight].Position[1]>=flMinY) && (m_pLight[nLight].Position[1]<=flMaxY)
					&& (m_pLight[nLight].Position[2]>=flMinZ) && (m_pLight[nLight].Position[2]<=flMaxZ)
				)
			{

				// add this light to the zone's light list
				nIndex=m_nZoneContainsLightCentre[nZone][INDEX_LIGHTCOUNT] ;
				if(nIndex<MAX_LIGHTPERZONE) // only add if we aren't already maxed out.
				{
					// we need to check this light isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nZoneContainsLightCentre[nZone][nPos]!=nLight)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nLight in the list
					{
						m_nZoneContainsLightCentre[nZone][nIndex]=nLight ;
						m_nZoneContainsLightCentre[nZone][INDEX_LIGHTCOUNT]++ ;

						// assign this zone as the light's centre.  We only allow one zone to be the light's centre zone,
						// so this will get overwritten if the light is on the border.
						m_pLight[nLight].CentreZone=nZone ;
					}
				}

			}// end if light centre contained by subzone

			/////////////////////////////////////////////////////////////////////////////////////

			// if light touches subzone (we store it in ZoneTouchesSubLight for now, will overwrite later)
			// if light AABB overlaps the zone AABB
			if(
						 (flMinX<m_pLight[nLight].Max[0]) && (flMaxX>m_pLight[nLight].Min[0])
					&& (flMinY<m_pLight[nLight].Max[1]) && (flMaxY>m_pLight[nLight].Min[1])
					&& (flMinZ<m_pLight[nLight].Max[2]) && (flMaxZ>m_pLight[nLight].Min[2])
				)
			{
				// add this light to the zone's light list
				nIndex=m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT] ;
				if(nIndex<MAX_LIGHTPERZONE) // only add if we aren't already maxed out.
				{
					// we need to check this light isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nZoneTouchesSubLight[nZone][nPos]!=nLight)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nPortal in the list
					{
						m_nZoneTouchesSubLight[nZone][nIndex]=nLight ;
						m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT]++ ;
					}
				}

				// add this zone to the light's list
				nIndex=m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT] ;
				if(nIndex<MAX_ZONEPERLIGHT)
				{
					// we need to check this zone isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nLightTouchesZone[nLight][nPos]!=nZone)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nZone in the list
					{
						m_nLightTouchesZone[nLight][nIndex]=nZone ;
						m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT]++ ;
					}
				}

			}// end if light touches contained by subzone

			/////////////////////////////////////////////////////////////////////////////////////




		}// end for light

	}// end for subzone


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// work out the sublights.  These are cut up boxes made from the light boxes, one per zone.
	// Most lights will just become a single sublight, but multizone lights will be cut up into
	// multiple sublights.  These are used for calculating light visibility.
	//

	m_nSubLightMax=0 ;
	int nMaxZone=0 ;
	int nZoneIndex=0 ;
	int nSubLightCentre=0 ;
	sublight_t TempSubLight ;
	for(nLight=0 ; nLight<nMaxLight ; nLight++)
	{

		m_pLight[nLight].ZoneCount=m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT] ; // in the light, note how many zones it touches

		if((m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT]>1) && (m_nMaxMultiZoneLight<MAX_LIGHT))// this is a multizone light
			m_nMultiZoneLight[m_nMaxMultiZoneLight++]=nLight ;


		m_pLight[nLight].SubLightStart=m_nSubLightMax ; // where this light's cut up sublights start in m_SubLight


		// break the light up into it's different sublights, cut by zones.
		nMaxZone=m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT] ;
		for(nZoneIndex=0 ; nZoneIndex<nMaxZone ; nZoneIndex++)
		{
			nZone=m_nLightTouchesZone[nLight][nZoneIndex] ;

			flMinX= m_pLight[nLight].Min[0]<m_ZoneBoundary[nZone].Min[0] ? m_ZoneBoundary[nZone].Min[0] : m_pLight[nLight].Min[0] ; 
			flMinY= m_pLight[nLight].Min[1]<m_ZoneBoundary[nZone].Min[1] ? m_ZoneBoundary[nZone].Min[1] : m_pLight[nLight].Min[1] ; 
			flMinZ= m_pLight[nLight].Min[2]<m_ZoneBoundary[nZone].Min[2] ? m_ZoneBoundary[nZone].Min[2] : m_pLight[nLight].Min[2] ;

			flMaxX= m_pLight[nLight].Max[0]>m_ZoneBoundary[nZone].Max[0] ? m_ZoneBoundary[nZone].Max[0] : m_pLight[nLight].Max[0] ; 
			flMaxY= m_pLight[nLight].Max[1]>m_ZoneBoundary[nZone].Max[1] ? m_ZoneBoundary[nZone].Max[1] : m_pLight[nLight].Max[1] ; 
			flMaxZ= m_pLight[nLight].Max[2]>m_ZoneBoundary[nZone].Max[2] ? m_ZoneBoundary[nZone].Max[2] : m_pLight[nLight].Max[2] ; 

			// add the cut down light as a sublight
			m_SubLight[m_nSubLightMax].Light=nLight ;
			m_SubLight[m_nSubLightMax].Zone=nZone ;
			m_SubLight[m_nSubLightMax].Min[0]=flMinX ;
			m_SubLight[m_nSubLightMax].Min[1]=flMinY ;
			m_SubLight[m_nSubLightMax].Min[2]=flMinZ ;
			m_SubLight[m_nSubLightMax].Max[0]=flMaxX ;
			m_SubLight[m_nSubLightMax].Max[1]=flMaxY ;
			m_SubLight[m_nSubLightMax].Max[2]=flMaxZ ;

			// remember which sublight is the centre
			if(
						 (m_pLight[nLight].Position[0]>=flMinX) && (m_pLight[nLight].Position[0]<=flMaxX)
					&& (m_pLight[nLight].Position[1]>=flMinY) && (m_pLight[nLight].Position[1]<=flMaxY)
					&& (m_pLight[nLight].Position[2]>=flMinZ) && (m_pLight[nLight].Position[2]<=flMaxZ)
				)
				nSubLightCentre=m_nSubLightMax ;

					
			m_nSubLightMax++ ; // we don't have to worry about bound checking this, because we've already checked there aren't too many lights.

		}// end for zoneindex			
		
		// move the sublight that contains the centre to the beginning of the sublights for this light.
		// We always want the first sublight to contain the centre to make the culling algos work better.
		TempSubLight=m_SubLight[ m_pLight[nLight].SubLightStart ] ;
		m_SubLight[ m_pLight[nLight].SubLightStart ] = m_SubLight[ nSubLightCentre ] ;
		m_SubLight[ nSubLightCentre ] = TempSubLight ;


		
	}// end for light

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// recalculate m_nZoneTouchesSubLight using the newly created sublights 
	// (instead of the complete lights that were originally used)
	//

	int nSubLight=0 ;


	// clear the light settings.
	for(nZone=0 ; nZone<m_nMaxZone ; nZone++)
		m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT]=0 ;

	for(nSubZone=0 ; nSubZone<nMaxSubZone ; nSubZone++)
	{
		nZone=m_pSubZones[nSubZone].Zone ;
		flMinX=m_pSubZones[nSubZone].Min[0] ;
		flMaxX=m_pSubZones[nSubZone].Max[0] ;
		flMinY=m_pSubZones[nSubZone].Min[1] ;
		flMaxY=m_pSubZones[nSubZone].Max[1] ;
		flMinZ=m_pSubZones[nSubZone].Min[2] ;
		flMaxZ=m_pSubZones[nSubZone].Max[2] ;

		// test all the lights to see if any have centers inside this subzone
		for(nSubLight=0 ; nSubLight<m_nSubLightMax ; nSubLight++)
		{
			// if light touches subzone (we store it in ZoneTouchesSubLight for now, will overwrite later)
			// if light AABB overlaps the zone AABB
			if(
						 (flMinX<m_SubLight[nSubLight].Max[0]) && (flMaxX>m_SubLight[nSubLight].Min[0])
					&& (flMinY<m_SubLight[nSubLight].Max[1]) && (flMaxY>m_SubLight[nSubLight].Min[1])
					&& (flMinZ<m_SubLight[nSubLight].Max[2]) && (flMaxZ>m_SubLight[nSubLight].Min[2])
				)
			{
				// add this light to the zone's light list
				nIndex=m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT] ;
				if(nIndex<MAX_LIGHTPERZONE) // only add if we aren't already maxed out.
				{
					// we need to check this light isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nZoneTouchesSubLight[nZone][nPos]!=nSubLight)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nSubLight in the list
					{
						m_nZoneTouchesSubLight[nZone][nIndex]=nSubLight ;
						m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT]++ ;
					}

				}

			}// end if overlap

		}// end for nSubLight

	}// end for nSubZone





	return 1 ;
}


int Q3Map::AssignLightsToPortals(void)
{
	float flMinX=0.0f ;
	float flMaxX=0.0f ;
	float flMinY=0.0f ;
	float flMaxY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxZ=0.0f ;
	int nMaxPortal=m_iNumPortals ;
	int nPortal=0 ;
	int nMaxLight=m_nLightMax ;
	int nLight=0 ;
	int nIndex=0 ;
	int nPos=0 ;



	// clear the light settings.
	for(nPortal=0 ; nPortal<nMaxPortal ; nPortal++)
		m_nPortalTouchesLight[nPortal][INDEX_PORTALLIGHTCOUNT]=0 ;

	// now test each portal against each light to see if they touch
	for(nPortal=0 ; nPortal<nMaxPortal ; nPortal++)
	{
		flMinX=m_pPortals[nPortal].Min[0] ;
		flMaxX=m_pPortals[nPortal].Max[0] ;
		flMinY=m_pPortals[nPortal].Min[1] ;
		flMaxY=m_pPortals[nPortal].Max[1] ;
		flMinZ=m_pPortals[nPortal].Min[2] ;
		flMaxZ=m_pPortals[nPortal].Max[2] ;

		// test all the lights to see if any touch this portal
		for(nLight=0 ; nLight<nMaxLight ; nLight++)
		{

			// if light AABB overlaps the portal AABB
			if(
						 (flMinX<m_pLight[nLight].Max[0]) && (flMaxX>m_pLight[nLight].Min[0])
					&& (flMinY<m_pLight[nLight].Max[1]) && (flMaxY>m_pLight[nLight].Min[1])
					&& (flMinZ<m_pLight[nLight].Max[2]) && (flMaxZ>m_pLight[nLight].Min[2])
				)
			{

				// add this light to the portal's light list
				nIndex=m_nPortalTouchesLight[nPortal][INDEX_PORTALLIGHTCOUNT] ;
				if(nIndex<MAX_LIGHTPERPORTAL) // only add if we aren't already maxed out.
				{
					// we need to check this light isn't already on the list.
					// it might have gotten on by another subzone, since a zone can have multiple subzones.
					nPos=-1 ;
					while((++nPos<nIndex) && (m_nPortalTouchesLight[nPortal][nPos]!=nLight)) ; 

					if(nPos==nIndex) // this can only be true if we didn't already find nLight in the list
					{
						m_nPortalTouchesLight[nPortal][nIndex]=nLight ;
						m_nPortalTouchesLight[nPortal][INDEX_PORTALLIGHTCOUNT]++ ;
					}
				}

				// we don't keep a list of the portals that a light touches, we don't use such a thing.

			}// end if light touches portal

		}// end for light

	}// end for portal

	return 1 ;
}

// work out all the other zones each zone touches, via portals
int Q3Map::AssignZonesToZones(void)
{
	

	int nCentralZone=0 ;
	int nPortalZone=0 ;
	int nMaxPortalZone=0 ;
	int nPortalZoneIndex=0 ;
	int nPortal=0 ;
	int nPortalIndex=0 ;
	int nMaxPortal=0 ;

	int nTouchZoneIndex=0 ;
	int nMaxTouchZone=0 ;
	int nAddZone=0 ;


	// scan through all the zones (consider them "central zones")
	for(nCentralZone=0 ; nCentralZone<m_nMaxZone ; nCentralZone++)
	{
		nMaxTouchZone=0 ;

		// scan through all the portals in this centralzone.
		nMaxPortal=m_nZoneTouchesPortal[nCentralZone][INDEX_PORTALCOUNT] ;
		for(nPortalIndex=0 ; nPortalIndex<nMaxPortal; nPortalIndex++)
		{
			nPortal=m_nZoneTouchesPortal[nCentralZone][nPortalIndex] ;

			// scan through all the zones this portal touches and add then to the central zone's list
			nMaxPortalZone=m_nPortalTouchesZone[nPortal][INDEX_PORTALZONECOUNT] ;
			for(nPortalZoneIndex=0 ; nPortalZoneIndex<nMaxPortalZone ; nPortalZoneIndex++)
			{
				nPortalZone=m_nPortalTouchesZone[nPortal][nPortalZoneIndex] ;

				// we've got a portal zone.  
				if(nPortalZone==nCentralZone) continue ; // if it's the central zone, skip it.

				// check we don't already have it listed.
				nAddZone=1 ;
				for(nTouchZoneIndex=0 ; nTouchZoneIndex<nMaxTouchZone ; nTouchZoneIndex++)
				{
					if(m_nZoneTouchesPortal[nCentralZone][nTouchZoneIndex]==nPortalZone)
					{
						nAddZone=0 ;
						break ;
					}// end if
				}// end for nTouchZoneIndex
	
				if(nAddZone)
				{
					m_nZoneTouchesZone[nCentralZone][nMaxTouchZone]=nPortalZone ;
					nMaxTouchZone++ ;
					if(nMaxTouchZone>=MAX_ZONEPERZONE) 
						nMaxTouchZone=MAX_ZONEPERZONE-1 ;

					
				}


			}// end for portal zone index


		}// end for portal index
		
		// set the maximum
		m_nZoneTouchesZone[nCentralZone][INDEX_ZONEPERZONECOUNT]=nMaxTouchZone ;

		/*
		sprintf(m_chBug, "CentralZone %i, TouchedZoneCount %i", nCentralZone, m_nZoneTouchesPortal[nCentralZone][INDEX_ZONEPERZONECOUNT]) ;
		Q3Bug.LogAddCR(m_chBug) ;
		for(nTouchZoneIndex=0 ; nTouchZoneIndex<m_nZoneTouchesZone[nCentralZone][INDEX_ZONEPERZONECOUNT] ; nTouchZoneIndex++)
		{
			sprintf(m_chBug, "     TouchedZone %i", m_nZoneTouchesZone[nCentralZone][nTouchZoneIndex]) ;
			Q3Bug.LogAddCR(m_chBug) ;
		}
		*/


	}// end for central zone




	return 1 ;
}



