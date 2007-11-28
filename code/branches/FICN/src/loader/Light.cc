
#include "xml/xmlParser.h"
#include "Light.h"

namespace light
{								
	Light::Light()
	{		
		
	}
	
	Light::Light(XMLNode xml)
	{
		this->loadParams(xml);	
	}
	
	void Light::loadParams(XMLNode xml)
	{
		// Here comes the tricky part... convert strings to int
		const char* diffuse = xml.getAttribute("diffuse-color");
		const char* coor = xml.getAttribute("abs-coor");
	}
}