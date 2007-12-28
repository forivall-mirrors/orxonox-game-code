#ifndef _Model_H__
#define _Model_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "Mesh.h"

class TiXmlElement; // Forward declaration

namespace orxonox
{

    class Model : public WorldEntity
    {
        public:
            Model();
            virtual ~Model();
            virtual void loadParams(TiXmlElement* xmlElem);
            bool create();

        private:
            std::string meshSrc_;
            Mesh mesh_;
            void registerAllVariables();
    };
}

#endif /* _Model_H__ */
