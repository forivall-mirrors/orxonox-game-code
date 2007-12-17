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
        std::cout << "blu_1\n";
        std::ostringstream name;
        std::cout << "blu_2\n";
        name << (Mesh::meshCounter_s++);
        std::cout << "blu_3\n";
        std::cout << ("Mesh" + name.str()) << std::endl;
        this->entity_ = Orxonox::getSingleton()->getSceneManager()->createEntity("Mesh" + name.str(), file);
        std::cout << "blu_4\n";
    }

    Mesh::~Mesh()
    {
        if (this->entity_)
            Orxonox::getSingleton()->getSceneManager()->destroyEntity(this->entity_);
    }
}
