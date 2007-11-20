#include "ObjectList.h"

namespace orxonox
{
    // ###############################
    // ###       ObjectList        ###
    // ###############################
    ObjectList::ObjectList()
    {
        this->first_ = 0;
        this->last_ = 0;
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
        if (!this->last_)
        {
            this->last_ = new ObjectListElement(object);
            this->first_ = this->last_;
        }
        else
        {
            ObjectListElement* temp = this->last_;
            this->last_ = new ObjectListElement(object);
            this->last_->prev_ = temp;
            temp->next_ = this->last_;
        }
    }

    void ObjectList::remove(OrxonoxClass* object, bool bIterateForwards)
    {
        if (!object || !this->first_ || !this->last_)
            return;

        if (this->first_ == this->last_)
        {
            if (this->first_->object_ == object)
            {
                delete this->first_;
                this->first_ = 0;
                this->last_ = 0;
            }

            return;
        }

        if (bIterateForwards)
        {
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
                    if (temp2)
                        temp2->prev_ = temp;
                    else
                        this->last_ = temp;

                    return;
                }

                temp = temp->next_;
            }
        }
        else
        {
            if (this->last_->object_ == object)
            {
                ObjectListElement* temp = this->last_->prev_;
                delete this->last_;
                this->last_ = temp;

                return;
            }

            ObjectListElement* temp = this->last_;
            while (temp->prev_)
            {
                if (temp->prev_->object_ == object)
                {
                    ObjectListElement* temp2 = temp->prev_->prev_;
                    delete temp->prev_;
                    temp->prev_ = temp2;
                    if (temp2)
                        temp2->next_ = temp;
                    else
                        this->first_ = temp;

                    return;
                }

                temp = temp->prev_;
            }
        }
    }


    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    ObjectListElement::ObjectListElement(OrxonoxClass* object)
    {
        this->object_ = object;
        this->next_ = 0;
        this->prev_ = 0;
    }

    ObjectListElement::~ObjectListElement()
    {
    }
}
