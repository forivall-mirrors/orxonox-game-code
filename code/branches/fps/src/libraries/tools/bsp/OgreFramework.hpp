/*
===========================================================================
Copyright (C) 2010 Jared Prince

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
//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreRenderSystem.h> 
#include <OgreOverlayContainer.h> 
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreHardwareOcclusionQuery.h>
#include <OgreTextureUnitState.h>
#include "OgreGpuCommandBufferFlush.h"


#include "ExampleFrameListener.h"
//#include <OgreFrameListener.h>
//#include <OgreRenderTargetListener.h>


#include <OIS/OIS.h>
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include "Q3Map.h"

//#include "JarDebug.h" //!! just for debugging, remove from final build


#define CHECKDELETE_ARRAY(x, y)		if(x) { delete [] x ; x=0 ; m_nNewCount-- ; m_nNewCheck[y]-- ;}
#define CHECKDELETE_POINTER(x, y)  if(x) { delete x		; x=0 ; m_nNewCount-- ; m_nNewCheck[y]-- ;}

enum {
	NEW_CHECK_m_pRawBspFile=0,
	NEW_CHECK_m_Q3Map,
	NEW_CHECK_m_pZoneMO,
	NEW_CHECK_m_pZoneMesh,
	NEW_CHECK_m_pZoneEntity,
	NEW_CHECK_m_pZoneEntityMaterialType,
	NEW_CHECK_m_pZoneEntityMaterial_Base,
	NEW_CHECK_m_pZoneEntityMaterial_Fast,
	NEW_CHECK_m_pZoneEntityMaterial_Black,
	NEW_CHECK_m_pZoneEntityMaterial_DfShColour,
	NEW_CHECK_m_pZoneEntityMaterial_Shadow,
	NEW_CHECK_m_pZoneEntityMaterial_ShadeFront,
	NEW_CHECK_m_pZoneEntityMaterial_ShadeBack,
	NEW_CHECK_m_pZoneEntityMaterial_DfShPosition,
	NEW_CHECK_m_pZoneEntityMaterial_DfShDiffuse,
	NEW_CHECK_m_pZoneEntityMaterial_DfShFuncTNB,
	NEW_CHECK_m_pZoneEntityMaterial_DfShSpecular,
	NEW_CHECK_m_pZoneEntityMaterial_DfShEmissive,
	NEW_CHECK_m_pZoneEntityMaterial_DfShMix,
	NEW_CHECK_pVertIndex,
	NEW_CHECK_m_pEntityInfo,
	NEW_CHECK_m_pVisibleEntity,
	NEW_CHECK_m_pFrustumEntity,
	MAX_NEW_CHECK
} ;

#define MAXFRAME 1000 // debugging/testing, copy out data on a per-frame basis

#define MAXGPUQUERY 4

#define MAXVISIBLELIGHT	128 // most shadow lights ever visible.
#define MAXLIGHTPERLOOP	1 // most shadow lights rendered per deferred shader sub-loop.  So shadow lights are rendered in lots of 4// no longer used

#define NEARCLIP 1			// camera default near clip distance
#define FARCLIP 10000		// camera default far clip distance

#define CAMERA_EPSILON  0.000001f // small value to check for up/down cameras that would break with Ogre's fixed Yaw.

#define IGNORE_LIGHT		-1

enum GoggleMode
{
	GOGGLE_MODE_OFF = 0,
	GOGGLE_MODE_TEST,
	GOGGLE_MODE_ON,
	GOGGLE_MODE_MAX
} ;

#define MAT_OPAQUE		1				// Typical solid stuff.  Can have emissive parts.
#define MAT_TRANS			2				// Unused
#define MAT_GLOW			4				// Only rendered in the deferred shader emmisive pass.  Used to create the hdr bloom for light points.
#define MAT_GEL				8				// Translucent, colours spotlights that pass through it.
#define MAT_ALPHAPF		16			// Same as MAT_OPAQUE, but with alpha testing.
#define MAT_LAMP			32			// The bounding boxes of deferred shading lights (lamps). Rendered as the last phase in deferred shading.

#define BZN_CONFIG_SIZE	1024

class rQueueListener: public Ogre::RenderQueueListener
{
public:
	
	rQueueListener() {}
	~rQueueListener() {}

	Ogre::HardwareOcclusionQuery* Query[MAXGPUQUERY] ;

	void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation) {}
	void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_SUBMESH						16	// most submeshes allowed in a mesh.
#define MAX_ZONEPERENTITY			8		// the most zones an entity can touch at once.
#define INDEX_ENTITYZONECOUNT	8		// index to value that holds the count of how many zones this entity touches
#define MAX_LIGHTPERENTITY		64	// most shadowing lights that an entity can be lit by
#define INDEX_LIGHTPERENTITY	64	// index to value
typedef struct
{
	int Active ;
	int Visible ;
	int Frustum ;
	int LightTouch ;

	int TriangleCount ;

	Ogre::Vector3 Postition ;
	Ogre::Quaternion Orientation ;

	Ogre::AxisAlignedBox AABB ;
	Ogre::Vector3 Centre ;
	float AABBMin[3] ;
	float AABBMax[3] ;

	unsigned short Zone[MAX_ZONEPERENTITY+1] ;		// the zones this entity touches.  Last value is a count of how many zones were touched.
	unsigned short Light[MAX_LIGHTPERENTITY+1] ;	// the shadow casting spotlights this entity touches.  Last value is a count of how many lights.

	int MaxSubMesh ;
	int SubMeshMaterialType[MAX_SUBMESH] ;

	Ogre::Entity*				pEnt ;
	Ogre::SceneNode*    pMasterNode ;

	Ogre::MaterialPtr		Material_Base[MAX_PROJECTORTEX][MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_Fast[MAX_PROJECTORTEX][MAX_SUBMESH] ;

	Ogre::MaterialPtr		Material_Black[MAX_SUBMESH] ;

	Ogre::MaterialPtr		Material_DfShColour[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_Shadow[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_ShadeFront[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_ShadeBack[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShPosition[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShDiffuse[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShFuncTNB[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShSpecular[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShEmissive[MAX_SUBMESH] ;
	Ogre::MaterialPtr		Material_DfShMix[MAX_SUBMESH] ;
}
ENTITYINFO;

typedef struct
{

	int Type ;

	Ogre::ParticleSystem*	PSystem ;
	Ogre::SceneNode*			PNode ;

}
PARTICLEINFO;

//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener, public RenderTargetListener
{
public:
	OgreFramework();
	~OgreFramework();

	int m_nNewCount ;
	int m_nNewCheck[MAX_NEW_CHECK] ;

	int m_nStartTime ;
	int m_nTime ;

//	CJarDebug OFBug ; //!! just for debugging, remove from final build
	char m_chBug[10240] ;
	int m_nFrameTime[MAXFRAME][10] ;
	int m_nFrame ;
	char m_chFrame[1024] ;
	int m_nGotInput ;

	void initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	void AdditionalSetup() ;

	void updateOgre(double timeSinceLastFrame);
	void updateStats();
	void moveCamera();
	void getInput();

	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}  

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	
	Ogre::Root*					m_pRoot;
	Ogre::SceneManager*			m_pSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Camera*				m_pCamera;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;

	Ogre::Camera* m_pCameraCopy ;
	Ogre::Camera* m_pCameraNoJitter ;

	float m_flAspectRatio ;
	float m_flFOV ;


	//RawFilePtr m_pRawBspFile ;	bool m_bRawBspFileIsLoaded ;

	char *m_pRawBspFile ;	
	bool m_bRawBspFileIsLoaded ;
	
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;

	int nMap ;
	Ogre::String m_MapName ;
	int m_nLoadToggle ;

	int m_nJumpToggle ;
	int m_nJumpVal;

	int m_nToggle ;

	void UnloadMap(bool bShutdown) ;
	int LoadMap(void) ;
	int m_nMapLoaded ;

	int m_nDebugLightBox ;
	int m_nZoneCullingActive ;
	int m_nPortalDebug ;
	int m_nPortalToggle ;
	int m_nPortalState ; 

	// Add this to OgreFramework and initialize it from 0 to MAXGPUQUERY
	// If you're good you'll make it private and add some get/set functions, 
	// I just made it public for now.

	// A value of 0 means don't mess with the GPU buffers at all.  There
	// might be some systems where the queries cause problems, so let the user 
	// deactivate the queries completely if desired.

	// A value of 1 means we flush every frame, so no GPU command buffering.
	// This is good for low FPS because even just 1 buffer gives noticable
	// input lag.  However users with high FPS can afford a few buffers.
	
	int m_nMaxGPUQuery ;


	int m_nDebugA ;
	int m_nDebugB ;
	int m_nDebugC ;
	int m_nVisibleLightCount ;
	int m_nVisibleZoneCount ;

	float m_flDebugMatrix[4][4] ;
	
	int m_nRecurseCount ;
	char m_chDebug[10240] ;
	float m_flDebug0 ;
	float m_flDebug1 ;
	float m_flDebug2 ;
	float m_flDebug3 ;
	float m_flDebug4 ;
	float m_flDebug5 ;


	
	Ogre::ManualObject* m_pGoggleL ;
	Ogre::ManualObject* m_pGoggleR ;
	Ogre::SceneNode*		m_pNodeGoggles ;
	int m_nGoggleMode ;
	float m_flGoggleAspectRatio	;
	float m_flGoggleZPos ;
	float m_flGoggleXGap ;
	float m_flGoggleXScale ;
	float m_flGoggleYScale ;
	int m_nGoggleTestImage ;
	void DestroyGoggles() ;
	int CreateGogglesTestImage() ;
	int CreateGoggles() ;


	void UpdateRenderTargets() ;

	double m_GameTime ;

	char m_chBZNConfig[BZN_CONFIG_SIZE] ;
	void ParseBZNConfig(void) ;

	void FinalShutdown(void) ;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);

	Ogre::Overlay*				m_pDebugOverlay;
	Ogre::Overlay*				m_pInfoOverlay;
	int							m_iNumScreenShots;

	bool						m_bShutDownOgre;
	
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real					m_MoveSpeed; 
	Ogre::Degree				m_RotateSpeed; 
	float						m_MoveScale; 
	Ogre::Degree				m_RotScale;

	int m_nKeyDown_Shift ;
	int m_nKeyDown_Ctrl ;


	


	//RawFileManager *mRawFileManager;

	char* m_TempMem ;
	Q3Map *m_Q3Map;


	Ogre::SceneNode*			m_pCubeNode;
	Ogre::Entity*				m_pCubeEntity;

	Ogre::SceneNode *m_pOpaqueNode[MAX_ZONE] ;
	int m_nOpaqueNodeUsed[MAX_ZONE] ; 
	Ogre::SceneNode *m_pTransNode[MAX_ZONE] ;
	int m_nTransNodeUsed[MAX_ZONE] ;
	Ogre::SceneNode *m_pLampNode[MAX_ZONE] ;
	int m_nLampNodeUsed[MAX_ZONE] ;
	Ogre::SceneNode *m_pGlowNode[MAX_ZONE] ;
	int m_nGlowNodeUsed[MAX_ZONE] ;

	Ogre::Matrix4	m_ViewMatrix ;
	Ogre::Matrix4	m_ProjectionMatrix ;

	Ogre::ManualObject** m_pZoneMO ; // the Zone's manualObject array
	Ogre::MeshPtr* m_pZoneMesh ;
	Ogre::Entity** m_pZoneEntity ;
	int* m_pZoneEntityMaterialType ;

	Ogre::MaterialPtr* m_pZoneEntityMaterial_Base[MAX_PROJECTORTEX] ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_Fast[MAX_PROJECTORTEX] ;

	Ogre::MaterialPtr* m_pZoneEntityMaterial_Black ;

	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShColour ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_Shadow ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_ShadeFront ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_ShadeBack ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShPosition ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShDiffuse ;
	//Ogre::MaterialPtr* m_pZoneEntityMaterial_DSNormal ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShFuncTNB ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShSpecular ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShEmissive ;
	//Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShData ;
	Ogre::MaterialPtr* m_pZoneEntityMaterial_DfShMix ;

	Ogre::MaterialPtr* m_pZoneEntityMaterial_BlurA ;


	/////////////////////////////////////////////////////////////////////////////////




	Ogre::MeshPtr* m_pGameMesh ;
	ENTITYINFO* m_pEntityInfo ;
	int* m_pVisibleEntity ; // entities that are in a visible zone
	int* m_pFrustumEntity ; // entities that are in a visible zone and also in the view frustum

	int m_nMaxGameMesh ;
	int m_nMaxEntity ;
	int m_nMaxVisibleEntity ;
	int m_nMaxFrustumEntity ;
	int m_nVisibleEntityTriangleCount ;
	int m_nFrustumEntityTriangleCount ;
	int m_nTotalTriangles ;
	int m_nZoneTriangleCount[MAX_ZONE] ;

	/////////////////////////////////////////////////////////////////////////////////

	// for debugging/testing, make light volumes into meshes
	Ogre::ManualObject** m_pLightMO ; // the manualObject array
	int AddLightCullingBoxes(void) ;

	// for debugging/testing, make portal volumes into meshes
	Ogre::ManualObject** m_pPortalMO ; // the manualObject array
	Ogre::Node** m_pPortalNode ; // the node of this portal
	int AddPortalBoxes(void) ;

	/////////////////////////////////////////////////////////////////////////////////

	
	


	void SetupResourceLocations() ;
	


	float m_flStereoFrustumTweak ; // in stereoscopic mode the portal frustums might be too tight, so enlarge them a bit.


	bool m_IsOpenGL ;

	
	void initLight() ;

	Ogre::Matrix4 CreateTextureViewProjectionMatrix(Ogre::Camera* pCamera) ;
	bool SetupSingleVisibleLightAndShadowCamera(int nLt, char* pLightVis, unsigned short* pVisibleLightList, Ogre::Camera* pCamera) ;
	

	int SetupParticles() ;
	Ogre::ParticleSystem*		m_ps;
	Ogre::SceneNode*				m_pParticleNode;
	
	int m_nMaxParticleSystem ;
	
	
	int ConstructMapFromTriangles(void) ;
	
	int SetupGameEntities(void) ;
	int SetupEntity(int nEntity, char *chMeshName) ;
	
	

	int m_nKeyToggle[OIS::KC_MEDIASELECT+1] ; // for preventing multiple key triggers when needed.

	int m_nFlashLight ;
	

	




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// left/mono visibility info
	char m_chZoneVisL[MAX_ZONE] ;
	char m_chLightVisL[MAX_LIGHT] ;
	char m_chSubLightVisL[MAX_SUBLIGHT] ;
	char m_chPortalVisL[MAX_PORTAL] ;
	int  m_nMaxVisibleZoneL ;
	unsigned short m_uVisibleZoneListL[MAX_ZONE] ;				// if MAX_ZONE goes above 65536 this will need to change to an int 
	int  m_nMaxVisibleLightL ;
	unsigned short m_uVisibleLightListL[MAXVISIBLELIGHT]; // if MAX_LIGHT goes above 65536 this will need to change to an int
	int m_nCameraZoneL ; // zone player left eye is in.
	
	// right visibility info
	char m_chZoneVisR[MAX_ZONE] ;
	char m_chLightVisR[MAX_LIGHT] ;
	char m_chSubLightVisR[MAX_SUBLIGHT] ;
	char m_chPortalVisR[MAX_PORTAL] ;
	int  m_nMaxVisibleZoneR ;
	unsigned short m_uVisibleZoneListR[MAX_ZONE] ;				// if MAX_ZONE goes above 65536 this will need to change to an int 
	int  m_nMaxVisibleLightR ;
	unsigned short m_uVisibleLightListR[MAXVISIBLELIGHT]; // if MAX_LIGHT goes above 65536 this will need to change to an int 
	int m_nCameraZoneR ; // zone player right eye is in.
	
	int m_nCameraZone ;  // zone player is in.
	
	



	char* m_pGlobal_ZoneVis ;
	char* m_pGlobal_LightVis ;
	unsigned short* m_pGlobal_VisibleZoneList ;
	int*  m_pGlobal_MaxVisibleZone ;
	unsigned short* m_pGlobal_VisibleLightList ;
	int*  m_pGlobal_MaxVisibleLight ;

	char m_chPortalState[MAX_PORTAL] ;
	
	int  m_nMaxVisibleLight ;







	int m_nZoneMOStart[MAX_ZONE+1] ; // index into m_pZoneMO for each zone, since each material within a zone is another manualObject.

	void CalculateZoneVisibility(Ogre::Camera* pCamera, int *pCameraZone, char* pZoneVis, char* pLightVis, char* pSubLightVis, char* pPortalVis, int* pMaxVisibleLight, unsigned short* pVisibleLightList) ;
	void SetupEntityZones(void) ;
	void CalculateEntityVisibility(Ogre::Camera* pCamera, char* pZoneVis) ;

	void SetAllLightsOff(void) ;
	void SetSingleVisibleLight(int nLt, bool bOn) ;
	void UpdateVisibleZoneList(char* pZoneVis, unsigned short* pVisibleZoneList, int* pMaxVisibleZone) ;
	
	void SetZoneNodeAttachments(unsigned short* pVisibleZoneList, int* pMaxVisibleZone, int nMaterialFlags) ;
	void SetZoneNodeAttachments(unsigned short* pVisibleZoneList, int* pMaxVisibleZone, int nMaterialFlags, int nLight) ;
	bool LightTouchesZone(int nLight, int nZone) ;

	void PortalScan(Ogre::Camera *pCamera, int nZone, Ogre::Real ScaleX, Ogre::Real ScaleY, char* pZoneVis, char* pLightVis, char* pSubLightVis, char* pPortalVis, int* pMaxVisibleLight, unsigned short* pVisibleLightList) ;
	int m_nOriginalZoneOpenPortals ;
	void CheckMultiZoneLights(int nCameraZone, char* pZoneVis, char* pLightVis, char* pSubLightVis) ;
	void AddZonesFromMultiZoneLights(char* pZoneVis, char* pLightVis, char* pSubLightVis) ;
	int CalculatePortalFrustum(Ogre::Camera *pCamera, int nPortal, Ogre::Real* pPortalL, Ogre::Real* pPortalR, Ogre::Real* pPortalT, Ogre::Real* pPortalB, Ogre::Real ScaleX, Ogre::Real ScaleY) ;




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	



	//int ApplyCGDefines(void) ;
	

	

	int m_nSpeedRender ;
	int m_nRenderHDR ;
	int m_nColouredShadow ;
	int m_nShadowMapSize ;
	int m_nShadowRGBSize ;

	int m_nDisplayInfoMode ;
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// render to texture quasi-deferred shading stuff
	
	void CreateRTTAssets() ;
	void DestroyRTTAssets() ;
	int m_nRTTAssetsExist ;

	// Shadow
	Rectangle2D*					miniScreen_Shadow;
	Ogre::SceneNode*			miniScreenNode_Shadow;
	Ogre::TexturePtr			RTT_Texture_Shadow ;
	Ogre::RenderTexture*	renderTexture_Shadow ;
	Ogre::MaterialPtr			RTT_Mat_Shadow ;
	Ogre::Technique*			RTT_Technique_Shadow ;

	// ShadeFront
	Rectangle2D*					miniScreen_ShadeFront;
	Ogre::SceneNode*			miniScreenNode_ShadeFront;
	Ogre::TexturePtr			RTT_Texture_ShadeFront ;
	Ogre::RenderTexture*	renderTexture_ShadeFront ;
	Ogre::MaterialPtr			RTT_Mat_ShadeFront ;
	Ogre::Technique*			RTT_Technique_ShadeFront ;

	// ShadeBack
	Rectangle2D*					miniScreen_ShadeBack;
	Ogre::SceneNode*			miniScreenNode_ShadeBack;
	Ogre::TexturePtr			RTT_Texture_ShadeBack ;
	Ogre::RenderTexture*	renderTexture_ShadeBack ;
	Ogre::MaterialPtr			RTT_Mat_ShadeBack ;
	Ogre::Technique*			RTT_Technique_ShadeBack ;

	// DfShPosition
	Rectangle2D*					miniScreen_DfShPosition;
	SceneNode*						miniScreenNode_DfShPosition;
	TexturePtr						RTT_Texture_DfShPosition ;
	RenderTexture*				renderTexture_DfShPosition ;
	MaterialPtr						RTT_Mat_DfShPosition ;
	Technique*						RTT_Technique_DfShPosition ;
	RenderTargetListener* Listener_DfShPosition ;

	// DfShDiffuse
	Rectangle2D*					miniScreen_DfShDiffuse;
	Ogre::SceneNode*			miniScreenNode_DfShDiffuse;
	Ogre::TexturePtr			RTT_Texture_DfShDiffuse ;
	Ogre::RenderTexture*	renderTexture_DfShDiffuse ;
	Ogre::MaterialPtr			RTT_Mat_DfShDiffuse ;
	Ogre::Technique*			RTT_Technique_DfShDiffuse ;

	/*
	// DSNormal // UNUSED
	Rectangle2D*					miniScreen_DSNormal;
	Ogre::SceneNode*			miniScreenNode_DSNormal;
	Ogre::TexturePtr			RTT_Texture_DSNormal ;
	Ogre::RenderTexture*	renderTexture_DSNormal ;
	Ogre::MaterialPtr			RTT_Mat_DSNormal ;
	Ogre::Technique*			RTT_Technique_DSNormal ;
	*/

	// DfShFuncTNB
	Rectangle2D*					miniScreen_DfShFuncTNB;
	Ogre::SceneNode*			miniScreenNode_DfShFuncTNB;
	Ogre::TexturePtr			RTT_Texture_DfShFuncTNB ;
	Ogre::RenderTexture*	renderTexture_DfShFuncTNB ;
	Ogre::MaterialPtr			RTT_Mat_DfShFuncTNB ;
	Ogre::Technique*			RTT_Technique_DfShFuncTNB ;

	// DfShSpecular
	Rectangle2D*					miniScreen_DfShSpecular;
	Ogre::SceneNode*			miniScreenNode_DfShSpecular;
	Ogre::TexturePtr			RTT_Texture_DfShSpecular ;
	Ogre::RenderTexture*	renderTexture_DfShSpecular ;
	Ogre::MaterialPtr			RTT_Mat_DfShSpecular ;
	Ogre::Technique*			RTT_Technique_DfShSpecular ;

	// DfShEmissive
	Rectangle2D*					miniScreen_DfShEmissive;
	Ogre::SceneNode*			miniScreenNode_DfShEmissive;
	Ogre::TexturePtr			RTT_Texture_DfShEmissive ;
	Ogre::RenderTexture*	renderTexture_DfShEmissive ;
	Ogre::MaterialPtr			RTT_Mat_DfShEmissive ;
	Ogre::Technique*			RTT_Technique_DfShEmissive ;

	// DfShLamp
	Rectangle2D*					miniScreen_DfShLamp;
	Ogre::SceneNode*			miniScreenNode_DfShLamp;
	Ogre::TexturePtr			RTT_Texture_DfShLamp ;
	Ogre::RenderTexture*	renderTexture_DfShLamp ;
	Ogre::MaterialPtr			RTT_Mat_DfShLamp ;
	Ogre::Technique*			RTT_Technique_DfShLamp ;

	/*
	// DfShData
	Rectangle2D*					miniScreen_DfShData;
	Ogre::SceneNode*			miniScreenNode_DfShData;
	Ogre::TexturePtr			RTT_Texture_DfShData ;
	Ogre::RenderTexture*	renderTexture_DfShData ;
	Ogre::MaterialPtr			RTT_Mat_DfShData ;
	Ogre::Technique*			RTT_Technique_DfShData ;
	*/

	// DfShMix
	Rectangle2D*					miniScreen_DfShMix;
	Ogre::SceneNode*			miniScreenNode_DfShMix;
	Ogre::TexturePtr			RTT_Texture_DfShMix ;
	Ogre::RenderTexture*	renderTexture_DfShMix ;
	Ogre::MaterialPtr			RTT_Mat_DfShMix ;
	Ogre::Technique*			RTT_Technique_DfShMix ;

	
	// DfShTemp
	Rectangle2D*					miniScreen_DfShTemp;
	Ogre::SceneNode*			miniScreenNode_DfShTemp;
	Ogre::TexturePtr			RTT_Texture_DfShTemp ;
	Ogre::RenderTexture*	renderTexture_DfShTemp ;
	Ogre::MaterialPtr			RTT_Mat_DfShTemp ;
	Ogre::Technique*			RTT_Technique_DfShTemp ;

	Rectangle2D*					miniScreen_DfShMaster;
	Ogre::SceneNode*			miniScreenNode_DfShMaster;

	int m_nRTTWidth ;
	int m_nRTTHeight ;

	// blur for hdr
	Rectangle2D*					miniScreen_BlurA;
	Ogre::SceneNode*			miniScreenNode_BlurA;
	Ogre::TexturePtr			RTT_Texture_BlurA ;
	Ogre::RenderTexture*	renderTexture_BlurA ;
	Ogre::MaterialPtr			RTT_Mat_BlurA ;
	Ogre::Technique*			RTT_Technique_BlurA ;

	Rectangle2D*					miniScreen_BlurB;
	Ogre::SceneNode*			miniScreenNode_BlurB;
	Ogre::TexturePtr			RTT_Texture_BlurB ;
	Ogre::RenderTexture*	renderTexture_BlurB ;
	Ogre::MaterialPtr			RTT_Mat_BlurB ;
	Ogre::Technique*			RTT_Technique_BlurB ;
	
	

	Ogre::MaterialPtr			pClearMaterial ;


};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||
