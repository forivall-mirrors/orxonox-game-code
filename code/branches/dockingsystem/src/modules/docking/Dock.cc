/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Dock.cc
	@brief Docking system main class
*/

#include "Dock.h"


namespace orxonox
{

	CreateFactory(Dock);

    Dock::Dock(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Dock);
		COUT(0) << "Registering dock..." << std::endl;
    }

    Dock::~Dock()
    {
    }


    void Dock::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLPort, xmlelement, mode);

        XMLPortObject(Dock, DockingEffect, "effects", addEffect, getEffect, xmlelement, mode);
        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);

        COUT(0) << "Dock created.." << std::endl;
    }

    void Dock::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);
    }


    bool Dock::execute(bool bTriggered, BaseObject* trigger)
	{
		COUT(0) << "Dock executed (bTriggered = " << (bTriggered? "true":"false") << ").." << std::endl;
		return true;
	}


	bool Dock::addEffect(DockingEffect* effect) {
		assert(effect);
		effects_.push_back(effect);
		return true;
	}
    
	const DockingEffect* Dock::getEffect(unsigned int index) const {
		int i = index;
        for (std::list<DockingEffect*>::const_iterator effect = this->effects_.begin(); effect != this->effects_.end(); ++effect)
        {
            if(i == 0)
               return *effect;

            i--;
        }
        return NULL;
	}

}
