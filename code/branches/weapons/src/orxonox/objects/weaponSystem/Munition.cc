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
 *      Martin Polak
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Munition.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(Munition);

    Munition::Munition(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Munition);

COUT(0) << "+Munition" << std::endl;
    }

    Munition::~Munition()
    {
COUT(0) << "~Munition" << std::endl;
    }

    unsigned int Munition::bullets()
    {
        if (this->bullets_ > 0)
            return bullets_;
        else
            return 0;
    }

    unsigned int Munition::magazines()
    {
        if (this->magazines_ > 0)
            return magazines_;
        else
            return 0;
    }

    void Munition::setMaxBullets(unsigned int amount)
    { this->maxBullets_ = amount; }

    void Munition::setMaxMagazines(unsigned int amount)
    { this->maxMagazines_ = amount; }

    void Munition::removeBullets(unsigned int amount)
    {
        if ( this->bullets_ != 0 )
            this->bullets_ = this->bullets_ - amount;
    }

    void Munition::removeMagazines(unsigned int amount)
    {
        if ( this->magazines_ != 0 )
            this->magazines_ = this->magazines_ - amount;
    }

    void Munition::addBullets(unsigned int amount)
    {
        if ( this->bullets_ == this->maxBullets_ )
        {
            //cannot add bullets to actual magazine
        }
        else
            this->bullets_ = this->bullets_ + amount;
    }

    void Munition::addMagazines(unsigned int amount)
    {
        if ( this->magazines_ == this->maxMagazines_ )
        {
            //no more capacity for another magazine
        }
        else
            this->magazines_ = this->magazines_ + amount;
    }


    void Munition::fillBullets()
    {
        this->bullets_ = this->maxBullets_;
    }

    void Munition::fillMagazines()
    {
        this->magazines_ = this->maxMagazines_;
    }
}
