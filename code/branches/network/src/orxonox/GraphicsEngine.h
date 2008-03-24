/**
  @file GraphicsEngine.h
  @brief Graphics Engine
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include <string>

#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include "OrxonoxPrereqs.h"


namespace orxonox {

/**
   * graphics engine manager class
 */
  class _OrxonoxExport GraphicsEngine {
    public:
      GraphicsEngine();
      inline void setConfigPath(std::string path) { this->configPath_ = path; };
      // find a better way for this
      inline Ogre::Root* getRoot() { return root_; };
      void setup();
      bool load();
      void loadRessourceLocations(std::string path);
      Ogre::SceneManager* getSceneManager();
      void startRender();

      // several window properties
      size_t getWindowHandle();
      int getWindowWidth() const;
      int getWindowHeight() const;

      virtual ~GraphicsEngine();
    private:
      Ogre::Root*         root_;        //!< Ogre's root
      std::string         configPath_;  //!< path to config file
      std::string         dataPath_;    //!< path to data file
      Ogre::SceneManager* scene_;       //!< scene manager of the game
      Ogre::RenderWindow* renderWindow_;//!< the current render window

  };

}

#endif /* _GraphicsEngine_H__ */
