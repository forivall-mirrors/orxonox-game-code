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
                this->elementForwards_ = ClassIdentifier<T>::getIdentifier()->objects_->first_;
                this->elementBackwards_ = ClassIdentifier<T>::getIdentifier()->objects_->last_;
                this->iteratingForwards_ = true;
            }

            Iterator<T> operator++(int step)
            {
                Iterator<T> copy = *this;

                if (step < 1)
                    step = 1;

                for (int i = 0; i < step; i++)
                    this->elementForwards_ = this->elementForwards_->next_;

                return copy;
            }

            Iterator<T> operator++()
            {
                this->elementForwards_ = this->elementForwards_->next_;
                return *this;
            }

            Iterator<T> operator--(int step)
            {
                Iterator<T> copy = *this;

                if (this->iteratingForwards_)
                {
                    this->iteratingForwards_ = false;
                }
                else
                {
                    if (step < 1)
                        step = 1;

                    for (int i = 0; i < step; i++)
                        this->elementBackwards_ = this->elementBackwards_->prev_;
                }
            }

            Iterator<T> operator--()
            {
                if (this->iteratingForwards_)
                    this->iteratingForwards_ = false;
                else
                    this->elementBackwards_ = this->elementBackwards_->prev_;

                return *this;
            }

            T* operator*()
            {
                if (this->iteratingForwards_)
                    return this->elementForwards_->object_;
                else
                    return this->elementBackwards_->object_;
            }

            T* operator->() const
            {
                if (this->iteratingForwards_)
                    return this->elementForwards_->object_;
                else
                    return this->elementBackwards_->object_;

            }

            operator bool()
            {
                if (this->iteratingForwards_)
                    return (this->elementForwards_ != 0);
                else
                    return (this->elementBackwards_->prev_ != 0);
            }

            bool operator!=(int compare)
            {
                if (compare != 0)
                    std::cout << "Warning: Comparing the " << ClassIdentifier<T>::getIdentifier()->getName() << "-List-Iterator with " << compare << " has no effect. Only comparison with 0 works.\n";

                if (this->iteratingForwards_)
                    return (this->elementForwards_ != 0);
                else
                    return (this->elementBackwards_->prev_ != 0);
            }


        private:
            ObjectListElement<T>* elementForwards_;
            ObjectListElement<T>* elementBackwards_;
            bool iteratingForwards_;
    };
}

#endif
