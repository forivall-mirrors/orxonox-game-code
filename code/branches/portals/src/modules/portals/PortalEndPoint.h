#ifndef _PortalEndPoint_H__
#define _PortalEndPoint_H__

#include "portals/PortalsPrereqs.h"

#include <set>
#include <string>
#include <map>

#include "worldentities/StaticEntity.h"
#include "graphics/Billboard.h"
#include "objects/triggers/DistanceMultiTrigger.h"
#include "core/EventIncludes.h"

namespace orxonox
{
    class _PortalsExport PortalEndPoint : public StaticEntity
    {
        public:
            PortalEndPoint(BaseObject* creator);
            virtual ~PortalEndPoint();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            inline void setTargets(const std::string & targets)
            {
                this->trigger_->addTargets(targets);
            }
            
            void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
            static std::map<unsigned int, PortalEndPoint *> idMap_s; //!< maps integer id values to portalendpoints
            inline void setID(unsigned int id)
            {
                this->id_ = id;
            }
            
            inline unsigned int getID() const
            {
                return this->id_;
            }
            inline void setTemplate(const std::string & name)
            {
                this->templateName_ = name;
                this->addTemplate(name);
            }
            inline const std::string & getTemplate()
            {
                return this->templateName_;
            }
            bool execute(bool bTriggered, BaseObject* trigger);
            void jumpOut(MobileEntity * entity);
        protected:
        private:
            unsigned int id_;
            DistanceMultiTrigger * trigger_;
            std::string templateName_;
            void setEventTemplate(const std::string & temp)
            {
                this->addTemplate(temp);
            }
            std::set<MobileEntity *> recentlyJumpedOut_; //!< Entities which recently jumped out of this EndPoint, hence they shouldn't be pulled in again if the endpoint is the beginning of a link
    };

}

#endif /* _Portals_H__ */
