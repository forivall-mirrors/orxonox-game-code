#include <string>

#include "OgreEntity.h"

#ifndef _Mesh_H__
#define _Mesh_H__

namespace orxonox
{
    class Mesh
    {
        public:
            Mesh();
            Mesh(const std::string& file);
            ~Mesh();
            inline Ogre::Entity* getEntity()
                { return this->entity_; }

        private:
            static unsigned int meshCounter_s;
            Ogre::Entity* entity_;
    };
}

#endif
