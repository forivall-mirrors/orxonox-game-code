/**
* Orxonox - www.orxonox.net
* Level Loader class
* 
* @author Nicolas Perrenoud <nicolape@ee.ethz.ch>
*/

#include <string>
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
	private:
		/*
		// XML Nodes 
		XMLNode rootNode;
		XMLNode loadingScreenNode;
		XMLNode worldNode;
		XMLNode audioNode;
		XMLNode scriptsNode;
		*/
		// Level information
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
			
	public:

		// Constructors
		LevelLoader(string file, string dir="levels");
		~LevelLoader();
		
		void showLoadingScreen();
		
		// Getters
		string name();
		string description();
		string image();		
		
		// Managers 
		
			
	};	
}
#endif
