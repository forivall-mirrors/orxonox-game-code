/**
* Orxonox - www.orxonox.net
* Level Loader class
*
* @author Nicolas Perrenoud <nicolape@ee.ethz.ch>
*/

#ifndef _LevelLoader_H__
#define _LevelLoader_H__

#include "LoaderPrereqs.h"

#include "misc/String.h"
#include "tinyxml/tinyxml.h"

class TiXmlDocument;   // Forward declaration
class TiXmlElement;    // Forward declaration

namespace loader
{
  class _LoaderExport LevelLoader
  {
  public:
    // Constructors, loads the level file and some information data
    LevelLoader(orxonox::String file, orxonox::String dir = "levels");
    // Destructor
    virtual ~LevelLoader();
    // Loads all level data
    void loadLevel();

    // Getters
    inline orxonox::String name() {return name_; };
    inline orxonox::String description() {return description_; };
    inline orxonox::String image() {return image_; };
  private:
    //! Level information
    orxonox::String name_;
    orxonox::String description_;
    orxonox::String image_;
    orxonox::String loadingBackgroundColor_;
    orxonox::String loadingBackgroundImage_;
    orxonox::String loadingBarImage_;
    orxonox::String loadingBarTop_;
    orxonox::String loadingBarLeft_;
    orxonox::String loadingBarWidth_;
    orxonox::String loadingBarHeight_;

    //! Set to true if it was possible to load the level file
    bool valid_;

    // Xml-Stuff
    TiXmlDocument doc_;
    TiXmlElement *rootElement_;
  };
}

#endif /* _LevelLoader_H__ */
