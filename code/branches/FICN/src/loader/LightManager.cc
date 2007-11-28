
#include "xml/xmlParser.h"
#include "LightManager.h"

namespace light
{
	LightManager::LightManager()
	{
		vector<Light> this->elements_ = new vector<Light>;
	}
	
	void LightManager::loadParams(XMLNode xml)
	{
		if (!xml.isEmpty())
		{
			int nLights = xml.nChildNode("light");
			for (int i=0; i<nLights;i++)
			{
				Light l = new Light(xml.getChildNode("light",i));
				this->elements_.append(l);
			}
		}		
		
	}	
		
	
}

