//|||||||||||||||||||||||||||||||||||||||||||||||
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

#include "OgreFramework.hpp"

#include <io.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <sys/stat.h>
#include "Q3Map_misc.h"	// safe pointer deletes



//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<class OgreFramework>::ms_Singleton = 0;



OgreFramework::OgreFramework()
{

	m_nNewCount=0 ;
	ZeroMemory((void*)m_nNewCheck, sizeof(m_nNewCheck)) ;

	m_MoveSpeed			= 0.1;
	m_RotateSpeed		= 0.3;

	m_bShutDownOgre		= false;
	m_iNumScreenShots	= 0;

	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd		= 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;

	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;

	m_pDebugOverlay		= 0;
	m_pInfoOverlay		= 0;

	//mRawFileManager=NULL ; // pointer for text file resource handler
	m_pRawBspFile=NULL ;
	m_bRawBspFileIsLoaded=false ;

	// pointers for q3map stuff
	m_TempMem=NULL ;
	m_Q3Map=NULL ;
	

	m_pZoneMO=NULL ;
	m_pZoneMesh=NULL ;
	m_pZoneEntity=NULL ;
	m_pZoneEntityMaterialType=NULL ;

	int nPTex=0 ;
	for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
	{
		m_pZoneEntityMaterial_Base[nPTex]=NULL ;
		m_pZoneEntityMaterial_Fast[nPTex]=NULL ;
	}

	m_pZoneEntityMaterial_Black=NULL ;

	m_pZoneEntityMaterial_DfShColour=NULL ;
	m_pZoneEntityMaterial_Shadow=NULL ;
	m_pZoneEntityMaterial_ShadeFront=NULL ;
	m_pZoneEntityMaterial_ShadeBack=NULL ;
	m_pZoneEntityMaterial_DfShPosition=NULL ;
	m_pZoneEntityMaterial_DfShDiffuse=NULL ;
	//m_pZoneEntityMaterial_DSNormal=NULL ;
	m_pZoneEntityMaterial_DfShFuncTNB=NULL ;
	m_pZoneEntityMaterial_DfShSpecular=NULL ;
	m_pZoneEntityMaterial_DfShEmissive=NULL ;
	//m_pZoneEntityMaterial_DfShData=NULL ;
	m_pZoneEntityMaterial_DfShMix=NULL ;


	/////////////////////////////////

	m_nMaxEntity=0 ;
	m_pEntityInfo=NULL ;
	m_nMaxVisibleEntity=0 ;
	m_pVisibleEntity=NULL ;
	m_nMaxFrustumEntity=0 ;
	m_pFrustumEntity=NULL ;


	// debug light meshes
	m_pLightMO=NULL ;
	m_pPortalMO=NULL ;
	m_pPortalNode=NULL ;

	m_pCubeNode			= 0;
	m_pCubeEntity		= 0;

	nMap=2 ;
	m_nLoadToggle=0 ;
	m_nJumpToggle=0 ;
	m_nJumpVal=-1 ;

	



	m_nDebugA=0 ;
	m_nDebugB=0 ;
	m_nDebugC=0 ;
	m_nRecurseCount=0 ;


	m_nDebugLightBox=0 ;
	m_nZoneCullingActive=1 ;
	
	m_nPortalDebug=0 ;
	m_nPortalToggle=0 ;
	m_nPortalState=1 ;

	m_nKeyDown_Shift=0 ;
	m_nKeyDown_Ctrl=0 ;



	m_flStereoFrustumTweak=0.0f ;

	m_nGoggleMode=GOGGLE_MODE_OFF ;
	m_pGoggleL=NULL ;
	m_pGoggleR=NULL ;
	m_pNodeGoggles=NULL ;

	// default goggle settings
	m_flGoggleZPos=-250 ;
	m_flGoggleXGap=0.0 ;
	m_flGoggleXScale=1.0 ;
	m_flGoggleYScale=1.0 ;

	m_nMapLoaded=0 ;

	m_nTime=0 ;

	

	ZeroMemory((void*)m_nFrameTime, sizeof(m_nFrameTime)) ;
	m_nFrame=0 ;
	m_nGotInput=0 ;

	ZeroMemory((void*)m_nKeyToggle, sizeof(m_nKeyToggle)) ;

//	OFBug.LogInit() ;

	m_nRTTAssetsExist=0 ;
	
	m_nToggle=0 ;
	
	m_nFlashLight=0 ;



	m_flAspectRatio=1.0f ;
	m_flFOV=60.0f ;

	// whether we are drawing the render or one of the info screens, such as wireframe or one of the deferred screens
	m_nDisplayInfoMode=0 ;

	m_nSpeedRender=0 ;

	//m_nRenderHDR=0 ;

	srand( (unsigned)time( NULL ) );

}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
//	OFBug.LogSave("OFBug.txt") ;

	/*
	char chMessage[1024] ;
	int nFrame=0 ;
	m_pLog->logMessage("   ");
	m_pLog->logMessage("   Mircosecs:   Physics    Render    Ideal   Average    Pragma     Total");
	m_pLog->logMessage("   ----------------------------------------");
	for(nFrame=0 ; nFrame<MAXFRAME ; nFrame++)
	{
		//sprintf(chMessage, "   Frame %3i:  %8i  %8i  %8i", nFrame, m_nFrameTime[nFrame][0], m_nFrameTime[nFrame][1], m_nFrameTime[nFrame][2]) ;
		sprintf(chMessage, "  Frame %3i:  %8i  %8i  %8i  %8i  %8i  %8i", nFrame, m_nFrameTime[nFrame][0], m_nFrameTime[nFrame][1], m_nFrameTime[nFrame][2], m_nFrameTime[nFrame][3], m_nFrameTime[nFrame][4], m_nFrameTime[nFrame][5]) ;
		m_pLog->logMessage(chMessage);
	}
	m_pLog->logMessage("");
	*/

	//UnloadMap(true) ;


	//mRawFileManager->~RawFileManager() ; //!! is this correct?  
	//DELETE_POINTER( mRawFileManager ) ;


	DELETE_POINTER( m_pKeyboard );
	DELETE_POINTER( m_pMouse );
	OIS::InputManager::destroyInputSystem(m_pInputMgr);

	
	//DELETE_POINTER( m_pRoot );
	//delete m_pRoot ;
}



//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
//	Ogre::LogManager* logMgr = new Ogre::LogManager();
	
	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
	
	m_pRoot = new Ogre::Root();

	// our manager for the raw bsp
	//mRawFileManager = new RawFileManager();
	//ResourceGroupManager::getSingleton().createResourceGroup("Raw Bsp") ;

	


	m_pRoot->showConfigDialog();

	if (!m_pRoot->restoreConfig() && !m_pRoot->showConfigDialog())
	{
		throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
	}

	
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));


	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 0, 0));
	m_pCamera->lookAt(Vector3(0,0,0));
	m_pCamera->setNearClipDistance(NEARCLIP);
  m_pCamera->setFarClipDistance(FARCLIP);
	m_pCamera->setFOVy( Degree(m_flFOV) ) ;

	m_pCameraCopy = m_pSceneMgr->createCamera("CameraCopy");
	m_pCameraCopy->setNearClipDistance(NEARCLIP);
  m_pCameraCopy->setFarClipDistance(FARCLIP);


	
	m_pCameraNoJitter = m_pSceneMgr->createCamera("CameraNoJitter");


	
	

	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);

	m_pViewport->setBackgroundColour(ColourValue(0.5, 0.5, 0.5, 1.0));

	m_flAspectRatio= (float)m_pViewport->getActualWidth() / (float)m_pViewport->getActualHeight() ;

	m_flGoggleAspectRatio=m_flAspectRatio ; // we use this to set up the Goggles
	m_pCamera->setAspectRatio( Real(m_flAspectRatio) );
	m_pCameraCopy->setAspectRatio(  Real(m_flAspectRatio) );
	
	m_pViewport->setCamera(m_pCamera);
	m_pViewport->setClearEveryFrame(true);//, FBT_DEPTH) ;

	


	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

  m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);

	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);

	SetupResourceLocations() ;

	// D3D or OGL
	if (Root::getSingleton().getRenderSystem()->getName().find("GL") != String::npos)
		m_IsOpenGL = true;
	else
		m_IsOpenGL = false;





	m_pTimer = new Ogre::Timer();
	m_pTimer->reset();
	

	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	

	

	

	m_nRTTWidth = m_pRenderWnd->getWidth() ;
	m_nRTTHeight = m_pRenderWnd->getHeight() ;
	

	m_pRenderWnd->addListener(this);
	m_pRenderWnd->setActive(true);


	// default values, may be changed from the bzn.cfg file
	m_nShadowMapSize=512 ;
	m_nShadowRGBSize=512 ;
	m_nColouredShadow=1 ;
	m_nRenderHDR=1 ;
	m_nMaxGPUQuery=1 ;
	
}

void OgreFramework::FinalShutdown(void)
{

	//Ogre::ResourceGroupManager::getSingleton().shutdownAll();
	//m_pRoot->destroySceneManager(m_pSceneMgr);
	//m_pRoot->shutdown() ;
	delete m_pRoot ;
}

void OgreFramework::AdditionalSetup()
{

	CreateRTTAssets() ;
}


void OgreFramework::SetupResourceLocations()
{
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}


// create the textures, materials and miniscreens needed for the RTTs
void OgreFramework::CreateRTTAssets()
{
	

	// I originally had problems with OpenGL 2.1.1 suddenly running very slow if I had more than 12 RTT surfaces
	// but that issue seems to have vanished, perhaps because all my surfaces are now FLOAT16_RGB or FLOAT16_RGBA.
	// It doesn't like FLOAT32 surfaces, and it also didn't seem to like mixing R8G8B8 with FLOAT16, at least
	// those configurations seem to have been problems in the past, so I try to keep everything FLOAT16_RGB where possible.

	// I go through quite a few RTT surfaces, and I don't use MRT since I couldn't get it working.

	MaterialManager::getSingleton().initialise() ;

	// need our own resource group so that the RTTs dont get destroyed when we load a new level.
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("RTTResources") ;

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	

	int RTTWidth = m_nRTTWidth ;//m_pRenderWnd->getWidth() ;
	int RTTHeight = m_nRTTHeight ; //m_pRenderWnd->getHeight() ;

	int DSWidth = m_nRTTWidth ;
	int DSHeight = m_nRTTHeight ;

	int DSWidthB = m_nSpeedRender ? m_nRTTWidth/2 : m_nRTTWidth ;
	int DSHeightB = m_nSpeedRender ? m_nRTTHeight/2 : m_nRTTHeight ;

	int TEMPWidth = m_nRTTWidth ;
	int TEMPHeight = m_nRTTHeight ;


	Ogre::ColourValue ClearColour=Ogre::ColourValue(0,0,0,1) ; // clears alpha as well. for RGBA textures 
	//!! for now use alpha 1, since Ogre seems to force it to 1 anyway on shadow render.


	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shadow
	
	RTT_Texture_Shadow = Ogre::TextureManager::getSingleton().createManual("RttTex_Shadow",
      "RTTResources", TEX_TYPE_2D, m_nShadowMapSize, m_nShadowMapSize, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_Shadow = RTT_Texture_Shadow->getBuffer()->getRenderTarget();
	renderTexture_Shadow->addViewport(m_pCamera);
	renderTexture_Shadow->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_Shadow->getViewport(0)->setBackgroundColour(ColourValue::White);
	renderTexture_Shadow->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_Shadow = MaterialManager::getSingleton().create("RttMat_Shadow", "RTTResources");
	RTT_Technique_Shadow = RTT_Mat_Shadow->createTechnique();
	RTT_Technique_Shadow->createPass();
	TextureUnitState* tState_Shadow = RTT_Mat_Shadow->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_Shadow");
	tState_Shadow->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_Shadow->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_Shadow->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	
	miniScreen_Shadow = new Ogre::Rectangle2D(true);
	miniScreen_Shadow->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_Shadow->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_Shadow = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_Shadow");
	miniScreenNode_Shadow->attachObject(miniScreen_Shadow);
	miniScreen_Shadow->setMaterial("RttMat_Shadow");


	if(m_nColouredShadow)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ShadeFront
		
		RTT_Texture_ShadeFront = Ogre::TextureManager::getSingleton().createManual("RttTex_ShadeFront",
				"RTTResources", TEX_TYPE_2D, m_nShadowRGBSize, m_nShadowRGBSize, 0, PF_FLOAT16_RGBA,
				TU_RENDERTARGET);

		
		renderTexture_ShadeFront = RTT_Texture_ShadeFront->getBuffer()->getRenderTarget();
		renderTexture_ShadeFront->addViewport(m_pCamera);
		renderTexture_ShadeFront->getViewport(0)->setClearEveryFrame(true);//, FBT_DEPTH);
		renderTexture_ShadeFront->getViewport(0)->setBackgroundColour(ColourValue::White);
		renderTexture_ShadeFront->getViewport(0)->setOverlaysEnabled(false);

		RTT_Mat_ShadeFront = MaterialManager::getSingleton().create("RttMat_ShadeFront", "RTTResources");
		RTT_Technique_ShadeFront = RTT_Mat_ShadeFront->createTechnique();
		RTT_Technique_ShadeFront->createPass();
		TextureUnitState* tState_ShadeFront = RTT_Mat_ShadeFront->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_ShadeFront");
		tState_ShadeFront->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
		RTT_Mat_ShadeFront->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		RTT_Mat_ShadeFront->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
		
		miniScreen_ShadeFront = new Ogre::Rectangle2D(true);
		miniScreen_ShadeFront->setCorners(-1.0001, 1.0001, 1.0, -1.0);
		miniScreen_ShadeFront->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
		miniScreenNode_ShadeFront = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_ShadeFront");
		miniScreenNode_ShadeFront->attachObject(miniScreen_ShadeFront);
		miniScreen_ShadeFront->setMaterial("RttMat_ShadeFront");
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ShadeBack
		
		RTT_Texture_ShadeBack = Ogre::TextureManager::getSingleton().createManual("RttTex_ShadeBack",
				"RTTResources", TEX_TYPE_2D, m_nShadowRGBSize, m_nShadowRGBSize, 0, PF_FLOAT16_RGBA,
				TU_RENDERTARGET);

		
		renderTexture_ShadeBack = RTT_Texture_ShadeBack->getBuffer()->getRenderTarget();
		renderTexture_ShadeBack->addViewport(m_pCamera);
		renderTexture_ShadeBack->getViewport(0)->setClearEveryFrame(true);//, FBT_DEPTH);
		renderTexture_ShadeBack->getViewport(0)->setBackgroundColour(ColourValue::White);
		renderTexture_ShadeBack->getViewport(0)->setOverlaysEnabled(false);

		RTT_Mat_ShadeBack = MaterialManager::getSingleton().create("RttMat_ShadeBack", "RTTResources");
		RTT_Technique_ShadeBack = RTT_Mat_ShadeBack->createTechnique();
		RTT_Technique_ShadeBack->createPass();
		TextureUnitState* tState_ShadeBack = RTT_Mat_ShadeBack->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_ShadeBack");
		tState_ShadeBack->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
		RTT_Mat_ShadeBack->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		RTT_Mat_ShadeBack->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
		
		miniScreen_ShadeBack = new Ogre::Rectangle2D(true);
		miniScreen_ShadeBack->setCorners(-1.0001, 1.0001, 1.0, -1.0);
		miniScreen_ShadeBack->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
		miniScreenNode_ShadeBack = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_ShadeBack");
		miniScreenNode_ShadeBack->attachObject(miniScreen_ShadeBack);
		miniScreen_ShadeBack->setMaterial("RttMat_ShadeBack");
	
	} // end if coloured shadows
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShPosition
	
	RTT_Texture_DfShPosition = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShPosition",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DfShPosition = RTT_Texture_DfShPosition->getBuffer()->getRenderTarget();
	renderTexture_DfShPosition->addViewport(m_pCamera);
	renderTexture_DfShPosition->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DfShPosition->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShPosition->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShPosition = MaterialManager::getSingleton().create("RttMat_DfShPosition", "RTTResources");
	RTT_Technique_DfShPosition = RTT_Mat_DfShPosition->createTechnique();
	RTT_Technique_DfShPosition->createPass();
	TextureUnitState* tState_DfShPosition = RTT_Mat_DfShPosition->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShPosition");
	tState_DfShPosition->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShPosition->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShPosition->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	
	
	//renderTexture_DfShPosition->addListener(this);

	miniScreen_DfShPosition = new Ogre::Rectangle2D(true);
	miniScreen_DfShPosition->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShPosition->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShPosition = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShPosition");
	miniScreenNode_DfShPosition->attachObject(miniScreen_DfShPosition);
	miniScreen_DfShPosition->setMaterial("RttMat_DfShPosition");
	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShDiffuse
	
	RTT_Texture_DfShDiffuse = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShDiffuse",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DfShDiffuse = RTT_Texture_DfShDiffuse->getBuffer()->getRenderTarget();
	renderTexture_DfShDiffuse->addViewport(m_pCamera);
	renderTexture_DfShDiffuse->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DfShDiffuse->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShDiffuse->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShDiffuse = MaterialManager::getSingleton().create("RttMat_DfShDiffuse", "RTTResources");
	RTT_Technique_DfShDiffuse = RTT_Mat_DfShDiffuse->createTechnique();
	RTT_Technique_DfShDiffuse->createPass();
	TextureUnitState* tState_DfShDiffuse = RTT_Mat_DfShDiffuse->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShDiffuse");
	tState_DfShDiffuse->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShDiffuse->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShDiffuse->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	
	
	//renderTexture_DfShDiffuse->addListener(this);

	miniScreen_DfShDiffuse = new Ogre::Rectangle2D(true);
	miniScreen_DfShDiffuse->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShDiffuse->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShDiffuse = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShDiffuse");
	miniScreenNode_DfShDiffuse->attachObject(miniScreen_DfShDiffuse);
	miniScreen_DfShDiffuse->setMaterial("RttMat_DfShDiffuse");

	/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DSNormal
	
	RTT_Texture_DSNormal = Ogre::TextureManager::getSingleton().createManual("RttTex_DSNormal",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DSNormal = RTT_Texture_DSNormal->getBuffer()->getRenderTarget();
	renderTexture_DSNormal->addViewport(m_pCamera);
	renderTexture_DSNormal->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DSNormal->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DSNormal->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DSNormal = MaterialManager::getSingleton().create("RttMat_DSNormal", "RTTResources");
	RTT_Technique_DSNormal = RTT_Mat_DSNormal->createTechnique();
	RTT_Technique_DSNormal->createPass();
	TextureUnitState* tState_DSNormal = RTT_Mat_DSNormal->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DSNormal");
	tState_DSNormal->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DSNormal->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DSNormal->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	
	
	
	//renderTexture_DSNormal->addListener(this);

	miniScreen_DSNormal = new Ogre::Rectangle2D(true);
	miniScreen_DSNormal->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DSNormal->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DSNormal = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DSNormal");
	miniScreenNode_DSNormal->attachObject(miniScreen_DSNormal);
	miniScreen_DSNormal->setMaterial("RttMat_DSNormal");
	
	*/

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShFuncTNB

	RTT_Texture_DfShFuncTNB = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShFuncTNB",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DfShFuncTNB = RTT_Texture_DfShFuncTNB->getBuffer()->getRenderTarget();
	renderTexture_DfShFuncTNB->addViewport(m_pCamera);
	renderTexture_DfShFuncTNB->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DfShFuncTNB->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShFuncTNB->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShFuncTNB = MaterialManager::getSingleton().create("RttMat_DfShFuncTNB", "RTTResources");
	RTT_Technique_DfShFuncTNB = RTT_Mat_DfShFuncTNB->createTechnique();
	RTT_Technique_DfShFuncTNB->createPass();
	TextureUnitState* tState_DfShFuncTNB = RTT_Mat_DfShFuncTNB->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShFuncTNB");
	tState_DfShFuncTNB->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShFuncTNB->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShFuncTNB->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	
	//renderTexture_DfShFuncTNB->addListener(this);

	miniScreen_DfShFuncTNB = new Ogre::Rectangle2D(true);
	miniScreen_DfShFuncTNB->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShFuncTNB->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShFuncTNB = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShFuncTNB");
	miniScreenNode_DfShFuncTNB->attachObject(miniScreen_DfShFuncTNB);
	miniScreen_DfShFuncTNB->setMaterial("RttMat_DfShFuncTNB");

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShSpecular

	RTT_Texture_DfShSpecular = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShSpecular",
      "RTTResources", TEX_TYPE_2D, DSWidthB, DSHeightB, 0, PF_FLOAT16_RGBA,
      TU_RENDERTARGET);

	
	renderTexture_DfShSpecular = RTT_Texture_DfShSpecular->getBuffer()->getRenderTarget();
	renderTexture_DfShSpecular->addViewport(m_pCamera);
	renderTexture_DfShSpecular->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DfShSpecular->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShSpecular->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShSpecular = MaterialManager::getSingleton().create("RttMat_DfShSpecular", "RTTResources");
	RTT_Technique_DfShSpecular = RTT_Mat_DfShSpecular->createTechnique();
	RTT_Technique_DfShSpecular->createPass();
	TextureUnitState* tState_DfShSpecular = RTT_Mat_DfShSpecular->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShSpecular");
	tState_DfShSpecular->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShSpecular->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	if(m_nSpeedRender)
		RTT_Mat_DfShSpecular->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_BILINEAR) ;
	else
		RTT_Mat_DfShSpecular->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	

	//renderTexture_DfShSpecular->addListener(this);

	miniScreen_DfShSpecular = new Ogre::Rectangle2D(true);
	miniScreen_DfShSpecular->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShSpecular->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShSpecular = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShSpecular");
	miniScreenNode_DfShSpecular->attachObject(miniScreen_DfShSpecular);
	miniScreen_DfShSpecular->setMaterial("RttMat_DfShSpecular");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShEmissive

	RTT_Texture_DfShEmissive = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShEmissive",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DfShEmissive = RTT_Texture_DfShEmissive->getBuffer()->getRenderTarget();
	renderTexture_DfShEmissive->addViewport(m_pCamera);
	renderTexture_DfShEmissive->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH);
	renderTexture_DfShEmissive->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShEmissive->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShEmissive = MaterialManager::getSingleton().create("RttMat_DfShEmissive", "RTTResources");
	RTT_Technique_DfShEmissive = RTT_Mat_DfShEmissive->createTechnique();
	RTT_Technique_DfShEmissive->createPass();
	TextureUnitState* tState_DfShEmissive = RTT_Mat_DfShEmissive->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShEmissive");
	tState_DfShEmissive->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShEmissive->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	if(m_nSpeedRender)
		RTT_Mat_DfShEmissive->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_BILINEAR) ;
	else
		RTT_Mat_DfShEmissive->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	

	//renderTexture_DfShEmissive->addListener(this);

	miniScreen_DfShEmissive = new Ogre::Rectangle2D(true);
	miniScreen_DfShEmissive->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShEmissive->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShEmissive = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShEmissive");
	miniScreenNode_DfShEmissive->attachObject(miniScreen_DfShEmissive);
	miniScreen_DfShEmissive->setMaterial("RttMat_DfShEmissive");

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShLamp

	RTT_Texture_DfShLamp = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShLamp",
      "RTTResources", TEX_TYPE_2D, DSWidth, DSHeight, 0, PF_FLOAT16_RGB, 
      TU_RENDERTARGET);//, 0, false, 6);
	
	renderTexture_DfShLamp = RTT_Texture_DfShLamp->getBuffer()->getRenderTarget();
	renderTexture_DfShLamp->addViewport(m_pCamera);
	renderTexture_DfShLamp->getViewport(0)->setClearEveryFrame(true);//, FBT_DEPTH); 
	renderTexture_DfShLamp->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShLamp->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShLamp = MaterialManager::getSingleton().create("RttMat_DfShLamp", "RTTResources");
	RTT_Technique_DfShLamp = RTT_Mat_DfShLamp->createTechnique();
	RTT_Technique_DfShLamp->createPass();
	TextureUnitState* tState_DfShLamp = RTT_Mat_DfShLamp->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShLamp");
	tState_DfShLamp->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShLamp->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShLamp->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	//renderTexture_DfShLamp->addListener(this);

	miniScreen_DfShLamp = new Ogre::Rectangle2D(true);
	miniScreen_DfShLamp->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShLamp->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShLamp = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShLamp");
	miniScreenNode_DfShLamp->attachObject(miniScreen_DfShLamp);
	miniScreen_DfShLamp->setMaterial("RttMat_DfShLamp");
	
	

	/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShData

	RTT_Texture_DfShData = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShData",
      "RTTResources", TEX_TYPE_2D, DSWidthB, DSHeightB, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);
	
	renderTexture_DfShData = RTT_Texture_DfShData->getBuffer()->getRenderTarget();
	renderTexture_DfShData->addViewport(m_pCamera);
	renderTexture_DfShData->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH); 
	renderTexture_DfShData->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShData->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShData = MaterialManager::getSingleton().create("RttMat_DfShData", "RTTResources");
	RTT_Technique_DfShData = RTT_Mat_DfShData->createTechnique();
	RTT_Technique_DfShData->createPass();
	TextureUnitState* tState_DfShData = RTT_Mat_DfShData->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShData");
	tState_DfShData->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShData->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	if(m_nSpeedRender)
		RTT_Mat_DfShData->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_BILINEAR) ;
	else
		RTT_Mat_DfShData->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	//renderTexture_DfShData->addListener(this);

	miniScreen_DfShData = new Ogre::Rectangle2D(true);
	miniScreen_DfShData->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShData->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShData = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShData");
	miniScreenNode_DfShData->attachObject(miniScreen_DfShData);
	miniScreen_DfShData->setMaterial("RttMat_DfShData");
	
	*/

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShTemp

	RTT_Texture_DfShTemp = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShTemp",
      "RTTResources", TEX_TYPE_2D, TEMPWidth, TEMPHeight, 0, PF_FLOAT16_RGB, 
      TU_RENDERTARGET);//, 0, false, 6);
	
	renderTexture_DfShTemp = RTT_Texture_DfShTemp->getBuffer()->getRenderTarget();
	renderTexture_DfShTemp->addViewport(m_pCamera);
	renderTexture_DfShTemp->getViewport(0)->setClearEveryFrame(true, FBT_DEPTH); 
	renderTexture_DfShTemp->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShTemp->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShTemp = MaterialManager::getSingleton().create("RttMat_DfShTemp", "RTTResources");
	RTT_Technique_DfShTemp = RTT_Mat_DfShTemp->createTechnique();
	RTT_Technique_DfShTemp->createPass();
	TextureUnitState* tState_DfShTemp = RTT_Mat_DfShTemp->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShTemp");
	tState_DfShTemp->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShTemp->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShTemp->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;
	//renderTexture_DfShTemp->addListener(this);

	miniScreen_DfShTemp = new Ogre::Rectangle2D(true);
	miniScreen_DfShTemp->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShTemp->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShTemp = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShTemp");
	miniScreenNode_DfShTemp->attachObject(miniScreen_DfShTemp);
	miniScreen_DfShTemp->setMaterial("RttMat_DfShTemp");


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DfShMix

		RTT_Texture_DfShMix = Ogre::TextureManager::getSingleton().createManual("RttTex_DfShMix",
      "RTTResources", TEX_TYPE_2D, RTTWidth, RTTHeight, 0, PF_FLOAT16_RGB,
      TU_RENDERTARGET);

	
	renderTexture_DfShMix = RTT_Texture_DfShMix->getBuffer()->getRenderTarget();
	renderTexture_DfShMix->addViewport(m_pCamera);
	renderTexture_DfShMix->getViewport(0)->setClearEveryFrame(false);//, FBT_DEPTH);
	renderTexture_DfShMix->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture_DfShMix->getViewport(0)->setOverlaysEnabled(false);

	RTT_Mat_DfShMix = MaterialManager::getSingleton().create("RttMat_DfShMix", "RTTResources");
	RTT_Technique_DfShMix = RTT_Mat_DfShMix->createTechnique();
	RTT_Technique_DfShMix->createPass();
	TextureUnitState* tState_DfShMix = RTT_Mat_DfShMix->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_DfShMix");
	tState_DfShMix->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	RTT_Mat_DfShMix->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	RTT_Mat_DfShMix->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_NONE) ;


	miniScreen_DfShMix = new Ogre::Rectangle2D(true);
	miniScreen_DfShMix->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShMix->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShMix = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShMix");
	miniScreenNode_DfShMix->attachObject(miniScreen_DfShMix);
	miniScreen_DfShMix->setMaterial("RttMat_DfShMix");
	
	if(m_nRenderHDR)
	{

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BlurA
		
		RTT_Texture_BlurA = Ogre::TextureManager::getSingleton().createManual("RttTex_BlurA",
				"RTTResources", TEX_TYPE_2D, DSWidth/2, DSHeight/2, 0, PF_FLOAT16_RGB,
				TU_RENDERTARGET);
		
		renderTexture_BlurA = RTT_Texture_BlurA->getBuffer()->getRenderTarget();
		renderTexture_BlurA->addViewport(m_pCamera);
		renderTexture_BlurA->getViewport(0)->setClearEveryFrame(false);
		renderTexture_BlurA->getViewport(0)->setBackgroundColour(ColourValue::Black);
		renderTexture_BlurA->getViewport(0)->setOverlaysEnabled(false);

		RTT_Mat_BlurA = MaterialManager::getSingleton().create("RttMat_BlurA", "RTTResources");
		RTT_Technique_BlurA = RTT_Mat_BlurA->createTechnique();
		RTT_Technique_BlurA->createPass();
		TextureUnitState* tState_BlurA = RTT_Mat_BlurA->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_BlurA");
		tState_BlurA->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
		RTT_Mat_BlurA->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		RTT_Mat_BlurA->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_BILINEAR) ;

		miniScreen_BlurA = new Ogre::Rectangle2D(true);
		miniScreen_BlurA->setCorners(-1.0001, 1.0001, 1.0, -1.0);
		miniScreen_BlurA->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
		miniScreenNode_BlurA = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_BlurA");
		miniScreenNode_BlurA->attachObject(miniScreen_BlurA);
		miniScreen_BlurA->setMaterial("RttMat_BlurA");

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BlurB
		
		RTT_Texture_BlurB = Ogre::TextureManager::getSingleton().createManual("RttTex_BlurB",
				"RTTResources", TEX_TYPE_2D, DSWidth/2, DSHeight/2, 0, PF_FLOAT16_RGB,
				TU_RENDERTARGET);
		
		renderTexture_BlurB = RTT_Texture_BlurB->getBuffer()->getRenderTarget();
		renderTexture_BlurB->addViewport(m_pCamera);
		renderTexture_BlurB->getViewport(0)->setClearEveryFrame(false);
		renderTexture_BlurB->getViewport(0)->setBackgroundColour(ColourValue::Black);
		renderTexture_BlurB->getViewport(0)->setOverlaysEnabled(false);

		RTT_Mat_BlurB = MaterialManager::getSingleton().create("RttMat_BlurB", "RTTResources");
		RTT_Technique_BlurB = RTT_Mat_BlurB->createTechnique();
		RTT_Technique_BlurB->createPass();
		TextureUnitState* tState_BlurB = RTT_Mat_BlurB->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex_BlurB");
		tState_BlurB->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
		RTT_Mat_BlurB->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		RTT_Mat_BlurB->getTechnique(0)->getPass(0)->setTextureFiltering(TFO_BILINEAR) ;

		miniScreen_BlurB = new Ogre::Rectangle2D(true);
		miniScreen_BlurB->setCorners(-1.0001, 1.0001, 1.0, -1.0);
		miniScreen_BlurB->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
		miniScreenNode_BlurB = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_BlurB");
		miniScreenNode_BlurB->attachObject(miniScreen_BlurB);
		miniScreen_BlurB->setMaterial("RttMat_BlurB");
		
	} // end if m_nRenderHDR	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// mixing quad
	miniScreen_DfShMaster = new Ogre::Rectangle2D(true);
	miniScreen_DfShMaster->setCorners(-1.0001, 1.0001, 1.0, -1.0);
	miniScreen_DfShMaster->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	miniScreenNode_DfShMaster = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode_DfShMaster");
	miniScreenNode_DfShMaster->attachObject(miniScreen_DfShMaster);
	//miniScreen_DfShMaster->setMaterial("RttMat_DfShDiffuse") ;
	//miniScreen_DfShMaster->setMaterial("DfShMix") ; // doesn't matter what material we use, since we mix from pre-rendered textures
	
	//RTT_Mat_Shadow->getTechnique(0)->getPass(0)->getTextureUnitState("RttTex_Shadow")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP) ;
	
}

void OgreFramework::DestroyRTTAssets()
{
}

void OgreFramework::UpdateRenderTargets()
{

	int nZoneList=0 ;
	int nZone=0 ;
	Ogre::Entity*	pEntity=NULL ;
	int nMO=0 ;
	int nZGStart=0 ;
	int nZGEnd=0 ;

	int nVisEnt=0 ;
	int nEntity=0 ;
//	int nMaxEntity=0 ;
	int nSubMesh=0 ;
	int nMaxSubMesh=0 ;

	int nMaxVisibleZone=*m_pGlobal_MaxVisibleZone ;

	SetAllLightsOff() ;

	
	// extra texture params
	// TexParam.x is 1 for d3d, -1 for ogl, needed for screen space UV calculations y flip
	// TexParam.y is game time, in 1/10th of a second since start of level

	Ogre::Vector4 TexParam ;
	if(m_IsOpenGL) 
		TexParam.x=-1 ; 
	else 
		TexParam.x= 1 ;
	TexParam.y=m_GameTime/100 ;
	TexParam.z=512.0f ; // pixelnoise subdivisions for the emmissive data function.


	// remove all the nodes, so we start with a clean slate
	m_pSceneMgr->getRootSceneNode()->removeAllChildren() ; // remove all the objects
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// deferred shading data textures
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_OPAQUE) ;

	// attach the entity masternode, update positions and orientations
	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		m_pEntityInfo[ nEntity ].pMasterNode->setPosition( m_pEntityInfo[nEntity].Postition ) ; // set position.
		m_pSceneMgr->getRootSceneNode()->addChild(m_pEntityInfo[ nEntity ].pMasterNode) ;
	}


	
	
	/////////////////////////////////////////////////////////////////////
	// DfShPosition
	
	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShPosition[nMO] ) ;
			}// end for entities

	}// end for zonelist


	// first time we set materials we need to set non-opaque to the be invisible.  After we don't need to do it, they'll already be invisible
	// Set the entity material

	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE)
			{
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShPosition[nSubMesh] ) ;
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(true) ; // first time around, make sure it is visible
			}
			else
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(false) ;
	}



	renderTexture_DfShPosition->update(true) ;
	
	/////////////////////////////////////////////////////////////////////
	// DfShFuncTNB

	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShFuncTNB[nMO] ) ;
			}// end for entities

	}// end for zonelist

	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE)
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShFuncTNB[nSubMesh] ) ;
	}


	renderTexture_DfShFuncTNB->update(true) ;
	
	
	/////////////////////////////////////////////////////////////////////
	// DfShDiffuse
	
	// the diffuse pass is used for the wireframe render as well



	if(m_nDisplayInfoMode!=1)
	{
		// change material
		for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
		{
			nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
			nZGStart	=	m_nZoneMOStart[nZone] ;
			nZGEnd		=	m_nZoneMOStart[nZone+1] ;
				
			for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
				if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
				{
					pEntity=m_pZoneEntity[nMO] ;
					pEntity->setMaterial( m_pZoneEntityMaterial_DfShDiffuse[nMO] ) ;
				}// end for entities

		}// end for zonelist

		// set the entity material

		for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
		{
			nEntity=m_pFrustumEntity[nVisEnt] ;
			nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

			for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE)
					m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShDiffuse[nSubMesh] ) ;
		}


		renderTexture_DfShDiffuse->update(true) ;
	}
	else // if we are doing wireframe we use this pass for it
	{
		// add gels just for the wireframe
		SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_GEL) ;	


		// clear the RTT texture, since our wireframe won't clear it.
		m_pRoot->getRenderSystem()->_setViewport(renderTexture_DfShDiffuse->getViewport(0)) ;
		m_pRoot->getRenderSystem()->clearFrameBuffer(FBT_COLOUR|FBT_DEPTH, Ogre::ColourValue(0,0,0,1)) ;
		
		// set the camera to wireframe mode
		m_pCamera->setPolygonMode(PM_WIREFRAME);

		// change material
		for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
		{
			nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
			nZGStart	=	m_nZoneMOStart[nZone] ;
			nZGEnd		=	m_nZoneMOStart[nZone+1] ;
				
			for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
				if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GEL))
				{
					pEntity=m_pZoneEntity[nMO] ;
					pEntity->setMaterial( MaterialManager::getSingleton().getByName("White") ) ;
				}// end for entities

		}// end for zonelist

		for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
		{
			nEntity=m_pFrustumEntity[nVisEnt] ;
			nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

			for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			{
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]!=MAT_OPAQUE) // any non-opaque submeshes need to be made visible
					m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(true) ;

				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial(MaterialManager::getSingleton().getByName("White") ) ;
			}
		}

		renderTexture_DfShDiffuse->update(true) ;
	
		// reset back to pre-wireframe setup
		m_pCamera->setPolygonMode(PM_SOLID);

		// remove everything and then add back OPAQUE
		m_pSceneMgr->getRootSceneNode()->removeAllChildren() ; // remove all the objects
		SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_OPAQUE) ;


		// reattach entities, make non-opaque subentities invisible
		for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
		{
			nEntity=m_pFrustumEntity[nVisEnt] ;
			nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

			m_pSceneMgr->getRootSceneNode()->addChild(m_pEntityInfo[  nEntity  ].pMasterNode) ;

			for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			{
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]!=MAT_OPAQUE) 
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(false) ;
			}
		}



	}

	
	
	/////////////////////////////////////////////////////////////////////
	// DfShSpecular

	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShSpecular[nMO] ) ;
			}// end for entities

	}// end for zonelist

	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

		// set the entity material
		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE)
				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShSpecular[nSubMesh] ) ;
	}

	renderTexture_DfShSpecular->update(true) ;
	
	/*
	/////////////////////////////////////////////////////////////////////
	// DfShData

	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShData[nMO] ) ;
			}// end for entities

	}// end for zonelist

	renderTexture_DfShData->update(true) ;
	*/

	
	/////////////////////////////////////////////////////////////////////
	// DfShEmissive

	// we add the glow entities for this pass, if we are doing HDR
	if(m_nRenderHDR)
		SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_GLOW) ;


	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GLOW))
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShEmissive[nMO] ) ;

				TexParam.z=512.0f ; // pixelnoise subdivisions for the emmissive data function.
				pEntity->getSubEntity(0)->setCustomParameter(4, TexParam) ; // emissive makes use of gametime for some lighting effects
			}// end for entities

	}// end for zonelist

	// set the entity material
	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GLOW))
			{
				// make glow stuff visible
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GLOW)
					m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(true) ;

				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShEmissive[nSubMesh] ) ;
			}
	}

	renderTexture_DfShEmissive->update(true) ;

	

	////////////////////////////////////////////////////////////////////
	// we need to know how to colour stuff if we're seeing it through gel
	// this colouring will be applied when we mix lamp and emmissive down to DfShMix
	//
	// Note that this leaves the light/lamp glow entities attached in HDR mode,
	// we need them to block the gels otherwise the blur on the lights/lamps 
	// comes out incorrectly coloured by gel that should be behind it. 

	// add the transparent stuff
	SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_GEL) ;

	// change material
	for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
	{
		nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
		nZGStart	=	m_nZoneMOStart[nZone] ;
		nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
		for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GEL) || (m_pZoneEntityMaterialType[nMO]==MAT_GLOW))
			{
				pEntity=m_pZoneEntity[nMO] ;
				pEntity->setMaterial( m_pZoneEntityMaterial_DfShColour[nMO] ) ;
			}// end for entities


	}// end for zonelist

	for(nVisEnt=0 ; nVisEnt<m_nMaxFrustumEntity ; nVisEnt++)
	{
		nEntity=m_pFrustumEntity[nVisEnt] ;
		nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

		// set the entity material
		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
			if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GLOW))
			{
				// make gel stuff visible (glow was already made visible on the emissive pass)
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL)
					m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(true) ;

				m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_DfShColour[nSubMesh] ) ;
			}
	}

	// render colour to the temp surface
	renderTexture_DfShTemp->update(true) ;

	

	//clear the mix buffer once
	m_pRoot->getRenderSystem()->_setViewport(renderTexture_DfShMix->getViewport(0)) ;
	m_pRoot->getRenderSystem()->clearFrameBuffer(FBT_COLOUR|FBT_DEPTH, Ogre::ColourValue(0,0,0,1)) ;

	// remove all the nodes
	m_pSceneMgr->getRootSceneNode()->removeAllChildren() ; // remove all the objects

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// deferred shading non-shadowing point lights
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	if(true)
	{
		SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_LAMP) ; // attach the lamp boxes

		// this renders our deferred shader lamps
		renderTexture_DfShLamp->update(true) ;

		// this shader combines the deferred light render with the emissive render and colours it with any gels
		miniScreen_DfShMaster->setMaterial("Mix_Lamps") ; 

		// get rid of all the scene geometry
		m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

		// put up a fullscreen quad so DfShMix has some UV coordinates.
		m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMaster) ;

		// update DfShMix, but don't swap buffers yet since we may be back again next loop with more lights to add.
		renderTexture_DfShMix->update(false) ; 

		// now DfShMix has all the deferred render stuff mixed onto it, as well as emissive stuff, 
		// and it's ready for forward rendered shadowing lights to be additively blended over it.
	} // end if deferred render active



	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// forward rendered shadowing spotlights
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// we now add entities from m_pVisibleEntity instead of m_pFrustumEntity since entities outside the frustum might still contribute shadows.
	//''
	if(true)
	{


		// multiple attempts at getting additive blending of the shadows failed, so I have to do an extra pass to "accumulate" the data :(

		int nMaxVisibleLight=*m_pGlobal_MaxVisibleLight ;

		// the texture view projection per light.  We use four Vector4 instead of a single Matrix4 because we have to pass them to the shaders as custom params
		Ogre::Vector4	tvpMat0 ;
		Ogre::Vector4	tvpMat1 ;
		Ogre::Vector4	tvpMat2 ;
		Ogre::Vector4	tvpMat3 ;



		int nVisibleLight=0 ;
		int nLt=0 ;
		int nProjector=0 ;
		int nEntLtIndex=0 ;
		int nEntLtIndexMax=0 ;

		m_nVisibleLightCount=0 ; // total shadow lights added from all loops 


		miniScreen_DfShMaster->setMaterial("Mix_ShadowLights") ;
		
		
		// loop through shadow casting lights
		for(nVisibleLight=0 ; nVisibleLight<nMaxVisibleLight ; nVisibleLight++)
		{
			 // copy the camera's original settings because we'll be modifying it
			m_pCameraCopy->synchroniseBaseSettingsWith(m_pCamera) ;

			nLt=m_pGlobal_VisibleLightList[nVisibleLight] ;

			// setup the light and light's shadow camera, skip if light isn't really visible
			if(SetupSingleVisibleLightAndShadowCamera(nLt, m_pGlobal_LightVis, m_pGlobal_VisibleLightList, m_pCamera))
			{
				// count the light
				m_nVisibleLightCount++ ;
				
				

				
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				// Render the shadow map

				// add all the opaque stuff
				m_pSceneMgr->getRootSceneNode()->removeAllChildren() ; 
				SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_OPAQUE) ; 

				
				for(nVisEnt=0 ; nVisEnt<m_nMaxVisibleEntity ; nVisEnt++)
				{
					// check if this entity touches this light
					nEntity=m_pVisibleEntity[nVisEnt] ;
					
					m_pSceneMgr->getRootSceneNode()->addChild(m_pEntityInfo[ nEntity ].pMasterNode) ;

					nEntLtIndexMax=m_pEntityInfo[nEntity].Light[INDEX_LIGHTPERENTITY] ;
					m_pEntityInfo[nEntity].LightTouch=0 ; // default, doesn't touch this light, use black material
					for(nEntLtIndex=0 ; nEntLtIndex<nEntLtIndexMax ; nEntLtIndex++)
						if(m_pEntityInfo[nEntity].Light[nEntLtIndex]==nLt) // found a match, entity is touched by this light
						{
							m_pEntityInfo[nEntity].LightTouch=1 ; // touches this light, don't use black
							break ;
						}

				}// end for nVisEnt
				






				// change to shadow pass material
				for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
				{
					nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
					nZGStart	=	m_nZoneMOStart[nZone] ;
					nZGEnd		=	m_nZoneMOStart[nZone+1] ;
						
					for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
						if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
						{
							pEntity=m_pZoneEntity[nMO] ;
							pEntity->setMaterial( m_pZoneEntityMaterial_Shadow[nMO] ) ;
						}// end for entities

				}// end for zonelist

				for(nVisEnt=0 ; nVisEnt<m_nMaxVisibleEntity ; nVisEnt++)
				{
					nEntity=m_pVisibleEntity[nVisEnt] ;
					nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

					// set the entity material
					for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
						if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE)
							m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_Shadow[nSubMesh] ) ;
						else
							m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(false) ; // non-opaque stuff is invisible
				}


				// render the shadow map out to the Shadow texture
				renderTexture_Shadow->update(true) ;

				// Opaque nodes are already in the scene, add all the Transparent stuff
				SetZoneNodeAttachments(m_uVisibleZoneListL, &m_nMaxVisibleZoneL, MAT_GEL) ; 

				// if this light is flagged as needing colourized shadows, we do the slower shadow method
				// this means rendering an extra couple of coloured shadow maps here, plus a different material for the view render
				if(m_nColouredShadow)
				{
					
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					// Render the ShadeFront map

					

					// change to shadow pass material
					for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
					{
						nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
						nZGStart	=	m_nZoneMOStart[nZone] ;
						nZGEnd		=	m_nZoneMOStart[nZone+1] ;
							
						for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
							if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GEL))
							{
								pEntity=m_pZoneEntity[nMO] ;
								pEntity->setMaterial( m_pZoneEntityMaterial_ShadeFront[nMO] ) ;
							}// end for entities

					}// end for zonelist

					for(nVisEnt=0 ; nVisEnt<m_nMaxVisibleEntity ; nVisEnt++)
					{
						nEntity=m_pVisibleEntity[nVisEnt] ;
						nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

						// set the entity material (pClearMaterial is already set)
						for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
							if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL))
							{
								// make gel stuff visible
								if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL)
									m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setVisible(true) ;

								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_ShadeFront[nSubMesh] ) ;
							}
					}

					// render the shadow map out to the Shadow texture
					renderTexture_ShadeFront->update(true) ;

					
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					// Render the ShadeBack map

					// change to shadow pass material
					for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
					{
						nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
						nZGStart	=	m_nZoneMOStart[nZone] ;
						nZGEnd		=	m_nZoneMOStart[nZone+1] ;
							
						for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
							if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GEL))
							{
								pEntity=m_pZoneEntity[nMO] ;
								pEntity->setMaterial( m_pZoneEntityMaterial_ShadeBack[nMO] ) ;
							}// end for entities

					}// end for zonelist

					for(nVisEnt=0 ; nVisEnt<m_nMaxVisibleEntity ; nVisEnt++)
					{
						nEntity=m_pVisibleEntity[nVisEnt] ;
						nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

						// set the entity material (pClearMaterial is already set)
						for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
							if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL))
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_ShadeBack[nSubMesh] ) ;
					}

					// render the shadow map out to the Shadow texture
					renderTexture_ShadeBack->update(true) ;
					
				} // end if we need coloured shadows


	




				/////////////////////////////////////////////////////////////////////////////////////////////////////
				// Apply the shadow map to the view

				// calculate the texture view projection matrix for this light
				Matrix4 TexViewProj=CreateTextureViewProjectionMatrix(m_pCamera) ;

				// change the matrix to four Vector4s so we can pass it in as a shader parameter 
				tvpMat0[0]=TexViewProj[0][0] ; tvpMat0[1]=TexViewProj[0][1] ; tvpMat0[2]=TexViewProj[0][2] ; tvpMat0[3]=TexViewProj[0][3] ;
				tvpMat1[0]=TexViewProj[1][0] ; tvpMat1[1]=TexViewProj[1][1] ; tvpMat1[2]=TexViewProj[1][2] ; tvpMat1[3]=TexViewProj[1][3] ;
				tvpMat2[0]=TexViewProj[2][0] ; tvpMat2[1]=TexViewProj[2][1] ; tvpMat2[2]=TexViewProj[2][2] ; tvpMat2[3]=TexViewProj[2][3] ;
				tvpMat3[0]=TexViewProj[3][0] ; tvpMat3[1]=TexViewProj[3][1] ; tvpMat3[2]=TexViewProj[3][2] ; tvpMat3[3]=TexViewProj[3][3] ;

				// put the camera back to its proper position and settings.
				m_pCamera->synchroniseBaseSettingsWith(m_pCameraCopy) ;
				m_pCamera->setFOVy( Degree(m_flFOV) ) ;
				m_pCamera->setAspectRatio( Real(m_flAspectRatio) );


				nProjector=m_Q3Map->m_pLight[nLt].Texture ; // what projector texture is this light using?

				// change material and set all the tvp custom params
				for(nZoneList=0 ; nZoneList<nMaxVisibleZone ; nZoneList++)
				{
					nZone=m_pGlobal_VisibleZoneList[nZoneList] ;
					nZGStart	=	m_nZoneMOStart[nZone] ;
					nZGEnd		=	m_nZoneMOStart[nZone+1] ;
						
					for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
						if((m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE) || (m_pZoneEntityMaterialType[nMO]==MAT_GEL))
						{
							pEntity=m_pZoneEntity[nMO] ;


							// since the spot light pass is one of the slowest, we set the material of all the zones that don't touch
							// this light to just a black material.  Only the zone of this light gets the real lighting material
							// We need to have the other zones though or we end up seeing through walls.
							// ALSO don't set gels to black, or else they block out the light in other zones.

							
							if((m_pZoneEntityMaterialType[nMO]==MAT_GEL) || (LightTouchesZone(nLt, nZone)) )
							{
								if(m_nColouredShadow)
									pEntity->setMaterial( m_pZoneEntityMaterial_Base[nProjector][nMO] ) ; // coloured shadows
								else
									pEntity->setMaterial( m_pZoneEntityMaterial_Fast[nProjector][nMO] ) ; // fast shadows
							}
							else
								pEntity->setMaterial( m_pZoneEntityMaterial_Black[nMO] ) ; // just flat black, alpha tested for some


							
				

							// set the custom parameter, our texture view projection matrix for this light
							pEntity->getSubEntity(0)->setCustomParameter(0, tvpMat0) ;
							pEntity->getSubEntity(0)->setCustomParameter(1, tvpMat1) ;
							pEntity->getSubEntity(0)->setCustomParameter(2, tvpMat2) ;
							pEntity->getSubEntity(0)->setCustomParameter(3, tvpMat3) ;

							pEntity->getSubEntity(0)->setCustomParameter(4, TexParam) ;

						}// end for entities

				}// end for zonelist

				for(nVisEnt=0 ; nVisEnt<m_nMaxVisibleEntity ; nVisEnt++)
				{
					nEntity=m_pVisibleEntity[nVisEnt] ;
					nMaxSubMesh=m_pEntityInfo[nEntity].MaxSubMesh ;

					// set the entity material
					for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
						if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_OPAQUE) || (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL))
						{

							// if it's gel or the light touches the entity, use proper material, else use black
							if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GEL) || m_pEntityInfo[nEntity].LightTouch)
							{
								if(m_nColouredShadow)
									m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_Base[nProjector][nSubMesh] ) ; // coloured shadows
								else
									m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_Fast[nProjector][nSubMesh] ) ; // fast shadows
							}
							else
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( m_pEntityInfo[nEntity].Material_Black[nSubMesh] ) ; // flat black



							// set the custom parameter, our texture view projection matrix for this light
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setCustomParameter(0, tvpMat0) ;
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setCustomParameter(1, tvpMat1) ;
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setCustomParameter(2, tvpMat2) ;
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setCustomParameter(3, tvpMat3) ;

								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setCustomParameter(4, TexParam) ;
						}
						else
								m_pEntityInfo[nEntity].pEnt->getSubEntity(nSubMesh)->setMaterial( pClearMaterial ) ;
				}




				// if this is the first visible light, add emmissive transparent/alphablend particles.  
				// they are added here so that they render correctly according to gels.
				// But since they aren't shadowed, we only need render them for one light.
				if(m_nVisibleLightCount==1)
					m_pSceneMgr->getRootSceneNode()->addChild(m_pParticleNode) ; // attach the particles



				/////////////////////////////////////////////////////////////////////////////////////////////
				// render the scene out to the DfShTemp texture.
				//

				renderTexture_DfShTemp->update(true) ; 

				//
				/////////////////////////////////////////////////////////////////////////////////////////////
				
				
				
				// remove the particles
				if(m_nVisibleLightCount==1)
					m_pSceneMgr->getRootSceneNode()->removeChild(m_pParticleNode) ;


				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Render Mix

				//?? unfortunately Ogre shares depth buffers in a manner that won't allow us to just additively render to DfShMix.
				// If we try, we get glitching from the buffer sharing between Shadow and DfShMix.
				// So instead we get rid of all geometry and additively render a fullscreen quad of DfShTemp to DfShMix.
				// Slows things down a little, but at least it works.

				// get rid of all the scene geometry
				m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

				// put up a fullscreen quad so DfShMix has some UV coordinates.
				m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMaster) ;

				// update DfShMix, but don't swap buffers yet since we may be back again next loop with more lights to add.
				renderTexture_DfShMix->update(false) ; 

			}// end if light is visible


		}// end for nVisibleLight
			
	}// end if forward render active

	///////////////////////////////////////////////////////////////////////////////////////



	// after we've finished with all the lights and mixing, time to swap the mix buffer
	renderTexture_DfShMix->swapBuffers(false) ;

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// HDR bloom
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(m_nRenderHDR)
	{
		/////////////////////////////////////////////////////////
		// copy DfShMix to BlurA

		// get rid of all the scene geometry
		m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

		// this shader renders DfShMix onto a surface 
		miniScreen_DfShMaster->setMaterial("HDR_MixToBlurA") ; 

		// put up a fullscreen quad so BlurA has some UV coordinates.
		m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMaster) ;

		// render DfShMix to BlurA
		renderTexture_BlurA->update(true) ;

		


		//////////////////////////////////////////////////////////////
		// now we do a few rounds of ping-pong bluring of A to B, B to A.
		int nMaxBlur=4 ;
//		int nLastBlur=nMaxBlur-1 ;
		int nBlurLoop=0 ;

		
		// opengl and direct3d give different results for the same blur shader, 
		// the issue is that d3d with bilinear filtering will offset stuff, 
		// leading to a right-down drift that we need to compensate for.
		// So we need two versions of the blur shaders, one for ogl and one for d3d.

		if(m_IsOpenGL)
		{
			miniScreen_BlurA->setMaterial("HDR_BlurA_ogl");
			miniScreen_BlurB->setMaterial("HDR_BlurB_ogl");
		}
		else
		{
			miniScreen_BlurA->setMaterial("HDR_BlurA_d3d");
			miniScreen_BlurB->setMaterial("HDR_BlurB_d3d");
		}

				
		// the main blur
		for(nBlurLoop=0 ; nBlurLoop<nMaxBlur ; nBlurLoop++)
		{
			// blur A onto B
			m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;
			m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_BlurA) ;
			renderTexture_BlurB->update(true) ;

			// blur B onto A
			m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;
			m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_BlurB) ;
			renderTexture_BlurA->update(true) ;
		}
		

		// If I don't set these materials back to their originals, I get a crash on exit. 
		// Not sure why.  Something to do with deleting the originals?
		// Doesn't matter, this fixes things.

		miniScreen_BlurA->setMaterial("RttMat_BlurA");
		miniScreen_BlurB->setMaterial("RttMat_BlurB");
		
		

		//////////////////////////////////////////////////////////////////
		// blend BlurA onto DfShMix

		// get rid of all the scene geometry
		m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

		// this shader blends BlurA onto a surface
		if(m_IsOpenGL)
			miniScreen_DfShMaster->setMaterial("HDR_BlurAToMix_ogl") ;
		else
			miniScreen_DfShMaster->setMaterial("HDR_BlurAToMix_d3d") ;

		// put up a fullscreen quad so DfShMix has some UV coordinates.
		m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMaster) ;

		// update DfShMix.
		renderTexture_DfShMix->update(true) ;

	}// end if HDR
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// display a rendered window
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	m_pSceneMgr->getRootSceneNode()->removeAllChildren() ; // remove all the objects
	
	
	if(m_nGoggleMode==GOGGLE_MODE_OFF) // stereo rendering not active
	{
		switch(m_nDisplayInfoMode)
		{

			case 0:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMix) ; break ;
			case 1:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShTemp) ; break ; // for wireframe view
			case 2:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShLamp) ; break ;
			case 3:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_BlurA) ; break ;

			case 4: // instead of showing the original position texture, we show a scaled version, since the original is outside 0-1 range

				// get rid of all the scene geometry
				m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

				// this shader renders DfShMix onto a surface 
				miniScreen_DfShMaster->setMaterial("Mix_ScalePosition") ; 

				// put up a fullscreen quad so BlurA has some UV coordinates.
				m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMaster) ;

				// render DfShPostion to DfShMix
				renderTexture_DfShMix->update(true) ;

				// display mix, which is a scaled copy of position.
				m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMix) ;
				break ;
			
			case 5:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShFuncTNB) ; break ;
			case 6:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShDiffuse) ; break ;
			case 7:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShSpecular) ; break ;			
			case 8:	m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShEmissive) ; break ;
		}




		
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShPosition) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShFuncTNB) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShSpecular) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShDiffuse) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShEmissive) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShData) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShLamp) ;

		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_BlurA) ;

		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShMix) ;
		
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_Shadow) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShTemp) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_ShadeBack) ;
		//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_ShadeFront) ;
	}
	else
	{
		// add and position the Goggles
		m_pSceneMgr->getRootSceneNode()->addChild(m_pNodeGoggles) ;
		m_pNodeGoggles->setOrientation( m_pCamera->getOrientation() ) ;
		m_pNodeGoggles->setPosition( m_pCamera->getPosition() ) ;
	}

	
	//m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;
	//m_pSceneMgr->getRootSceneNode()->addChild(miniScreenNode_DfShDiffuse) ;
	
	
	m_pRenderWnd->update(true) ;


	//m_pSceneMgr->getRootSceneNode()->removeChild(miniScreenNode_DfShSpecular) ;
	m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

	


	///////////////////////////////////////////
	// blittomemory experiment
	//uchar pixelData[64] ;
	//RTT_Texture_DfShPosition->getBuffer()->blitToMemory( Box( 0, 0, 1, 1), PixelBox(2, 2, 1, PF_FLOAT16_RGB, (void*)pixelData) ) ;


}

// generate the texture view Projection matrix used in shadow mapping
Ogre::Matrix4 OgreFramework::CreateTextureViewProjectionMatrix(Ogre::Camera* pCamera)
{

	const Matrix4 PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE(
		      0.5,    0,    0,  0.5,
			    0,   -0.5,    0,  0.5,
				  0,      0,    1,    0,
					0,      0,    0,    1);

	Matrix4 TexViewProj = PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE * pCamera->getProjectionMatrixWithRSDepth() * pCamera->getViewMatrix();

	return TexViewProj ;

}

void OgreFramework::DestroyGoggles()
{
	//m_pGoggleL->~ManualObject() ;
	//m_pGoggleR->~ManualObject() ;

	//m_pNodeGoggles->~SceneNode() ;
	

}

// create the "virtual" virtual reality goggles we use to get around stereoscopic driver problems.
int OgreFramework::CreateGoggles()
{

	
	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;


	// if they already exist, destroy them so we can recreate them
	if(m_pNodeGoggles!=NULL)	m_pNodeGoggles->detachAllObjects() ;
	if(m_pGoggleL!=NULL)			m_pGoggleL->clear() ;
	if(m_pGoggleR!=NULL)			m_pGoggleR->clear() ;
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// left goggle

		if(m_pGoggleL==NULL) m_pGoggleL=m_pSceneMgr->createManualObject("GoggleL") ;
		m_pGoggleL->begin("RttMat_DfShMix", RenderOperation::OT_TRIANGLE_LIST) ;
	
		flMinX=-100*m_flGoggleAspectRatio*m_flGoggleXScale ;
		flMinY=-50*m_flGoggleYScale ;


		flMaxX=0 ;
		flMaxY=50*m_flGoggleYScale ;

		
		m_pGoggleL->position(flMinX-m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(0.0, 0.0) ;
		m_pGoggleL->position(flMaxX-m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(1.0, 0.0) ;
		m_pGoggleL->position(flMaxX-m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(1.0, 1.0) ;
		m_pGoggleL->position(flMinX-m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(0.0, 1.0) ;

		m_pGoggleL->quad(3, 2, 1, 0) ;
		m_pGoggleL->end() ;

		m_pGoggleL->setCastShadows(false) ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// right goggle

		if(m_pGoggleR==NULL) m_pGoggleR=m_pSceneMgr->createManualObject("GoggleR") ;
		m_pGoggleR->begin("RttMat_DfShMix", RenderOperation::OT_TRIANGLE_LIST) ;


		flMinX=0 ;
		flMinY=-50*m_flGoggleYScale ;


		flMaxX=100*m_flGoggleAspectRatio*m_flGoggleXScale ;
		flMaxY=50*m_flGoggleYScale ;

		
		m_pGoggleR->position(flMinX+m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(0.0, 0.0) ;
		m_pGoggleR->position(flMaxX+m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(1.0, 0.0) ;
		m_pGoggleR->position(flMaxX+m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(1.0, 1.0) ;
		m_pGoggleR->position(flMinX+m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(0.0, 1.0) ;

		m_pGoggleR->quad(3, 2, 1, 0) ;
		m_pGoggleR->end() ;

		m_pGoggleR->setCastShadows(false) ;
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// attach the goggles to the node.
	if(m_pNodeGoggles==NULL) m_pNodeGoggles=m_pSceneMgr->getRootSceneNode()->createChildSceneNode("NodeGoggles") ;
	m_pNodeGoggles->attachObject(m_pGoggleL) ;
	m_pNodeGoggles->attachObject(m_pGoggleR) ;

	return 1 ;
}

// create the "virtual" virtual reality goggles we use to get around stereoscopic driver problems.
// This version of the goggles just has a test image for setting up the correct real driver settings,
// so that the left eye sees only the left "goggle" and the right eye only the right "goggle",
// and the goggles have the right depth relative to the screen (flat to it)
int OgreFramework::CreateGogglesTestImage() 
{

	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;


	// if they already exist, destroy them so we can recreate them
	if(m_pNodeGoggles!=NULL)	m_pNodeGoggles->detachAllObjects() ;
	if(m_pGoggleL!=NULL)			m_pGoggleL->clear() ;
	if(m_pGoggleR!=NULL)			m_pGoggleR->clear() ;
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// left goggle

		if(m_pGoggleL==NULL) m_pGoggleL=m_pSceneMgr->createManualObject("GoggleL") ;
		m_pGoggleL->begin("GoggleTest", RenderOperation::OT_TRIANGLE_LIST) ;
	
		flMinX=-100*m_flGoggleAspectRatio*m_flGoggleXScale ;
		flMinY=-50*m_flGoggleYScale ;


		flMaxX=0 ;
		flMaxY=50*m_flGoggleYScale ;

		
		m_pGoggleL->position(flMinX-m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(0.0, 0.0) ;
		m_pGoggleL->position(flMaxX-m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(1.0, 0.0) ;
		m_pGoggleL->position(flMaxX-m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(1.0, 1.0) ;
		m_pGoggleL->position(flMinX-m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleL->normal(0.0, 0.0, -1) ;
		m_pGoggleL->textureCoord(0.0, 1.0) ;

		m_pGoggleL->quad(3, 2, 1, 0) ;
		m_pGoggleL->end() ;

		m_pGoggleL->setCastShadows(false) ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// right goggle

		if(m_pGoggleR==NULL) m_pGoggleR=m_pSceneMgr->createManualObject("GoggleR") ;
		m_pGoggleR->begin("GoggleTest", RenderOperation::OT_TRIANGLE_LIST) ;


		flMinX=0 ;
		flMinY=-50*m_flGoggleYScale ;


		flMaxX=100*m_flGoggleAspectRatio*m_flGoggleXScale ;
		flMaxY=50*m_flGoggleYScale ;

		
		m_pGoggleR->position(flMinX+m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(1.0, 1.0) ;
		m_pGoggleR->position(flMaxX+m_flGoggleXGap, flMaxY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(0.0, 1.0) ;
		m_pGoggleR->position(flMaxX+m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(0.0, 0.0) ;
		m_pGoggleR->position(flMinX+m_flGoggleXGap, flMinY, m_flGoggleZPos) ;
		m_pGoggleR->normal(0.0, 0.0, -1) ;
		m_pGoggleR->textureCoord(1.0, 0.0) ;

		m_pGoggleR->quad(3, 2, 1, 0) ;
		m_pGoggleR->end() ;

		m_pGoggleR->setCastShadows(false) ;
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// attach the goggles to the node.
	if(m_pNodeGoggles==NULL) m_pNodeGoggles=m_pSceneMgr->getRootSceneNode()->createChildSceneNode("NodeGoggles") ;
	m_pNodeGoggles->attachObject(m_pGoggleL) ;
	m_pNodeGoggles->attachObject(m_pGoggleR) ;

	return 1 ;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	//m_pLog->logMessage("OgreFramework::keyPressed");
	
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
			m_bShutDownOgre = true;
			return true;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		std::ostringstream ss;
		ss << "screenshot_" << ++m_iNumScreenShots << ".png";
		m_pRenderWnd->writeContentsToFile(ss.str());
		return true;
	}

	/*
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCamera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
			 m_pCamera->setPolygonMode(PM_WIREFRAME);
			 mode = 1;
		}
		else if(mode == 1)
		{
			m_pCamera->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
	*/

	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pDebugOverlay)
		{
			if(!m_pDebugOverlay->isVisible())
				m_pDebugOverlay->show();
			else
				m_pDebugOverlay->hide();
		}
	}



	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1));
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{




	// update the game time
	m_GameTime+=timeSinceLastFrame ;


	m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
		
	m_TranslateVector = Vector3::ZERO;

//	char chMessage[1024] ;
	//sprintf(chMessage, "winmessage %u, %u, %u", m_pRenderWnd->m_Test_uMsg, (UINT)m_pRenderWnd->m_Test_wParam, (UINT)m_pRenderWnd->m_Test_lParam) ; 
	//m_pLog->logMessage(chMessage) ;



	/*
	if((m_pRenderWnd->m_Test_uMsg==258) && (m_pRenderWnd->m_Test_wParam==0x78))
	//if(GetAsyncKeyState(0x78))
	{

		m_TranslateVector.z = -m_MoveScale;
		//m_nGotInput=1 ;
		
		//sprintf(chMessage, "T %i: winmessage %u, %u, %u", m_nTime, m_pRenderWnd->m_Test_uMsg, (UINT)m_pRenderWnd->m_Test_wParam, (UINT)m_pRenderWnd->m_Test_lParam) ; 
		//m_pLog->logMessage(chMessage) ;
	}
	else
		m_nGotInput=0 ;
	*/

	// add a test particle to the system
	/*
	if( m_ps->getNumParticles() < m_ps->getParticleQuota() )
	{
		Ogre::Particle *p = m_ps->createParticle() ;
		p->setDimensions( m_ps->getDefaultWidth(), m_ps->getDefaultHeight() ) ;
		p->colour = Ogre::ColourValue::White;
		p->direction = Ogre::Vector3::UNIT_Y;
		p->position = Ogre::Vector3::ZERO;
		p->timeToLive = p->totalTimeToLive = 10;
		p->rotation = Ogre::Radian(0);   
		p->rotationSpeed = Ogre::Radian(0);
	}
	*/


	m_ps->_update(timeSinceLastFrame/1000.0f) ;

	getInput();

	moveCamera();


	//Ogre::Vector3 Pos ;
	//float CamPos[3] ;
	//Pos=m_pCamera->getPosition() ;
	//CamPos[0]=Pos.x ;
	//CamPos[1]=Pos.y ;
	//CamPos[2]=Pos.z ;



	if(m_nGoggleMode==GOGGLE_MODE_OFF)
	{


		m_pCameraCopy->synchroniseBaseSettingsWith(m_pCamera) ; // copy the camera's original settings because we'll be modifying it for the LR views.
		m_pCameraCopy->setFOVy( Degree(m_flFOV) ) ;
		m_pCameraCopy->setAspectRatio( Real(m_flAspectRatio) );



		if(m_nMapLoaded)
		{
			SetupEntityZones() ;
			
			CalculateZoneVisibility(m_pCameraCopy, &m_nCameraZoneL, m_chZoneVisL, m_chLightVisL, m_chSubLightVisL, m_chPortalVisL, &m_nMaxVisibleLightL, m_uVisibleLightListL) ;
			CalculateEntityVisibility(m_pCameraCopy, m_chZoneVisL) ;

			UpdateVisibleZoneList(m_chZoneVisL, m_uVisibleZoneListL, &m_nMaxVisibleZoneL) ;

			//m_nVisibleLightCount=m_nMaxVisibleLightL ;
			m_nVisibleZoneCount=m_nMaxVisibleZoneL ;
		}



		m_pGlobal_VisibleZoneList=m_uVisibleZoneListL ;
		m_pGlobal_MaxVisibleZone=&m_nMaxVisibleZoneL ;

		m_pGlobal_VisibleLightList	=	m_uVisibleLightListL ;
		m_pGlobal_MaxVisibleLight		=	&m_nMaxVisibleLightL ;
		m_pGlobal_LightVis					=	m_chLightVisL ;


	}
	else
	{
		// "virtual" stereo goggle mode.  Render everything twice, L and R view, and output to L quad and a R quad.

		m_pCameraCopy->synchroniseBaseSettingsWith(m_pCamera) ; // copy the camera's original settings because we'll be modifying it for the LR views.

		if(m_nMapLoaded)
		{
			SetupEntityZones() ;

			CalculateZoneVisibility(m_pCameraCopy, &m_nCameraZoneL, m_chZoneVisL, m_chLightVisL, m_chSubLightVisL, m_chPortalVisL, &m_nMaxVisibleLightL, m_uVisibleLightListL) ;
			UpdateVisibleZoneList(m_chZoneVisL, m_uVisibleZoneListL, &m_nMaxVisibleZoneL) ;

			
			//m_nVisibleLightCount=m_nMaxVisibleLightL ;
			m_nVisibleZoneCount=m_nMaxVisibleZoneL ;
		}

		m_pGlobal_VisibleZoneList=m_uVisibleZoneListL ;
		m_pGlobal_MaxVisibleZone=&m_nMaxVisibleZoneL ;
	}


	updateStats();


	//,,
	//sprintf(m_chFrame, "End of updateOgre frame %i",m_nFrame) ;
	//OFBug.LogAddCR(m_chFrame) ;
	


	m_nFrame++ ;


	
}



//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateStats() 
{ 
	
	
	Ogre::Vector3 Pos ;
	Pos=m_pCamera->getPosition() ;
	Ogre::Real nCamX=Pos.x ;
	Ogre::Real nCamY=Pos.y ;
	Ogre::Real nCamZ=Pos.z ;


	static String currFps = "FPS: "; 
    static String avgFps = "Av FPS: "; 
    static String bestFps = "Best FPS: "; 
    static String worstFps = "Worst FPS: "; 
    static String tris = "Triangle Count: "; 
    static String batches = "Batch Count: "; 

 
    OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps"); 
    OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps"); 
    OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps"); 
    OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps"); 

	const RenderTarget::FrameStats& stats = m_pRenderWnd->getStatistics();
    
		//int LastFrameTime=1000000/stats.lastFPS ;
		float LastFrameTime=1000/stats.lastFPS ;





		switch(m_nDisplayInfoMode)
		{

			case 0:
				if(m_IsOpenGL)
					guiCurr->setCaption("OGL FPS: " + StringConverter::toString(stats.lastFPS) +" "+StringConverter::toString(LastFrameTime)+" ms"); 
				else
					guiCurr->setCaption("D3D FPS: " + StringConverter::toString(stats.lastFPS) +" "+StringConverter::toString(LastFrameTime)+" ms"); 
				break ;

			case 1: guiCurr->setCaption("Wireframe") ; break ;
			case 2: guiCurr->setCaption("Lamps") ; break ;
			case 3: guiCurr->setCaption("HDR Blur") ; break ;
			case 4: guiCurr->setCaption("DefShade Position (Scaled)") ; break ;
			case 5: guiCurr->setCaption("DefShade Normals") ; break ;
			case 6: guiCurr->setCaption("DefShade Diffuse") ; break ;
			case 7: guiCurr->setCaption("DefShade Specular") ; break ;
			case 8: guiCurr->setCaption("DefShade Emissive") ; break ;

		}

		guiAvg->setCaption("Lt: " + StringConverter::toString(m_nVisibleLightCount)
			+ " Zn: " + StringConverter::toString(m_nVisibleZoneCount)  
			+ " Cam: "+StringConverter::toString(nCamX)+" "+StringConverter::toString(nCamY)+" "+StringConverter::toString(nCamZ));




		//m_flDebug0=m_nTotalTriangles ;
		//m_flDebug1=m_nMaxVisibleEntity ;
		//m_flDebug2=m_nMaxFrustumEntity ;
		guiWorst->setCaption("ZnEnt: " + StringConverter::toString(m_nMaxVisibleEntity) + " Tri: " + StringConverter::toString(m_nVisibleEntityTriangleCount) + " FwdTri: " + StringConverter::toString(m_nVisibleEntityTriangleCount)    ) ;
		guiBest->setCaption("FrEnt: " + StringConverter::toString(m_nMaxFrustumEntity) + " Tri: " + StringConverter::toString(m_nFrustumEntityTriangleCount) + " DefTri: " + StringConverter::toString(m_nFrustumEntityTriangleCount)    ) ;




		//guiWorst->setCaption(StringConverter::toString(m_flDebug0) + " " + StringConverter::toString(m_flDebug1) + " " + StringConverter::toString(m_flDebug2)) ;
		//guiBest->setCaption(StringConverter::toString(m_flDebug3) + " " + StringConverter::toString(m_flDebug4) + " " + StringConverter::toString(m_flDebug5)) ;
		/*
		guiWorst->setCaption(
			" A "+StringConverter::toString(m_flDebugMatrix[0][0])+" "+StringConverter::toString(m_flDebugMatrix[0][1])+" "+StringConverter::toString(m_flDebugMatrix[0][2])+" "+StringConverter::toString(m_flDebugMatrix[0][3]) +
			" B "+StringConverter::toString(m_flDebugMatrix[1][0])+" "+StringConverter::toString(m_flDebugMatrix[1][1])+" "+StringConverter::toString(m_flDebugMatrix[1][2])+" "+StringConverter::toString(m_flDebugMatrix[1][3]) ) ;

		guiBest->setCaption(
			" C "+StringConverter::toString(m_flDebugMatrix[2][0])+" "+StringConverter::toString(m_flDebugMatrix[2][1])+" "+StringConverter::toString(m_flDebugMatrix[2][2])+" "+StringConverter::toString(m_flDebugMatrix[2][3]) +
			" D "+StringConverter::toString(m_flDebugMatrix[3][0])+" "+StringConverter::toString(m_flDebugMatrix[3][1])+" "+StringConverter::toString(m_flDebugMatrix[3][2])+" "+StringConverter::toString(m_flDebugMatrix[3][3]) ) ;
		*/
	
		
		
		
		//guiWorst->setCaption(StringConverter::toString(flVarA)+" "+StringConverter::toString(flVarB)+" "+StringConverter::toString(flVarC)+" "+StringConverter::toString(flVarD)
   
	//guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS) 
  //          +" "+StringConverter::toString(stats.bestFrameTime)+" ms"); 
	//guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS) 
  //          +" "+StringConverter::toString(stats.worstFrameTime)+" ms"); 
	
	//guiWorst->setCaption(worstFps + StringConverter::toString(nCamX)+" " + StringConverter::toString(nCamY)+" " + StringConverter::toString(nCamZ)+" ") ;
    
	
	OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris"); 
	guiTris->setCaption("MpTri: " + StringConverter::toString(m_nTotalTriangles)); 

	OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches"); 
    guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount)); 

	OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText"); 
		guiDbg->setCaption("A:"+StringConverter::toString(m_nDebugA) + " B:" + StringConverter::toString(m_nDebugB) + " C:" + StringConverter::toString(m_nDebugC) + " ") ;
	
	//guiDbg->setCaption(
	//	" A "+StringConverter::toString(m_flDebugMatrix[0][0])+" "+StringConverter::toString(m_flDebugMatrix[0][1])+" "+StringConverter::toString(m_flDebugMatrix[0][2])+" "+StringConverter::toString(m_flDebugMatrix[0][3]) +
	//	" B "+StringConverter::toString(m_flDebugMatrix[1][0])+" "+StringConverter::toString(m_flDebugMatrix[1][1])+" "+StringConverter::toString(m_flDebugMatrix[1][2])+" "+StringConverter::toString(m_flDebugMatrix[1][3]) +
	//	" C "+StringConverter::toString(m_flDebugMatrix[2][0])+" "+StringConverter::toString(m_flDebugMatrix[2][1])+" "+StringConverter::toString(m_flDebugMatrix[2][2])+" "+StringConverter::toString(m_flDebugMatrix[2][3]) +
	//	" D "+StringConverter::toString(m_flDebugMatrix[3][0])+" "+StringConverter::toString(m_flDebugMatrix[3][1])+" "+StringConverter::toString(m_flDebugMatrix[3][2])+" "+StringConverter::toString(m_flDebugMatrix[3][3]) ) ;




	//guiDbg->setCaption("");
} 

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::moveCamera()
{
	if( m_nKeyDown_Ctrl && m_nKeyDown_Shift ) // super slow
			m_pCamera->moveRelative(m_TranslateVector/50);
		else
			if(m_nKeyDown_Shift)	// fast
				m_pCamera->moveRelative(m_TranslateVector*10);
			else
				if(m_nKeyDown_Ctrl) // slow 
					m_pCamera->moveRelative(m_TranslateVector/5);
				else
					m_pCamera->moveRelative(m_TranslateVector*2); // normal
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::getInput()
{
	if(m_pKeyboard->isKeyDown(OIS::KC_LCONTROL))
		m_nKeyDown_Ctrl=1 ;
	else
		m_nKeyDown_Ctrl=0 ;

	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_nKeyDown_Shift=1 ;
	else
		m_nKeyDown_Shift=0 ;






	if(m_pKeyboard->isKeyDown(OIS::KC_A))
	{
		m_TranslateVector.x = -m_MoveScale;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_D))
	{
		m_TranslateVector.x = m_MoveScale;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_W))
	{
		m_TranslateVector.z = -m_MoveScale;
		m_nGotInput=1 ;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_S))
	{
		m_TranslateVector.z = m_MoveScale;
	}


	// generic toggle for debugging
	if(m_pKeyboard->isKeyDown(OIS::KC_T))
	{
		if(m_nKeyToggle[OIS::KC_T]==0)
		{
			m_nKeyToggle[OIS::KC_T]=1 ; // toggle to stop key repeating
			m_nToggle=1-m_nToggle ;
			//OFBug.MessageInt(m_nToggle) ;
		}
	}
	else
		m_nKeyToggle[OIS::KC_T]=0 ;

	//m_flDebug0=m_nKeyToggle[OIS::KC_T] ;
	//m_flDebug1=m_nToggle ;

	/*
	if(m_pKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		m_pCamera->yaw(m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		m_pCamera->yaw(-m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_UP))
	{
		m_pCamera->pitch(m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		m_pCamera->pitch(-m_RotScale);
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Stereo controls

	

	// toggle stereo test image mode
	if(m_pKeyboard->isKeyDown(OIS::KC_DELETE))
	{
		if(m_nKeyToggle[OIS::KC_DELETE]==0)
		{
			m_nKeyToggle[OIS::KC_DELETE]=1 ; // toggle to stop key repeating
			
			m_nGoggleMode++ ; // cyle through the goggle modes
			if(m_nGoggleMode>=GOGGLE_MODE_MAX)
				m_nGoggleMode=GOGGLE_MODE_OFF ;

			if(m_nGoggleMode==GOGGLE_MODE_TEST)
				CreateGogglesTestImage() ; // create the test goggles
			else
				if(m_nGoggleMode==GOGGLE_MODE_ON)
					CreateGoggles() ; // create the stereo goggles
		}
	}
	else
		m_nKeyToggle[OIS::KC_DELETE]=0 ;

	if(m_nGoggleMode==GOGGLE_MODE_TEST) // controls for setting up the test image
	{
		int nDoPause=0 ; // if any control gets used we do a quick pause to make the rate of change a bit more consistent, not as frame-rate dependent

		if(m_pKeyboard->isKeyDown(OIS::KC_PGUP))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift)
				m_flGoggleZPos-=1.0 ;
			else
				if(m_nKeyDown_Ctrl)
					m_flGoggleZPos-=0.01 ;
				else
					m_flGoggleZPos-=0.1 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_PGDOWN))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift)
				m_flGoggleZPos+=1.0 ;
			else
				if(m_nKeyDown_Ctrl)
					m_flGoggleZPos+=0.01 ;
				else
					m_flGoggleZPos+=0.1 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_HOME))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift)
				m_flGoggleXGap-=0.1 ;
			else
				if(m_nKeyDown_Ctrl)
					m_flGoggleXGap-=0.001 ;
				else
					m_flGoggleXGap-=0.01 ;

			if(m_flGoggleXGap<0)
				m_flGoggleXGap=0 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_END))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift)
				m_flGoggleXGap+=0.1 ;
			else
				if(m_nKeyDown_Ctrl)
					m_flGoggleXGap+=0.001 ;
				else
					m_flGoggleXGap+=0.01 ;

			if(m_flGoggleXGap<0)
				m_flGoggleXGap=0 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_UP))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift && m_nKeyDown_Ctrl)
				m_flGoggleYScale=1.0 ;
			else
				if(m_nKeyDown_Shift)
					m_flGoggleYScale+=0.002 ;
				else
					if(m_nKeyDown_Ctrl)
						m_flGoggleYScale+=0.00002 ;
					else
						m_flGoggleYScale+=0.0002 ;

				if(m_flGoggleYScale<0)
					m_flGoggleYScale=0 ;

				CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_DOWN))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift && m_nKeyDown_Ctrl)
				m_flGoggleYScale=1.0 ;
			else
				if(m_nKeyDown_Shift)
					m_flGoggleYScale-=0.002 ;
				else
					if(m_nKeyDown_Ctrl)
						m_flGoggleYScale-=0.00002 ;
					else
						m_flGoggleYScale-=0.0002 ;

			if(m_flGoggleYScale<0)
				m_flGoggleYScale=0 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_LEFT))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift && m_nKeyDown_Ctrl)
				m_flGoggleXScale=1.0 ;
			else
				if(m_nKeyDown_Shift)
					m_flGoggleXScale+=0.002 ;
				else
					if(m_nKeyDown_Ctrl)
						m_flGoggleXScale+=0.00002 ;
					else
						m_flGoggleXScale+=0.0002 ;

			if(m_flGoggleXScale<0)
				m_flGoggleXScale=0 ;

			CreateGogglesTestImage() ;
		}

		if(m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
		{
			nDoPause=1 ;

			if(m_nKeyDown_Shift && m_nKeyDown_Ctrl)
				m_flGoggleXScale=1.0 ;
			else
				if(m_nKeyDown_Shift)
					m_flGoggleXScale-=0.002 ;
				else
					if(m_nKeyDown_Ctrl)
						m_flGoggleXScale-=0.00002 ;
					else
						m_flGoggleXScale-=0.0002 ;

			if(m_flGoggleXScale<0)
				m_flGoggleXScale=0 ;

			CreateGogglesTestImage() ;
		}

		

		if(nDoPause) Sleep(20) ; // pause makes rate more consisent, less frame rate dependent.

	}// end test image controls

	

	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// flashlight
	if(m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		if(m_nKeyToggle[OIS::KC_F]==0)
		{
			m_nKeyToggle[OIS::KC_F]=1 ; // toggle to stop key repeating
			m_nFlashLight=1-m_nFlashLight ;
			m_nDebugA=m_nFlashLight ;
			//OFBug.MessageInt(m_nToggle) ;
		}
	}
	else
		m_nKeyToggle[OIS::KC_F]=0 ;


	if(m_pKeyboard->isKeyDown(OIS::KC_J))
	{
		if(m_nJumpToggle==0)
		{
			m_nJumpToggle=1 ;


			m_nJumpVal++ ;
			if(m_nJumpVal>=m_Q3Map->m_nLightMax)
				m_nJumpVal=0 ;

			//m_flDebug0=m_nJumpVal ;
			
			
			//m_nJumpVal=12 ; // for now always jump to light 0
			//m_pCamera->setPosition(m_Q3Map->m_pLight[m_nJumpVal].Position[0], m_Q3Map->m_pLight[m_nJumpVal].Position[1], m_Q3Map->m_pLight[m_nJumpVal].Position[2]) ;
			//m_nJumpVal=13 ;
			//m_pCamera->lookAt(Vector3(m_Q3Map->m_pLight[m_nJumpVal].Position[0], m_Q3Map->m_pLight[m_nJumpVal].Position[1], m_Q3Map->m_pLight[m_nJumpVal].Position[2]));
			
			//m_pCamera->setPosition(166, 222, -111) ;
			//m_pCamera->setDirection(0.707, 0, -0.707) ;

			//m_pCamera->setPosition(m_Q3Map->m_pLight[0].Position[0], m_Q3Map->m_pLight[0].Position[1], m_Q3Map->m_pLight[0].Position[2]) ;
			//m_pCamera->setDirection(m_Q3Map->m_pLight[0].Direction[0], m_Q3Map->m_pLight[0].Direction[1], m_Q3Map->m_pLight[0].Direction[2]) ;
			//m_pCamera->setFOVy( Degree(m_Q3Map->m_pLight[0].Angle ) ) ;

			//m_pCamera->setDirection(m_pLight[m_nJumpVal].Direction[0], m_pLight[m_nJumpVal].Direction[1], m_pLight[m_nJumpVal].Direction[2]) ;
			// 880 120 -340
			//m_pCamera->lookAt(Vector3(880,120,-340));
			//m_pCamera->lookAt(Vector3(915,155,-340));


			m_pCamera->setPosition(600, 200, -250) ;
			//m_pCamera->setDirection(1.0, 0.0, 0.0) ;
			m_pCamera->lookAt(700, 200, -450);

			

		}
	}
	else
		m_nJumpToggle=0 ;
	

	if(m_pKeyboard->isKeyDown(OIS::KC_L))
	{
		if(m_nLoadToggle==0)
		{
			UnloadMap(false) ;
			nMap++ ;
			LoadMap() ;
			//m_nLoadToggle=1 ;
		}
	}
	else
		m_nLoadToggle=0 ;


		// generic toggle for debugging
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		if(m_nKeyToggle[OIS::KC_M]==0)
		{
			m_nKeyToggle[OIS::KC_M]=1 ; // toggle to stop key repeating

			m_nDisplayInfoMode++ ;
			if(m_nDisplayInfoMode==9)
				m_nDisplayInfoMode=0 ;



		}
	}
	else
		m_nKeyToggle[OIS::KC_M]=0 ;





	//if(m_pKeyboard->isKeyDown(OIS::KC_K)) m_nVideoRestart=1 ;


	if(m_pKeyboard->isKeyDown(OIS::KC_P))
	{
		if(m_nPortalToggle==0)
		{
			m_nPortalState=1-m_nPortalState ;
			m_nPortalToggle=1 ;
		}
	}
	else
		m_nPortalToggle=0 ;

	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
			m_bShutDownOgre = true;
	}

}

//|||||||||||||||||||||||||||||||||||||||||||||||
//shutdown
// the dynamic memory deletes are all safe, so it's ok to try and delete them even if they don't exist.
void OgreFramework::UnloadMap(bool bShutdown)
{	

	//if(!bShutdown) DestroyRTTAssets() ;

	//if(ResourceGroupManager::getSingleton().isResourceGroupLoaded("Raw Bsp"))
	//	ResourceGroupManager::getSingleton().clearResourceGroup("Raw Bsp") ; 


	// only do if there was a definitely a map loaded.
	if(m_nMapLoaded)
	{
		char chSceneNodeName[1024] ;
		// make sure all the zone nodes are re-attached to the root node or they won't get deleted
		for(int nZone=0 ; nZone<m_Q3Map->m_nMaxZone ; nZone++)
		{
			// Opaque nodes
			if(m_nOpaqueNodeUsed[nZone]) // only do this if the node actually exists
			{
				if(!m_pOpaqueNode[nZone]->isInSceneGraph())
					m_pSceneMgr->getRootSceneNode()->addChild(m_pOpaqueNode[nZone]) ;

				sprintf(chSceneNodeName, "Opaque%05i", nZone) ;
				m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild(chSceneNodeName) ;

			}

			// Trans nodes
			if(m_nTransNodeUsed[nZone]) // only do this if the node actually exists
			{
				if(!m_pTransNode[nZone]->isInSceneGraph())
					m_pSceneMgr->getRootSceneNode()->addChild(m_pTransNode[nZone]) ;

				sprintf(chSceneNodeName, "Trans%05i", nZone) ;
				m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild(chSceneNodeName) ;

			}

			// Lamp nodes
			if(m_nLampNodeUsed[nZone]) // only do this if the node actually exists
			{
				if(!m_pLampNode[nZone]->isInSceneGraph())
					m_pSceneMgr->getRootSceneNode()->addChild(m_pLampNode[nZone]) ;

				sprintf(chSceneNodeName, "Lamp%05i", nZone) ;
				m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild(chSceneNodeName) ;

			}

			// Glow nodes
			if(m_nGlowNodeUsed[nZone]) // only do this if the node actually exists
			{
				if(!m_pGlowNode[nZone]->isInSceneGraph())
					m_pSceneMgr->getRootSceneNode()->addChild(m_pGlowNode[nZone]) ;

				sprintf(chSceneNodeName, "Glow%05i", nZone) ;
				m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild(chSceneNodeName) ;

			}

		}


		// reattach all entity scenenodes.
		int nLoop=0 ;
//		int nSubMesh=0 ;
//		int nMaxSubMesh=0 ;
		for(nLoop=0 ; nLoop<m_nMaxEntity ; nLoop++)
			m_pSceneMgr->getRootSceneNode()->addChild(m_pEntityInfo[nLoop].pMasterNode) ;

	}// end if map loaded


	// free ogre stuff
	//Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) ;

	//Ogre::ResourcePtr RTT=Ogre::TextureManager::getSingleton().getByName("RttTex") ;
	//Ogre::TextureManager::getSingleton().remove(RTT) ;
	
	//RTT_Texture->unload() ;

	


	
	

	//m_pSceneMgr->clearScene() ;
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren() ; // destroy all scenenodes
	
	m_pSceneMgr->destroyAllEntities() ;
	m_pSceneMgr->destroyAllLights() ;
	m_pSceneMgr->destroyAllManualObjects() ;
	

	/*
	// unload all the meshes
	int nMaxMesh=m_nZoneMOStart[m_Q3Map->m_nMaxZone] ;
	int nMesh=0 ;
	for(nMesh=0 ; nMesh<nMaxMesh ; nMesh++)
	{
		m_pSceneMgr->destroyEntity(m_pZoneEntity[nMesh]) ;
		//m_pZoneMesh[nMesh]->cleanupDictionary() ;
		//m_pZoneMesh[nMesh]->unload() ;


	}
	*/
	
	MeshManager::getSingleton().removeAll() ; // this destroys all the meshes

	

	//OFBug.MessageInt(333) ;
	//Ogre::TextureManager::getSingleton().unloadAll() ;
	//Ogre::TextureManager::getSingleton().removeAll() ;
	
	//Ogre::MaterialManager::getSingleton().unloadAll() ;
	//Ogre::MaterialManager::getSingleton().removeAll() ;

	



	//SetupResourceLocations() ;

	// delete our array of Zone group manual objects 
	//CHECKDELETE_ARRAY( m_pZoneMO ) ;
	CHECKDELETE_ARRAY( m_pZoneMesh, NEW_CHECK_m_pZoneMesh ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntity, NEW_CHECK_m_pZoneEntity  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterialType, NEW_CHECK_m_pZoneEntityMaterialType  ) ; 


	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Shadow, NEW_CHECK_m_pZoneEntityMaterial_Shadow  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_ShadeFront, NEW_CHECK_m_pZoneEntityMaterial_ShadeFront  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_ShadeBack, NEW_CHECK_m_pZoneEntityMaterial_ShadeBack  ) ; 

	int nPTex=0 ;
	for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
	{
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Base[nPTex], NEW_CHECK_m_pZoneEntityMaterial_Base  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Fast[nPTex], NEW_CHECK_m_pZoneEntityMaterial_Fast  ) ; 
	}

	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Black, NEW_CHECK_m_pZoneEntityMaterial_Black  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShColour, NEW_CHECK_m_pZoneEntityMaterial_DfShColour  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShPosition, NEW_CHECK_m_pZoneEntityMaterial_DfShPosition  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShDiffuse, NEW_CHECK_m_pZoneEntityMaterial_DfShDiffuse  ) ; 
	//CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DSNormal ) ;
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShFuncTNB, NEW_CHECK_m_pZoneEntityMaterial_DfShFuncTNB  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShSpecular, NEW_CHECK_m_pZoneEntityMaterial_DfShSpecular  ) ; 
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShEmissive, NEW_CHECK_m_pZoneEntityMaterial_DfShEmissive  ) ; 
	//CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShData ) ;
	CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShMix, NEW_CHECK_m_pZoneEntityMaterial_DfShMix  ) ; 

	////////////////////////////////////////////////////////
	CHECKDELETE_ARRAY( m_pEntityInfo, NEW_CHECK_m_pEntityInfo  ) ; 
	CHECKDELETE_ARRAY( m_pVisibleEntity, NEW_CHECK_m_pVisibleEntity  ) ;
	CHECKDELETE_ARRAY( m_pFrustumEntity, NEW_CHECK_m_pFrustumEntity  ) ;


	// was leaking before, but this seems to have fixed things.
	Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) ;

	// delete our debug light manualobjects/zones
//	CHECKDELETE_ARRAY( m_pLightMO ) ;

//	CHECKDELETE_ARRAY( m_pPortalMO ) ;
//	CHECKDELETE_ARRAY( m_pPortalNode ) ;

	// free our dynamic memory
	if(m_Q3Map)
	{
		m_Q3Map->~Q3Map() ;		// get rid of all the m_Q3Map stuff
		CHECKDELETE_POINTER( m_Q3Map, NEW_CHECK_m_Q3Map) ;  
		m_Q3Map=NULL ;
	}

	if(m_bRawBspFileIsLoaded) 
	{
		//m_pRawBspFile->unload() ;
		CHECKDELETE_ARRAY(m_pRawBspFile, NEW_CHECK_m_pRawBspFile ) ;  
		m_bRawBspFileIsLoaded=false ;
	}

	// indicate that there is no map.
	m_nMapLoaded=0 ;

	

	
	int nCheck=0 ;
	for(nCheck=0 ; nCheck<MAX_NEW_CHECK ; nCheck++)
	if(m_nNewCheck[nCheck]!=0)
	{
		sprintf(m_chBug, "MEMORY LEAK: NewCheck %i, NewCount %i", nCheck, m_nNewCheck[nCheck]) ;
		m_pLog->logMessage(m_chBug) ;
	}


		//OFBug.MessageInt(333) ;
		//Sleep(50) ;
}








 /***********************************************************************************************************\
                                               
                                                 LOAD MAP 
                                                
 \***********************************************************************************************************/




int OgreFramework::LoadMap(void)
{	
	m_pLog->logMessage("LoadMap.") ;



	ZeroMemory((void*)m_nKeyToggle, sizeof(m_nKeyToggle)) ; // good place to clear the keytoggle just in case

	m_pLog->logMessage("Pre InitAllResourceGroups.") ;
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	m_pLog->logMessage("Post InitAllResourceGroups.") ;

	// setup the clear material
	pClearMaterial=MaterialManager::getSingleton().getByName("Clear") ;

	
	//CreateRTTAssets() ;



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_pDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
		m_pDebugOverlay->show();

		//m_MapName="test04e.bsp" ;
		//m_MapName="test04g.bsp" ;
		//m_MapName="test03f.bsp" ;

		//m_MapName="testnew00a.bsp" ;
		//m_MapName="test04gNew.bsp" ;

		//m_MapName="test10a.bsp" ;
		m_MapName="testmap06.bsp" ;
		

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Load the file, get the data
	


		// open the stream to the bsp
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(m_MapName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

		// create memory to hold the bsp
		size_t Size = stream->size() ;
		m_pRawBspFile= new char[Size] ;
		if( m_pRawBspFile==NULL) 
			{ m_pLog->logMessage("Error loading map: Initial load out of memory.")			; UnloadMap(false) ; return 0 ; }
		else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pRawBspFile]++ ; }

		// stream bsp into memory
		stream->read((void*)m_pRawBspFile, Size) ;

		// close the stream, don't need it anymore
		stream->close() ;
		
		// indicate that the raw bsp is loaded
		m_bRawBspFileIsLoaded=true ;



	

		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Setup Q3Map

		m_Q3Map=new Q3Map();
		if(m_Q3Map==NULL) 
			{ m_pLog->logMessage("Error loading map: New Map.")			; UnloadMap(false) ; return 0 ; }
		else
			{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_Q3Map]++ ; }



		m_pLog->logMessage("pre ParseMap") ;

		// process the bsp, log errors and fail if needed.
		switch(m_Q3Map->ParseAndTriangulateMap( m_pRawBspFile, Size ) )
		{
			case ERROR_ParseMap:						m_pLog->logMessage("Error loading map: Parse Map.") ;							UnloadMap(false) ; return 0 ;
			case ERROR_ParseEntities:				m_pLog->logMessage("Error loading map: Parse Entities.") ;				UnloadMap(false) ; return 0 ;
			case ERROR_AllocateVertex:			m_pLog->logMessage("Error loading map: Allocate Vertex.") ;				UnloadMap(false) ; return 0 ;
			case ERROR_AllocateTriangle:		m_pLog->logMessage("Error loading map: Allocate Triangle.") ;			UnloadMap(false) ; return 0 ;
			case ERROR_InitializeFaces:			m_pLog->logMessage("Error loading map: Initialize Faces") ;				UnloadMap(false) ; return 0 ;
			case ERROR_ConvertFaces:				m_pLog->logMessage("Error loading map: Convert Faces.") ;					UnloadMap(false) ; return 0 ;
			case ERROR_ConvertPatches:			m_pLog->logMessage("Error loading map: Convert Patches.") ;				UnloadMap(false) ; return 0 ;
			case ERROR_ConvertTexLamp:			m_pLog->logMessage("Error loading map: Convert TexLamps.") ;			UnloadMap(false) ; return 0 ;
			case ERROR_ConvertLamps:				m_pLog->logMessage("Error loading map: Convert Lamps.") ;					UnloadMap(false) ; return 0 ;
			case ERROR_ConvertLampGlow:			m_pLog->logMessage("Error loading map: Convert Lamp Glow.") ;			UnloadMap(false) ; return 0 ;
			case ERROR_ConvertLightGlow:		m_pLog->logMessage("Error loading map: Convert Light Glow.") ;		UnloadMap(false) ; return 0 ;
			case ERROR_AssignTriangles:			m_pLog->logMessage("Error loading map: Assign Triangles") ;				UnloadMap(false) ; return 0 ;
			case ERROR_SortTriangles:				m_pLog->logMessage("Error loading map: Sort Triangles") ;					UnloadMap(false) ; return 0 ;
			case ERROR_SetupTransTextures:	m_pLog->logMessage("Error loading map: Setup Trans Textures") ;		UnloadMap(false) ; return 0 ;
			case ERROR_SortGroups:					m_pLog->logMessage("Error loading map: Sort Groups") ;						UnloadMap(false) ; return 0 ;
		}

		m_pLog->logMessage("post ParseMap") ;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// use the triangles to build our zone/texture manualObjects, which make up the map
		if(!ConstructMapFromTriangles()){ m_pLog->logMessage("Error loading map: Construct Map") ;		UnloadMap(false) ; return 0 ; }

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// delete all the stuff that was only needed for parsing the bsp and converting it into manaulObjects.
		m_Q3Map->FreeParseMem() ;
	


		

		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Create the scenenodes for the zones and attach the manualObjects

		int nZone=0 ;
		char chSceneNodeName[1024] ;
		int nMO=0 ;
		int nZGStart=0 ;
		int nZGEnd=0 ;

		char chMessage[1024] ;
		sprintf(chMessage, "Max Zones %i", m_Q3Map->m_nMaxZone) ; m_pLog->logMessage(chMessage) ;
		for(nZone=0 ; nZone<m_Q3Map->m_nMaxZone ; nZone++)
		{
			//sprintf(chMessage, "Zone %i Top", nZone) ; m_pLog->logMessage(chMessage) ;


			// create Opaque node if needed
			if(m_nOpaqueNodeUsed[nZone])
			{
				sprintf(chSceneNodeName, "Opaque%05i", nZone) ;
				m_pOpaqueNode[nZone]=m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName) ;
			}

			// create Trans node if needed
			if(m_nTransNodeUsed[nZone])
			{
				sprintf(chSceneNodeName, "Trans%05i", nZone) ;
				m_pTransNode[nZone]=m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName) ;
			}

			// create Lamp node if needed
			if(m_nLampNodeUsed[nZone])
			{
				sprintf(chSceneNodeName, "Lamp%05i", nZone) ;
				m_pLampNode[nZone]=m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName) ;
			}

			// create Glow node if needed
			if(m_nGlowNodeUsed[nZone])
			{
				sprintf(chSceneNodeName, "Glow%05i", nZone) ;
				m_pGlowNode[nZone]=m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName) ;
			}

			// get start and end of this zone's batches.  
			// If there's none, the start and end will be the same. (don't have to worry about checking for m_pZoneMO[nZone]==NULL)
			nZGStart	=	m_nZoneMOStart[nZone] ;
			nZGEnd		=	m_nZoneMOStart[nZone+1] ;
			
			//sprintf(chMessage, "  Zone %i, ZGStart %i, ZGEnd %i", nZone, nZGStart, nZGEnd) ; m_pLog->logMessage(chMessage) ;

			for(nMO=nZGStart ; nMO<nZGEnd ; nMO++)
			{
				//sprintf(chMessage, "    Zone %i, nMO %i", nZone, nMO) ; m_pLog->logMessage(chMessage) ;

				// attach the entity to the appropriate node

				if(m_pZoneEntityMaterialType[nMO]==MAT_OPAQUE)
					m_pOpaqueNode[nZone]->attachObject( m_pZoneEntity[nMO] ) ;
				else
				if(m_pZoneEntityMaterialType[nMO]==MAT_GEL)
					m_pTransNode[nZone]->attachObject( m_pZoneEntity[nMO] ) ;
				else
				if(m_pZoneEntityMaterialType[nMO]==MAT_LAMP)
					m_pLampNode[nZone]->attachObject( m_pZoneEntity[nMO] ) ;
				else
				if(m_pZoneEntityMaterialType[nMO]==MAT_GLOW)
					m_pGlowNode[nZone]->attachObject( m_pZoneEntity[nMO] ) ;
			}

		}


		// add the game entities
		if(!SetupGameEntities()) { m_pLog->logMessage("Error loading map: Setup Entities") ;		UnloadMap(false) ; return 0 ; }


		// prior to the first frame, all zone scenenodes are detached.
		m_pSceneMgr->getRootSceneNode()->removeAllChildren() ;

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Setup the lights



		
		initLight() ; 

		//if(m_nDebugLightBox)
		//	AddLightCullingBoxes() ;


		//if(m_nPortalDebug)
		//	AddPortalBoxes() ;


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// misc finalization of the map

		m_nCameraZone=-1 ; // indicate that we need to look up the zone the player is in.
		m_nCameraZoneL=-1 ;
		m_nCameraZoneR=-1 ;

		m_nMapLoaded=1 ; // map was loaded ok



		// set the camera's start position here.
		// I tried setting it during startup, but for some strange reason it didn't work IF the camera was in a zone 
		// but was ok if the camera started outside a zone.  Bit of a mystery... but it makes more sense to set it here anyway.
		m_pCamera->setPosition(Vector3(0, 0, 0));

		if(m_Q3Map->m_nLightMax>0) // make sure there's at least one light before we point to it!
			m_pCamera->lookAt(Ogre::Vector3(m_Q3Map->m_pLight[0].Position[0], m_Q3Map->m_pLight[0].Position[1], m_Q3Map->m_pLight[0].Position[2])) ;


		

		if(m_nGoggleMode==GOGGLE_MODE_TEST)
				CreateGogglesTestImage() ; // create the test goggles
			else
				if(m_nGoggleMode==GOGGLE_MODE_ON)
					CreateGoggles() ; // create the stereo goggles


		SetupParticles() ;


		m_pLog->logMessage("LoadMap bottom.") ;

		// reset the game timer
		m_GameTime=0.0 ;
		m_nFrame=0 ;



		//OFBug.MessageInt(m_nNewCount) ;

		return 1 ;
}

//''
int OgreFramework::SetupParticles()
{
			ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout

		

    // create some nice fireworks and place it at the origin
    //m_ps = m_pSceneMgr->createParticleSystem("Fireworks", "particle/fallspark00");
		m_ps = m_pSceneMgr->createParticleSystem("Fireworks", "particle/spark00");
		m_pParticleNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ParticleNode") ;
		m_pParticleNode->attachObject(m_ps) ;

		//m_pParticleNode->setPosition(360.0f, 360.0f, -860.0f) ;
		m_pParticleNode->setPosition(586.0f, 228.0f, -520.0f) ;


	return 1 ;
}




















// take the sorted list of triangles and contruct our Zone manualObjects array, m_pZoneMO, etc
// and setup all the materials.
int OgreFramework::ConstructMapFromTriangles(void)
{


	int nVert=0 ;
	int nMaxVertex=0 ;
	
	float flPosX=0.0f ;
	float flPosY=0.0f ;
	float flPosZ=0.0f ;
	float	flNormX=0.0f ;
	float	flNormY=0.0f ;
	float	flNormZ=0.0f ;
	float flTexU=0.0f ;
	float flTexV=0.0f ;

	float flLampPosX=0.0f ;
	float flLampPosY=0.0f ;
	float flLampPosZ=0.0f ;
	float flLampBrightness=0.0f ;
	float flLampR=0.0f ;
	float flLampG=0.0f ;
	float flLampB=0.0f ;

	int nVertA=0 ;
	int nVertB=0 ;
	int nVertC=0 ;

	char chMessage[1024] ;

	int nZone=0 ;
	char chManualName[1024] ; // name for manual objects we create
	char chMeshName[1024] ;

	int nTriangle=0 ;
//	int nStart=0 ;
	int nEnd=0 ;
	int nMaterial=0 ;
	int nGroup=0 ;
	int nTri=0 ;
	int nPos=0 ;

	ZeroMemory((void*)m_nOpaqueNodeUsed, sizeof(m_nOpaqueNodeUsed)) ; // set all as unused
	ZeroMemory((void*)m_nTransNodeUsed, sizeof(m_nTransNodeUsed)) ; // set all as unused
	ZeroMemory((void*)m_nLampNodeUsed, sizeof(m_nLampNodeUsed)) ; // set all as unused
	ZeroMemory((void*)m_nGlowNodeUsed, sizeof(m_nGlowNodeUsed)) ; // set all as unused
	
	char chMaterial_Base[1024] ;
	MaterialPtr pMaterial_Base ; // used for checking if material scripts exist, and stored for the material switching that happens in deferred shading
	
	char chMaterial_Fast[1024] ;
	MaterialPtr pMaterial_Fast ; // faster version of the base, mainly non-coloured shadowning.

	char chMaterial_Black[1024] ;
	MaterialPtr pMaterial_Black ; // just plain black, or black with alpha testing.

	char chMaterial_DfShColour[1024] ;
	MaterialPtr pMaterial_DfShColour ; // used for checking if material scripts exist, and stored for the material switching that happens in deferred shading
	

	char chMaterial_Shadow[1024] ;
	MaterialPtr pMaterial_Shadow ;

	char chMaterial_ShadeFront[1024] ;
	MaterialPtr pMaterial_ShadeFront ;

	char chMaterial_ShadeBack[1024] ;
	MaterialPtr pMaterial_ShadeBack ;

	char chMaterial_DfShPosition[1024] ;
	MaterialPtr pMaterial_DfShPosition ;
	
	char chMaterial_DfShDiffuse[1024] ;
	MaterialPtr pMaterial_DfShDiffuse ;
	
	//char chMaterial_DSNormal[1024] ;
	//MaterialPtr pMaterial_DSNormal ;
	
	char chMaterial_DfShFuncTNB[1024] ;
	MaterialPtr pMaterial_DfShFuncTNB ;

	char chMaterial_DfShSpecular[1024] ;
	MaterialPtr pMaterial_DfShSpecular ;

	char chMaterial_DfShEmissive[1024] ;
	MaterialPtr pMaterial_DfShEmissive ;

	//char chMaterial_DfShData[1024] ;
	//MaterialPtr pMaterial_DfShData ;

	char chMaterial_DfShMix[1024] ;
	MaterialPtr pMaterial_DfShMix ;

	char chCompare[1024] ;

	// for calculating bounds of mesh
	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;
//	float flMidX=0.0f ;
//	float flMidY=0.0f ;
//	float flMidZ=0.0f ;
	float flDisX=0.0f ;
	float flDisY=0.0f ;
	float flDisZ=0.0f ;
	float flRadius=0.0f ;
	AxisAlignedBox AABB ;
	unsigned short src, dest; // for tangent vectors


	// Work out how many manual objects we are going to need.  We need one for every group within every material within every zone.
	// Groups tend to all be 0 except for transparent triangles which have different groups within the same material/zone,
	// due to the need to sort and render transparent things in a specific depth order relative to the camera.


	int nMOCount=0 ;
	nMaterial=-1 ;
	nZone=-1 ;
	nGroup=-1 ;

	// loop through all triangles to count how many manual objects we'll need.
	for(nTri=0 ; nTri<m_Q3Map->m_nTriangleMax ; nTri++)
	{
		
		if(m_Q3Map->m_pTriangle[nTri].Zone!=nZone) // found a new zone
		{
			nZone=m_Q3Map->m_pTriangle[nTri].Zone ; 
			nMaterial=m_Q3Map->m_pTriangle[nTri].Texture ;
			nGroup=m_Q3Map->m_pTriangle[nTri].Group ;
			nMOCount++ ;
		}
		else
			if(m_Q3Map->m_pTriangle[nTri].Texture!=nMaterial) // found a new material within the current zone
			{
				nMaterial=m_Q3Map->m_pTriangle[nTri].Texture ;
				nGroup=m_Q3Map->m_pTriangle[nTri].Group ;
				nMOCount++ ;
			}
			else
				if(m_Q3Map->m_pTriangle[nTri].Group!=nGroup) // found a new group within the current material within the current zone
				{
					nGroup=m_Q3Map->m_pTriangle[nTri].Group ;
					nMOCount++ ;
				}

	}// end for tri
	


	// create the manualobject array for our zones and materials
	m_pZoneMO = new Ogre::ManualObject*[nMOCount] ;
	if(m_pZoneMO==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneMO]++ ; }

	m_pZoneMesh = new Ogre::MeshPtr[nMOCount] ;
	if(m_pZoneMesh==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneMesh]++ ; }

	// our entities for the meshes
	m_pZoneEntity = new Ogre::Entity*[nMOCount] ;
	if(m_pZoneEntity==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntity]++ ; }

	// need to keep track of which entities are transparent
	m_pZoneEntityMaterialType = new int[nMOCount] ;
	if(m_pZoneEntityMaterialType==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterialType]++ ; }
	ZeroMemory((void*)m_pZoneEntityMaterialType, nMOCount*sizeof(int)) ; // set all as non-transparent


	int nPTex=0 ;
	for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
	{
		// our base materials for the meshes
		m_pZoneEntityMaterial_Base[nPTex] = new Ogre::MaterialPtr[nMOCount] ;
		if(m_pZoneEntityMaterial_Base[nPTex]==NULL) 
			return 0 ; // fail, out of mem
		else
			{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_Base]++ ; }

		// our fast materials for the meshes
		m_pZoneEntityMaterial_Fast[nPTex] = new Ogre::MaterialPtr[nMOCount] ;
		if(m_pZoneEntityMaterial_Fast[nPTex]==NULL) 
			return 0 ; // fail, out of mem
		else
			{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_Fast]++ ; }
	}

	// our black materials for the meshes
	m_pZoneEntityMaterial_Black = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_Black==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_Black]++ ; }

	// our base materials for the meshes
	m_pZoneEntityMaterial_DfShColour = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShColour==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShColour]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_Shadow = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_Shadow==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_Shadow]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_ShadeFront = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_ShadeFront==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_ShadeFront]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_ShadeBack = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_ShadeBack==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_ShadeBack]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_DfShPosition = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShPosition==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShPosition]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_DfShDiffuse = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShDiffuse==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShDiffuse]++ ; }

	// materials for the meshes
	//m_pZoneEntityMaterial_DSNormal = new Ogre::MaterialPtr[nMOCount] ;
	//if(m_pZoneEntityMaterial_DSNormal==NULL) return 0 ; // fail, out of mem

	// materials for the meshes
	m_pZoneEntityMaterial_DfShFuncTNB = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShFuncTNB==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShFuncTNB]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_DfShSpecular = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShSpecular==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShSpecular]++ ; }

	// materials for the meshes
	m_pZoneEntityMaterial_DfShEmissive = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShEmissive==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShEmissive]++ ; }


	// materials for the meshes
	//m_pZoneEntityMaterial_DfShData = new Ogre::MaterialPtr[nMOCount] ;
	//if(m_pZoneEntityMaterial_DfShData==NULL) return 0 ; // fail, out of mem

	// materials for the meshes
	m_pZoneEntityMaterial_DfShMix = new Ogre::MaterialPtr[nMOCount] ;
	if(m_pZoneEntityMaterial_DfShMix==NULL) 
		return 0 ; // fail, out of mem
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pZoneEntityMaterial_DfShMix]++ ; }


	// memory for keeping track of re-assigned verts, since we only add verts to a manualobject if they are needed, we don't add every vertex
	int* pVertIndex = new int [m_Q3Map->m_nVertexMax] ; // at most, there can be m_Q3Map->m_nVertexMax verts in a zone if the map is one big subzone.
	if(pVertIndex==NULL) //{ CHECKDELETE_ARRAY( m_pZoneMO ) ; return 0 ; } // fail, out of mem
		return 0 ;
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_pVertIndex]++ ; }





	// go through all the zones and create their manualobjects in m_pZoneMO, one for each material
	int nMOIndex=0 ;
	for(nZone=0 ; nZone<m_Q3Map->m_nMaxZone ; nZone++)
	{
		m_nZoneTriangleCount[nZone]=0 ; // just for our stats


		m_nZoneMOStart[nZone]=nMOIndex ; // record where we start from.

		// because the triangles have been sorted in order of zone, if there are any triangles in this zone
		// then nTriangle will already be on the first example.  However it could be there is a zone that contains
		// no triangles.  If we have such an empty zone, we don't create any manual object for it and set its pointer
		// to NULL.

		if(m_Q3Map->m_pTriangle[nTriangle].Zone!=nZone)
		{
			m_pZoneMO[nMOIndex]=NULL ;
			continue ;
			//break ;
		}

		


		// scan through all triangles in this zone
		while((nTriangle<m_Q3Map->m_nTriangleMax) && (m_Q3Map->m_pTriangle[nTriangle].Zone==nZone))
		{
			

			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// start adding a begin/end block of all triangles in a zone with the same material/group
			//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




			
			nMaterial=m_Q3Map->m_pTriangle[nTriangle].Texture ;
			nGroup=m_Q3Map->m_pTriangle[nTriangle].Group ;
			

			

			// create the zone's manualobject
			sprintf(chManualName, "ZoneMO_%04i_%04i_%05i", nZone, nMaterial, nTriangle) ;
			m_pZoneMO[nMOIndex]=m_pSceneMgr->createManualObject(chManualName) ;
			
			

			// scan forward to find the end of this block of triangles with the same texture and group
			nEnd=nTriangle+1 ;
			while(
								(nEnd<m_Q3Map->m_nTriangleMax) 
							&&(m_Q3Map->m_pTriangle[nEnd].Zone==nZone)
							&&(m_Q3Map->m_pTriangle[nEnd].Texture==nMaterial)
							&&(m_Q3Map->m_pTriangle[nEnd].Group==nGroup)
						)
				nEnd++ ;

			// copy the material name.
			// q3 texture names are a max of 64 characters ( Q3NAMESIZE ) and may not be null terminated.  They have no extension either.
			nPos=-1 ;
			while((++nPos<Q3NAMESIZE) && (m_Q3Map->m_pTexture[nMaterial].name[nPos]!=' ') && (m_Q3Map->m_pTexture[nMaterial].name[nPos]!='\0'))
				chMaterial_Base[nPos]=m_Q3Map->m_pTexture[nMaterial].name[nPos] ;

			// make sure name is null terminated
			chMaterial_Base[nPos]='\0' ;



			//..

			//if(strcmp(chMaterial_Base, "textures/common/exptex")==0) strcpy(chMaterial_Base, "GEL_ShinyGlassPlateTL") ; 
			//if(strcmp(chMaterial_Base, "textures/common/exptex")==0) strcpy(chMaterial_Base, "GEL_DSC_Prototype") ; 
			//if(strcmp(chMaterial_Base, "textures/common/exptex")==0) strcpy(chMaterial_Base, "GEL_DSC_Prototype") ; 
			if(strcmp(chMaterial_Base, "textures/common/exptex")==0) strcpy(chMaterial_Base, "GEL_DSC_glassred") ; 
			if(strcmp(chMaterial_Base, "textures/common/exptex2")==0) strcpy(chMaterial_Base, "GEL_GreyGlass") ; 
			if(strcmp(chMaterial_Base, "textures/common/alphaplate")==0) strcpy(chMaterial_Base, "ALPHAPF_DSCE_Prototype") ;
			//if(strcmp(chMaterial_Base, "textures/common/alphaplate")==0) strcpy(chMaterial_Base, "OPAQUE_DSC_SimplePlateB") ;
			//if(strcmp(chMaterial_Base, "textures/common/alphaplate")==0) strcpy(chMaterial_Base, "ALPHAPF_DS_alphaplategreen") ;


			
			// if the material is a lamp material we need to specify either an OpenGl or Direct3D version
			strcpy(chCompare, chMaterial_Base) ;
			chCompare[12]='\0' ;
			if(strcmp("lights/lamp_", chCompare)==0) // material is LAMP 
			{
				if(m_IsOpenGL)
					strcat(chMaterial_Base, "_ogl") ;
				else
					strcat(chMaterial_Base, "_d3d") ;

			}

			strcpy(chCompare, chMaterial_Base) ;
			chCompare[17]='\0' ;
			if(strcmp("lights/lamp2pass_", chCompare)==0) // material is LAMP, 2 pass version 
			{
				if(m_IsOpenGL)
					strcat(chMaterial_Base, "_ogl") ;
				else
					strcat(chMaterial_Base, "_d3d") ;
			}
			


			// check that this material script exists, if not set to the default
			pMaterial_Base=MaterialManager::getSingleton().getByName(chMaterial_Base) ;
			if(pMaterial_Base.isNull())
			{
				strcat(chMaterial_Base, " *** MISSING ***") ;
//				OFBug.LogAddCR(chMaterial_Base) ;
				
				//sprintf(chMessage, "Missing material, using default: %s", chMaterial_Base) ;
				//m_pLog->logMessage(chMessage);

				/*
				switch(rand()%6)
				{
					case 0: strcpy(chMaterial_Base, "OPAQUE_DSC_Plate2x2A") ;					break ;
					case 1: strcpy(chMaterial_Base, "OPAQUE_DSC_HexColourA") ;				break ;
					case 2: strcpy(chMaterial_Base, "OPAQUE_DSC_SimplePlateB") ;			break ;
					case 3: strcpy(chMaterial_Base, "OPAQUE_DSC_FloorA") ;						break ;
					case 4: strcpy(chMaterial_Base, "OPAQUE_DSCE_WallTrimUD_A") ;			break ;
					case 5: strcpy(chMaterial_Base, "OPAQUE_DSCE_TechRadar") ;				break ;
				}
				*/

				//strcpy(chMaterial_Base, "OPAQUE_DSC_SimplePlateB") ;	

				//strcpy(chMaterial_Base, "OPAQUE_DSC_Plate2x2A") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSC_HexColourA") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSC_SimplePlateB") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSC_FloorA") ;
				strcpy(chMaterial_Base, "textures/metaltech/OPAQUE_DSCE_TechRadar") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSCE_WallTrimA") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSCE_WallTrimB") ;

				//strcpy(chMaterial_Base, "Standard_Normal") ;
				//strcpy(chMaterial_Base, "test") ;
				//strcpy(chMaterial_Base, "metalN") ;
				//strcpy(chMaterial_Base, "OPAQUE_DSC_Prototype") ;
				
				//strcpy(chMaterial_Base, "textures/common/exptex") ;
				//strcpy(chMaterial_Base, "metalN_DSNormal") ;
				//strcpy(chMaterial_Base, "metalN_DfShFuncTNB") ;
				//strcpy(chMaterial_Base, "metalN_DfShDiffuse") ;
				//strcpy(chMaterial_Base, "metalN_DfShPosition") ;
				//strcpy(chMaterial_Base, "metalD") ;
				//strcpy(chMaterial_Base, "metalR") ;	
				//strcpy(chMaterial_Base, "WonderShader_Lim3_Fast") ;
				//strcpy(chMaterial_Base, "reliefMaterial") ;
				//strcpy(chMaterial_Base, "tute02") ;reliefMaterial
				//strcpy(chMaterial_Base, "Examples/OffsetMapping/Specular") ;
				pMaterial_Base=MaterialManager::getSingleton().getByName(chMaterial_Base) ;
			}
			else
//				OFBug.LogAddCR(chMaterial_Base) ;



			if(nMOIndex==20)
			{
				sprintf(m_chBug, "****** %s *******", chMaterial_Base) ;
				m_pLog->logMessage(m_chBug);
			}




			// setup whether this material and it's entities are transparent or whatever

			if(strstr(chMaterial_Base, "OPAQUE")!=NULL) // material and its entity are OPAQUE
			{
				m_nOpaqueNodeUsed[nZone]=1 ;
				m_pZoneEntityMaterialType[nMOIndex]=MAT_OPAQUE ; // note that the derived entity will be opaque
			}
			else
			{

				if(strstr(chMaterial_Base, "ALPHAPF")!=NULL) // material and its entity are ALPHAPF
				{
					m_nOpaqueNodeUsed[nZone]=1 ;
					m_pZoneEntityMaterialType[nMOIndex]=MAT_OPAQUE ; // note that the derived entity will be opaque
				}
				else
				{

					if(strstr(chMaterial_Base, "GEL")!=NULL) // material and its entity are GEL
					{
						m_nTransNodeUsed[nZone]=1 ;
						m_pZoneEntityMaterialType[nMOIndex]=MAT_GEL ; // note that the derived entity will be transparent
					}
					else
					{

						strcpy(chCompare, chMaterial_Base) ;
						chCompare[12]='\0' ;
						if(strcmp("lights/lamp_", chCompare)==0) // material and its entity are LAMP (spotlights don't ever make triangles, only lamps)
						{
							m_nLampNodeUsed[nZone]=1 ;
							m_pZoneEntityMaterialType[nMOIndex]=MAT_LAMP ; // note that the derived entity will be lamp
						}
						else
						{
							strcpy(chCompare, chMaterial_Base) ;
							chCompare[17]='\0' ;
							if(strcmp("lights/lamp2pass_", chCompare)==0) // material and its entity are LAMP (spotlights don't ever make triangles, only lamps)
							{
								m_nLampNodeUsed[nZone]=1 ;
								m_pZoneEntityMaterialType[nMOIndex]=MAT_LAMP ; // note that the derived entity will be lamp
							}
							else
							{

								if(strstr(chMaterial_Base, "GLOW_")!=NULL) // material and its entity are GLOW
								{
									m_nGlowNodeUsed[nZone]=1 ;
									m_pZoneEntityMaterialType[nMOIndex]=MAT_GLOW ; // note that the derived entity will be glow
								}
							}	
						}					
					}
				}
			}






			///////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// Setup the different material names and pointers needed for various passes
			//
			// Lamps need only one material, pMaterial_Base, which is already assigned above, so it skips all this.
			//
			// Glow needs two materials, pMaterial_Base plus pMaterial_DfShColour for the coloured gel deferred pass.
			// However it's better to make a third, and duplicate the pMaterial_Base to pMaterial_Emissive, since
			// when glow gets rendered on the Emmisive pass it would be odd to switch to the base material instead
			// of the emmisive.
			//
			// Other objects need multiple materials for various different deferred shading and FX passes
			
			if((m_pZoneEntityMaterialType[nMOIndex]!=MAT_LAMP) && (m_pZoneEntityMaterialType[nMOIndex]!=MAT_GLOW))
			{

				// material for fast shading
				strcpy(chMaterial_Fast, chMaterial_Base) ;
				strcat(chMaterial_Fast, "_Fast") ;

				pMaterial_Fast=MaterialManager::getSingleton().getByName(chMaterial_Fast) ;
				if(pMaterial_Fast.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_Fast) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_Fast, "Fast") ;
					pMaterial_Fast=MaterialManager::getSingleton().getByName(chMaterial_Fast) ;
				}

				// material for black shading
				strcpy(chMaterial_Black, chMaterial_Base) ;
				strcat(chMaterial_Black, "_Black") ;
				pMaterial_Black=MaterialManager::getSingleton().getByName(chMaterial_Black) ;
				if(pMaterial_Black.isNull())
				{
					// only issue a warning if this is an alpha testing material, others don't need specialized Black materials
					if(strstr(chMaterial_Base, "ALPHAPF")!=NULL)
					{
						sprintf(chMessage, "Missing material, using default: %s", chMaterial_Black) ;
						m_pLog->logMessage(chMessage);
					}

					strcpy(chMaterial_Black, "Black") ;
					pMaterial_Black=MaterialManager::getSingleton().getByName(chMaterial_Black) ;
				}
				
				// material for shadow casting
				strcpy(chMaterial_Shadow, chMaterial_Base) ;
				strcat(chMaterial_Shadow, "_Shadow") ;
				pMaterial_Shadow=MaterialManager::getSingleton().getByName(chMaterial_Shadow) ;
				if(pMaterial_Shadow.isNull())
				{
					strcpy(chMaterial_Shadow, "OPAQUE_Shadow") ;
					pMaterial_Shadow=MaterialManager::getSingleton().getByName(chMaterial_Shadow) ;
				}

				
				
				// material for shadow casting
				strcpy(chMaterial_ShadeFront, chMaterial_Base) ;
				strcat(chMaterial_ShadeFront, "_ShadeFront") ;
				pMaterial_ShadeFront=MaterialManager::getSingleton().getByName(chMaterial_ShadeFront) ;
				if(pMaterial_ShadeFront.isNull())
				{
					 // standard opaque ShadeFront.  
					// Transparent surfaces MUST have their own _ShadeFront material or they will not colour light and can interfere with other trans surfaces.
					strcpy(chMaterial_ShadeFront, "OPAQUE_ShadeFront") ;

					pMaterial_ShadeFront=MaterialManager::getSingleton().getByName(chMaterial_ShadeFront) ;
				}


				// material for shadow casting
				strcpy(chMaterial_ShadeBack, chMaterial_Base) ;
				strcat(chMaterial_ShadeBack, "_ShadeBack") ;
				pMaterial_ShadeBack=MaterialManager::getSingleton().getByName(chMaterial_ShadeBack) ;
				if(pMaterial_ShadeBack.isNull())
				{
					 // standard opaque ShadeBack.  
					// Transparent surfaces MUST have their own _ShadeBack material or they will not colour light and can interfere with other trans surfaces.
					strcpy(chMaterial_ShadeBack, "OPAQUE_ShadeBack") ;

					pMaterial_ShadeBack=MaterialManager::getSingleton().getByName(chMaterial_ShadeBack) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShColour, chMaterial_Base) ;
				strcat(chMaterial_DfShColour, "_DfShColour") ;
				pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;
				if(pMaterial_DfShColour.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShColour) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShColour, "DfShColour") ;
					pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;
				}
				

				// material for deferred shading
				strcpy(chMaterial_DfShPosition, chMaterial_Base) ;
				strcat(chMaterial_DfShPosition, "_DfShPosition") ;
				pMaterial_DfShPosition=MaterialManager::getSingleton().getByName(chMaterial_DfShPosition) ;
				if(pMaterial_DfShPosition.isNull())
				{
					// only issue a warning if this is an alpha testing material, others don't need specialized DfShPosition materials
					if(strstr(chMaterial_Base, "ALPHAPF")!=NULL)
					{
						sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShPosition) ;
						m_pLog->logMessage(chMessage);
					}

					strcpy(chMaterial_DfShPosition, "DfShPosition") ;
					pMaterial_DfShPosition=MaterialManager::getSingleton().getByName(chMaterial_DfShPosition) ;
				}
				
				// material for deferred shading
				strcpy(chMaterial_DfShDiffuse, chMaterial_Base) ;
				strcat(chMaterial_DfShDiffuse, "_DfShDiffuse") ;
				pMaterial_DfShDiffuse=MaterialManager::getSingleton().getByName(chMaterial_DfShDiffuse) ;
				if(pMaterial_DfShDiffuse.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShDiffuse) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShDiffuse, "DfShDiffuse") ;
					pMaterial_DfShDiffuse=MaterialManager::getSingleton().getByName(chMaterial_DfShDiffuse) ;
				}

				/*
				// material for deferred shading // UNUSED
				strcpy(chMaterial_DSNormal, chMaterial_Base) ;
				strcat(chMaterial_DSNormal, "_DSNormal") ;
				pMaterial_DSNormal=MaterialManager::getSingleton().getByName(chMaterial_DSNormal) ;
				if(pMaterial_DSNormal.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DSNormal) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DSNormal, "DSNormal") ;
					pMaterial_DSNormal=MaterialManager::getSingleton().getByName(chMaterial_DSNormal) ;
				}
				*/

				// material for deferred shading
				strcpy(chMaterial_DfShFuncTNB, chMaterial_Base) ;
				strcat(chMaterial_DfShFuncTNB, "_DfShFuncTNB") ;
				pMaterial_DfShFuncTNB=MaterialManager::getSingleton().getByName(chMaterial_DfShFuncTNB) ;
				if(pMaterial_DfShFuncTNB.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShFuncTNB) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShFuncTNB, "DfShFuncTNB") ;
					pMaterial_DfShFuncTNB=MaterialManager::getSingleton().getByName(chMaterial_DfShFuncTNB) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShSpecular, chMaterial_Base) ;
				strcat(chMaterial_DfShSpecular, "_DfShSpecular") ;
				pMaterial_DfShSpecular=MaterialManager::getSingleton().getByName(chMaterial_DfShSpecular) ;
				if(pMaterial_DfShSpecular.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShSpecular) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShSpecular, "DfShSpecular") ;
					pMaterial_DfShSpecular=MaterialManager::getSingleton().getByName(chMaterial_DfShSpecular) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShEmissive, chMaterial_Base) ;
				strcat(chMaterial_DfShEmissive, "_DfShEmissive") ;
				pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
				if(pMaterial_DfShEmissive.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShEmissive) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShEmissive, "DfShEmissive") ;
					pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
				}


				/*
				// material for deferred shading
				strcpy(chMaterial_DfShData, chMaterial_Base) ;
				strcat(chMaterial_DfShData, "_DfShData") ;
				pMaterial_DfShData=MaterialManager::getSingleton().getByName(chMaterial_DfShData) ;
				if(pMaterial_DfShData.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShData) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShData, "DfShData") ; 
					pMaterial_DfShData=MaterialManager::getSingleton().getByName(chMaterial_DfShData) ;
				}
				*/

				// material for deferred shading
				strcpy(chMaterial_DfShMix, chMaterial_Base) ;
				strcat(chMaterial_DfShMix, "_DfShMix") ;
				pMaterial_DfShMix=MaterialManager::getSingleton().getByName(chMaterial_DfShMix) ;
				if(pMaterial_DfShMix.isNull())
				{
					// Pretty much all materials just use the default mix material,
					// so don't issue warnings if there isn't a specialized version
					//sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShMix) ;
					//m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShMix, "DfShMix") ;
					pMaterial_DfShMix=MaterialManager::getSingleton().getByName(chMaterial_DfShMix) ;
				}
				
			



			}// end if not MAT_LAMP or MAT_GLOW
			else
				if(m_pZoneEntityMaterialType[nMOIndex]==MAT_GLOW) // the two materials that glow needs
				{

					// when I get around to allowing user defined GLOW materials as well as the automatic ones,
					// this'll be the place to add it.  
					// For now, just the automatically generated lamp/light stuff has a glow.
					
					// if the base material was "GLOW_lamp"	
					strcpy(chCompare, chMaterial_Base) ;
					chCompare[strlen("GLOW_lamp")]='\0' ;
					if(strcmp("GLOW_lamp", chCompare)==0) 
					{
						strcpy(chMaterial_DfShColour, "GLOW_lampcol") ;
						pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;

						strcpy(chMaterial_DfShEmissive, "GLOW_lamp") ;
						pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
					}
					else
					{

						// if the base material was "GLOW_light"
						strcpy(chCompare, chMaterial_Base) ;
						chCompare[strlen("GLOW_light")]='\0' ;
						if(strcmp("GLOW_light", chCompare)==0) 
						{
							strcpy(chMaterial_DfShColour, "GLOW_lightcol") ;
							pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;

							strcpy(chMaterial_DfShEmissive, "GLOW_light") ;
							pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
						}

					}
				}






			// work out which vertices we'll need, we don't add every vertex, just used ones.
			nMaxVertex=0 ;
			for(nVert=0 ; nVert<m_Q3Map->m_nVertexMax ; nVert++)
				pVertIndex[nVert]=-1 ; // initialize all verts as unused.
			

			// start defining the manualObject
			m_pZoneMO[nMOIndex]->begin(chMaterial_Base, RenderOperation::OT_TRIANGLE_LIST) ;


			// reset the min max stuff
			flMinX=flMinY=flMinZ=MINMAXLIMIT ;
			flMaxX=flMaxY=flMaxZ=-MINMAXLIMIT ;
	

			// add the verts.  We avoid repeating the same vert multiple times where it is used by many triangles
			for(nTri=nTriangle ; nTri<nEnd ; nTri++)
				for(nVert=0 ; nVert<3 ; nVert++)
				{
					if(pVertIndex[ m_Q3Map->m_pTriangle[nTri].VIndex[nVert] ]==-1) // this vert hasn't been added yet
					{
						pVertIndex[ m_Q3Map->m_pTriangle[nTri].VIndex[nVert] ]=nMaxVertex++ ; // so we can correctly look it up when we add the triangles

						flPosX=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]  ].position[0] ;
						flPosY=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].position[1] ;
						flPosZ=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].position[2] ;
						flNormX=m_Q3Map->m_pVertex[ m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].normal[0] ;
						flNormY=m_Q3Map->m_pVertex[ m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].normal[1] ;
						flNormZ=m_Q3Map->m_pVertex[ m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].normal[2] ;
						
						
						
						// lamps have some different vertex data, also glow
						// lamp needs colour and also info about the origin, which is stored in the UVs.
						// glow only needs the colour, but it doesn't hurt to store the orgin too, no harm and simpler code.
						if((m_pZoneEntityMaterialType[nMOIndex]==MAT_LAMP) || (m_pZoneEntityMaterialType[nMOIndex]==MAT_GLOW))
						{
							flLampPosX =			m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[0][0] ;
							flLampPosY =			m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[0][1] ;
							flLampPosZ =			m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[1][0] ;
							flLampBrightness=	m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[1][1] ;

							flLampR=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].color[0]/255.0f ;
							flLampG=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].color[1]/255.0f ;
							flLampB=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].color[2]/255.0f ;

							// set the vertex data
							m_pZoneMO[nMOIndex]->position(flPosX, flPosY, flPosZ) ;
							m_pZoneMO[nMOIndex]->normal(flNormX, flNormY, flNormZ) ;
							m_pZoneMO[nMOIndex]->textureCoord(flLampPosX, flLampPosY, flLampPosZ, flLampBrightness) ;
							m_pZoneMO[nMOIndex]->colour(flLampR, flLampG, flLampB) ;

						}
						else // non-lamps
						{
							flTexU=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[0][0] ;
							flTexV=m_Q3Map->m_pVertex[	m_Q3Map->m_pTriangle[nTri].VIndex[nVert]	].texcoord[0][1] ;

							// set the vertex data
							m_pZoneMO[nMOIndex]->position(flPosX, flPosY, flPosZ) ;
							m_pZoneMO[nMOIndex]->normal(flNormX, flNormY, flNormZ) ;
							m_pZoneMO[nMOIndex]->textureCoord(flTexU, flTexV) ;
						}

						// update bounds
						if(flPosX<flMinX) flMinX=flPosX ;
						if(flPosY<flMinY) flMinY=flPosY ;
						if(flPosZ<flMinZ) flMinZ=flPosZ ;
						if(flPosX>flMaxX) flMaxX=flPosX ;
						if(flPosY>flMaxY) flMaxY=flPosY ;
						if(flPosZ>flMaxZ) flMaxZ=flPosZ ;

					}// end if adding vert
				}

			// add the triangles
			for(nTri=nTriangle ; nTri<nEnd ; nTri++)
			{
				nVertA=pVertIndex[ m_Q3Map->m_pTriangle[nTri].VIndex[0] ] ;
				nVertB=pVertIndex[ m_Q3Map->m_pTriangle[nTri].VIndex[1] ] ;
				nVertC=pVertIndex[ m_Q3Map->m_pTriangle[nTri].VIndex[2] ] ;
				m_pZoneMO[nMOIndex]->triangle(nVertA, nVertC, nVertB) ;
				m_nZoneTriangleCount[nZone]++ ;
				//m_pZoneMO[nMOIndex]->triangle(nVertA, nVertB, nVertC) ;
			}


			// all done for this block of material
			m_pZoneMO[nMOIndex]->end() ;
			m_pZoneMO[nMOIndex]->setCastShadows(true) ;
			m_pZoneMO[nMOIndex]->setDynamic(false) ;

			// convert the manualobject to a mesh
			sprintf(chMeshName, "ZoneMesh_%04i_%04i_%05i", nZone, nMaterial, nTriangle) ;
			m_pZoneMesh[nMOIndex]= m_pZoneMO[nMOIndex]->convertToMesh(chMeshName);

			// don't need the manual object anymore
			m_pSceneMgr->destroyManualObject(m_pZoneMO[nMOIndex]) ;


			// temp cludge to add thickness
			flMinX-=1.0f ;
			flMinY-=1.0f ;
			flMinZ-=1.0f ;
			flMaxX+=1.0f ;
			flMaxY+=1.0f ;
			flMaxZ+=1.0f ;

			// set axis aligned bounding box of this mesh
			AABB.setMinimumX(flMinX) ; 
			AABB.setMinimumY(flMinY) ; 
			AABB.setMinimumZ(flMinZ) ;
			AABB.setMaximumX(flMaxX) ; 
			AABB.setMaximumY(flMaxY) ; 
			AABB.setMaximumZ(flMaxZ) ;
			m_pZoneMesh[nMOIndex]->_setBounds(AABB, false) ;
			
			// set bounding sphere of this mesh
			flDisX=flMaxX-flMinX ;
			flDisY=flMaxY-flMinY ;
			flDisZ=flMaxZ-flMinZ ;
			flRadius=sqrt( flDisX*flDisX + flDisY*flDisY + flDisZ*flDisZ )/2.0 ;
			m_pZoneMesh[nMOIndex]->_setBoundingSphereRadius(flRadius) ;


			//if(strcmp(chMaterial_Base, "textures/metaltech/GEL_DSC_glassred")==0)
			//	OFBug.MessageFloat(flRadius, flMinX, flMinY, flMinZ, flMaxX, flMaxY, flMaxZ) ;

			//if(strcmp(chMaterial_Base, "textures/metaltech/GEL_DSC_glassgreen")==0)
			//	OFBug.MessageFloat(flRadius, flMinX, flMinY, flMinZ, flMaxX, flMaxY, flMaxZ) ;

			// calculate tangents for the mesh		
			if((m_pZoneEntityMaterialType[nMOIndex]!=MAT_LAMP) && (m_pZoneEntityMaterialType[nMOIndex]!=MAT_GLOW))// lamps and glow don't need tangent vectors
			{
				m_pZoneMesh[nMOIndex]->suggestTangentVectorBuildParams(VES_TANGENT, src, dest) ;
				m_pZoneMesh[nMOIndex]->buildTangentVectors(VES_TANGENT, src, dest);
			}
   	
			m_pZoneMesh[nMOIndex]->load() ;

			m_pZoneMesh[nMOIndex]->getSubMesh(0)->generateExtremes(8) ;

			// create an entity for this mesh.  We need this so we can change the material during deferred shading
			m_pZoneEntity[nMOIndex]=m_pSceneMgr->createEntity(m_pZoneMesh[nMOIndex]->getName(), m_pZoneMesh[nMOIndex]->getName()) ;

			// if the entity is a lamp or glow, set its material because they never get changed
			if((m_pZoneEntityMaterialType[nMOIndex]==MAT_LAMP) || (m_pZoneEntityMaterialType[nMOIndex]==MAT_GLOW)) 
				m_pZoneEntity[nMOIndex]->setMaterial( pMaterial_Base) ;


			//AABB=m_pZoneEntity[nMOIndex]->getBoundingBox() ;
			//Ogre::Vector3 mn, mx ;
			//mn=AABB.getMinimum() ;
			//mx=AABB.getMaximum() ;
			//if(strcmp(chMaterial_Base, "textures/metaltech/GEL_DSC_glassred")==0)
			//	OFBug.MessageFloat(mn.x, mn.y, mn.z, mx.x, mx.y, mx.z) ; 






			// store the pointers to the materials of this entity
			//m_pZoneEntityMaterial_Base[nMOIndex]=pMaterial_Base ;
			//m_pZoneEntityMaterial_Fast[nMOIndex]=pMaterial_Fast ;
			m_pZoneEntityMaterial_Black[nMOIndex]=pMaterial_Black ;
			m_pZoneEntityMaterial_DfShColour[nMOIndex]=pMaterial_DfShColour ;
			m_pZoneEntityMaterial_Shadow[nMOIndex]=pMaterial_Shadow ;
			m_pZoneEntityMaterial_ShadeFront[nMOIndex]=pMaterial_ShadeFront ;
			m_pZoneEntityMaterial_ShadeBack[nMOIndex]=pMaterial_ShadeBack ;
			m_pZoneEntityMaterial_DfShPosition[nMOIndex]=pMaterial_DfShPosition ;
			m_pZoneEntityMaterial_DfShDiffuse[nMOIndex]=pMaterial_DfShDiffuse ;
			//m_pZoneEntityMaterial_DSNormal[nMOIndex]=pMaterial_DSNormal ;
			m_pZoneEntityMaterial_DfShFuncTNB[nMOIndex]=pMaterial_DfShFuncTNB ;
			m_pZoneEntityMaterial_DfShSpecular[nMOIndex]=pMaterial_DfShSpecular ;
			m_pZoneEntityMaterial_DfShEmissive[nMOIndex]=pMaterial_DfShEmissive ;
			//m_pZoneEntityMaterial_DfShData[nMOIndex]=pMaterial_DfShData ;
			m_pZoneEntityMaterial_DfShMix[nMOIndex]=pMaterial_DfShMix ;


			//////////////////////////////////////////////////////////////////////////////////////////

			char chNewName[1024] ;
			for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
			{
				Ogre::AliasTextureNamePairList AliasList ;
				
				// check the necessary texture exists
				//if(TextureManager::getSingleton().getByName(m_Q3Map->m_chSpotlightTexture[nPTex]).isNull())
				//	OFBug.MessageInt(666) ;
				
				
				AliasList.insert(AliasTextureNamePairList::value_type("projector_tex", m_Q3Map->m_chSpotlightTexture[nPTex]));

				

				sprintf(chNewName, "%s_%i", chMaterial_Base, 0) ;
				m_pZoneEntityMaterial_Base[nPTex][nMOIndex]=pMaterial_Base->clone(chNewName) ;
				// change the texture unit
				m_pZoneEntityMaterial_Base[nPTex][nMOIndex]->applyTextureAliases(AliasList) ;

				sprintf(chNewName, "%s_%i", chMaterial_Fast, 0) ;
				m_pZoneEntityMaterial_Fast[nPTex][nMOIndex]=pMaterial_Fast->clone(chNewName) ;
				// change the texture unit
				m_pZoneEntityMaterial_Fast[nPTex][nMOIndex]->applyTextureAliases(AliasList) ;

			}



			//////////////////////////////////////////////////////////////////////////////////////////


			// move to the end of this block of materials
			nTriangle=nEnd ;
			nMOIndex++ ;


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// end adding a begin/end block of all triangles in a zone with the same material
			//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




		}// end scanning triangles in a zone



	}// end looping through all zones

	m_nZoneMOStart[m_Q3Map->m_nMaxZone]=nMOIndex ; // indicates the end pos of all the manual objects 

	CHECKDELETE_ARRAY( pVertIndex, NEW_CHECK_pVertIndex  );  // free the vert index memory we used

	CHECKDELETE_ARRAY( m_pZoneMO, NEW_CHECK_m_pZoneMO  ) ;  // free the manual object memory we used
	// quick check
	if(nMOCount!=nMOIndex)
	{
		sprintf(chMessage, "ERROR ConstructMap: MOIndex %i does not match MOCount %i", nMOIndex, nMOCount) ;
		m_pLog->logMessage(chMessage);
		//CHECKDELETE_ARRAY( m_pZoneMO ) ;
		CHECKDELETE_ARRAY( m_pZoneMesh, NEW_CHECK_m_pZoneMesh  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntity, NEW_CHECK_m_pZoneEntity  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterialType, NEW_CHECK_m_pZoneEntityMaterialType  ) ; 

		for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
		{
			CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Base[nPTex], NEW_CHECK_m_pZoneEntityMaterial_Base  ) ; 
			CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Fast[nPTex], NEW_CHECK_m_pZoneEntityMaterial_Fast  ) ; 
		}

		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Black, NEW_CHECK_m_pZoneEntityMaterial_Black  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShColour, NEW_CHECK_m_pZoneEntityMaterial_DfShColour  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_Shadow, NEW_CHECK_m_pZoneEntityMaterial_Shadow  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_ShadeFront, NEW_CHECK_m_pZoneEntityMaterial_ShadeFront  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_ShadeBack, NEW_CHECK_m_pZoneEntityMaterial_ShadeBack  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShPosition, NEW_CHECK_m_pZoneEntityMaterial_DfShPosition  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShDiffuse, NEW_CHECK_m_pZoneEntityMaterial_DfShDiffuse  ) ; 
		//CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DSNormal ) ;
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShFuncTNB, NEW_CHECK_m_pZoneEntityMaterial_DfShFuncTNB  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShSpecular, NEW_CHECK_m_pZoneEntityMaterial_DfShSpecular  ) ; 
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShEmissive, NEW_CHECK_m_pZoneEntityMaterial_DfShEmissive  ) ; 
		//CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShData ) ;
		CHECKDELETE_ARRAY( m_pZoneEntityMaterial_DfShMix, NEW_CHECK_m_pZoneEntityMaterial_DfShMix  ) ; 


		return 0 ;
	}


	return 1 ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// setup the game entities
//

int OgreFramework::SetupGameEntities(void)
{
	int nEntity=0 ;
	char chMeshName[1024] ;
	

	// work out the maximum number of entities we need
	m_nMaxEntity=10 ;
	int nHalfMax=m_nMaxEntity/2 ;
	



	// create the dynamic memory for the entities
	m_pEntityInfo=new ENTITYINFO[m_nMaxEntity] ;
	if(m_pEntityInfo==NULL) 
		return 0 ;
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pEntityInfo]++ ; }

	m_pVisibleEntity=new int[m_nMaxEntity] ;
	if(m_pVisibleEntity==NULL) 
		return 0 ;
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pVisibleEntity]++ ; }

	m_pFrustumEntity=new int[m_nMaxEntity] ;
	if(m_pFrustumEntity==NULL) 
		return 0 ;
	else
		{m_nNewCount++ ; m_nNewCheck[NEW_CHECK_m_pFrustumEntity]++ ; }



	// setup materials and default data for the entities
	for(nEntity=0 ; nEntity<m_nMaxEntity ; nEntity++)
	{
		//switch(nEntity%3)
		//{
		//	case 0: strcpy(chMeshName, "robot.mesh") ; break ;
		//	case 1: strcpy(chMeshName, "ninja.mesh") ; break ;
		//	case 2: strcpy(chMeshName, "athene.mesh") ; break ;
		//}

		strcpy(chMeshName, "robot.mesh") ;

		SetupEntity(nEntity, chMeshName) ;
	}




	// setup start data for each entity
	for(nEntity=0 ; nEntity<nHalfMax ; nEntity++)
	{
		m_pEntityInfo[nEntity].Postition=Ogre::Vector3(750.0f, 128.0f, -1950.0f-nEntity*40.0f) ;
		//m_pEntityInfo[nEntity].pMasterNode->setPosition(750.0f, 128.0f, -1950.0f-nEntity*40.0f) ;
	}

	for(nEntity=nHalfMax ; nEntity<m_nMaxEntity ; nEntity++)
	{
		m_pEntityInfo[nEntity].Postition=Ogre::Vector3(590.0f, 128.0f, -320.0f-nEntity*40.0f) ;
		//m_pEntityInfo[nEntity].pMasterNode->setPosition(590.0f, 128.0f, -320.0f-nEntity*40.0f) ;
	}

	

	for(nEntity=0 ; nEntity<m_nMaxEntity ; nEntity++)
	{
		m_pEntityInfo[nEntity].Active=1 ;
		m_pVisibleEntity[nEntity]=nEntity ;
		m_pFrustumEntity[nEntity]=nEntity ;
	}


	m_nMaxVisibleEntity=m_nMaxEntity ;
	m_nMaxFrustumEntity=m_nMaxEntity ;

	return 1 ;

}


int OgreFramework::SetupEntity(int nEntity, char *chMeshName)
{

	char chEntityName[1024] ;
	//char chMeshName[1024] ;


	int nSubMesh=0 ;
	int nMaxSubMesh=0 ;



	Ogre::String MaterialName ;


	char chMaterial_Base[1024] ;
	MaterialPtr pMaterial_Base ; // used for checking if material scripts exist, and stored for the material switching that happens in deferred shading
	
	char chMaterial_Fast[1024] ;
	MaterialPtr pMaterial_Fast ; // faster version of the base, mainly non-coloured shadowning.

	char chMaterial_Black[1024] ;
	MaterialPtr pMaterial_Black ; // just plain black, or black with alpha testing.

	char chMaterial_DfShColour[1024] ;
	MaterialPtr pMaterial_DfShColour ; // used for checking if material scripts exist, and stored for the material switching that happens in deferred shading
	

	char chMaterial_Shadow[1024] ;
	MaterialPtr pMaterial_Shadow ;

	char chMaterial_ShadeFront[1024] ;
	MaterialPtr pMaterial_ShadeFront ;

	char chMaterial_ShadeBack[1024] ;
	MaterialPtr pMaterial_ShadeBack ;

	char chMaterial_DfShPosition[1024] ;
	MaterialPtr pMaterial_DfShPosition ;
	
	char chMaterial_DfShDiffuse[1024] ;
	MaterialPtr pMaterial_DfShDiffuse ;
	
	//char chMaterial_DSNormal[1024] ;
	//MaterialPtr pMaterial_DSNormal ;
	
	char chMaterial_DfShFuncTNB[1024] ;
	MaterialPtr pMaterial_DfShFuncTNB ;

	char chMaterial_DfShSpecular[1024] ;
	MaterialPtr pMaterial_DfShSpecular ;

	char chMaterial_DfShEmissive[1024] ;
	MaterialPtr pMaterial_DfShEmissive ;

	//char chMaterial_DfShData[1024] ;
	//MaterialPtr pMaterial_DfShData ;

	char chMaterial_DfShMix[1024] ;
	MaterialPtr pMaterial_DfShMix ;

	char chCompare[1024] ;

	unsigned short src, dest; // for tangent vectors

	char chMasterNodeName[1024] ;
	char chSubNodeName[1024] ;

	char chMessage[1024] ;
	int nPTex=0 ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		sprintf(chEntityName, "Entity_%05i", nEntity) ;
		//strcpy(chMeshName, "robot.mesh") ;


		// load the mesh

		m_pEntityInfo[nEntity].pEnt=m_pSceneMgr->createEntity( chEntityName, chMeshName );

		m_pEntityInfo[nEntity].TriangleCount=0 ;
		

		// create the master scenenode
		sprintf(chMasterNodeName, "Entity_%05i", nEntity) ;
		m_pEntityInfo[nEntity].pMasterNode=m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chMasterNodeName) ;
		m_pEntityInfo[nEntity].pMasterNode->attachObject(m_pEntityInfo[nEntity].pEnt) ;

		nMaxSubMesh=m_pEntityInfo[nEntity].pEnt->getMesh()->getNumSubMeshes() ;
		if(nMaxSubMesh>MAX_SUBMESH)
		{
			sprintf(m_chBug, "MESH ERROR: Mesh %s has %i submeshes, max is %i", chMeshName, m_pEntityInfo[nEntity].MaxSubMesh, MAX_SUBMESH) ;
			m_pLog->logMessage(m_chBug);
			nMaxSubMesh=MAX_SUBMESH ;
		}
		m_pEntityInfo[nEntity].MaxSubMesh=nMaxSubMesh ;

		sprintf(m_chBug, "Entity %i, Mesh %s, MaxSubMesh %i", nEntity, chMeshName,  m_pEntityInfo[nEntity].MaxSubMesh) ;
		m_pLog->logMessage(m_chBug);

		// assign all the materials needed for all the submeshes
		for(nSubMesh=0 ; nSubMesh<nMaxSubMesh ; nSubMesh++)
		{
			// we count the triangles just for stats
			m_pEntityInfo[nEntity].TriangleCount += m_pEntityInfo[nEntity].pEnt->getMesh()->getSubMesh(nSubMesh)->indexData->indexCount/3 ;


			MaterialName=m_pEntityInfo[nEntity].pEnt->getMesh()->getSubMesh(nSubMesh)->getMaterialName() ;
		
			
			

			if(MaterialName.size()<512) // make sure name isn't too long before we copy it.
				strcpy(chMaterial_Base, MaterialName.c_str()) ;
			else
			{
				sprintf(m_chBug, "MESH ERROR: Mesh %s submesh %i material name is too long.", chMeshName, nSubMesh) ;
				m_pLog->logMessage(m_chBug);
				m_pLog->logMessage(MaterialName) ;
				chMaterial_Base[0]='\0' ; // no material
			}

			sprintf(m_chBug, "Entity %i SubMesh %i, Material %s", nEntity, nSubMesh, chMaterial_Base) ;
			m_pLog->logMessage(m_chBug);


			// if the material is a lamp material we need to specify either an OpenGl or Direct3D version
			strcpy(chCompare, chMaterial_Base) ;
			chCompare[12]='\0' ;
			if(strcmp("lights/lamp_", chCompare)==0) // material is LAMP 
			{
				if(m_IsOpenGL)
					strcat(chMaterial_Base, "_ogl") ;
				else
					strcat(chMaterial_Base, "_d3d") ;

			}

			strcpy(chCompare, chMaterial_Base) ;
			chCompare[17]='\0' ;
			if(strcmp("lights/lamp2pass_", chCompare)==0) // material is LAMP, 2 pass version 
			{
				if(m_IsOpenGL)
					strcat(chMaterial_Base, "_ogl") ;
				else
					strcat(chMaterial_Base, "_d3d") ;
			}
			
			// check that this material script exists, if not set to the default
			pMaterial_Base=MaterialManager::getSingleton().getByName(chMaterial_Base) ;
			if(true)//pMaterial_Base.isNull())
			{
				strcat(chMaterial_Base, " *** MISSING ***") ;
//				OFBug.LogAddCR(chMaterial_Base) ;
				
				strcpy(chMaterial_Base, "textures/metaltech/OPAQUE_DSCE_TechRadar") ;
				pMaterial_Base=MaterialManager::getSingleton().getByName(chMaterial_Base) ;
			}


			// setup whether this material and it's entities are transparent or whatever
			
			m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=0 ;// default to nothing
			
			if(strstr(chMaterial_Base, "OPAQUE")!=NULL) // material and its entity are OPAQUE
			{
				m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_OPAQUE ; // note that the derived entity will be opaque

			}
			else
			{

				if(strstr(chMaterial_Base, "ALPHAPF")!=NULL) // material and its entity are ALPHAPF
				{
					m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_OPAQUE ; // note that the derived entity will be opaque
				}
				else
				{

					if(strstr(chMaterial_Base, "GEL")!=NULL) // material and its entity are GEL
					{
						m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_GEL ; // note that the derived entity will be transparent
					}
					else
					{

						strcpy(chCompare, chMaterial_Base) ;
						chCompare[12]='\0' ;
						if(strcmp("lights/lamp_", chCompare)==0) // material and its entity are LAMP (spotlights don't ever make triangles, only lamps)
						{
							m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_LAMP ; // note that the derived entity will be lamp
						}
						else
						{
							strcpy(chCompare, chMaterial_Base) ;
							chCompare[17]='\0' ;
							if(strcmp("lights/lamp2pass_", chCompare)==0) // material and its entity are LAMP (spotlights don't ever make triangles, only lamps)
							{
								m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_LAMP ; // note that the derived entity will be lamp
							}
							else
							{

								if(strstr(chMaterial_Base, "GLOW_")!=NULL) // material and its entity are GLOW
								{
									m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]=MAT_GLOW ; // note that the derived entity will be glow
								}
							}	
						}					
					}
				}
			}



			

			///////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// Setup the different material names and pointers needed for various passes
			//
			// Lamps need only one material, pMaterial_Base, which is already assigned above, so it skips all this.
			//
			// Glow needs two materials, pMaterial_Base plus pMaterial_DfShColour for the coloured gel deferred pass.
			// However it's better to make a third, and duplicate the pMaterial_Base to pMaterial_Emissive, since
			// when glow gets rendered on the Emmisive pass it would be odd to switch to the base material instead
			// of the emmisive.
			//
			// Other objects need multiple materials for various different deferred shading and FX passes
			
			if((m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]!=MAT_LAMP) && (m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]!=MAT_GLOW))
			{

				// material for fast shading
				strcpy(chMaterial_Fast, chMaterial_Base) ;
				strcat(chMaterial_Fast, "_Fast") ;
				pMaterial_Fast=MaterialManager::getSingleton().getByName(chMaterial_Fast) ;
				if(pMaterial_Fast.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_Fast) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_Fast, "Fast") ;
					pMaterial_Fast=MaterialManager::getSingleton().getByName(chMaterial_Fast) ;
				}

				// material for black shading
				strcpy(chMaterial_Black, chMaterial_Base) ;
				strcat(chMaterial_Black, "_Black") ;
				pMaterial_Black=MaterialManager::getSingleton().getByName(chMaterial_Black) ;
				if(pMaterial_Black.isNull())
				{
					// only issue a warning if this is an alpha testing material, others don't need specialized Black materials
					if(strstr(chMaterial_Base, "ALPHAPF")!=NULL)
					{
						sprintf(chMessage, "Missing material, using default: %s", chMaterial_Black) ;
						m_pLog->logMessage(chMessage);
					}

					strcpy(chMaterial_Black, "Black") ;
					pMaterial_Black=MaterialManager::getSingleton().getByName(chMaterial_Black) ;
				}
				
				// material for shadow casting
				strcpy(chMaterial_Shadow, chMaterial_Base) ;
				strcat(chMaterial_Shadow, "_Shadow") ;
				pMaterial_Shadow=MaterialManager::getSingleton().getByName(chMaterial_Shadow) ;
				if(pMaterial_Shadow.isNull())
				{
					strcpy(chMaterial_Shadow, "OPAQUE_Shadow") ;
					pMaterial_Shadow=MaterialManager::getSingleton().getByName(chMaterial_Shadow) ;
				}

				
				
				// material for shadow casting
				strcpy(chMaterial_ShadeFront, chMaterial_Base) ;
				strcat(chMaterial_ShadeFront, "_ShadeFront") ;
				pMaterial_ShadeFront=MaterialManager::getSingleton().getByName(chMaterial_ShadeFront) ;
				if(pMaterial_ShadeFront.isNull())
				{
					 // standard opaque ShadeFront.  
					// Transparent surfaces MUST have their own _ShadeFront material or they will not colour light and can interfere with other trans surfaces.
					strcpy(chMaterial_ShadeFront, "OPAQUE_ShadeFront") ;

					pMaterial_ShadeFront=MaterialManager::getSingleton().getByName(chMaterial_ShadeFront) ;
				}


				// material for shadow casting
				strcpy(chMaterial_ShadeBack, chMaterial_Base) ;
				strcat(chMaterial_ShadeBack, "_ShadeBack") ;
				pMaterial_ShadeBack=MaterialManager::getSingleton().getByName(chMaterial_ShadeBack) ;
				if(pMaterial_ShadeBack.isNull())
				{
					 // standard opaque ShadeBack.  
					// Transparent surfaces MUST have their own _ShadeBack material or they will not colour light and can interfere with other trans surfaces.
					strcpy(chMaterial_ShadeBack, "OPAQUE_ShadeBack") ;

					pMaterial_ShadeBack=MaterialManager::getSingleton().getByName(chMaterial_ShadeBack) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShColour, chMaterial_Base) ;
				strcat(chMaterial_DfShColour, "_DfShColour") ;
				pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;
				if(pMaterial_DfShColour.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShColour) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShColour, "DfShColour") ;
					pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;
				}
				

				// material for deferred shading
				strcpy(chMaterial_DfShPosition, chMaterial_Base) ;
				strcat(chMaterial_DfShPosition, "_DfShPosition") ;
				pMaterial_DfShPosition=MaterialManager::getSingleton().getByName(chMaterial_DfShPosition) ;
				if(pMaterial_DfShPosition.isNull())
				{
					// only issue a warning if this is an alpha testing material, others don't need specialized DfShPosition materials
					if(strstr(chMaterial_Base, "ALPHAPF")!=NULL)
					{
						sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShPosition) ;
						m_pLog->logMessage(chMessage);
					}

					strcpy(chMaterial_DfShPosition, "DfShPosition") ;
					pMaterial_DfShPosition=MaterialManager::getSingleton().getByName(chMaterial_DfShPosition) ;
				}
				
				// material for deferred shading
				strcpy(chMaterial_DfShDiffuse, chMaterial_Base) ;
				strcat(chMaterial_DfShDiffuse, "_DfShDiffuse") ;
				pMaterial_DfShDiffuse=MaterialManager::getSingleton().getByName(chMaterial_DfShDiffuse) ;
				if(pMaterial_DfShDiffuse.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShDiffuse) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShDiffuse, "DfShDiffuse") ;
					pMaterial_DfShDiffuse=MaterialManager::getSingleton().getByName(chMaterial_DfShDiffuse) ;
				}

				/*
				// material for deferred shading // UNUSED
				strcpy(chMaterial_DSNormal, chMaterial_Base) ;
				strcat(chMaterial_DSNormal, "_DSNormal") ;
				pMaterial_DSNormal=MaterialManager::getSingleton().getByName(chMaterial_DSNormal) ;
				if(pMaterial_DSNormal.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DSNormal) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DSNormal, "DSNormal") ;
					pMaterial_DSNormal=MaterialManager::getSingleton().getByName(chMaterial_DSNormal) ;
				}
				*/

				// material for deferred shading
				strcpy(chMaterial_DfShFuncTNB, chMaterial_Base) ;
				strcat(chMaterial_DfShFuncTNB, "_DfShFuncTNB") ;
				pMaterial_DfShFuncTNB=MaterialManager::getSingleton().getByName(chMaterial_DfShFuncTNB) ;
				if(pMaterial_DfShFuncTNB.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShFuncTNB) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShFuncTNB, "DfShFuncTNB") ;
					pMaterial_DfShFuncTNB=MaterialManager::getSingleton().getByName(chMaterial_DfShFuncTNB) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShSpecular, chMaterial_Base) ;
				strcat(chMaterial_DfShSpecular, "_DfShSpecular") ;
				pMaterial_DfShSpecular=MaterialManager::getSingleton().getByName(chMaterial_DfShSpecular) ;
				if(pMaterial_DfShSpecular.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShSpecular) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShSpecular, "DfShSpecular") ;
					pMaterial_DfShSpecular=MaterialManager::getSingleton().getByName(chMaterial_DfShSpecular) ;
				}

				// material for deferred shading
				strcpy(chMaterial_DfShEmissive, chMaterial_Base) ;
				strcat(chMaterial_DfShEmissive, "_DfShEmissive") ;
				pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
				if(pMaterial_DfShEmissive.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShEmissive) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShEmissive, "DfShEmissive") ;
					pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
				}


				/*
				// material for deferred shading
				strcpy(chMaterial_DfShData, chMaterial_Base) ;
				strcat(chMaterial_DfShData, "_DfShData") ;
				pMaterial_DfShData=MaterialManager::getSingleton().getByName(chMaterial_DfShData) ;
				if(pMaterial_DfShData.isNull())
				{
					sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShData) ;
					m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShData, "DfShData") ; 
					pMaterial_DfShData=MaterialManager::getSingleton().getByName(chMaterial_DfShData) ;
				}
				*/

				// material for deferred shading
				strcpy(chMaterial_DfShMix, chMaterial_Base) ;
				strcat(chMaterial_DfShMix, "_DfShMix") ;
				pMaterial_DfShMix=MaterialManager::getSingleton().getByName(chMaterial_DfShMix) ;
				if(pMaterial_DfShMix.isNull())
				{
					// Pretty much all materials just use the default mix material,
					// so don't issue warnings if there isn't a specialized version
					//sprintf(chMessage, "Missing material, using default: %s", chMaterial_DfShMix) ;
					//m_pLog->logMessage(chMessage);

					strcpy(chMaterial_DfShMix, "DfShMix") ;
					pMaterial_DfShMix=MaterialManager::getSingleton().getByName(chMaterial_DfShMix) ;
				}
				
			



			}// end if not MAT_LAMP or MAT_GLOW
			else
				if(m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]==MAT_GLOW) // the two materials that glow needs
				{

					// when I get around to allowing user defined GLOW materials as well as the automatic ones,
					// this'll be the place to add it.  
					// For now, just the automatically generated lamp/light stuff has a glow.
					
					// if the base material was "GLOW_lamp"	
					strcpy(chCompare, chMaterial_Base) ;
					chCompare[strlen("GLOW_lamp")]='\0' ;
					if(strcmp("GLOW_lamp", chCompare)==0) 
					{
						strcpy(chMaterial_DfShColour, "GLOW_lampcol") ;
						pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;

						strcpy(chMaterial_DfShEmissive, "GLOW_lamp") ;
						pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
					}
					else
					{

						// if the base material was "GLOW_light"
						strcpy(chCompare, chMaterial_Base) ;
						chCompare[strlen("GLOW_light")]='\0' ;
						if(strcmp("GLOW_light", chCompare)==0) 
						{
							strcpy(chMaterial_DfShColour, "GLOW_lightcol") ;
							pMaterial_DfShColour=MaterialManager::getSingleton().getByName(chMaterial_DfShColour) ;

							strcpy(chMaterial_DfShEmissive, "GLOW_light") ;
							pMaterial_DfShEmissive=MaterialManager::getSingleton().getByName(chMaterial_DfShEmissive) ;
						}

					}
				}




			// if the entity is a lamp or glow, set its material because they never get changed
			//if((m_pZoneEntityMaterialType[nMOIndex]==MAT_LAMP) || (m_pZoneEntityMaterialType[nMOIndex]==MAT_GLOW)) 
			//	m_pZoneEntity[nMOIndex]->setMaterial( pMaterial_Base) ;


			// store the pointers to the materials of this entity
			//m_pZoneEntityMaterial_Base[nMOIndex]=pMaterial_Base ;
			//m_pZoneEntityMaterial_Fast[nMOIndex]=pMaterial_Fast ;
			m_pEntityInfo[nEntity].Material_Black[nSubMesh]=pMaterial_Black ;
			m_pEntityInfo[nEntity].Material_DfShColour[nSubMesh]=pMaterial_DfShColour ;
			m_pEntityInfo[nEntity].Material_Shadow[nSubMesh]=pMaterial_Shadow ;
			m_pEntityInfo[nEntity].Material_ShadeFront[nSubMesh]=pMaterial_ShadeFront ;
			m_pEntityInfo[nEntity].Material_ShadeBack[nSubMesh]=pMaterial_ShadeBack ;
			m_pEntityInfo[nEntity].Material_DfShPosition[nSubMesh]=pMaterial_DfShPosition ;
			m_pEntityInfo[nEntity].Material_DfShDiffuse[nSubMesh]=pMaterial_DfShDiffuse ;
			//m_pEntityInfo[nEntity].Material_DSNormal[nSubMesh]=pMaterial_DSNormal ;
			m_pEntityInfo[nEntity].Material_DfShFuncTNB[nSubMesh]=pMaterial_DfShFuncTNB ;
			m_pEntityInfo[nEntity].Material_DfShSpecular[nSubMesh]=pMaterial_DfShSpecular ;
			m_pEntityInfo[nEntity].Material_DfShEmissive[nSubMesh]=pMaterial_DfShEmissive ;
			//m_pEntityInfo[nEntity].Material_DfShData[nSubMesh]=pMaterial_DfShData ;
			m_pEntityInfo[nEntity].Material_DfShMix[nSubMesh]=pMaterial_DfShMix ;

			
			//////////////////////////////////////////////////////////////////////////////////////////

			char chNewName[1024] ;
			for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
			{
				Ogre::AliasTextureNamePairList AliasList ;
				
				// check the necessary texture exists
				//if(TextureManager::getSingleton().getByName(m_Q3Map->m_chSpotlightTexture[nPTex]).isNull())
				//	OFBug.MessageInt(666) ;
				
				AliasList.insert(AliasTextureNamePairList::value_type("projector_tex", m_Q3Map->m_chSpotlightTexture[nPTex]));

				sprintf(chNewName, "%s_%i", chMaterial_Base, 0) ;
				m_pEntityInfo[nEntity].Material_Base[nPTex][nSubMesh]=pMaterial_Base->clone(chNewName) ;
				// change the texture unit
				m_pEntityInfo[nEntity].Material_Base[nPTex][nSubMesh]->applyTextureAliases(AliasList) ;

				sprintf(chNewName, "%s_%i", chMaterial_Fast, 0) ;
				m_pEntityInfo[nEntity].Material_Fast[nPTex][nSubMesh]=pMaterial_Fast->clone(chNewName) ;
				// change the texture unit
				m_pEntityInfo[nEntity].Material_Fast[nPTex][nSubMesh]->applyTextureAliases(AliasList) ;

			}

			// create the sub scenenodes
			//sprintf(chSubNodeName, "Entity_%05i_%05i", nEntity, nSubMesh) ;
			//m_pEntityInfo[nEntity].pSubNode[nSubMesh]=m_pEntityInfo[nEntity].pMasterNode->createChildSceneNode(chSubNodeName) ;


			sprintf(m_chBug, "Node %s, Subnode %s, Type %i", chMasterNodeName, chSubNodeName, m_pEntityInfo[nEntity].SubMeshMaterialType[nSubMesh]) ;
			m_pLog->logMessage(m_chBug);
		
		}// end for submesh

		// build tangent vectors for the mesh
		m_pEntityInfo[nEntity].pEnt->getMesh()->suggestTangentVectorBuildParams(VES_TANGENT, src, dest) ;
		m_pEntityInfo[nEntity].pEnt->getMesh()->buildTangentVectors(VES_TANGENT, src, dest);


	// default to not active, not visible and in no zones
	m_pEntityInfo[nEntity].Active=0 ;
	m_pEntityInfo[nEntity].Visible=0 ;
	m_pEntityInfo[nEntity].Zone[INDEX_ENTITYZONECOUNT]=0 ;

	// add some info about the bounding box
	Ogre::AxisAlignedBox aabb ;
	Ogre::Vector3 Minimum ;
	Ogre::Vector3 Maximum ;

	aabb=m_pEntityInfo[nEntity].pEnt->getBoundingBox() ;
	Minimum=aabb.getMinimum() ;
	Maximum=aabb.getMaximum() ;

	m_pEntityInfo[nEntity].AABB=aabb ;
	m_pEntityInfo[nEntity].Centre.x=(Maximum.x-Minimum.x)/2.0f ;
	m_pEntityInfo[nEntity].Centre.y=(Maximum.y-Minimum.y)/2.0f ;
	m_pEntityInfo[nEntity].Centre.z=(Maximum.z-Minimum.z)/2.0f ;

	m_pEntityInfo[nEntity].AABBMin[0]=Minimum.x ;
	m_pEntityInfo[nEntity].AABBMin[1]=Minimum.y ;
	m_pEntityInfo[nEntity].AABBMin[2]=Minimum.z ;
	m_pEntityInfo[nEntity].AABBMax[0]=Maximum.x ;
	m_pEntityInfo[nEntity].AABBMax[1]=Maximum.y ;
	m_pEntityInfo[nEntity].AABBMax[2]=Maximum.z ;


	m_pEntityInfo[nEntity].Postition=Ogre::Vector3(0.0f, 0.0f, 0.0f) ;



	return 1 ;
}


// Hmm... wanted to control some CG compiling, mainly to turn shadow filtering on or off, but I couln't get this to work.
// I ended up just manually creating filtered and unfiltered version of the materials.
/*
int OgreFramework::ApplyCGDefines(void)
{
	int nZoneMO=0 ;
	int nMaxZoneMO=m_nZoneMOStart[m_Q3Map->m_nMaxZone] ;
	int nPTex=0 ;
	
	Ogre::Pass *CGPass ;
	int nPass=0 ;
	int nMaxPass=0 ;

	for(nZoneMO=0 ; nZoneMO<nMaxZoneMO ; nZoneMO++)
	{
		//OFBug.MessageInt(nZoneMO, nMaxZoneMO, m_pZoneEntityMaterialType[nZoneMO]) ;
		
		if(m_pZoneEntityMaterialType[nZoneMO]==MAT_OPAQUE)
			for(nPTex=0 ; nPTex<MAX_PROJECTORTEX ; nPTex++)
			{
				//m_pZoneEntityMaterial_Base[nPTex][nZoneMO]->getTechnique(0)->getPass(1)->getFragmentProgram()->setParameter("compile_arguments", "-DFILTER_ON 1") ;
				nMaxPass=m_pZoneEntityMaterial_Base[nPTex][nZoneMO]->getTechnique(0)->getNumPasses() ;
				
				for(nPass=0 ; nPass<nMaxPass ; nPass++)
				{
					m_pZoneEntityMaterial_Base[nPTex][nZoneMO]->getTechnique(0)->getPass(nPass)->getFragmentProgram()->setParameter("compile_arguments", "-DFILTER_ON=1") ;
					//m_pZoneEntityMaterial_Base[nPTex][nZoneMO]->getTechnique(0)->getPass(nPass)->getFragmentProgram()->reload();
				}	
				//CGPass=m_pZoneEntityMaterial_Base[nPTex][nZoneMO]->getTechnique(0)->getPass(0) ;
			}
	}
	

	//m_pZoneEntityMaterial_Base[0][0]->getTechnique(0)->getPass(1)->getFragmentProgram()->setParameter("compile_arguments", "-DFILTER_ON 1") ;
		

	return 1 ;


}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create light manual objects, just for debugging/testing.
// This is an array of manualobject AABBs that show the light culling box (double sided) for each light

int OgreFramework::AddLightCullingBoxes()
{
	int nLt=0 ;
	// for calculating bounds of mesh
	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;
	int nMaxLight=m_Q3Map->m_nLightMax ;

	char chSceneNodeName[1024] ;
	char chManualName[1024] ;

	m_pLightMO = new Ogre::ManualObject*[nMaxLight] ;
	if(m_pLightMO==NULL) return 0 ; // fail, out of mem

	for(nLt=0 ; nLt<nMaxLight ; nLt++)
	{
		
		sprintf(chManualName, "LightMO_%05i", nLt) ;
		m_pLightMO[nLt]=m_pSceneMgr->createManualObject(chManualName) ;

		// start defining the manualObject
		m_pLightMO[nLt]->begin("lighttest", RenderOperation::OT_TRIANGLE_LIST) ;

		
		flMinX=m_Q3Map->m_pLight[nLt].Min[0] ;
		flMinY=m_Q3Map->m_pLight[nLt].Min[1] ;
		flMinZ=m_Q3Map->m_pLight[nLt].Min[2] ;

		flMaxX=m_Q3Map->m_pLight[nLt].Max[0] ;
		flMaxY=m_Q3Map->m_pLight[nLt].Max[1] ;
		flMaxZ=m_Q3Map->m_pLight[nLt].Max[2] ;
		
		
		//////////////////////////////////////////////////////
		// back face
		m_pLightMO[nLt]->position(flMinX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->position(flMinX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->quad(0, 1, 2, 3) ;
		m_pLightMO[nLt]->quad(3, 2, 1, 0) ;

		//////////////////////////////////////////////////////
		// front face
		m_pLightMO[nLt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->position(flMinX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->quad(7, 6, 5, 4) ;
		m_pLightMO[nLt]->quad(4, 5, 6, 7) ;

		//////////////////////////////////////////////////////
		// left face
		m_pLightMO[nLt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMinX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->position(flMinX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->position(flMinX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->quad(8, 9, 10, 11) ;
		m_pLightMO[nLt]->quad(11, 10, 9, 8) ;

		
		//////////////////////////////////////////////////////
		// right face
		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, 1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->quad(15, 14, 13, 12) ;
		m_pLightMO[nLt]->quad(12, 13, 14, 15) ;

		//////////////////////////////////////////////////////
		// top face
		m_pLightMO[nLt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMinX, flMaxY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->quad(16, 17, 18, 19) ;
		m_pLightMO[nLt]->quad(19, 18, 17, 16) ;

		//////////////////////////////////////////////////////
		// bottom face
		m_pLightMO[nLt]->position(flMinX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 0.0) ;

		m_pLightMO[nLt]->position(flMaxX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(1.0, 1.0) ;

		m_pLightMO[nLt]->position(flMinX, flMinY, flMinZ) ;
		m_pLightMO[nLt]->normal(0.0, 0.0, -1) ;
		m_pLightMO[nLt]->textureCoord(0.0, 1.0) ;

		m_pLightMO[nLt]->quad(23, 22, 21, 20) ;
		m_pLightMO[nLt]->quad(20, 21, 22, 23) ;
		//////////////////////////////////////////////////////

		m_pLightMO[nLt]->end() ;



		sprintf(chSceneNodeName, "LSN%05i", nLt) ;
		m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName)->attachObject(m_pLightMO[nLt]) ;

		
	}

	return 1 ;
}

int OgreFramework::AddPortalBoxes()
{
	int nPt=0 ;
	// for calculating bounds of mesh
	float flMinX=0.0f ;
	float flMinY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxX=0.0f ;
	float flMaxY=0.0f ;
	float flMaxZ=0.0f ;
	int nMaxPortal=m_Q3Map->m_iNumPortals ;

	char chSceneNodeName[1024] ;
	char chManualName[1024] ;

	m_pPortalMO = new Ogre::ManualObject*[nMaxPortal] ;
	if(m_pPortalMO==NULL) return 0 ; // fail, out of mem

	m_pPortalNode = new Ogre::Node*[nMaxPortal] ;
	if(m_pPortalNode==NULL) 
	{
		//CHECKDELETE_ARRAY( m_pPortalMO ) ;
		return 0 ; // fail, out of mem
	}

	for(nPt=0 ; nPt<nMaxPortal ; nPt++)
	{
		
		sprintf(chManualName, "PortalMO_%05i", nPt) ;
		m_pPortalMO[nPt]=m_pSceneMgr->createManualObject(chManualName) ;

		// start defining the manualObject
		m_pPortalMO[nPt]->begin("white", RenderOperation::OT_TRIANGLE_LIST) ;

		flMinX=m_Q3Map->m_pPortals[nPt].Min[0] ;
		flMinY=m_Q3Map->m_pPortals[nPt].Min[1] ;
		flMinZ=m_Q3Map->m_pPortals[nPt].Min[2] ;

		flMaxX=m_Q3Map->m_pPortals[nPt].Max[0] ;
		flMaxY=m_Q3Map->m_pPortals[nPt].Max[1] ;
		flMaxZ=m_Q3Map->m_pPortals[nPt].Max[2] ;
		
		
		//////////////////////////////////////////////////////
		// back face
		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->quad(0, 1, 2, 3) ;
		m_pPortalMO[nPt]->quad(3, 2, 1, 0) ;

		//////////////////////////////////////////////////////
		// front face
		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->quad(7, 6, 5, 4) ;
		m_pPortalMO[nPt]->quad(4, 5, 6, 7) ;

		//////////////////////////////////////////////////////
		// left face
		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->quad(8, 9, 10, 11) ;
		m_pPortalMO[nPt]->quad(11, 10, 9, 8) ;

		
		//////////////////////////////////////////////////////
		// right face
		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, 1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->quad(15, 14, 13, 12) ;
		m_pPortalMO[nPt]->quad(12, 13, 14, 15) ;

		//////////////////////////////////////////////////////
		// top face
		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMaxY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->quad(16, 17, 18, 19) ;
		m_pPortalMO[nPt]->quad(19, 18, 17, 16) ;

		//////////////////////////////////////////////////////
		// bottom face
		m_pPortalMO[nPt]->position(flMinX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMaxZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 0.0) ;

		m_pPortalMO[nPt]->position(flMaxX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(1.0, 1.0) ;

		m_pPortalMO[nPt]->position(flMinX, flMinY, flMinZ) ;
		m_pPortalMO[nPt]->normal(0.0, 0.0, -1) ;
		m_pPortalMO[nPt]->textureCoord(0.0, 1.0) ;

		m_pPortalMO[nPt]->quad(23, 22, 21, 20) ;
		m_pPortalMO[nPt]->quad(20, 21, 22, 23) ;
		//////////////////////////////////////////////////////

		m_pPortalMO[nPt]->end() ;



		sprintf(chSceneNodeName, "PSN%05i", nPt) ;
		m_pSceneMgr->getRootSceneNode()->createChildSceneNode(chSceneNodeName)->attachObject(m_pPortalMO[nPt]) ;
		m_pPortalNode[nPt]=m_pSceneMgr->getRootSceneNode()->getChild(chSceneNodeName) ;
		
	}

	return 1 ;
}















///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// this function calculates which zones are visible.  
// If they are in the view frustum and any connecting portals are open, they're visible.  
// It calls a recursive functions that goes into each visible zone and scans it's portals 
// for more visible zones, cutting down the frustum as it goes.
//
// This function also checks which lights are visible.
// First it scans the original zone the camera is in, any light whose centre is in that zone
// is visible regardless of how many other zones that light touches.
// Lights that are completely contained in any of the further zones are checked against the
// current cut down frustum.
void OgreFramework::CalculateZoneVisibility(Ogre::Camera *pCamera, int *pCameraZone, char* pZoneVis, char* pLightVis, char* pSubLightVis, char* pPortalVis, int* pMaxVisibleLight, unsigned short* pVisibleLightList)
{

	
	int nZone=0 ;
//	int nZoneIndex=0 ;
	int nSubZone=0 ;
//	int nSubZoneIndex=0 ;
	int nMaxSubZone=0 ;
	int nPortal=0 ;
//	int nPortalIndex=0 ;
	int nMaxPortal=0 ;
//	int nMaxPortalZone=0 ;
//	int nPortalZone=0 ;
//	int nPortalZoneIndex=0 ;
//	int nMaxZone=0 ;
	

	int nMaxLight=m_Q3Map->m_nLightMax ;
	int nLt=0 ;

//	int nMaxSubLight=m_Q3Map->m_nSubLightMax ;
	int nSubLt=0 ;
	
	m_nMaxVisibleLight=0 ;

	

	Ogre::Vector3 Pos=pCamera->getPosition() ;
	float flPos[3]={Pos.x, Pos.y, Pos.z} ;

	Ogre::AxisAlignedBox AAB ;



	// initially all lights are invisible
	// set all remaining real lights to invisible
	SetAllLightsOff() ;

	m_nTotalTriangles=0 ;

	*pMaxVisibleLight=0 ; // no lights in the list 
	
	if(m_nZoneCullingActive) // if zone culling is active
	{
		
		// by default, all zones are not visible
		nZone=m_Q3Map->m_nMaxZone ;
		while(nZone)
			pZoneVis[--nZone]=ZONE_UNCHECKED ;

		// by default, all lights are not visible
		nLt=nMaxLight ;
		while(nLt)
			pLightVis[--nLt]=LIGHT_UNCHECKED ;

		// by default, all lights are not visible
		nSubLt=m_Q3Map->m_nSubLightMax ;
		while(nSubLt)
			pSubLightVis[--nSubLt]=LIGHT_UNCHECKED ;

		// by default, all portals are not visible
		nPortal=m_Q3Map->m_iNumPortals ;
		while(nPortal)
			pPortalVis[--nPortal]=PORTAL_UNCHECKED ;
	}
	else
	{
		// for debugging, all zones are visible
		nZone=m_Q3Map->m_nMaxZone ;
		while(nZone)
			pZoneVis[--nZone]=ZONE_VISIBLE ;


		// by default, all lights are visible
		nLt=nMaxLight ;
		while(nLt)
			pLightVis[--nLt]=LIGHT_VISIBLE ;

		nSubLt=m_Q3Map->m_nSubLightMax ;
		while(nSubLt)
			pSubLightVis[--nSubLt]=LIGHT_VISIBLE ;

		// by default, all portals are visible
		nPortal=m_Q3Map->m_iNumPortals ;
		while(nPortal)
			pPortalVis[--nPortal]=PORTAL_VISIBLE ;

		return ;
	}








	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	


	/////////////////////////////////////////////////////////////////////////////////////
	// check camera is actually in the zone designated, otherwise update it.

	if((*pCameraZone==-1) || (!m_Q3Map->PointInZone(flPos, *pCameraZone)) )
	{

		nSubZone=m_Q3Map->GetNextSubZone(flPos, -1, m_Q3Map->m_iNumSubZones) ;
			
		// subzone might still be -1 if the point isn't in any zone.  If this happens, set all zones in the view frustum as visible.
		// this shouldn't generally happen, but if we aren't clipping and are flying around testing, it can.
		// Don't worry about optimizing this.



		if(nSubZone==-1)
		{

			nMaxSubZone=m_Q3Map->m_iNumSubZones ;
			for(nSubZone=0 ; nSubZone<nMaxSubZone ; nSubZone++)
			{
				AAB.setExtents(m_Q3Map->m_pSubZones[nSubZone].Min[0], m_Q3Map->m_pSubZones[nSubZone].Min[1], m_Q3Map->m_pSubZones[nSubZone].Min[2], m_Q3Map->m_pSubZones[nSubZone].Max[0], m_Q3Map->m_pSubZones[nSubZone].Max[1], m_Q3Map->m_pSubZones[nSubZone].Max[2]) ;

				if(pCamera->isVisible(AAB))
					pZoneVis[ m_Q3Map->m_pSubZones[nSubZone].Zone ]=ZONE_VISIBLE ;
			}	

			*pCameraZone=-1 ;
			return ; // skip everything else
		}

		*pCameraZone=m_Q3Map->m_pSubZones[nSubZone].Zone ; // update the zone
	
	}

	//
	/////////////////////////////////////////////////////////////////////////////////////


	// update the portal states
	nMaxPortal=m_Q3Map->m_iNumPortals ;
	nPortal=nMaxPortal ;
	while(nPortal)
	{
		nPortal-- ;

		if(m_nPortalDebug==0)
			m_chPortalState[nPortal]=PORTALSTATE_OPEN ; 
		else
		{
			//if(m_nPortalState==1)
				m_chPortalState[nPortal]=PORTALSTATE_OPEN ; 
			//else
			//	m_chPortalState[nPortal]=PORTALSTATE_CLOSED ;


			
		}
		
		//m_chPortalState[nPortal]|=PORTAL_FLAG_UNCHECKED ; // by default all portals are unchecked
		
		//m_chPortalState[nPortal]&=~(PORTAL_FLAG_VISCHECK|PORTAL_FLAG_VISIBLE) ; // by default, all portals are vis unchecked and not visible
	}

	//m_chPortalState[0]=PORTAL_FLAG_CLOSED ;
	if(m_nPortalState==1)
		m_chPortalState[1]=PORTALSTATE_OPEN ;
	else
		m_chPortalState[1]=PORTALSTATE_CLOSED ;




	//Ogre::Camera* pTestCamera ;
	//pTestCamera->synchroniseBaseSettingsWith(pCamera) ;

	//Ogre::Real OrigL, OrigR, OrigT, OrigB ; // original frustum extents
	//pCamera->getFrustumExtents(OrigL, OrigR, OrigT, OrigB) ;
	
	//pCamera->setFrustumExtents(OrigL/2.0, OrigR/2.0, OrigT/2.0, OrigB/2.0) ; // reset the original frustum extents

	m_nRecurseCount=0 ;

	
	// work out the aspect scaling we'll need
	Ogre::Real OrigL, OrigR, OrigT, OrigB, ScaleX, ScaleY ; // frustum extents per each portal in this zone
	Ogre::Real NewL, NewR, NewT, NewB ;
	pCamera->getFrustumExtents(OrigL, OrigR, OrigT, OrigB) ;

	m_ViewMatrix=pCamera->getViewMatrix(true) ;
	m_ProjectionMatrix=pCamera->getProjectionMatrix() ;
	

	//if(m_nOriginalZoneOpenPortals=0) // no portals on player zone are open, so we can only see the current zones.
	//	pZoneVis[nZone]=ZONE_VISIBLE ; // flag this subzone as visible
	//else
	{

		/*
		// apply the stereofrustum tweak if needed
		if(m_flStereoFrustumTweak!=0.0f)
		{

			NewL=OrigL-m_flStereoFrustumTweak ;
			NewR=OrigR+m_flStereoFrustumTweak ;
			NewT=OrigT ;
			NewB=OrigB ;
			pCamera->setFrustumExtents(NewL, NewR, NewT, NewB) ;
		}
		else
		*/
		{
			NewL=OrigL ;
			NewR=OrigR ;
			NewT=OrigT ;
			NewB=OrigB ;
		}
		
		ScaleX=NewR ;
		ScaleY=NewT ;


		PortalScan(pCamera, *pCameraZone, ScaleX, ScaleY, pZoneVis, pLightVis, pSubLightVis, pPortalVis, pMaxVisibleLight, pVisibleLightList) ; // recursively scan portals and work out zone visibility

		
		pCamera->setFrustumExtents(OrigL, OrigR, OrigT, OrigB) ; // reset the original frustum extents

		CheckMultiZoneLights(*pCameraZone, pZoneVis, pLightVis, pSubLightVis) ;
		AddZonesFromMultiZoneLights(pZoneVis, pLightVis, pSubLightVis) ;

		// check entity visibility
		

	}







	
}


void OgreFramework::SetupEntityZones(void)
{
	// work out what zones any active entities are in.
	int nEntity=0 ;
	float Centre[3] ;
	float AABBMin[3] ;
	float AABBMax[3] ;
	float* pLightMin ;
	float* pLightMax ;
	
	int nOldZone=0 ;
	int nCurrentZone=0 ;
	int nMaxOldZone=0 ;
	int nZonePerZoneIndex=0 ;
	int nZone=0 ;
	int nZoneIndex=0 ;
	int nMaxZone=0 ;
	int nMaxSubLightIndex=0 ;
	int nSubLightIndex=0 ;
	int nSubLight=0 ;

//	int nLight=0;
	int nLightIndex=0 ;


	

	for(nEntity=0 ; nEntity<m_nMaxEntity ; nEntity++)
		if(m_pEntityInfo[nEntity].Active)
		{
			nLightIndex=0 ;
			// to calculate what zones an entity is in, we test the centre of it's bounding box to get the main zone.
			// we then test all zones that touch the central zone and see if they touch the entity bounding box.

			// check if the entity central point is still in the same zone as last time.
			Centre[0]=m_pEntityInfo[nEntity].Centre.x + m_pEntityInfo[nEntity].Postition.x ;
			Centre[1]=m_pEntityInfo[nEntity].Centre.y + m_pEntityInfo[nEntity].Postition.y ;
			Centre[2]=m_pEntityInfo[nEntity].Centre.z + m_pEntityInfo[nEntity].Postition.z ;

			nMaxOldZone=m_pEntityInfo[nEntity].Zone[INDEX_ENTITYZONECOUNT] ;



			if(nMaxOldZone>0)
			{
				nOldZone=m_pEntityInfo[nEntity].Zone[0] ;
				if(m_Q3Map->PointInZone(Centre, nOldZone))
					nCurrentZone=nOldZone ;	// still in the same zone as last round
				else
				{
					// see if we are in one of the other zones that were touched by this entity's aabb last round

					nCurrentZone=-1 ;
					for(nZoneIndex=1 ; nZoneIndex<nMaxOldZone ; nZoneIndex++)
					{
						if(m_Q3Map->PointInZone(Centre, m_pEntityInfo[nEntity].Zone[nZoneIndex]))
						{
							nCurrentZone=m_pEntityInfo[nEntity].Zone[nZoneIndex] ; // found the current zone we are in
							break ;
						}// end if point in zone

					}// end for zoneindex

				}// end if not in old zone.
			}
			else
				nCurrentZone=-1 ;


			// if we haven't found the current zone so far, we need to check every zone.
			if(nCurrentZone==-1)
			{
				nZone=m_Q3Map->m_nMaxZone ;
				while(nZone)
					if(m_Q3Map->PointInZone(Centre, --nZone))
					{
						nCurrentZone=nZone ;
						break ;
					}
			}// end if nCurrentZone -1


			
			// now either nCurrentZone is the zone the central point is in, or the entity isn't in any zone
			if(nCurrentZone==-1)
				m_pEntityInfo[nEntity].Zone[INDEX_ENTITYZONECOUNT]=0 ;
			else
			{
				// scan through all the zones that touch this entitys central zone
				// see if the entitys AABB touches those other zones.
				m_pEntityInfo[nEntity].Zone[0]=nCurrentZone ;
				nZonePerZoneIndex=1 ;
				
				AABBMin[0]=m_pEntityInfo[nEntity].Postition.x + m_pEntityInfo[nEntity].AABBMin[0] ; 
				AABBMin[1]=m_pEntityInfo[nEntity].Postition.y + m_pEntityInfo[nEntity].AABBMin[1] ; 
				AABBMin[2]=m_pEntityInfo[nEntity].Postition.z + m_pEntityInfo[nEntity].AABBMin[2] ;

				AABBMax[0]=m_pEntityInfo[nEntity].Postition.x + m_pEntityInfo[nEntity].AABBMax[0] ; 
				AABBMax[1]=m_pEntityInfo[nEntity].Postition.y + m_pEntityInfo[nEntity].AABBMax[1] ; 
				AABBMax[2]=m_pEntityInfo[nEntity].Postition.z + m_pEntityInfo[nEntity].AABBMax[2] ;


				nMaxZone=m_Q3Map->m_nZoneTouchesZone[nCurrentZone][INDEX_ZONEPERZONECOUNT] ;
				for(nZoneIndex=0 ; nZoneIndex<nMaxZone ; nZoneIndex++)
				{
					nZone=m_Q3Map->m_nZoneTouchesZone[nCurrentZone][nZoneIndex] ;

					if(m_Q3Map->AABBTouchesZone(AABBMin, AABBMax, nZone)) 
					{
						m_pEntityInfo[nEntity].Zone[nZonePerZoneIndex]=nZone ;
						nZonePerZoneIndex++ ;

						if(nZonePerZoneIndex>MAX_ZONEPERENTITY)
							nZonePerZoneIndex=MAX_ZONEPERENTITY ;
					}

					

				}// end for nZoneIndex


				m_pEntityInfo[nEntity].Zone[INDEX_ENTITYZONECOUNT]=nZonePerZoneIndex ;

				// go through all the zones and check which lights touch this entity AABB
				for(nZoneIndex=0 ; nZoneIndex<nZonePerZoneIndex ; nZoneIndex++)
				{
					nZone=m_pEntityInfo[nEntity].Zone[nZoneIndex] ;

					nMaxSubLightIndex=m_Q3Map->m_nZoneTouchesSubLight[nZone][MAX_LIGHTPERZONE] ;
					for(nSubLightIndex=0 ; nSubLightIndex<nMaxSubLightIndex ; nSubLightIndex++)
					{
						nSubLight=m_Q3Map->m_nZoneTouchesSubLight[nZone][nSubLightIndex] ;
						pLightMin=m_Q3Map->m_SubLight[nSubLight].Min ;
						pLightMax=m_Q3Map->m_SubLight[nSubLight].Max ;

						// if the entity overlaps the sublight
						if(
									 (AABBMin[0]<pLightMax[0]) && (AABBMax[0]>pLightMin[0])
								&& (AABBMin[1]<pLightMax[1]) && (AABBMax[1]>pLightMin[1])
								&& (AABBMin[2]<pLightMax[2]) && (AABBMax[2]>pLightMin[2])
							)
						{
							// add this to the list of lights.
							// it's possible it might get listed more than once if this is a multizone light, that's ok.
			
							m_pEntityInfo[nEntity].Light[nLightIndex]=m_Q3Map->m_SubLight[nSubLight].Light ;
							nLightIndex++ ;
							if(nLightIndex>MAX_LIGHTPERENTITY)
								nLightIndex=MAX_LIGHTPERENTITY ;


						} // end if overlap

					}// end for nSubLightIndex

				}// end for nZoneIndex

			}// end if we have a central zone
			
			
			m_pEntityInfo[nEntity].Light[INDEX_LIGHTPERENTITY]=nLightIndex ;


		}// end if entity is active
}

// spotlights need entities set as visible if they are in a visible zone, even if they aren't in the frustum, 
// because they might cast shadows that are in the frustum.
// However the deferred render only needs consider entities which are in a visible zone AND in the frustum.
// m_pVisibleEntity holds entities in visible zones,
// m_pFrustumEntity holds entities in visible zones that are also in the view frustum.
void OgreFramework::CalculateEntityVisibility(Ogre::Camera* pCamera, char* pZoneVis)
{

	int nEntity=0 ;
	int nMaxZone=0 ;
	int nZone=0 ;
	int nZoneIndex=0 ;
//	int nVisible=0 ;
	Ogre::AxisAlignedBox AABB ;


	m_nMaxVisibleEntity=0 ; // default to an empty list 
	m_nMaxFrustumEntity=0 ; // default to an empty list 

	m_nVisibleEntityTriangleCount=0 ;
	m_nFrustumEntityTriangleCount=0 ;

	for(nEntity=0 ; nEntity<m_nMaxEntity ; nEntity++)
		if(m_pEntityInfo[nEntity].Active)
		{
			m_pEntityInfo[nEntity].Visible=0 ; // default to not visible
			m_pEntityInfo[nEntity].Frustum=0 ; // default to not visible and not in frustum

			nMaxZone=m_pEntityInfo[nEntity].Zone[INDEX_ENTITYZONECOUNT] ;



				for(nZoneIndex=0 ; nZoneIndex<nMaxZone ; nZoneIndex++)
				{
					nZone=m_pEntityInfo[nEntity].Zone[nZoneIndex] ;

					// if a zone this entity touches is visible, test if it is in the view frustrum	
					if(pZoneVis[nZone]==ZONE_VISIBLE)
					{

						m_pEntityInfo[nEntity].Visible=1 ;
						m_nVisibleEntityTriangleCount+=m_pEntityInfo[nEntity].TriangleCount ;
						m_pVisibleEntity[m_nMaxVisibleEntity++]=nEntity ;
						

						// is this entity also in the view frustum?
						AABB.setMinimum( m_pEntityInfo[nEntity].AABB.getMinimum() + m_pEntityInfo[nEntity].Postition ) ;
						AABB.setMaximum( m_pEntityInfo[nEntity].AABB.getMaximum() + m_pEntityInfo[nEntity].Postition ) ;

						if(pCamera->isVisible(AABB))
						{
							m_pEntityInfo[nEntity].Frustum=1 ;
							m_nFrustumEntityTriangleCount+=m_pEntityInfo[nEntity].TriangleCount ;
							m_pFrustumEntity[m_nMaxFrustumEntity++]=nEntity ;
						}





						break ; // get out of the loop, we know the entity is visible.

					}// end if zone is visible

				}// end for nZoneIndex

		}// end if entity is active

}





void OgreFramework::SetAllLightsOff(void)
{
	Light *light;
	char chLightName[1024] ;
	int nLt=0 ;
	for(nLt=0 ; nLt<MAXLIGHTPERLOOP ; nLt++)
	{
		sprintf(chLightName, "LT%02i", nLt) ;
		light=m_pSceneMgr->getLight(chLightName) ;
		light->setVisible(false) ;
	}
}

void OgreFramework::SetSingleVisibleLight(int nLt, bool bOn)
{
	Light *light;

	light=m_pSceneMgr->getLight("LT00") ;


	// if we're just turning the light off, make it invisible and return.
	if(bOn==false)
	{
		light->setVisible(false) ;
		return ;
	}

	
			
	light->setDiffuseColour(Ogre::ColourValue(1,1,1));
	light->setSpecularColour(Ogre::ColourValue(1,1,1));
			
	light->setAttenuation(m_Q3Map->m_pLight[nLt].Cutoff, 1.0, 1.0, 0.005);
	light->setPosition(Ogre::Vector3(m_Q3Map->m_pLight[nLt].Position[0], m_Q3Map->m_pLight[nLt].Position[1], m_Q3Map->m_pLight[nLt].Position[2]));
	light->setDirection(Ogre::Vector3(m_Q3Map->m_pLight[nLt].Direction[0], m_Q3Map->m_pLight[nLt].Direction[1], m_Q3Map->m_pLight[nLt].Direction[2]));
			
	light->setSpotlightRange(Degree(m_Q3Map->m_pLight[nLt].Angle-5.0), Degree(m_Q3Map->m_pLight[nLt].Angle), 1.0) ;
	light->setVisible(true) ;
}


// sets up a single visible light from the visible light list and assigns the camera to its position and settings
// returns true if successful, false if not (such as if the light isn't actually visible)

bool OgreFramework::SetupSingleVisibleLightAndShadowCamera(int nLt, char* pLightVis, unsigned short* pVisibleLightList, Ogre::Camera* pCamera)
{
	//if(nLt!=4) return false ;
	
	if((pLightVis[nLt]&LIGHT_VISIBLE)==0) // we need to check this again, since there's a chance a multi-zone light might have been culled at the last stage
		return false ;

	
	Light *light=m_pSceneMgr->getLight("LT00") ;
	
	light->setDiffuseColour(Ogre::ColourValue(m_Q3Map->m_pLight[nLt].Colour[0], m_Q3Map->m_pLight[nLt].Colour[1], m_Q3Map->m_pLight[nLt].Colour[2]));

	float	flRange=m_Q3Map->m_pLight[nLt].Cutoff ; // set the range
	if(flRange<10240.0) //!! Ogre seems to cut off lights wrongly if the range is low, so set it to a minimum.
		flRange=10240 ;

	// The light attenuation parameters have been co-opted by me... they no longer represent range, constant, linear, quadratic
	// instead, 
	// first parameter is the above range, just set high so Ogre doesn't turn lights off itself.  The shaders ignore this parameter.
	// second parameter is the cutoff used in the shader.  Light suddenly cuts off after this distance.
	// third parameter is the brightness.  This controls the quadratic falloff of the light.
	// fourth parameter is unused, leave as 1.0 for now.

	light->setAttenuation(flRange, m_Q3Map->m_pLight[nLt].Cutoff, m_Q3Map->m_pLight[nLt].Brightness, 1.0) ;
	
	
	
	// Ogre uses a fixed yaw axis which will cause pCamera->setDirection to malfunction if we point directly up or down.
	// To avoid this we need to check for such lights and shift their direction a little.
	
	float DirX=m_Q3Map->m_pLight[nLt].Direction[0] ;
	float DirY=m_Q3Map->m_pLight[nLt].Direction[1] ;
	float DirZ=m_Q3Map->m_pLight[nLt].Direction[2] ;
	

	if((DirY>1.0-CAMERA_EPSILON) && (DirY<1.0+CAMERA_EPSILON)) // camera is pointing directly up
	{
		DirX=0.0f ;
		DirY=1.0f ;
		DirZ=0.000001f ;
	}
	else
		if((DirY>-1.0-CAMERA_EPSILON) && (DirY<-1.0+CAMERA_EPSILON)) // camera is pointing directly down
		{
			DirX=0.0f ;
			DirY=-1.0f ;
			DirZ=0.000001f ;
		}
	
	light->setPosition(Ogre::Vector3(m_Q3Map->m_pLight[nLt].Position[0], m_Q3Map->m_pLight[nLt].Position[1], m_Q3Map->m_pLight[nLt].Position[2]));
	light->setDirection(Ogre::Vector3(DirX, DirY, DirZ));
			
	light->setSpotlightRange(Degree(m_Q3Map->m_pLight[nLt].Angle-5.0), Degree(m_Q3Map->m_pLight[nLt].Angle), 1.0) ;
	light->setVisible(true) ;

	// setup the camera
	pCamera->setAspectRatio(1) ;
	pCamera->yaw(Ogre::Radian(0)) ;
	pCamera->roll(Ogre::Radian(0)) ;
	pCamera->pitch(Ogre::Radian(0)) ;
	pCamera->setPosition(m_Q3Map->m_pLight[nLt].Position[0], m_Q3Map->m_pLight[nLt].Position[1], m_Q3Map->m_pLight[nLt].Position[2]) ;
	pCamera->setDirection(DirX, DirY, DirZ) ;
	pCamera->setFOVy( Degree(m_Q3Map->m_pLight[nLt].Angle ) ) ;

	return true ;

}


void OgreFramework::UpdateVisibleZoneList(char* pZoneVis, unsigned short* pVisibleZoneList, int* pMaxVisibleZone)
{
	int nZone=0 ;
	int nMaxVisibleZone=0 ;

	for(nZone=0 ; nZone<m_Q3Map->m_nMaxZone ; nZone++)
		if(pZoneVis[nZone]==ZONE_VISIBLE)
			pVisibleZoneList[nMaxVisibleZone++]=nZone ;

	*pMaxVisibleZone=nMaxVisibleZone ;

}

void OgreFramework::SetZoneNodeAttachments(unsigned short* pVisibleZoneList, int* pMaxVisibleZone, int nMaterialFlags)
{

		int nListPos=0 ;
		int nZone=0 ;
		int nMaxListPos=*pMaxVisibleZone ;



		for(nListPos=0 ; nListPos<nMaxListPos ; nListPos++)
		{
			nZone=pVisibleZoneList[nListPos] ;

				// if the zone node is not already attached, attach it.

				if( m_nOpaqueNodeUsed[nZone] && (nMaterialFlags&MAT_OPAQUE) && !m_pOpaqueNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pOpaqueNode[nZone]) ;

				if( m_nTransNodeUsed[nZone] && (nMaterialFlags&MAT_GEL) && !m_pTransNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pTransNode[nZone]) ;

				if( m_nLampNodeUsed[nZone] && (nMaterialFlags&MAT_LAMP) && !m_pLampNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pLampNode[nZone]) ;

				if( m_nGlowNodeUsed[nZone] && (nMaterialFlags&MAT_GLOW) && !m_pGlowNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pGlowNode[nZone]) ;

		}


			
/*
		if(m_nDebugLightBox)
		{
			int nLt=0 ;
			int nMaxLight=m_Q3Map->m_nLightMax ;

			for(nLt=0 ; nLt<nMaxLight ; nLt++)
				if(m_chLightVis[nLt]==LIGHT_VISIBLE)
				{
					m_pLightMO[nLt]->setVisible(true) ;

				}
				else
					m_pLightMO[nLt]->setVisible(false) ;



		}

		if(m_nPortalDebug)
		{
			int nPt=0 ;
			int nMaxPortal=m_Q3Map->m_iNumPortals ;

			for(nPt=0 ; nPt<nMaxPortal ; nPt++)
				if(
							(m_chPortalVis[nPt]&PORTAL_VISIBLE) 
							&&
							(m_chPortalState[nPt]&PORTALSTATE_CLOSED)
					)
					m_pPortalMO[nPt]->setVisible(true) ;
				else
					m_pPortalMO[nPt]->setVisible(false) ;
		}		
			
	*/
}


void OgreFramework::SetZoneNodeAttachments(unsigned short* pVisibleZoneList, int* pMaxVisibleZone, int nMaterialFlags, int nLight)
{

		nLight=IGNORE_LIGHT ;

		int nListPos=0 ;
		int nZone=0 ;
		int nMaxListPos=*pMaxVisibleZone ;

		int nLightZone=0 ;
		int nMaxLightZone=0 ;
		int nFoundZone=0 ;


		for(nListPos=0 ; nListPos<nMaxListPos ; nListPos++)
		{
			nZone=pVisibleZoneList[nListPos] ;

			// is this zone touched by this light?
			if(nLight!=IGNORE_LIGHT)
			{
				nMaxLightZone=m_Q3Map->m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT] ;

				nFoundZone=0 ;
				for(nLightZone=0 ; nLightZone<nMaxLightZone ; nLightZone++)
					if(m_Q3Map->m_nLightTouchesZone[nLight][nLightZone]==nZone)
					{
						nFoundZone=1 ;
						break ;
					}

			}
			else
				nFoundZone=1 ;


			if(nFoundZone) // if the light touches this zone, or we're ignoring lights
			{
				// if the zone node is not already attached, attach it.

				if( m_nOpaqueNodeUsed[nZone] && (nMaterialFlags&MAT_OPAQUE) && !m_pOpaqueNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pOpaqueNode[nZone]) ;

				if( m_nTransNodeUsed[nZone] && (nMaterialFlags&MAT_GEL) && !m_pTransNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pTransNode[nZone]) ;

				if( m_nLampNodeUsed[nZone] && (nMaterialFlags&MAT_LAMP) && !m_pLampNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pLampNode[nZone]) ;

				if( m_nGlowNodeUsed[nZone] && (nMaterialFlags&MAT_GLOW) && !m_pGlowNode[nZone]->isInSceneGraph() )
					m_pSceneMgr->getRootSceneNode()->addChild(m_pGlowNode[nZone]) ;
			}
		}

}

bool OgreFramework::LightTouchesZone(int nLight, int nZone)
{
		int nLightZone=0 ;
		int nMaxLightZone=0 ;

		nMaxLightZone=m_Q3Map->m_nLightTouchesZone[nLight][INDEX_LIGHTZONECOUNT] ;
				
		for(nLightZone=0 ; nLightZone<nMaxLightZone ; nLightZone++)
			if(m_Q3Map->m_nLightTouchesZone[nLight][nLightZone]==nZone)
				return true ;

		return false ;
}


// last chance to cull some more multizone lights
// Then we set all zones the remaining multizone lights touch to visible so that we get correct shadows
void OgreFramework::CheckMultiZoneLights(int nCameraZone, char* pZoneVis, char* pLightVis, char* pSubLightVis)
{
	int nLightMax=m_Q3Map->m_nMaxMultiZoneLight ;
	int nLight=0 ;
	int nLightIndex=0 ;
//	int nLightVal=0 ;
	int nOpenPortals=0 ;
//	int nZoneHasOpenPortals=0 ;
	int nPortal=0 ;
	int nPortalIndex=0 ;
	int nMaxPortal=0 ;
//	int nZone=0 ;
//	int nMaxZone=0 ;
//	int nZoneIndex=0 ;

	int nSubLightMax=0 ;
//	int nSubLightIndex=0 ;
	int nSubLight=0 ;
	int nSubLightStart=0 ;

	int nPortalLightMax=0 ;
	int nPortalLightIndex=0 ;
//	int nPortalLight=0 ;

	int nCentreZone=0 ;


	for(nLightIndex=0 ; nLightIndex<nLightMax ; nLightIndex++)
	{
		nLight=m_Q3Map->m_nMultiZoneLight[nLightIndex] ;
		if(pLightVis[nLight]==LIGHT_VISIBLE) // try to kill multizone lights that are currently set as visible
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////////////.
			// if the zone that contains the centre isn't visible, 
			// and the portals that the light touches are closed,
			// then this light can't be visible anywhere else

			nCentreZone=m_Q3Map->m_pLight[nLight].CentreZone ;
			
			
			// is the zone that contains the light centre not visible?
			if(pZoneVis[nCentreZone]!=ZONE_VISIBLE)
			{
				// check the portals on the centre sublight zone.  
				// If the ones that the light touches are closed, 
				// then this light can't be visible

				
				nMaxPortal=m_Q3Map->m_nZoneTouchesPortal[nCentreZone][INDEX_PORTALCOUNT] ;
				nOpenPortals=0 ;
				nSubLightStart=m_Q3Map->m_pLight[nLight].SubLightStart ; // the first sublight also contains the light centre

				for(nPortalIndex=0 ; nPortalIndex<nMaxPortal ; nPortalIndex++)
				{
					nPortal=m_Q3Map->m_nZoneTouchesPortal[nCentreZone][nPortalIndex] ;

					// if portal's open...
					if(m_chPortalState[nPortal]&PORTALSTATE_OPEN)
					{
						nPortalLightMax=m_Q3Map->m_nPortalTouchesLight[nPortal][INDEX_PORTALLIGHTCOUNT] ;
						
						// scan all the lights that touch this portal to see if our current light is one of them
						for(nPortalLightIndex=0 ; nPortalLightIndex<nPortalLightMax ; nPortalLightIndex++)
							if(m_Q3Map->m_nPortalTouchesLight[nPortal][nPortalLightIndex]==nLight)
							{
								nOpenPortals=1 ;
								break ;
							}

					}// end if portal is open

					if(nOpenPortals) break ; // get out if we found an open portal that the light touches
				}// end for portals



				// if none of the portals touched by the centre light were open, set this light and it's sublights to not visible
				if(nOpenPortals==0)
				{
					pLightVis[nLight]=LIGHT_UNCHECKED ;

					nSubLightStart=m_Q3Map->m_pLight[nLight].SubLightStart ; // the first sublight also contains the light centre
					nSubLightMax=nSubLightStart+m_Q3Map->m_pLight[nLight].ZoneCount ; 

					for(nSubLight=nSubLightStart ; nSubLight<nSubLightMax ; nSubLight++)
						pSubLightVis[nSubLightStart]=LIGHT_UNCHECKED ;

				}// end if no light centre portals that touched lights were open



			}// end if light centre zone isn't visible

		}// end if light is visibile

	}// end for lightindex

}


// for lights that touch multiple zones, work out what additional zones we need to make
// visible in order that lighting will come out with correct shadows.
void OgreFramework::AddZonesFromMultiZoneLights(char* pZoneVis, char* pLightVis, char* pSubLightVis)
{
	int nLightMax=m_Q3Map->m_nMaxMultiZoneLight ;
	int nLight=0 ;
	int nLightIndex=0 ;
	float flMinX=0.0f ;
	float flMaxX=0.0f ;
	float flMinY=0.0f ;
	float flMaxY=0.0f ;
	float flMinZ=0.0f ;
	float flMaxZ=0.0f ;

	int nSubLightMax=0 ;
//	int nSubLightIndex=0 ;
	int nSubLight=0 ;
	int nSubLightStart=0 ;
	int nSubLightFirst=0 ;

	int nInnerLight=0 ;


	for(nLightIndex=0 ; nLightIndex<nLightMax ; nLightIndex++)
	{
		nLight=m_Q3Map->m_nMultiZoneLight[nLightIndex] ;
		if(pLightVis[nLight]==LIGHT_VISIBLE)
		{
			nSubLightStart=m_Q3Map->m_pLight[nLight].SubLightStart ; // the first sublight also contains the light centre
			nSubLightFirst=nSubLightStart+1 ;// start from the first sublight after the one containing the centre
			nSubLightMax=nSubLightStart+m_Q3Map->m_pLight[nLight].ZoneCount ; 
			

			for(nSubLight=nSubLightStart ; nSubLight<nSubLightMax ; nSubLight++)	
				if(pSubLightVis[nSubLight]==LIGHT_VISIBLE) // sublight is visible (and unchecked)
				{
					// create the new AABB
					// this is bounded by this sublight and the centre holding sublight.  
					flMinX = m_Q3Map->m_SubLight[nSubLight].Min[0] < m_Q3Map->m_SubLight[nSubLightStart].Min[0] ? m_Q3Map->m_SubLight[nSubLight].Min[0] : m_Q3Map->m_SubLight[nSubLightStart].Min[0] ; 
					flMinY = m_Q3Map->m_SubLight[nSubLight].Min[1] < m_Q3Map->m_SubLight[nSubLightStart].Min[1] ? m_Q3Map->m_SubLight[nSubLight].Min[1] : m_Q3Map->m_SubLight[nSubLightStart].Min[1] ; 
					flMinZ = m_Q3Map->m_SubLight[nSubLight].Min[2] < m_Q3Map->m_SubLight[nSubLightStart].Min[2] ? m_Q3Map->m_SubLight[nSubLight].Min[2] : m_Q3Map->m_SubLight[nSubLightStart].Min[2] ; 
				
					flMaxX = m_Q3Map->m_SubLight[nSubLight].Max[0] > m_Q3Map->m_SubLight[nSubLightStart].Max[0] ? m_Q3Map->m_SubLight[nSubLight].Max[0] : m_Q3Map->m_SubLight[nSubLightStart].Max[0] ; 
					flMaxY = m_Q3Map->m_SubLight[nSubLight].Max[1] > m_Q3Map->m_SubLight[nSubLightStart].Max[1] ? m_Q3Map->m_SubLight[nSubLight].Max[1] : m_Q3Map->m_SubLight[nSubLightStart].Max[1] ; 
					flMaxZ = m_Q3Map->m_SubLight[nSubLight].Max[2] > m_Q3Map->m_SubLight[nSubLightStart].Max[2] ? m_Q3Map->m_SubLight[nSubLight].Max[2] : m_Q3Map->m_SubLight[nSubLightStart].Max[2] ; 
				
					for(nInnerLight=nSubLightStart ; nInnerLight<nSubLightMax ; nInnerLight++)
					{

						
						if(
									(nInnerLight!=nSubLight) // no need to test itself
								&&(pZoneVis[  m_Q3Map->m_SubLight[nInnerLight].Zone  ] != ZONE_VISIBLE) // don't test if already visible
								// if overlap, InnerLight's zone is visible
								&&(flMinX<m_Q3Map->m_SubLight[nInnerLight].Max[0]) && (flMaxX>m_Q3Map->m_SubLight[nInnerLight].Min[0])
								&&(flMinY<m_Q3Map->m_SubLight[nInnerLight].Max[1]) && (flMaxY>m_Q3Map->m_SubLight[nInnerLight].Min[1])
								&&(flMinZ<m_Q3Map->m_SubLight[nInnerLight].Max[2]) && (flMaxZ>m_Q3Map->m_SubLight[nInnerLight].Min[2])
							)
						{
							pZoneVis[  m_Q3Map->m_SubLight[nInnerLight].Zone  ]=ZONE_VISIBLE ; // flag this zone as visible
							m_nTotalTriangles+=m_nZoneTriangleCount[ m_Q3Map->m_SubLight[nInnerLight].Zone ] ; // just some stats for interest
						}

					} // end for innerlight
					
				}// end if sublight is visible

		}// end if light is visible

	}// end for nLightIndex

}

// scan all the unchecked portals in a subzone to see if they are visible, 
// then move into any visible subzones and recurse.
// scalex and scaley will convert the screen space numbers (-1 to 1) into frustum numbers (varies, often -0.5ish to 0.5ish) which is probably due to screen aspect ratios etc

// we also add visible lights if they are in the view frustum
void OgreFramework::PortalScan(Ogre::Camera *pCamera, int nZone, Ogre::Real ScaleX, Ogre::Real ScaleY, char* pZoneVis, char* pLightVis, char* pSubLightVis, char* pPortalVis, int* pMaxVisibleLight, unsigned short* pVisibleLightList)
{
	Ogre::Real OrigL, OrigR, OrigT, OrigB ; // original frustum extents
	Ogre::Real PortalL, PortalR, PortalT, PortalB ; // frustum extents per each portal in this zone
	pCamera->getFrustumExtents(OrigL, OrigR, OrigT, OrigB) ;
	

	

	Ogre::AxisAlignedBox AAB ;


	int nPortalMax=m_Q3Map->m_nZoneTouchesPortal[nZone][INDEX_PORTALCOUNT] ;
	int nPortalIndex=0 ;
	int nPortal=0 ;

	int nPortalZoneMax=0 ;
	int nPortalZone=0 ;
	int nPortalZoneIndex=0 ;


	//////////////////////////////////////////////////////////////////////////////////////
	//
	// LIGHTING CHECKS
	//

	int nLight=0;
	int nLightVal=0 ;
//	int nLightIndex=0 ;
//	int nMaxLight=0 ;

	int nSubLight=0 ;
	int nSubLightIndex=0 ;
	int nMaxSubLight=0 ;
//	int nSubLightVal=0 ;

	
	// check all lights completely contained in this zone to see if they're visible
	nMaxSubLight=m_Q3Map->m_nZoneTouchesSubLight[nZone][INDEX_LIGHTCOUNT] ;
	for(nSubLightIndex=0 ; nSubLightIndex<nMaxSubLight ; nSubLightIndex++)
	{
		nSubLight=m_Q3Map->m_nZoneTouchesSubLight[nZone][nSubLightIndex] ;
		//nSubLightVal=m_chSubLightVis[nSubLight] ;

		nLight=m_Q3Map->m_SubLight[nSubLight].Light ;
		nLightVal=pLightVis[nLight] ;

		// if we haven't previously established that the light is visible or out of the frustum, check it.

			AAB.setExtents(m_Q3Map->m_SubLight[nSubLight].Min[0], m_Q3Map->m_SubLight[nSubLight].Min[1], m_Q3Map->m_SubLight[nSubLight].Min[2], m_Q3Map->m_SubLight[nSubLight].Max[0], m_Q3Map->m_SubLight[nSubLight].Max[1], m_Q3Map->m_SubLight[nSubLight].Max[2]) ;
			if(pCamera->isVisible(AAB)) // sublight is visible
			{
				// flag this light and sublight as visible
				if((nLightVal!=LIGHT_VISIBLE) && (*pMaxVisibleLight<MAXVISIBLELIGHT))
				{
					pVisibleLightList[*pMaxVisibleLight]=nLight ;
					*pMaxVisibleLight=*pMaxVisibleLight+1 ;
				}
				pSubLightVis[nSubLight] = pLightVis[nLight] = LIGHT_VISIBLE ;
				
			}


	}// end for lights in zone
	

	//
	///////////////////////////////////////////////////////////////////////////////////////





	pZoneVis[nZone]=ZONE_VISIBLE ; // flag this zone as visible
	m_nTotalTriangles+=m_nZoneTriangleCount[nZone] ; // just some stats for interest


	// loop through all the portals in this zone
	for(nPortalIndex=0 ; nPortalIndex<nPortalMax ; nPortalIndex++)
	{
		

		nPortal=m_Q3Map->m_nZoneTouchesPortal[nZone][nPortalIndex] ;


		if((pPortalVis[nPortal]&PORTAL_VISCHECK)==0) // portal hasn't already been checked
		{

			pPortalVis[nPortal] |= PORTAL_VISCHECK ; // flag portal as vischecked
			AAB.setExtents(m_Q3Map->m_pPortals[nPortal].Min[0], m_Q3Map->m_pPortals[nPortal].Min[1], m_Q3Map->m_pPortals[nPortal].Min[2], m_Q3Map->m_pPortals[nPortal].Max[0], m_Q3Map->m_pPortals[nPortal].Max[1], m_Q3Map->m_pPortals[nPortal].Max[2]) ;
		
			if(pCamera->isVisible(AAB)) // portal is in the view frustum
			{			

				pPortalVis[nPortal] |= PORTAL_VISIBLE ; // flag portal as visible

				// if the portal is open, go in and check the new zone.
				if(m_chPortalState[nPortal]&PORTALSTATE_OPEN)
				{
			
					// calculate the new frustum extents through this portal
					PortalL=OrigL ;
					PortalR=OrigR ;
					PortalT=OrigT ;
					PortalB=OrigB ;

					if(CalculatePortalFrustum(pCamera, nPortal, &PortalL, &PortalR, &PortalT, &PortalB, ScaleX, ScaleY))
					{





						// Loop through all the zones attached to this portal
						nPortalZoneMax=m_Q3Map->m_nPortalTouchesZone[nPortal][INDEX_PORTALZONECOUNT] ;
						for(nPortalZoneIndex=0 ; nPortalZoneIndex<nPortalZoneMax ; nPortalZoneIndex++)
						{
							nPortalZone=m_Q3Map->m_nPortalTouchesZone[nPortal][nPortalZoneIndex] ;
							if(pZoneVis[nPortalZone]==ZONE_UNCHECKED) // don't go back into zones we've already scanned.
							{

								pCamera->setFrustumExtents(PortalL, PortalR, PortalT, PortalB) ; // set the frustum extents

								// recurse
								//m_nRecurseCount++ ;
								PortalScan(pCamera, nPortalZone, ScaleX, ScaleY, pZoneVis, pLightVis, pSubLightVis, pPortalVis, pMaxVisibleLight, pVisibleLightList) ;
								//m_nRecurseCount-- ;

								
								
							}// end if zone hasn't already been checked

						}// end scanning all zones connected by this portal

					}// end if frustum is still active

				}// end if portal is open
				
			}// end if portal is in the view frustum
		
		}// end if portal hasn't been checked	

		// reset the frustum
		pCamera->setFrustumExtents(OrigL, OrigR, OrigT, OrigB) ; // reset the original frustum extents
	
	}// end for portalindex


}

// gives new frustum settings, and returns 0 if the frustum has been killed completely.
int OgreFramework::CalculatePortalFrustum(Ogre::Camera *pCamera, int nPortal, Ogre::Real* pPortalL, Ogre::Real* pPortalR, Ogre::Real* pPortalT, Ogre::Real* pPortalB, Ogre::Real ScaleX, Ogre::Real ScaleY)
{


	// if the camera is actually inside the portal then don't change the frustum but return OK
	Vector3 CamPos=pCamera->getPosition() ;

	if(
				(CamPos.x>=m_Q3Map->m_pPortals[nPortal].Min[0]) && (CamPos.x<=m_Q3Map->m_pPortals[nPortal].Max[0])
			&&(CamPos.y>=m_Q3Map->m_pPortals[nPortal].Min[1]) && (CamPos.y<=m_Q3Map->m_pPortals[nPortal].Max[1])
			&&(CamPos.z>=m_Q3Map->m_pPortals[nPortal].Min[2]) && (CamPos.z<=m_Q3Map->m_pPortals[nPortal].Max[2])
		)
		return 1 ;





	int nLoop=0 ;
	int nBehindPoint=0 ; // keep track of how many transformed points are behind the camera
	//Real OrigL=*pPortalL ;
	//Real OrigR=*pPortalR ;
	//Real OrigT=*pPortalT ;
	//Real OrigB=*pPortalB ;

	Real MinX=MINMAXLIMIT ;
	Real MaxX=-MINMAXLIMIT ;
	Real MaxY=-MINMAXLIMIT ;
	Real MinY=MINMAXLIMIT ;


	// set portal corners as 8 individual points.  order is xyz, Xyz, xYz, XYz, xyZ, XyZ, xYZ, XYZ, 
	Vector3 xyz[8] ;

	
	xyz[0].x=m_Q3Map->m_pPortals[nPortal].Min[0] ;
	xyz[0].y=m_Q3Map->m_pPortals[nPortal].Min[1] ;
	xyz[0].z=m_Q3Map->m_pPortals[nPortal].Min[2] ;

	xyz[1].x=m_Q3Map->m_pPortals[nPortal].Max[0] ;
	xyz[1].y=m_Q3Map->m_pPortals[nPortal].Min[1] ;
	xyz[1].z=m_Q3Map->m_pPortals[nPortal].Min[2] ;

	xyz[2].x=m_Q3Map->m_pPortals[nPortal].Min[0] ;
	xyz[2].y=m_Q3Map->m_pPortals[nPortal].Max[1] ;
	xyz[2].z=m_Q3Map->m_pPortals[nPortal].Min[2] ;

	xyz[3].x=m_Q3Map->m_pPortals[nPortal].Max[0] ;
	xyz[3].y=m_Q3Map->m_pPortals[nPortal].Max[1] ;
	xyz[3].z=m_Q3Map->m_pPortals[nPortal].Min[2] ;

	xyz[4].x=m_Q3Map->m_pPortals[nPortal].Min[0] ;
	xyz[4].y=m_Q3Map->m_pPortals[nPortal].Min[1] ;
	xyz[4].z=m_Q3Map->m_pPortals[nPortal].Max[2] ;

	xyz[5].x=m_Q3Map->m_pPortals[nPortal].Max[0] ;
	xyz[5].y=m_Q3Map->m_pPortals[nPortal].Min[1] ;
	xyz[5].z=m_Q3Map->m_pPortals[nPortal].Max[2] ;

	xyz[6].x=m_Q3Map->m_pPortals[nPortal].Min[0] ;
	xyz[6].y=m_Q3Map->m_pPortals[nPortal].Max[1] ;
	xyz[6].z=m_Q3Map->m_pPortals[nPortal].Max[2] ;

	xyz[7].x=m_Q3Map->m_pPortals[nPortal].Max[0] ;
	xyz[7].y=m_Q3Map->m_pPortals[nPortal].Max[1] ;
	xyz[7].z=m_Q3Map->m_pPortals[nPortal].Max[2] ;

	// transform the points to eyespace
	for(nLoop=0 ; nLoop<8 ; nLoop++)
		xyz[nLoop]=m_ViewMatrix*xyz[nLoop] ;//pCamera->getViewMatrix(true)*xyz[nLoop] ;

	// transform the points to screen space
	for(nLoop=0 ; nLoop<8 ; nLoop++)
		if(xyz[nLoop].z<0) // less than zero means point is in front of camera
		{
			xyz[nLoop]=m_ProjectionMatrix*xyz[nLoop] ;// pCamera->getProjectionMatrix()*xyz[nLoop] ;
			if(xyz[nLoop].x<MinX) MinX=xyz[nLoop].x ;
			if(xyz[nLoop].x>MaxX) MaxX=xyz[nLoop].x ;
			if(xyz[nLoop].y<MinY) MinY=xyz[nLoop].y ;
			if(xyz[nLoop].y>MaxY) MaxY=xyz[nLoop].y ;

		}
		else // point is behind the camera
			nBehindPoint++ ;
		


	
	MinX*=ScaleX ;
	MaxX*=ScaleX ;
	MinY*=ScaleY ;
	MaxY*=ScaleY ;
	
	// apply the stereofrustum tweak if needed
	//float flTweak=0.0f ;
	//if(m_flStereoFrustumTweak!=0.0f)
	//{
	//	MinX-=m_flStereoFrustumTweak ;
	//	MaxX+=m_flStereoFrustumTweak ;
	//}


	// if we have more than three behindpoints, don't cull
	if(nBehindPoint>3) return 1 ;

	// use these to cut down the frustum
	if(MinX>*pPortalL) *pPortalL=MinX ;
	if(MaxX<*pPortalR) *pPortalR=MaxX ;
	if(MinY>*pPortalB) *pPortalB=MinY ;
	if(MaxY<*pPortalT) *pPortalT=MaxY ;



	

	

	// check if frustum has been cut out of existence
	// culling at this point didn't work, so this is a hack to make if function.
	if((*pPortalL>=*pPortalR) || (*pPortalB>=*pPortalT))
		return 0 ;


	/*
	if(*pPortalL>=*pPortalR)
	{
		*pPortalL=(*pPortalL+*pPortalR)/2.0-0.01 ;
		*pPortalR=*pPortalL+0.02 ;
	}

	if(*pPortalB>=*pPortalT)
	{
		*pPortalB=(*pPortalB+*pPortalT)/2.0-0.01 ;
		*pPortalT=*pPortalB+0.02 ;
	}
	*/

		
		//return 0 ;


	return 1 ;
}








 /***********************************************************************************************************\
                                               
                                            LIGHTING SETUP
                                                
 \***********************************************************************************************************/





void OgreFramework::initLight() 
{
//	char chMessage[1024] ;

		// these lights only have placeholder settings to begin with, since they get moved around and reused thoughout the level.
	
		int nLt=0 ;
		int nMaxLt=MAXLIGHTPERLOOP ;
		Light *light;
		char chLightName[1024] ;


		

		for(nLt=0 ; nLt<nMaxLt ; nLt++)
		{
			sprintf(chLightName, "LT%02i", nLt) ;
			
			light = m_pSceneMgr->createLight(chLightName);
			light->setType(Light::LT_SPOTLIGHT);
			light->setCastShadows(true);
			light->setVisible(false) ;

		}
	
}

 /***********************************************************************************************************\
                                               
                                            CONFIG FILE
                                                
 \***********************************************************************************************************/

void OgreFramework::ParseBZNConfig(void)
{
	const int KVSIZE=1024 ;
	// make sure the config is a string
	int nConfigPos=-1 ;
//	int nPos=0 ;
	int nKeyPos=0 ;
	int nValuePos=0 ;

	char chKey[KVSIZE] ;
	char chValue[KVSIZE] ;
	int nValue=0 ;

	

	while(nConfigPos<BZN_CONFIG_SIZE)
	{
		// scan forward to find the next key
		while	(			
								(++nConfigPos<BZN_CONFIG_SIZE)
						&&	((m_chBZNConfig[nConfigPos]<'A') || (m_chBZNConfig[nConfigPos]>'z'))
					) ;

		if(nConfigPos<BZN_CONFIG_SIZE)
		{



			// find the end of the key
			nKeyPos=0 ;
			while	((m_chBZNConfig[nConfigPos]!=' ') && (m_chBZNConfig[nConfigPos]!='/') && (m_chBZNConfig[nConfigPos]!=0x0A) && (m_chBZNConfig[nConfigPos]!=0x0D))
			{
				chKey[nKeyPos++]=m_chBZNConfig[nConfigPos++] ;
				if((nConfigPos==BZN_CONFIG_SIZE) || (nKeyPos==KVSIZE))
					break ;
			}

			if((nConfigPos<BZN_CONFIG_SIZE) && (nKeyPos<KVSIZE))
			{
				chKey[nKeyPos]='\0' ; // null terminate the key
			
				// scan off the the number
				nValuePos=0 ;
				

				while	((m_chBZNConfig[nConfigPos]!=0x0A) && (m_chBZNConfig[nConfigPos]!=0x0D) && (m_chBZNConfig[nConfigPos]!='/'))
				{
					if(
									((m_chBZNConfig[nConfigPos]>='0') && (m_chBZNConfig[nConfigPos]<='9'))
							||	(m_chBZNConfig[nConfigPos]=='-') 
							||	(m_chBZNConfig[nConfigPos]=='.')
						)
						chValue[nValuePos++]=m_chBZNConfig[nConfigPos] ;

					nConfigPos++ ;
					if((nConfigPos==BZN_CONFIG_SIZE) || (nKeyPos==KVSIZE))
						break ;
				}

				if((nConfigPos<BZN_CONFIG_SIZE) && (nValuePos<KVSIZE))
				{
					chValue[nValuePos]='\0' ; // null terminate the value
				
					// convert value string to a float
					nValue=atoi(chValue) ;

					// assign that value
					if(strcmp("r_shadowmapsize", chKey)==0)		{ m_nShadowMapSize=nValue ; if(m_nShadowMapSize<2) m_nShadowMapSize=2 ; }
					if(strcmp("r_shadowrgbsize", chKey)==0)		{ m_nShadowRGBSize=nValue ; if(m_nShadowRGBSize<2) m_nShadowRGBSize=2 ; }
					if(strcmp("r_colouredshadow", chKey)==0)	{ m_nColouredShadow=nValue ; }
					if(strcmp("r_renderhdr", chKey)==0)				{ m_nRenderHDR=nValue ; }
					if(strcmp("r_maxgpuquery", chKey)==0)			{ m_nMaxGPUQuery=nValue ; if(m_nMaxGPUQuery<0) m_nMaxGPUQuery=0 ; if(m_nMaxGPUQuery>MAXGPUQUERY) m_nMaxGPUQuery=MAXGPUQUERY ; }

					//sprintf(m_chBug, "Key: %s, Value: %i", chKey, nValue) ;
					//m_pLog->logMessage(m_chBug);
				
				}// end found end of value







			}// end found end of key





		}// end found start of key
						


	}// end scanning whole config


}
