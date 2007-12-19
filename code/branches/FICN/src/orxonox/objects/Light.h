#include <string>

#include "OgreLight.h"
#include "OgreColourValue.h"

#ifndef _Light_H__
#define _Light_H__

namespace orxonox
{
    class Light
    {
        public:
            Light();
            ~Light();
            void setLight(Ogre::Light::LightTypes type = Ogre::Light::LT_POINT, const Ogre::ColourValue& diffuse = Ogre::ColourValue(1.0, 1.0, 1.0), const Ogre::ColourValue& specular = Ogre::ColourValue(1.0, 1.0, 1.0));

            inline Ogre::Light* getLight()
                { return this->light_; }

            inline const std::string& getName() const
                { return this->light_->getName(); }

        private:
            static unsigned int lightCounter_s;
            Ogre::Light* light_;
    };
}

#endif
