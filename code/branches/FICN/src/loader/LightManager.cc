
#include "xml/xmlParser.h"
#include "LightManager.h"

namespace light
{
	LightManager::LightManager()
	{
    std::vector<Light> elements_ = *(new std::vector<Light>);
	}
	
	void LightManager::loadParams(XMLNode xml)
	{
		if (!xml.isEmpty())
		{
			int nLights = xml.nChildNode((const wchar_t*)"light");
			for (int i=0; i<nLights;i++)
			{
				Light l = *(new Light(xml.getChildNode((const wchar_t*)"light",i)));
        this->elements_.insert(elements_.end(),l);
			}
		}		
		
	}	
		
	
}

