#include <sstream>

#include "Light.h"
#include "../Orxonox.h"

namespace orxonox
{
    unsigned int Light::lightCounter_s = 0;

    Light::Light()
    {
        this->light_ = 0;
    }

    void Light::setLight(Ogre::Light::LightTypes type, const Ogre::ColourValue& diffuse, const Ogre::ColourValue& specular)
    {
        std::ostringstream name;
        name << (Light::lightCounter_s++);
        this->light_ = Orxonox::getSingleton()->getSceneManager()->createLight("Light" + name.str());
        this->light_->setType(type);
        this->light_->setDiffuseColour(diffuse);
        this->light_->setSpecularColour(specular);
    }

    Light::~Light()
    {
        if (this->light_)
            Orxonox::getSingleton()->getSceneManager()->destroyLight(this->light_);
    }
}
