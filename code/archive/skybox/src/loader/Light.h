
#include "../xml/xmlParser.h"

#ifndef __MODULE_LIGHT__
#define __MODULE_LIGHT__

namespace light
{
	class Light()
	{
	public:      
		Light();
		Light(XMLNode xml);
		~Light();
		void loadParams(XMLNode xml);
	
	private:
		float diffuseColor_r_;
		float diffuseColor_g_;
		float diffuseColor_b_;

		int coord_x_;
		int coord_y_;
		int coord_z_;
	};
}

#endif