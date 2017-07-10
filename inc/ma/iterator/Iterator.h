#ifndef ITERATOR_H
#define ITERATOR_H

#include <type_traits>
#include <memory>

#include <ma/detail/base.h>

#include <ma/iterator/iteratorFunction.h>

namespace ma
{
    namespace iterator
    {
        class IteratorBase
        {
        public:
            virtual ~IteratorBase(){}

            virtual SizeT value(DiffT pos = 0) const = 0;

            virtual IteratorBase& increase(DiffT pos) = 0;

            virtual std::unique_ptr<IteratorBase> makeUniqueCopy() const = 0;

            virtual SizeT id() const = 0;
        };

        template<typename T>
        class IteratorImpl : public IteratorBase
        {
            using IteratorM = T;
            using IteratorT = IteratorImpl<T>;
            using IteratorPtr = std::unique_ptr<IteratorBase>;

            IteratorM it_;

        public:
            IteratorImpl(IteratorM it):it_(it){}
            virtual ~IteratorImpl(){}

            virtual SizeT value(DiffT pos = 0) const
            {
                return *it_;
            }

            virtual IteratorBase& increase(DiffT pos)
            {
                it_ += pos;
                return *this;
            }

            virtual IteratorPtr makeUniqueCopy() const
            {
                return IteratorPtr{new IteratorT(it_)};
            }

            virtual SizeT id() const
            {
                return iteratorId(it_);
            }
        };

        class Iterator
        {
            using IteratorPtr = std::unique_ptr<IteratorBase>;

            IteratorPtr it_;
        public:
            Iterator(SizeT start = 0, DiffT step = 1)
                :it_(new IteratorImpl<LinearIterator>(LinearIterator(start, step)))
            {}

            template<
                typename T,
                typename = typename std::enable_if<std::is_base_of<IteratorBase, T>::value>::type
            >
            Iterator(T const &it)
                :it_(new T(it))
            {}

            Iterator(Iterator const & ii)
                :it_(ii.it_->makeUniqueCopy())
            {}

            Iterator(Iterator && ii)
                :it_(std::move(ii.it_))
            {}

            Iterator& operator=(Iterator const & ii)
            {
                it_ = ii.makeUniqueCopy();
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

            bool equal(Iterator const & it) const
            {
                return it_->id() == it.it_->id();
            }

            SizeT operator[](DiffT pos) const {return it_->value(pos);}
            SizeT operator*() const {return it_->value();}
            // operator SizeT() const {return it_->value();}

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


            Iterator operator+(DiffT pos)
            {
                return add(pos);
            }
            Iterator operator-(DiffT pos)
            {
                return add(-pos);
            }
            bool operator==(Iterator const & li)
            {
                return equal(li);
            }
            bool operator!=(Iterator const & li)
            {
                return !equal(li);
            }

            IteratorPtr makeUniqueCopy() const
            {
                return it_->makeUniqueCopy();
            }
        };
    }
}

#endif //ITERATOR_H
