#ifndef MA_ITERATOR_ITERATOR_H
#define MA_ITERATOR_ITERATOR_H

#include <memory>

#include <ma/type.h>
#include <ma/traits.h>

#include <ma/iterator/iteratorFunction.h>

#include <iostream>

namespace ma
{
    namespace iterator
    {
        class IteratorInterface;

        using uIterator = std::unique_ptr<IteratorInterface>;

        class IteratorInterface
        {
        public:
            virtual ~IteratorInterface() = default;

            virtual SizeT value(DiffT pos = 0) const = 0;

            virtual IteratorInterface& increase(DiffT pos) = 0;

            virtual uIterator clone() const = 0;

            /**
             * aim to compare if iterator is equal
             **/
            virtual SizeT id() const = 0;
        };

        template<typename Iterator>
        class IteratorImpl : public IteratorInterface
        {
            Iterator it_;

        public:
            IteratorImpl(Iterator it):it_(it){}

            IteratorImpl(const IteratorImpl &) = default;
            IteratorImpl(IteratorImpl &&) = default;

            IteratorImpl& operator=(const IteratorImpl &) = default;
            IteratorImpl& operator=(IteratorImpl &&) = default;

            SizeT value(DiffT pos = 0) const override
            {
                return *it_ + pos;
            }

            IteratorInterface& increase(DiffT pos) override
            {
                it_ += pos;
                return *this;
            }

            uIterator clone() const override
            {
                return uIterator(new IteratorImpl(it_));
            }

            SizeT id() const override
            {
                return iteratorId(it_);
            }
        };

        class Iterator
        {
        protected:
            uIterator it_;

        public:
            explicit Iterator(SizeT start = 0, DiffT step = 1)
                :it_(new IteratorImpl<LinearIterator>(LinearIterator(start, step)))
            {}

            // template<
            //     typename T,
            //     typename BaseT = decay_t<T>,
            //     typename = enable_if_t<is_base_of<IteratorInterface, BaseT>::value>
            // >
            // Iterator(T && it)
            //     :it_(new BaseT(std::forward<T>(it)))
            // {}

            template<
                typename T, typename BaseT = decay_t<T>,
                typename = IsNotSame<Iterator, BaseT>,
                typename = IsRandomIt<BaseT>
            >
            Iterator(T && it)
                :it_(new IteratorImpl<BaseT>(std::forward<T>(it)))
            {}

            Iterator(const Iterator & ii)
                :it_(ii.it_->clone())
            {}

            Iterator(Iterator && ii) = default;

            Iterator& operator=(const Iterator & ii)
            {
                it_ = ii.clone();
                return *this;
            }

            Iterator& operator=(Iterator && ii)
            {
                it_ = std::move(ii.it_);
                return *this;
            }

            Iterator add(DiffT pos) const
            {
                Iterator tmp(*this);
                tmp += pos;
                return tmp;
            }

            Iterator& increase(DiffT pos)
            {
                it_->increase(pos);
                return *this;
            }

            bool equal(const Iterator & it) const
            {
                return it_->id() == it.it_->id();
            }

            SizeT operator[](DiffT pos) const {return it_->value(pos);}
            
            SizeT operator*() const {return it_->value();}

            Iterator& operator++() {it_->increase(1); return *this;}
            
            Iterator operator++(int)
            {
                Iterator tmp(*this);
                it_->increase(1);
                return tmp;
            }

            Iterator& operator--() {it_->increase(-1); return *this;}

            Iterator operator--(int)
            {
                Iterator tmp(*this);
                it_->increase(-1);
                return tmp;
            }

            Iterator& operator+=(DiffT pos) {return increase(pos);}
            Iterator& operator-=(DiffT pos) {return increase(-pos);}


            Iterator operator+(DiffT pos) const
            {
                return add(pos);
            }

            Iterator operator-(DiffT pos) const
            {
                return add(-pos);
            }
            bool operator==(const Iterator & li) const
            {
                return equal(li);
            }
            bool operator!=(const Iterator & li) const
            {
                return !equal(li);
            }

            uIterator clone() const
            {
                return it_->clone();
            }
        };
    }
}

template<> struct std::iterator_traits<ma::iterator::Iterator>
{
        using value_type = ma::SizeT;
        using difference_type = ma::DiffT;
        using pointer = void;
        using reference = void;
        using iterator_category = std::random_access_iterator_tag;
};

#endif //MA_ITERATOR_ITERATOR_H
