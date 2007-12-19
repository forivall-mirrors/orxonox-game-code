#include <sstream>

#include "Mesh.h"
#include "../Orxonox.h"

namespace orxonox
{
    unsigned int Mesh::meshCounter_s = 0;

    Mesh::Mesh()
    {
        this->entity_ = 0;
    }

    void Mesh::setMesh(const std::string& file)
    {
        std::ostringstream name;
        name << (Mesh::meshCounter_s++);
        this->entity_ = Orxonox::getSingleton()->getSceneManager()->createEntity("Mesh" + name.str(), file);
    }

    Mesh::~Mesh()
    {
        if (this->entity_)
            Orxonox::getSingleton()->getSceneManager()->destroyEntity(this->entity_);
    }
}
