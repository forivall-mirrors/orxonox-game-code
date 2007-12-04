/**
* Orxonox - www.orxonox.net
* Level Loader class
* 
* @author Nicolas Perrenoud <nicolape@ee.ethz.ch>
*/

#include <string>
#include <iostream>

#include "loader_platform.h"
#include "xml/xmlParser.h"

#ifndef __MODULE_LEVELLOADER__
#define __MODULE_LEVELLOADER__

using namespace std;

namespace loader
{
	class _LoaderExport LevelLoader
	{
	private:
		
		// XML Nodes 
		XMLNode rootNode;
		XMLNode worldNode;
		XMLNode scriptNode;
		XMLNode cameraNode;
		XMLNode lightNode;
		
		// Level information
		string name_;
		string description_;
		string image_;
			
	public:

		// Constructors
		LevelLoader(string file, string dir="levels");
		~LevelLoader();
		
		void loadingScreen();
		
		// Getters
		string name();
		string description();
		string image();		
		
		// Managers 
		
			
	};	
}
#endif
