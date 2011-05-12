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
            inline void setTargets(const std::string & targets)                 //!< add types which are allowed to activate the PortalEndPoint
            {
                this->trigger_->addTargets(targets);
            }
            
            void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
            static std::map<unsigned int, PortalEndPoint *> idMap_s; //!< Maps the id of each PortalEndPoint to a pointer to that PortalEndPoint
            inline void setID(unsigned int id)
            {
                this->id_ = id;
            }
            
            inline unsigned int getID() const
            {
                return this->id_;
            }
            
            /// \brief Set templateName_ (the name of the design Template) and add that Template to this Object
            inline void setTemplate(const std::string & name)
            {
                this->templateName_ = name;
                this->addTemplate(name);
            }

            /// \brief Get the name of the attached design template
            inline const std::string & getTemplate()
            {
                return this->templateName_;
            }

            /*! \brief This function is called each time the DistanceMultiTrigger of this PortalEndPoint changed
             * \param bTriggered true if the trigger was triggered on, false if the trigger has switched to off
             * \param trigger the MultiTriggerContainer containing the triggering BaseObject (and trigger_ the portal's MultiDistanceTrigger which we already know)
             * 
             * if bTriggered is \c true the triggering entity enters this portal (if it is an entrance)
             * otherwise the triggering entity is removed from the set of entities who recently jumped out of this portal */
            bool execute(bool bTriggered, BaseObject* trigger);

            /*! \brief Let an Entity jump out of this portal no matter where it was before
             * \param entity The Entity which should jump out of this portal */
            void jumpOut(MobileEntity * entity);
        protected:
            
        private:
            static const std::string EVENTFUNCTIONNAME; //!< = "execute"
            
            unsigned int id_;            //!< the hopefully (depends on the writer of the levelfile) unique id, which is used to establish links between PortalEndPoints
            DistanceMultiTrigger * trigger_;      //!< the DistanceMultiTrigger which notices near entities of the defined type
            std::string templateName_;            //!< The name of the design template used for this endpoint

            std::set<MobileEntity *> recentlyJumpedOut_; //!< Entities which recently jumped out of this EndPoint, hence they shouldn't be pulled in again if the endpoint is the beginning of a link
    };

}

#endif /* _Portals_H__ */
