#ifndef _Mesh_H__
#define _Mesh_H__

#include <string>

#include "OgreEntity.h"

namespace orxonox
{
    class Mesh
    {
        public:
            Mesh();
            ~Mesh();
            void setMesh(const std::string& file);

            inline Ogre::Entity* getEntity()
                { return this->entity_; }

            inline const std::string& getName() const
                { return this->entity_->getName(); }

        private:
            static unsigned int meshCounter_s;
            Ogre::Entity* entity_;
    };
}

#endif /* _Mesh_H__ */
