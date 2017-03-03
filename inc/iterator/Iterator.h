#ifndef ITERATOR_H
#define ITERATOR_H

#include <type_traits>
#include <memory>

#include "../common/type.h"

#include "iteratorFunction.h"
#include "LinearIterator.h"

namespace ma
{
    namespace iterator
    {
        class IteratorBase
        {
        public:
            virtual ~IteratorBase(){}

            virtual SizeT operator[](SizeT pos) const = 0;
            virtual SizeT operator*() const = 0;
            virtual operator SizeT() const = 0;

            virtual IteratorBase& operator++() = 0;

            virtual IteratorBase& operator--() = 0;

            virtual IteratorBase& operator+=(SizeT pos) = 0;
            virtual IteratorBase& operator-=(SizeT pos) = 0;

            virtual std::unique_ptr<IteratorBase> makeUniqueCopy() const = 0;

            virtual SizeT id() const = 0;
            // virtual bool equal(IteratorBase const & it) const = 0;
        };

        template<typename T>
        class Iterator : public IteratorBase
        {
            using IteratorM = T;
            using IteratorT = Iterator<T>;
            using IteratorPtr = std::unique_ptr<IteratorBase>;

            IteratorM it_;

        public:
            Iterator(IteratorM it):it_(it){}
            virtual ~Iterator(){}

            virtual SizeT operator[](SizeT pos) const {return it_[pos];}
            virtual SizeT operator*() const {return *it_;}
            virtual operator SizeT() const {return *it_;};

            virtual IteratorBase& operator++() {++it_; return *this;}

            virtual IteratorBase& operator--() {--it_; return *this;}

            virtual IteratorBase& operator+=(SizeT pos) {it_ += pos; return *this;}
            virtual IteratorBase& operator-=(SizeT pos) {it_ -= pos; return *this;}

            virtual IteratorPtr makeUniqueCopy() const
            {
                return IteratorPtr{new IteratorT(it_)};
            }

            virtual SizeT id() const
            {
                return iteratorId(it_);
            }
            // virtual bool equal(IteratorBase const & it) const
            // {
            //
            // }
        };

        class IteratorInterface
        {
            using IteratorPtr = std::unique_ptr<IteratorBase>;

            IteratorPtr it_;
        public:

            template<
                typename T,
                typename = typename std::enable_if<std::is_base_of<IteratorBase, T>::value>::type
            >
            IteratorInterface(T const &it)
                :it_(new T(it))
            {}

            IteratorInterface(IteratorInterface const & ii)
                :it_(ii.it_->makeUniqueCopy())
            {}

            IteratorInterface(IteratorInterface && ii)
                :it_(std::move(ii.it_))
            {}

            IteratorInterface& operator=(IteratorInterface const & ii)
            {
                it_ = ii.makeUniqueCopy();
                return *this;
            }

            IteratorInterface& operator=(IteratorInterface && ii)
            {
                it_ = std::move(ii.it_);
                return *this;
            }

            SizeT operator[](SizeT pos) const {return (*it_)[pos];}
            SizeT operator*() const {return *(*it_);}
            operator SizeT() const {return *it_;}

            IteratorInterface& operator++() {++(*it_); return *this;}
            IteratorInterface operator++(int)
            {
                IteratorInterface tmp(*this);
                ++(*it_);
                return tmp;
            }

            IteratorInterface& operator--() {--(*it_); return *this;}
            IteratorInterface operator--(int)
            {
                IteratorInterface tmp(*this);
                --(*it_);
                return tmp;
            }

            IteratorInterface& operator+=(SizeT pos) {return increase(pos);}
            IteratorInterface& operator-=(SizeT pos) {return increase(-pos);}

            IteratorInterface add(SizeT pos) const
            {
                IteratorInterface tmp(*this);
                tmp += pos;
                return tmp;
            }

            IteratorInterface& increase(SizeT pos)
            {
                *it_ += pos;
                return *this;
            }

            bool equal(IteratorInterface const & it) const
            {
                return it_->id() == it.it_->id();
            }

            IteratorInterface operator+(SizeT pos)
            {
                return add(pos);
            }
            IteratorInterface operator-(SizeT pos)
            {
                return add(-pos);
            }
            bool operator==(IteratorInterface const & li)
            {
                return equal(li);
            }
            bool operator!=(IteratorInterface const & li)
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
