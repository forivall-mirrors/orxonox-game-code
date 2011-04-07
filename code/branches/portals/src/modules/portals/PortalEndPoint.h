#ifndef _PortalEndPoint_H__
#define _PortalEndPoint_H__

#include <set>
#include <string>
#include <map>

#include "StaticEntity.h"
#include "graphics/Billboard.h"
#include "modules/objects/triggers/DistanceMultiTrigger.h"

namespace orxonox
{
    class _OrxonoxExport PortalEndPoint : public DistanceMultiTrigger
    {
        public:
            PortalEndPoint(BaseObject* creator);
            virtual ~PortalEndPoint();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            //virtual void tick(float dt);
            static std::map<unsigned int, PortalEndPoint *> idMap_s; //!< maps integer id values to portalendpoints
            inline void setID(unsigned int id)
            {
                this->id_ = id;
            }
            
            inline unsigned int getID() const
            {
                return this->id_;
            }
            void jumpOut(WorldEntity * entity); //!< relocate an entity to the position of the endpoint and add it to the set of recentlyPortedOut entities
            void tick(float dt);
            bool hasRecentlyJumpedOut(WorldEntity * entity); //!< check if a certain entity recently jumped out of this endpoint
        protected:
        private:
            unsigned int id_;
            std::set<WorldEntity *> recentlyJumpedOut_; //!< Entities which recently jumped out of this EndPoint, hence they shouldn't be pulled in again if the endpoint is the beginning of a link
            Billboard billboard_;
    };

}

#endif /* _Portals_H__ */
