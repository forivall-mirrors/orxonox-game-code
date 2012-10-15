/*
 * HUDFindClosestEnemy.h
 *
 *  Created on: Oct 15, 2012
 *      Author: soroa
 */


#ifndef _HUDFindClosestEnemy_H__
#define _HUDFindClosestEnemy_H__

#include "overlays/OverlaysPrereqs.h"

#include <map>
#include <string>

#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/RadarListener.h"
#include "overlays/OrxonoxOverlay.h"
#include "HUDNavigation.h"

namespace orxonox
{
    class _OverlaysExport HUDFindClosestEnemy : public HUDNavigation
    {
        public:
            HUDFindClosestEnemy(BaseObject* creator);
            virtual ~HUDFindClosestEnemy();
            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        private:



    };
}




#endif /* _HUDFindClosestEnemy_H__ */
