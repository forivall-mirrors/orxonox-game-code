/*
*   ORXONOX - the hottest 3D action shooter ever to exist
*
*
*   License notice:
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
*   Author:
*      Reto Grieder
*   Co-authors:
*      ...
*/

/**
* RunManager is the basic control object during the game.
* 
* The RunManger class is designed to actually "run" the main part of the
* game. The Idea is, that you could derive from the RunManager in order
* to distinguish between a first person shooter or a space craft shooter.
* RunManager loads and initialises everything in the scene (like the ship,
* the enemies in the scene, any scripts, the physics, window events,
* environment, HUD, etc.).
* It also captures any input from keyboard, mous, joystick (optional) or
* Ogre (window events).
*/

#include "RunManager.h"


/**
* Contructor only needs the render window and the Root object which are both
* the OgreControl object.
* Right now the constructor does all the initialisation work. This could also
* be done in a new method "initialize()", for whatever purpose.
* 
* 
* @param mOgre The OgreControl object holding the render window and the Root
*/
RunManager::RunManager(OgreControl * mOgre, bool bufferedKeys,
                       bool bufferedMouse, bool bufferedJoy )
                       : mOgre(mOgre), mWindow(mOgre->getRenderWindow()), leftButtonDown(false),
                       mStatsOn(true), mNumScreenShots(0), mTimeUntilNextToggle(0),
                       mFiltering(TFO_BILINEAR), mAniso(1), mSceneDetailIndex(0),
                       mDebugOverlay(0), mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
{

  // SETTING UP THE SCENE

  // create one new SceneManger
  mSceneMgr = mOgre->getRoot()->createSceneManager(ST_GENERIC, "mScene");

  // background scene (world objects, skybox, lights, etc.)
  mScene = new OrxonoxScene(mSceneMgr);

  // PLAYER SPACESHIP

  // create a steerable SceneNode (not derived!) object. The idea is that this
  // object only receives the mouse and the keyboard input (not specifi keys,
  // more like up, down, left, right, roll left, roll right, move down,
  // move up). The steering class can then decide how to control the node for
  // the spaceship. This gives a certain flexibility.
  // It should also be considered, that this class should provide another Node
  // for a camera to be attached (otherwise the spaceship in front of the
  // would be very static, never moving at all.
  mShipNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ShipNode",
    Vector3(20, 20, 20));

  // Construct a new spaceship and attach it to the node
  mShip = new OrxonoxShip(mSceneMgr, mShipNode);


  // RESOURCE LOADING (using ResourceGroups if implemented)

  // load all resources and create the entities by calling the initialise()
  // methods for each object (don't initialise in the constructor!).
  mScene->initialise();
  mShip->initialise();


  // CAMERA AND VIEWPORT
  // TODO: create a camera manager. It should be able to change its position
  // around the space ship (predefined states would be nice too). And it should
  // also be able to switch between different locations (like ship, spactator,
  // certain fixed positions (e.g. finish line, etc.)). These are just ideas.

  // create camera and viewport
  createCamera();
  createViewports();


  // Set default mipmap level (NB some APIs ignore this)
  TextureManager::getSingleton().setDefaultNumMipmaps(5);

  
  // BULLET LIST FOR THE TEST APPLICATION

  // TODO: Use STL to make life easier. But it works this way too..
  mBullets = new Bullet*[10];
  mBulletsPosition = 0;
  mBulletsSize = 10;


  // HUMAN INTERFACE

  using namespace OIS;

  mDebugOverlay = OverlayManager::getSingleton()
    .getByName("Core/DebugOverlay");

  LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = InputManager::createInputSystem( pl );

  // Create all devices (We only catch joystick exceptions here,
  // as, most people have Key/Mouse)
  mKeyboard = static_cast<Keyboard*>(mInputManager
    ->createInputObject( OISKeyboard, bufferedKeys ));
  mMouse = static_cast<Mouse*>(mInputManager
    ->createInputObject( OISMouse, bufferedMouse ));
  try {
    mJoy = static_cast<JoyStick*>(mInputManager
      ->createInputObject( OISJoyStick, bufferedJoy ));
  }
  catch(...) {
    mJoy = 0;
  }

  //Set initial mouse clipping size
  windowResized(mWindow);

  showDebugOverlay(true);

  // REGISTER THIS OBJECT AS A WINDOW EVENT LISTENER IN OGRE
  // It will then receive events liek windowClosed, windowResized, etc.
  WindowEventUtilities::addWindowEventListener(mWindow, this);
}

/**
* Standard destructor.
* Removes this object as a window event listener and deletes all created
* variables.
*/
RunManager::~RunManager()
{
  //Remove ourself as a Window listener
  WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);

  if (mScene)
    delete mScene;
  if (mShip)
    delete mShip;

  // clean up the bullet list
  for (int i = 0; i < mBulletsPosition; i++)
    delete mBullets[i];
  delete mBullets;
}


/**
* Method to compute anyting between 2 frames.
* 
* Everything that needs to be computed during the games happens right here.
* The only exception are the listeners (which should only set variables,
* not actually do something).
* 
* @param time Absolute play time
* @param deltaTime Time passed since last frame
* @return Return false to end rendering
*/
bool RunManager::tick(unsigned long time, float deltaTime)
{
  // synchronize with internal class timer
  mTime = time;

  // Call tick() for every object
  // This could be done by registering (needs a factory..)
  mScene->tick(time, deltaTime);
  mShip->tick(time, deltaTime);


  // Update the 'HUD'
  updateStats();

  // update the bullet positions
  for (int i = 0; i < mBulletsPosition; i++)
  {
    mBullets[i]->mNode->translate(mBullets[i]->mSpeed*deltaTime);
    mBullets[i]->mNode->yaw(Degree(deltaTime*100));
    mBullets[i]->mNode->roll(Degree(deltaTime*300));
  }

  // HUMAN INTERFACE

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
    // one of the input modes is immediate, so setup what
    // is needed for immediate movement
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

  // keep rendering
  return true;
}


/**
* Adjust mouse clipping area.
* This method is called by Ogre without regards of tick()!
* Avoid doing too much in this call.
* @param rw render window
*/
void RunManager::windowResized(RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}


/**
* Unattach OIS before window shutdown (very important under Linux).
* Again, avoid computing a lot in this function.
* @param rw Render Window
*/
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

/**
* Processes the Keyboard input.
* TODO: Use listeners to improve performance.
* A lookup table should be implemented to bind any key to a specific action.
* @return Return true to keep rendering
*/
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
  // mDebugText = "P: " + StringConverter::toString(mCamera
  //      ->getDerivedPosition()) + " " + "O: "
  //      + StringConverter::toString(mCamera->getDerivedOrientation());

  // Return true to continue rendering
  return true;
}


/**
* Processes the Mouse input.
* TODO: Use listeners to improve performance.
* A lookup table should be implemented to bind ANY button or movement
* to a specific action.
* @return Return true to keep rendering
*/
bool RunManager::processUnbufferedMouseInput()
{
  using namespace OIS;

  const MouseState &ms = mMouse->getMouseState();

  // This is a 'hack' to show some flying barrels..
  // Usually, the Bullet created by the ship should be managed
  // by the physics engine..
  if (ms.buttonDown(MB_Left) && !leftButtonDown)
  {
    // Prevent continuous fire for the moment.
    leftButtonDown = true;
    
    // let ship fire one shot with its only weapon (Barrels..)
    Bullet *mTempBullet = mShip->fire();

    // resize array if neccessary (double the size then)
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

    // add the bullet to the list
    mBullets[mBulletsPosition++] = mTempBullet;

  }
  else if (!ms.buttons)
    leftButtonDown = false;

  // space ship steering. This should definitely be done in the steering object
  // Simply give it the mouse movements.
  mShip->mRootNode->pitch(Degree(-ms.Y.rel * 0.13), Ogre::Node::TransformSpace::TS_LOCAL);
  mShip->mRootNode->yaw(Degree(-ms.X.rel * 0.13), Ogre::Node::TransformSpace::TS_PARENT);

  // keep rendering
  return true;
}

/**
* Show the debug overlay of desired.
* @param show Whether or not to show the debug overlay
*/
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


/**
* Show stats (e.g. FPS) in the left lower corner of the screen.
* Copied from the ExampleFrameListener.h in the Ogre SDK
*/
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
    OverlayElement* guiAvg = OverlayManager::getSingleton()
      .getOverlayElement("Core/AverageFps");
    OverlayElement* guiCurr = OverlayManager::getSingleton()
      .getOverlayElement("Core/CurrFps");
    OverlayElement* guiBest = OverlayManager::getSingleton()
      .getOverlayElement("Core/BestFps");
    OverlayElement* guiWorst = OverlayManager::getSingleton()
      .getOverlayElement("Core/WorstFps");

    const RenderTarget::FrameStats& stats = mWindow->getStatistics();
    guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
    guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
    guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
      +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
    guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
      +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

    OverlayElement* guiTris = OverlayManager::getSingleton()
      .getOverlayElement("Core/NumTris");
    guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

    OverlayElement* guiBatches = OverlayManager::getSingleton()
      .getOverlayElement("Core/NumBatches");
    guiBatches->setCaption(batches
      + StringConverter::toString(stats.batchCount));

    OverlayElement* guiDbg = OverlayManager::getSingleton()
      .getOverlayElement("Core/DebugText");
    guiDbg->setCaption(mDebugText);
  }
  catch(...) { /* ignore */ }
}



/**
* Simple camera creator.
* mShipNode->attachObject(mCamera) should no be here! This is what the camera
* manager is for. Right now, this method should do just fine, setting the
* cam behind the ship.
*/
void RunManager::createCamera(void)
{
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mShipNode->attachObject(mCamera);
  mCamera->setNearClipDistance(5);
  mCamera->setPosition(Vector3(0,10,500));
  mCamera->lookAt(Vector3(0,0,0));
}

/**
* Simple viewport creator.
* TODO: fully understand the concept of viewports concerning orxnox.
* E.g. do we need splitscreen mode?
* For now the viewport uses the entire render window and is based on the one
* camera created so far.
*/
void RunManager::createViewports(void)
{
  // Create one viewport, entire window
  Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(ColourValue(0,0,0));

  // Alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(
    Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}


