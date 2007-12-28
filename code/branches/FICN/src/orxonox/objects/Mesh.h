#ifndef _Mesh_H__
#define _Mesh_H__

#include <OgreEntity.h>

#include "../OrxonoxPrereqs.h"

#include "misc/String.h"

namespace orxonox
{
    class Mesh
    {
        public:
            Mesh();
            ~Mesh();
            void setMesh(const String& file);

            inline Ogre::Entity* getEntity()
                { return this->entity_; }

            inline const String& getName() const
                { return this->entity_->getName(); }

        private:
            static unsigned int meshCounter_s;
            Ogre::Entity* entity_;
    };
}

#endif /* _Mesh_H__ */
