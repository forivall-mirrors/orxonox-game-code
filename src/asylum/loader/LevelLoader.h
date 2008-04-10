/**
* Orxonox - www.orxonox.net
* Level Loader class
*
* @author Nicolas Perrenoud <nicolape@ee.ethz.ch>
*/

#ifndef _LevelLoader_H__
#define _LevelLoader_H__

#include <string>

#include "LoaderPrereqs.h"
#include "util/tinyxml/tinyxml.h"

class TiXmlDocument;   // Forward declaration
class TiXmlElement;    // Forward declaration

namespace loader
{
  class _LoaderExport LevelLoader
  {
  public:
    // Constructors, loads the level file and some information data
    LevelLoader(std::string file, std::string dir = "levels");
    // Destructor
    virtual ~LevelLoader();
    // Loads all level data
    void loadLevel();

    // Getters
    inline std::string name() {return name_; };
    inline std::string description() {return description_; };
    inline std::string image() {return image_; };
  private:
    //! Level information
    std::string name_;
    std::string description_;
    std::string image_;
    std::string loadingBackgroundColor_;
    std::string loadingBackgroundImage_;
    std::string loadingBarImage_;
    std::string loadingBarTop_;
    std::string loadingBarLeft_;
    std::string loadingBarWidth_;
    std::string loadingBarHeight_;

    //! Set to true if it was possible to load the level file
    bool valid_;

    // Xml-Stuff
    TiXmlDocument doc_;
    TiXmlElement *rootElement_;
  };
}

#endif /* _LevelLoader_H__ */
