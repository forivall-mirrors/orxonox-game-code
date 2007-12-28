#ifndef _Light_H__
#define _Light_H__

#include <OgreLight.h>

#include "../OrxonoxPrereqs.h"

#include "misc/String.h"
#include "misc/ColourValue.h"

namespace orxonox
{
    class Light
    {
        public:
            Light();
            ~Light();
            void setLight(Ogre::Light::LightTypes type = Ogre::Light::LT_POINT, const ColourValue& diffuse = ColourValue(1.0, 1.0, 1.0), const ColourValue& specular = ColourValue(1.0, 1.0, 1.0));

            inline Ogre::Light* getLight()
                { return this->light_; }

            inline const String& getName() const
                { return this->light_->getName(); }

        private:
            static unsigned int lightCounter_s;
            Ogre::Light* light_;
    };
}

#endif /* _Light_H__ */
