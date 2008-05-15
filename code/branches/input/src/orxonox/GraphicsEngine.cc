/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (atl your option) any later version.
 *lo
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      Reto Grieder
 *
 */

 /**
    @file orxonox.cc
    @brief Orxonox class
  */

#include "OrxonoxStableHeaders.h"
#include "GraphicsEngine.h"

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreLogManager.h>
#include <OgreTextureManager.h>
#include "core/InputManager.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Debug.h"
#include "core/CommandExecutor.h"


namespace orxonox {

  /**
    @brief Returns the singleton instance and creates it the first time.
    @return The only instance of GraphicsEngine.
  */
  /*static*/ GraphicsEngine& GraphicsEngine::getSingleton()
  {
    static GraphicsEngine theOnlyInstance;
    return theOnlyInstance;
  }

  /**
    @brief Only use constructor to initialise variables and pointers!
  */
  GraphicsEngine::GraphicsEngine() :
    root_(0),
    scene_(0),
    renderWindow_(0),
    //configPath_(""),
    dataPath_(""),
    ogreLogfile_("")
  {
    RegisterObject(GraphicsEngine);

    this->setConfigValues();
    CCOUT(4) << "Constructed" << std::endl;
  }

  void GraphicsEngine::setConfigValues()
  {
    SetConfigValue(dataPath_, "../../Media/").description("relative path to media data");
    SetConfigValue(ogreLogfile_, "ogre.log").description("Logfile for messages from Ogre. Use \"\" to suppress log file creation.");
    SetConfigValue(ogreLogLevelTrivial_ , 5).description("Corresponding orxonox debug level for ogre Trivial");
    SetConfigValue(ogreLogLevelNormal_  , 4).description("Corresponding orxonox debug level for ogre Normal");
    SetConfigValue(ogreLogLevelCritical_, 2).description("Corresponding orxonox debug level for ogre Critical");
  }

  /**
    @brief Called after main() --> call destroyObjects()!
  */
  GraphicsEngine::~GraphicsEngine()
  {
    this->destroy();
  }

  /**
    @brief Destroys all the internal objects. Call this method when you
           normally would call the destructor.
  */
  void GraphicsEngine::destroy()
  {
    CCOUT(4) << "Destroying objects..." << std::endl;
    Ogre::WindowEventUtilities::removeWindowEventListener(this->renderWindow_, this);
    if (this->root_)
      delete this->root_;
    this->root_ = 0;
    this->scene_ = 0;
    this->renderWindow_ = 0;
    // delete the ogre log and the logManager (since we have created it).
    if (Ogre::LogManager::getSingletonPtr() != 0)
    {
      Ogre::LogManager::getSingleton().getDefaultLog()->removeListener(this);
      Ogre::LogManager::getSingleton().destroyLog(Ogre::LogManager::getSingleton().getDefaultLog());
      delete Ogre::LogManager::getSingletonPtr();
    }
    CCOUT(4) << "Destroying objects done" << std::endl;
  }

  /**
    @brief Creates the Ogre Root object and sets up the ogre log.
  */
  bool GraphicsEngine::setup(std::string& dataPath)
  {
    CCOUT(3) << "Setting up..." << std::endl;
    // temporary overwrite of dataPath, change ini file for permanent change
    if (dataPath != "")
      dataPath_ = dataPath;
    if (dataPath_ == "")
      return false;
    if (dataPath_[dataPath_.size() - 1] != '/')
      dataPath_ += "/";

    //TODO: Check if file exists (maybe not here)
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC && defined(_DEBUG)
    std::string plugin_filename = "plugins_d.cfg";
#else
    std::string plugin_filename = "plugins.cfg";
#endif

/*    // create a logManager
    // note: If there's already a logManager, Ogre will complain by a failed assertation.
    // but that shouldn't happen, since this is the first time to create a logManager..
    Ogre::LogManager* logger = new Ogre::LogManager();
    CCOUT(4) << "Ogre LogManager created" << std::endl;

    // create our own log that we can listen to
    Ogre::Log *myLog;
    if (this->ogreLogfile_ == "")
      myLog = logger->createLog("ogre.log", true, false, true);
    else
          myLog = logger->createLog(this->ogreLogfile_, true, false, false);
    CCOUT(4) << "Ogre Log created" << std::endl;

    myLog->setLogDetail(Ogre::LL_BOREME);
    myLog->addListener(this);*/

    // Root will detect that we've already created a Log
    CCOUT(4) << "Creating Ogre Root..." << std::endl;
    root_ = new Ogre::Root(plugin_filename);
    CCOUT(4) << "Creating Ogre Root done" << std::endl;

    // specify where Ogre has to look for resources. This call doesn't parse anything yet!
    declareRessourceLocations();

    CCOUT(3) << "Set up done." << std::endl;
    return true;
  }

  void GraphicsEngine::declareRessourceLocations()
  {
    CCOUT(4) << "Declaring Resources" << std::endl;
    //TODO: Specify layout of data file and maybe use xml-loader
    //TODO: Work with ressource groups (should be generated by a special loader)
    // Load resource paths from data file using configfile ressource type
    Ogre::ConfigFile cf;
    cf.load(dataPath_ + "resources.cfg");

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, typeName, archName;
    while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
        typeName = i->first; // for instance "FileSystem" or "Zip"
        archName = i->second; // name (and location) of archive

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                                           std::string(dataPath_ + archName),
                                           typeName, secName);
      }
    }
  }

  bool GraphicsEngine::loadRenderer()
  {
    CCOUT(4) << "Configuring Renderer" << std::endl;
    if (!root_->restoreConfig() && !root_->showConfigDialog())
      return false;

    CCOUT(4) << "Creating render window" << std::endl;
    this->renderWindow_ = root_->initialise(true, "OrxonoxV2");
    if (!root_->isInitialised())
    {
      CCOUT(2) << "Error: Creating Ogre root object failed" << std::endl;
      return false;
    }
    Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, this);
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    return true;
  }

  bool GraphicsEngine::initialiseResources()
  {
    CCOUT(4) << "Initialising resources" << std::endl;
    //TODO: Do NOT load all the groups, why are we doing that? And do we really do that? initialise != load...
    try
    {
      Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      /*Ogre::StringVector str = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
      for (unsigned int i = 0; i < str.size(); i++)
      {
        Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(str[i]);
      }*/
    }
    catch (Ogre::Exception e)
    {
      CCOUT(2) << "Error: There was an Error when initialising the resources." << std::endl;
      CCOUT(2) << "ErrorMessage: " << e.getFullDescription() << std::endl;
      return false;
    }
    return true;
  }

  /**
   * @brief Creates the SceneManager
   */
  bool GraphicsEngine::createNewScene()
  {
    CCOUT(4) << "Creating new SceneManager" << std::endl;
    if (scene_)
    {
      CCOUT(2) << "SceneManager already exists! Skipping." << std::endl;
      return false;
    }
    scene_ = root_->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
    CCOUT(3) << "Created SceneManager: " << scene_ << std::endl;
    return true;
  }

  /**
    Returns the window handle of the render window.
    At least the InputHandler uses this to create the OIS::InputManager
    @return The window handle of the render window
  */
  size_t GraphicsEngine::getWindowHandle()
  {
    if (this->renderWindow_)
    {
      size_t windowHnd = 0;
      this->renderWindow_->getCustomAttribute("WINDOW", &windowHnd);
      return windowHnd;
    }
    else
      return 0;
  }

  /**
    Get the width of the current render window
    @return The width of the current render window
  */
  int GraphicsEngine::getWindowWidth() const
  {
    if (this->renderWindow_)
      return this->renderWindow_->getWidth();
    else
      return 0;
  }

  /**
    Get the height of the current render window
    @return The height of the current render window
  */
  int GraphicsEngine::getWindowHeight() const
  {
    if (this->renderWindow_)
      return this->renderWindow_->getHeight();
    else
      return 0;
  }

  /**
    @brief Method called by the LogListener interface from Ogre.
    We use it to capture Ogre log messages and handle it ourselves.
    @param message The message to be logged
    @param lml The message level the log is using
    @param maskDebug If we are printing to the console or not
    @param logName the name of this log (so you can have several listeners
                   for different logs, and identify them)
  */
  void GraphicsEngine::messageLogged(const std::string& message,
    Ogre::LogMessageLevel lml, bool maskDebug, const std::string &logName)
  {
    int orxonoxLevel;
    switch (lml)
    {
      case Ogre::LML_TRIVIAL:
        orxonoxLevel = this->ogreLogLevelTrivial_;
        break;
      case Ogre::LML_NORMAL:
        orxonoxLevel = this->ogreLogLevelNormal_;
        break;
      case Ogre::LML_CRITICAL:
        orxonoxLevel = this->ogreLogLevelCritical_;
        break;
      default:
        orxonoxLevel = 0;
    }
    OutputHandler::getOutStream().setOutputLevel(orxonoxLevel)
        << "Ogre: " << message << std::endl;
  }

  /**
  * Window has resized.
  * @param rw The render window it occured in
  */
  void GraphicsEngine::windowMoved(Ogre::RenderWindow *rw)
  {
    // note: this doesn't change the window extents
  }

  /**
  * Window has resized.
  * @param rw The render window it occured in
  * @note GraphicsEngine has a render window stored itself. This is the same
  *       as rw. But we have to be careful when using multiple render windows!
  */
  void GraphicsEngine::windowResized(Ogre::RenderWindow *rw)
  {
    // change the mouse clipping size for absolute mouse movements
    int w = rw->getWidth();
    int h = rw->getHeight();
    InputManager::setWindowExtents(w, h);
  }

  /**
  * Window has resized.
  * @param rw The render window it occured in
  */
  void GraphicsEngine::windowFocusChanged(Ogre::RenderWindow *rw)
  {
    // note: this doesn't change the window extents
  }

  /**
  * Window has resized.
  * @param rw The render window it occured in
  */
  void GraphicsEngine::windowClosed(Ogre::RenderWindow *rw)
  {
    // using CommandExecutor in order to avoid depending on Orxonox class.
    CommandExecutor::execute("exit", false);
  }
}
