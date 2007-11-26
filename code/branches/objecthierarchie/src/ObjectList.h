#ifndef _ObjectList_H__
#define _ObjectList_H__

namespace orxonox
{
    class OrxonoxClass;

    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    template <class T>
    class ObjectListElement
    {
        public:
            ObjectListElement(T* object);
            ~ObjectListElement();

            T* object_;
            ObjectListElement* next_;
            ObjectListElement* prev_;
    };

    template <class T>
    ObjectListElement<T>::ObjectListElement(T* object)
    {
        this->object_ = object;
        this->next_ = 0;
        this->prev_ = 0;
    }

    template <class T>
    ObjectListElement<T>::~ObjectListElement()
    {
    }


    // ###############################
    // ###       ObjectList        ###
    // ###############################
    template <class T>
    class Iterator;

    template <class T>
    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();
            ObjectListElement<T>* add(T* object);
            void remove(OrxonoxClass* object, bool bIterateForwards = true);

            inline static Iterator<T> start()
                { Iterator<T>(pointer_s->first_); }
            inline static Iterator<T> end()
                { Iterator<T>(pointer_s->last_); }

            ObjectListElement<T>* first_;
            ObjectListElement<T>* last_;

        private:
            static ObjectList<T>* pointer_s;
    };

    template <class T>
    ObjectList<T>* ObjectList<T>::pointer_s = 0;

    template <class T>
    ObjectList<T>::ObjectList()
    {
        this->first_ = 0;
        this->last_ = 0;

        this->pointer_s = this;
    }

    template <class T>
    ObjectList<T>::~ObjectList()
    {
        ObjectListElement<T>* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    template <class T>
    ObjectListElement<T>* ObjectList<T>::add(T* object)
    {
        if (!this->last_)
        {
            this->last_ = new ObjectListElement<T>(object);
            this->first_ = this->last_;
        }
        else
        {
            ObjectListElement<T>* temp = this->last_;
            this->last_ = new ObjectListElement<T>(object);
            this->last_->prev_ = temp;
            temp->next_ = this->last_;
        }

        return this->last_;
    }

    template <class T>
    void ObjectList<T>::remove(OrxonoxClass* object, bool bIterateForwards)
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
                ObjectListElement<T>* temp = this->first_->next_;
                delete this->first_;
                this->first_ = temp;
                this->first_->prev_ = 0;

                return;
            }

            ObjectListElement<T>* temp = this->first_;
            while (temp->next_)
            {
                if (temp->next_->object_ == object)
                {
                    ObjectListElement<T>* temp2 = temp->next_->next_;
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
                ObjectListElement<T>* temp = this->last_->prev_;
                delete this->last_;
                this->last_ = temp;
                this->last_->next_ = 0;

                return;
            }

            ObjectListElement<T>* temp = this->last_;
            while (temp->prev_)
            {
                if (temp->prev_->object_ == object)
                {
                    ObjectListElement<T>* temp2 = temp->prev_->prev_;
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
}

#endif
