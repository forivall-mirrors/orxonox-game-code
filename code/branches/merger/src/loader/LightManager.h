#include <vector>

#include "loader_platform.h"
#include "xml/xmlParser.h"
#include "Light.h"

#ifndef __MODULE_LIGHTMANAGER__
#define __MODULE_LIGHTMANAGER__

namespace light
{
	class _LoaderExport LightManager
	{      
	public:
		LightManager();
		~LightManager();
		void loadParams(XMLNode xml);		
				
	private:
		float ambient_r_;
		float ambient_g_;
		float ambient_b_;
    std::vector<Light> elements_;
	};
}

#endif