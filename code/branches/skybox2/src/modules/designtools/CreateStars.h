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
 *      Gion-Andri Cantieni
 *   Co-authors:
 *      ...
 *
 */


#include "core/BaseObject.h"

#include "graphics/Billboard.h"
 
 
namespace orxonox
{
    class CreateStars : public BaseObject
    {
        public:
            CreateStars(orxonox::BaseObject* creator);
            virtual ~CreateStars();

            void createBillboards();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void setNumStars(int num) {
                this->numStars_ = num;
            }

            int getNumStars(){
                return this->numStars_;
            }

            void setMaterial(const std::string& material) {
                this->material_ = material;
            }

            std::string& getMaterial(){
                return this->material_;
            }

            void setRadius(unsigned int dist) {
                this->radius_ = dist;
                this->createBillboards();
            }

            unsigned int getRadius(){
                return this->radius_;
            }
            
            Vector3 PolarToCartesian(float alpha, float beta, float radius);

        private:
            int numStars_;
            std::string material_;
            std::vector<Billboard*> billboards_;
            unsigned int radius_;
            
    };
}
