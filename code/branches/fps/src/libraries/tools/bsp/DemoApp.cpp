//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::DemoApp()
{
	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
	StereoConfig_Write() ; // write out the stereo configuration.  Just rough for now, a binary file

	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::startDemo()
{
	
	
	m_bShutdown = false;
	
	





		new OgreFramework();

		

		OgreFramework::getSingletonPtr()->initOgre("Video Start", this, 0);
		
		BZNConfig_Read() ;

		OgreFramework::getSingletonPtr()->AdditionalSetup() ;

		if(!setupDemoScene()) return ; // failure setting up level
		
		
		
		//runDemo_Default();
		runDemo_MethodA();

		OgreFramework::getSingletonPtr()->UnloadMap(true) ;
		//OgreFramework::getSingletonPtr()->OFBug.MessageInt(333) ;
		//delete OgreFramework::getSingletonPtr(); // causes a crash on exit in 1.7, was ok in 1.6.3
		//OgreFramework::getSingletonPtr()->
		
		OgreFramework::getSingletonPtr()->FinalShutdown() ;

		
		//OgreFramework::getSingletonPtr()->OFBug.MessageInt(333) ;

}

//|||||||||||||||||||||||||||||||||||||||||||||||

int DemoApp::setupDemoScene()
{
	return OgreFramework::getSingletonPtr()->LoadMap() ;
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// four different versions of runDemo() depending on method of GPUBuffer flushing.

int DemoApp::runDemo_Default()
{

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop (Default)...");

//	char chMessage[1024] ;
	
	UINT uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	UINT uFrameTotalTime=0 ;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics() ;
	
	



	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			Ogre::WindowEventUtilities::messagePump() ;
#endif	




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{


         // get start time of frame
         uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
            
         // update input and physics
         OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
         OgreFramework::getSingletonPtr()->m_pMouse->capture();
         OgreFramework::getSingletonPtr()->updateOgre(uFrameTotalTime/1000.0f);


         // render the frame
				 OgreFramework::getSingletonPtr()->UpdateRenderTargets() ;



				

			// calculate frame time.
			uFrameTotalTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds()-uFrameStartTime ;

		}
		else
		{
			Sleep(1000);
		}


	}



	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit (Default)");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");


	return 1 ;
}

int DemoApp::runDemo_MethodA()
{

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop (MethodA)...");

//	char chMessage[1024] ;
	
	UINT uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	UINT uFrameTotalTime=0 ;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics() ;
	
	


	/////////////////////////////////////////////////////////////////////////////////////////////
	// use a renderQueueListener with 1 to 4 HardwareOcclusionQueries to prevent the CPU from 
	// getting too far ahead of the GPU and causing input lag from keyboard and mouse.
	// thanks to Sinbad for this suggestion and code outline.
	// We aren't actually doing Hardware Occlusion Culling, just exploiting the way we can
	// make it flush the GPU buffer for prior frames.
	// Messing with the GPU command buffer can be turned off completely by seting m_nMaxGPUQuery to 0 

	
	// get the maximum gpu queries to be used.  
	int nMaxGPUQuery=OgreFramework::getSingletonPtr()->m_nMaxGPUQuery ;
	unsigned int dummy=0 ;
	int nNewQuery=0 ;
	int nOldQuery=0 ;
	rQueueListener* rqListener=NULL ;
	Ogre::HardwareOcclusionQuery** pHOQ=NULL ;

	int nQueryReady[MAXGPUQUERY] ;
		

	if(nMaxGPUQuery!=0) // if querying is turned on
	{
		// make sure it is in range.
		if(nMaxGPUQuery<1) 
			nMaxGPUQuery=1 ;
		else
			if(nMaxGPUQuery>MAXGPUQUERY) 
			nMaxGPUQuery=MAXGPUQUERY ;


		rqListener = new rQueueListener ;
		
		OgreFramework::getSingletonPtr()->m_pSceneMgr->addRenderQueueListener(rqListener) ;

		// create our queries
		pHOQ=rqListener->Query ;
		for(nNewQuery=0 ; nNewQuery<nMaxGPUQuery ; nNewQuery++)
			pHOQ[nNewQuery] = OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->createHardwareOcclusionQuery() ;

		nNewQuery=nOldQuery-1 ;
		if(nNewQuery<0)
			nNewQuery+=nMaxGPUQuery ;

		// set the queries to not ready
		for(int nReadyLoop=0 ; nReadyLoop<MAXGPUQUERY ; nReadyLoop++)
			nQueryReady[nReadyLoop]=0 ;
	}
	//
	/////////////////////////////////////////////////////////////////////////////////////////////



	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			Ogre::WindowEventUtilities::messagePump() ;
#endif	


			//OgreFramework::getSingletonPtr()->m_pLog->logMessage("Update START");


		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{

			// get start time of frame
			uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
				
			// update input and physics
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
			OgreFramework::getSingletonPtr()->updateOgre(uFrameTotalTime/1000.0f);

			
			if(nMaxGPUQuery==0) // querying the GPU command buffer is disabled
			{
				// render the frame
				OgreFramework::getSingletonPtr()->UpdateRenderTargets() ;
			}
			else								// querying the GPU command buffer is enabled
			{
				// increment the buffer.  
				nNewQuery=nOldQuery ;
				nOldQuery++ ;
				if(nOldQuery==nMaxGPUQuery)
					nOldQuery=0 ;


				// define query beginning for this frame
				pHOQ[ nNewQuery ]->beginOcclusionQuery() ;

				// render the frame
				//OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
				OgreFramework::getSingletonPtr()->UpdateRenderTargets() ;

				// define query end for this frame
				pHOQ[ nNewQuery ]->endOcclusionQuery() ;
				nQueryReady[ nNewQuery ]=1 ;
			


				// pull query for a prior frame.  Flushes GPU command buffer up to the end of a prior frame but no further.
				if(nQueryReady[ nOldQuery ])
				{
					pHOQ[ nOldQuery ]->pullOcclusionQuery(&dummy) ;
					nQueryReady[ nOldQuery ]=0 ;
				}
			}
			


			// calculate frame time.
			uFrameTotalTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds()-uFrameStartTime ;

		}
		else
		{
			Sleep(1000);
		}


	}

	if(nMaxGPUQuery>0) // if necessary, clean up our HOQ queries and renderQueueListener
	{
	
		for(nNewQuery=0 ; nNewQuery<nMaxGPUQuery ; nNewQuery++)
			OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->destroyHardwareOcclusionQuery( pHOQ[nNewQuery] ) ;
		
		OgreFramework::getSingletonPtr()->m_pSceneMgr->removeRenderQueueListener(rqListener) ;
		delete rqListener ;
	}


	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit (MethodA)");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");


	return 1 ;
}

int DemoApp::runDemo_MethodB()
{

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop (MethodB)...");

//	char chMessage[1024] ;
	
	UINT uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	UINT uFrameTotalTime=0 ;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics() ;
	

	
	int numberOfQueuedFrames=1 ;
	bool GPUBufferSetupDone=false ;


	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			Ogre::WindowEventUtilities::messagePump() ;
#endif	


		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{


			   if(GPUBufferSetupDone==false) // I added it here because I assume I can be very sure there's an active render window by now.
         {
            GPUBufferSetupDone=true ;
            mBufferFlush.start(numberOfQueuedFrames);
         }

         // get start time of frame
         uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
            
         // update input and physics
         OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
         OgreFramework::getSingletonPtr()->m_pMouse->capture();
         OgreFramework::getSingletonPtr()->updateOgre(uFrameTotalTime/1000.0f);


         // render the frame
				 OgreFramework::getSingletonPtr()->UpdateRenderTargets() ;




			// calculate frame time.
			uFrameTotalTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds()-uFrameStartTime ;

		}
		else
		{
			Sleep(1000);
		}



	}


	mBufferFlush.stop() ;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit (MethodB)");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");


	return 1 ;
}


int DemoApp::runDemo_MethodC()
{

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop (MethodC)...");

	char chMessage[1024] ;
	
	int uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	int uFrameRenderTime=0 ;
	int uFrameTargetTime=10000 ;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics() ;
	
	

	int uSleepTime=0 ;

	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			Ogre::WindowEventUtilities::messagePump() ;
#endif	




		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{


       // get start time of frame
       uFrameStartTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
            
         // update input and physics
         OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
         OgreFramework::getSingletonPtr()->m_pMouse->capture();
         OgreFramework::getSingletonPtr()->updateOgre(uFrameTargetTime/1000.0f);


         // render the frame
				 OgreFramework::getSingletonPtr()->UpdateRenderTargets() ;


			// calculate frame time.
			uFrameRenderTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds()-uFrameStartTime ;
			

			uFrameTargetTime-=1000 ;
			if(uFrameTargetTime<0) uFrameTargetTime=0 ;

			if(uFrameRenderTime>uFrameTargetTime-10000)
				uFrameTargetTime=uFrameRenderTime+20000 ;

			if(uFrameTargetTime>100000) uFrameTargetTime=100000 ;


			


			uSleepTime=(int)((uFrameTargetTime-uFrameRenderTime)/1000.0f) ;
			if(uSleepTime>0) Sleep(uSleepTime) ;

			sprintf(chMessage, "render %i, target %i, sleep %i", uFrameRenderTime, uFrameTargetTime, uSleepTime) ;
			OgreFramework::getSingletonPtr()->m_pLog->logMessage(chMessage);

		}
		else
		{
			Sleep(1000);
		}


	}



	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit (MethodC)");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");


	return 1 ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







void DemoApp::ResetSectionTimer()
{
	m_uLastTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
}

UINT DemoApp::GetSectionTimer()
{
	UINT uThisTime=OgreFramework::getSingletonPtr()->m_pTimer->getMicroseconds() ;
	UINT uSectionTime=uThisTime-m_uLastTime ;
	m_uLastTime=uThisTime ;
	return uSectionTime ;
}

//|||||||||||||||||||||||||||||||||||||||||||||||


bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		 //do something
	}
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	
	return true;
}


//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::StereoConfig_Write( void )
{
	int fh;
	if( (fh = _open( "stereoscopic.cfg", _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, 
                               _S_IREAD | _S_IWRITE )) != -1 )
	{
		write( fh, (void*)&OgreFramework::getSingletonPtr()->m_nGoggleMode,						sizeof(int)) ;
		write( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleZPos,					sizeof(float))	;
		write( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleXGap,					sizeof(float))	;
		write( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleXScale,				sizeof(float))	;
		write( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleYScale,				sizeof(float))	;
		_close( fh );
	}
}
bool DemoApp::StereoConfig_Read(void)
{
 	int fh;
	bool found_file=true ;

	// Open file for input
	if( (fh = _open( "stereoscopic.cfg", _O_RDONLY | _O_BINARY )) == -1 )
	{
		found_file=false ;
	}
	else
	{
		read( fh, (void*)&OgreFramework::getSingletonPtr()->m_nGoggleMode,						sizeof(int)) ;
		read( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleZPos,						sizeof(float))	;
		read( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleXGap,						sizeof(float))	;
		read( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleXScale,					sizeof(float))	;
		read( fh, (void*)&OgreFramework::getSingletonPtr()->m_flGoggleYScale,					sizeof(float))	;
		_close( fh );
	 }//  end if else
	return found_file ;
}


void DemoApp::BZNConfig_Write(void)
{
	int fh;
	if( (fh = _open( "bzn.cfg", _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, 
                               _S_IREAD | _S_IWRITE )) != -1 )
	{
		write( fh, (void*)OgreFramework::getSingletonPtr()->m_chBZNConfig,						BZN_CONFIG_SIZE) ;
		_close( fh );
	}
}

bool DemoApp::BZNConfig_Read(void)
{

	int fh;
	bool found_file=true ;

	// make sure the config memory is blank
	ZeroMemory((void*)OgreFramework::getSingletonPtr()->m_chBZNConfig, BZN_CONFIG_SIZE) ;

	// Open file for input
	if( (fh = _open( "bzn.cfg", _O_RDONLY | _O_BINARY )) == -1 )
	{
		found_file=false ;
	}
	else
	{
		read( fh, (void*)OgreFramework::getSingletonPtr()->m_chBZNConfig,						BZN_CONFIG_SIZE) ;
		_close( fh );
		OgreFramework::getSingletonPtr()->ParseBZNConfig() ;
	 }//  end if else
	
	
	return found_file ;

}
