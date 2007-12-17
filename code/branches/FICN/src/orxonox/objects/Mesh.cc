#include <sstream>

#include "Mesh.h"
#include "../orxonox.h"

namespace orxonox
{
    unsigned int Mesh::meshCounter_s = 0;

    Mesh::Mesh()
    {
        this->entity_ = 0;
    }

    Mesh::Mesh(const std::string& file)
    {
        std::cout << "Mesh: 1\n";
        std::ostringstream name;
        std::cout << "Mesh: 2\n";
        name << (Mesh::meshCounter_s++);
        std::cout << "Mesh: 3, entity before creation: " << this->entity_ << "\n";
        std::cout << ("Mesh: name: Mesh" + name.str()) << std::endl;
        std::cout << "Mesh: file: " << file << std::endl;
        this->entity_ = Orxonox::getSingleton()->getSceneManager()->createEntity("Mesh" + name.str() + "gugus", file);
        std::cout << "Mesh: 4, entity after creation: " << this->entity_ << "\n";
    }

    Mesh::~Mesh()
    {
        if (this->entity_)
            Orxonox::getSingleton()->getSceneManager()->destroyEntity(this->entity_);
    }
}
