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
 *      Noe Pedrazzini
 *   Co-authors:
 *      ...
 *
 */

#ifndef _PartDestructionEvent_H__
#define _PartDestructionEvent_H__

#include "OrxonoxPrereqs.h"
#include "Item.h"

#include <string>


namespace orxonox // tolua_export
{ // tolua_export
    class _OrxonoxExport PartDestructionEvent // tolua_export
        : public Item
    { // tolua_export

        public:

            PartDestructionEvent(Context* context);
            virtual ~PartDestructionEvent();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void execute();

            void setTargetType(std::string type);
            inline std::string getTargetType()
                { return this->targetType_; }

            void setTargetName(std::string name);
            inline std::string getTargetName()
                { return this->targetName_; }

            void setTargetParam(std::string param);
            inline std::string getTargetParam()
                { return this->targetName_; }

            void setOperation(std::string operation);
            inline std::string getOperation()
                { return this->operation_; }

            void setEventValue(float value);
            inline float getEventValue()
                { return this->value_; }

        protected:

        private:

            bool isValid_;

            std::string targetType_;
            std::string targetName_;
            std::string targetParam_;
            std::string operation_;

            float value_;


    }; // tolua_export
} // tolua_export

#endif /* _PartDestructionEvent_H__ */
