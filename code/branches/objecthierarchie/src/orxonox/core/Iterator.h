#ifndef _Iterator_H__
#define _Iterator_H__

namespace orxonox
{
    template <class T>
    class Iterator
    {
        public:
            Iterator()
            {
                this->element_ = 0;
            }

            Iterator(ObjectListElement<T>* element)
            {
                this->element_ = element;
            }

            Iterator<T> operator++()
            {
                this->element_ = this->element_->next_;
                return *this;
            }

            Iterator<T> operator--()
            {
                this->element_ = this->element_->prev_;
                return *this;
            }

            T* operator*()
            {
                return this->element_->object_;
            }

            T* operator->() const
            {
                return this->element_->object_;

            }

            operator bool()
            {
                return (this->element_ != 0);
            }

            bool operator!=(int compare)
            {
                if (compare != 0)
                    std::cout << "Warning: Comparing the " << ClassIdentifier<T>::getIdentifier()->getName() << "-List-Iterator with " << compare << " has no effect. Only comparison with 0 works.\n";

                return (this->element_ != 0);
            }

        private:
            ObjectListElement<T>* element_;
    };
}

#endif
