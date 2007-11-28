#include "ObjectList.h"

namespace orxonox
{
    // ###############################
    // ###       ObjectList        ###
    // ###############################
    ObjectList::ObjectList()
    {
        this->first_ = 0;
    }

    ObjectList::~ObjectList()
    {
        ObjectListElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    void ObjectList::add(OrxonoxClass* object)
    {
        ObjectListElement* temp = this->first_;
        this->first_ = new ObjectListElement(object);
        this->first_->next_ = temp;
    }

    void ObjectList::remove(OrxonoxClass* object)
    {
        if (!object)
            return;

        if (this->first_->object_ == object)
        {
            ObjectListElement* temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;

            return;
        }

        ObjectListElement* temp = this->first_;
        while (temp->next_)
        {
            if (temp->next_->object_ == object)
            {
                ObjectListElement* temp2 = temp->next_->next_;
                delete temp->next_;
                temp->next_ = temp2;

                return;
            }

            temp = temp->next_;
        }
    }


    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    ObjectListElement::ObjectListElement(OrxonoxClass* object)
    {
        this->object_ = object;
        this->next_ = 0;
    }

    ObjectListElement::~ObjectListElement()
    {
    }
}
