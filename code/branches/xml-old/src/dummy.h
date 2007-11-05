/*

#include <iostream>
using namespace std;


#ifndef __DUMMY__
#define __DUMMY__

/*
class SomeNode
{
	virtual void loadParams(const XMLNode* root);
	};

class Light : public SomeNode
{
	public:
		virual void loadParams(const XMLNode* root)
		
		{
			SomeNode::loadParams(root);
			
			root->getAttribute("BLA");
			
			}
	};



class LightManager
{
	public:

	
		void addLight(const char* diff, const char* coord);
		void setAmbient(const char* amb); 

  //  void loadParams(const XMLNode* root);
  //
		LightManager();

};

inline LightManager::LightManager() 
{

}

inline void LightManager::addLight(const char* diff, const char* coord)
{
	cout << "Adding light: "<< diff << " coords: " << coord << "\n";
}

inline void LightManager::setAmbient(const char* amb)
{
	cout << "Setting ambient light: "<< amb << "\n";
}



class CameraManager
{
	public:
		void addCamera();
};

inline	void CameraManager::addCamera()
{
	cout << "Loading Camera\n";
}

class ScriptManager
{
	public:
		void addScript(const char* file);
};

inline void ScriptManager::addScript(const char* file)
{
	cout << "Loading Lua Script"<<file<<"\n";
}

class WorldManager
{
	public:
		void addScript(const char* file);
};

inline void WorldManager::addScript(const char* file)
{
	cout << "Loading Lua Script"<<file<<"\n";
}

#endif
*/