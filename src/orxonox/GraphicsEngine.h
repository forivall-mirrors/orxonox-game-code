/**
  @file GraphicsEngine.h
  @brief Graphics Engine
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include <string>

#include <OgrePrerequisites.h>
#include <OgreLog.h>

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"


namespace orxonox {

  /**
    @brief Graphics engine manager class
  */
  class _OrxonoxExport GraphicsEngine : public OrxonoxClass, public Ogre::LogListener
  {
    friend class ClassIdentifier<GraphicsEngine>;
    public:
      void setConfigPath(std::string path) { this->configPath_ = path; };
      void setConfigValues();
      void setup();
      bool load(std::string path);
      void loadRessourceLocations(std::string path);
      Ogre::SceneManager* getSceneManager();
      void initialise();
      void destroy();

      // several window properties
      Ogre::RenderWindow* getRenderWindow() { return this->renderWindow_; }
      size_t getWindowHandle();
      int getWindowWidth() const;
      int getWindowHeight() const;

      static GraphicsEngine& getSingleton();

    private:
      // don't mess with singletons
      GraphicsEngine();
      GraphicsEngine(GraphicsEngine&) { }
      ~GraphicsEngine();

      //! Method called by the LogListener from Ogre
      void messageLogged(const std::string&, Ogre::LogMessageLevel,
                         bool, const std::string&);

      Ogre::Root*         root_;        //!< Ogre's root
      Ogre::SceneManager* scene_;       //!< scene manager of the game
      Ogre::RenderWindow* renderWindow_;//!< the current render window
      //bool               bOverwritePath_; //!< overwrites path
      std::string         configPath_;  //!< path to config file
      std::string         dataPath_;    //!< path to data file
      std::string         ogreLogfile_; //!< log file name for Ogre log messages
      int ogreLogLevelTrivial_;         //!< Corresponding Orxonx debug level for LL_TRIVIAL
      int ogreLogLevelNormal_;          //!< Corresponding Orxonx debug level for LL_NORMAL
      int ogreLogLevelCritical_;        //!< Corresponding Orxonx debug level for LL_CRITICAL

  };

}

#endif /* _GraphicsEngine_H__ */
