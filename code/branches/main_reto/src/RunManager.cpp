#include "RunManager.h"


// Constructor takes a RenderWindow because it uses that to determine input context
RunManager::RunManager(OgreControl * mOgre, bool bufferedKeys, bool bufferedMouse,
					   bool bufferedJoy ) :
mOgre(mOgre), mWindow(mOgre->getRenderWindow()), leftButtonDown(false),
mStatsOn(true), mNumScreenShots(0),
mTimeUntilNextToggle(0), mFiltering(TFO_BILINEAR),
mAniso(1), mSceneDetailIndex(0), mDebugOverlay(0),
mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
{
	// create new SceneManger
	mSceneMgr = mOgre->getRoot()->createSceneManager(ST_GENERIC,"mScene");


	// create various objects
	// background scene
	mScene = new OrxonoxScene(mSceneMgr);

	// create a steerable SceneNode for the spaceship to be attached to
	mShipNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ShipNode", Vector3(20, 20, 20));

	// spaceship
	mShip = new OrxonoxShip(mSceneMgr, mShipNode);

	// load all resources and create the entities
	mScene->initialise();
	mShip->initialise();

	// create camera and viewport
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// initialise bullets list
	mBullets = new Bullet*[10];
	mBulletsPosition = 0;
	mBulletsSize = 10;

	using namespace OIS;

	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = InputManager::createInputSystem( pl );

	//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, bufferedKeys ));
	mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, bufferedMouse ));
	try {
		mJoy = static_cast<JoyStick*>(mInputManager->createInputObject( OISJoyStick, bufferedJoy ));
	}
	catch(...) {
		mJoy = 0;
	}

	//Set initial mouse clipping size
	windowResized(mWindow);

	showDebugOverlay(true);

	//Register as a Window listener
	WindowEventUtilities::addWindowEventListener(mWindow, this);
}


RunManager::~RunManager()
{
	//Remove ourself as a Window listener
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	if (mScene)
		delete mScene;

	for (int i = 0; i < mBulletsPosition; i++)
		delete mBullets[i];
	delete mBullets;
}


// Override frameStarted event to process that (don't care about frameEnded)
bool RunManager::tick(unsigned long time, float deltaTime)
{
	mTime = time;

	updateStats();
	
	mScene->tick(time, deltaTime);
	mShip->tick(time, deltaTime);

	// update the bullet positions
	for (int i = 0; i < mBulletsPosition; i++)
	{
		mBullets[i]->mNode->translate(mBullets[i]->mSpeed*deltaTime);
		mBullets[i]->mNode->yaw(Degree(deltaTime*100));
		mBullets[i]->mNode->roll(Degree(deltaTime*300));
	}

	using namespace OIS;

	if(mWindow->isClosed())	return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	if( mJoy ) mJoy->capture();

	bool buffJ = (mJoy) ? mJoy->buffered() : true;

	//Check if one of the devices is not buffered
	if( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
	{
		// one of the input modes is immediate, so setup what is needed for immediate movement
		if (mTimeUntilNextToggle >= 0)
			mTimeUntilNextToggle -= deltaTime;
	}

	//Check to see which device is not buffered, and handle it
	if( !mKeyboard->buffered() )
		if( processUnbufferedKeyInput() == false )
			return false;
	if( !mMouse->buffered() )
		if( processUnbufferedMouseInput() == false )
			return false;

	return true;
}


//Adjust mouse clipping area
void RunManager::windowResized(RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


//Unattach OIS before window shutdown (very important under Linux)
void RunManager::windowClosed(RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );
			mInputManager->destroyInputObject( mJoy );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}


bool RunManager::processUnbufferedKeyInput()
{
	using namespace OIS;

	if(mKeyboard->isKeyDown(KC_A) || mKeyboard->isKeyDown(KC_LEFT))
		mShip->setSideThrust(1);
	else if(mKeyboard->isKeyDown(KC_D) || mKeyboard->isKeyDown(KC_RIGHT))
		mShip->setSideThrust(-1);
	else
		mShip->setSideThrust(0);

	if(mKeyboard->isKeyDown(KC_UP) || mKeyboard->isKeyDown(KC_W) )
		mShip->setThrust(1);
	else if(mKeyboard->isKeyDown(KC_DOWN) || mKeyboard->isKeyDown(KC_S) )
		mShip->setThrust(-1);
	else
		mShip->setThrust(0);

	if( mKeyboard->isKeyDown(KC_ESCAPE) || mKeyboard->isKeyDown(KC_Q) )
		return false;

	if( mKeyboard->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0 )
	{
		mStatsOn = !mStatsOn;
		showDebugOverlay(mStatsOn);
		mTimeUntilNextToggle = 1;
	}

	if( mKeyboard->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0 )
	{
		switch(mFiltering)
		{
		case TFO_BILINEAR:
			mFiltering = TFO_TRILINEAR;
			mAniso = 1;
			break;
		case TFO_TRILINEAR:
			mFiltering = TFO_ANISOTROPIC;
			mAniso = 8;
			break;
		case TFO_ANISOTROPIC:
			mFiltering = TFO_BILINEAR;
			mAniso = 1;
			break;
		default: break;
		}
		MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
		MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

		showDebugOverlay(mStatsOn);
		mTimeUntilNextToggle = 1;
	}

	if(mKeyboard->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
	{
		std::ostringstream ss;
		ss << "screenshot_" << ++mNumScreenShots << ".png";
		mWindow->writeContentsToFile(ss.str());
		mTimeUntilNextToggle = 0.5;
		mDebugText = "Saved: " + ss.str();
	}

	if(mKeyboard->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
	{
		mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
		switch(mSceneDetailIndex) {
				case 0 : mCamera->setPolygonMode(PM_SOLID); break;
				case 1 : mCamera->setPolygonMode(PM_WIREFRAME); break;
				case 2 : mCamera->setPolygonMode(PM_POINTS); break;
		}
		mTimeUntilNextToggle = 0.5;
	}

	static bool displayCameraDetails = false;
	if(mKeyboard->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
	{
		displayCameraDetails = !displayCameraDetails;
		mTimeUntilNextToggle = 0.5;
		if (!displayCameraDetails)
			mDebugText = "";
	}

	// Print camera details
	if(displayCameraDetails)
		mDebugText = StringConverter::toString(mShip->getThrust())
		+ " | Speed = " + StringConverter::toString(mShip->speed);
		// mDebugText = "P: " + StringConverter::toString(mCamera->getDerivedPosition()) +
		// " " + "O: " + StringConverter::toString(mCamera->getDerivedOrientation());

	// Return true to continue rendering
	return true;
}


bool RunManager::processUnbufferedMouseInput()
{
	using namespace OIS;

	// Rotation factors, may not be used if the second mouse button is pressed
	// 2nd mouse button - slide, otherwise rotate
	const MouseState &ms = mMouse->getMouseState();

	if (ms.buttonDown(MB_Left) && !leftButtonDown)
	{
		leftButtonDown = true;
		// fire
		Bullet *mTempBullet = mShip->fire();
		if (mBulletsPosition >= mBulletsSize)
		{
			// redimension the array
			Bullet **mTempArray = new Bullet*[2*mBulletsSize];
			for (int i = 0; i < mBulletsSize; i++)
				mTempArray[i] = mBullets[i];
			mBulletsSize *= 2;
			delete mBullets;
			mBullets = mTempArray;
		}
		mBullets[mBulletsPosition++] = mTempBullet;

	}
	else if (!ms.buttons)
		leftButtonDown = false;

	mShip->setYaw(Degree(-ms.X.rel * 0.13));
	mShip->setPitch(Degree(-ms.Y.rel * 0.13));

	return true;
}


void RunManager::showDebugOverlay(bool show)
{
	if (mDebugOverlay)
	{
		if (show)
			mDebugOverlay->show();
		else
			mDebugOverlay->hide();
	}
}



void RunManager::updateStats(void)
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";
	static String batches = "Batch Count: ";

	// update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
		//OverlayElement* asfd = OverlayManager::getSingleton().getOverlayElement(

		const RenderTarget::FrameStats& stats = mWindow->getStatistics();
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);
	}
	catch(...) { /* ignore */ }
}



// create camera
void RunManager::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mShipNode->attachObject(mCamera);
	mCamera->setNearClipDistance(5);
	mCamera->setPosition(Vector3(0,10,500));
	mCamera->lookAt(Vector3(0,0,0));
}


void RunManager::createViewports(void)
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}


