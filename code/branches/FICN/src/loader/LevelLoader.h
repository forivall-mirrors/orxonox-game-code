/**
* Orxonox - www.orxonox.net
* Level Loader class
*
* @author Nicolas Perrenoud <nicolape@ee.ethz.ch>
*/

#include <string>
#include <vector>
#include <iostream>

#include "loader_platform.h"
#include "tinyxml/tinyxml.h"

//#include "xml/xmlParser.h"

#ifndef __MODULE_LEVELLOADER__
#define __MODULE_LEVELLOADER__

using namespace std;

namespace loader
{
  class _LoaderExport LevelLoader
  {
  public:
    // Constructors, loads the level file and some information data
    LevelLoader(string file, string dir="levels");
    // Destructor
    ~LevelLoader();
    // Loads all level data
    void loadLevel();

    // Getters
    inline string name() {return name_; };
    inline string description() {return description_; };
    inline string image() {return image_; };
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
    TiXmlDocument doc;
    TiXmlElement* rootElement;


  };
}
#endif
