#include "PortalEndPoint.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(PortalEndPoint);

    std::map<unsigned int, PortalEndPoint *> PortalEndPoint::idMap_s;

    PortalEndPoint::PortalEndPoint(BaseObject* creator) : StaticEntity(creator), id_(0), material_(""), billboard_(0)
    {
        RegisterObject(PortalEndPoint);
        trigger_.setDistance(10);
        trigger_.setStayActive(true);
    }
    
    PortalEndPoint::~PortalEndPoint()
    {
      
    }

    void PortalEndPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PortalEndPoint, XMLPort, xmlelement, mode);
        XMLPortParam(PortalEndPoint, "id", setID, getID, xmlelement, mode);
        XMLPortParamExtern(PortalEndPoint, Billboard, this->billboard_, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParamExtern(PortalEndPoint, Billboard, this->billboard_, "colour", setColour, getColour, xmlelement, mode).defaultValues(ColourValue::White);
        
        if(mode == XMLPort::LoadObject)
        {
            idMap_s[this->id_] = this;
        }
    }
}
