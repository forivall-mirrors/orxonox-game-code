/**
  @file GraphicsEngine.h
  @brief Graphics Engine
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include "misc/String.h"


namespace orxonox {

/**
   * graphics engine manager class
 */
  class GraphicsEngine {
    public:
      GraphicsEngine();
      inline void setConfigPath(String path) { this->configPath_ = path; };
      // find a better way for this
      inline Ogre::Root* getRoot() { return root_; };
      void setup();
      bool load();
      void loadRessourceLocations(String path);
      Ogre::SceneManager* getSceneManager();
      void startRender();

      virtual ~GraphicsEngine();
    private:
      Ogre::Root*         root_;        //!< Ogre's root
      String         configPath_;  //!< path to config file
      String         dataPath_;    //!< path to data file
      Ogre::SceneManager* scene_;       //!< scene manager of the game

  };

}

#endif /* _GraphicsEngine_H__ */
