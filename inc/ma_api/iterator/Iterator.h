/**********************************************************************************
 * Copyright (c) 2016-2019 Julien Bernard, https://github.com/raplonu/MultiArray
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
#ifndef MA_ITERATOR_ITERATOR_H
#define MA_ITERATOR_ITERATOR_H

#include <memory>

#include <ma_api/config.h>
#include <ma_api/type.h>
#include <ma_api/traits.h>

#include <ma_api/detail/Variant.h>

#include <ma_api/iterator/iteratorFunction.h>

namespace ma
{
    namespace iterator
    {
        class IteratorInterface;

        template<typename> class IteratorImpl;

        using uIterator = std::unique_ptr<IteratorInterface>;

        class IteratorInterface
        {
        public:
            virtual ~IteratorInterface() = default;

            virtual SizeT value(DiffT pos = 0) const = 0;

            virtual void increase(DiffT pos) = 0;

            virtual uIterator clone() const = 0;

            /**
             * aim to compare if iterator is equal
             **/
            virtual SizeT id() const = 0;
        };

        template<typename Iterator>
        uIterator makeIteratorImpl(Iterator && iterator)
        {
            return uIterator(new IteratorImpl<decay_t<Iterator>>(std::forward<Iterator>(iterator)));
        }

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

            void increase(DiffT pos) override
            {
                it_ += pos;
            }

            uIterator clone() const override
            {
                return makeIteratorImpl(it_);
            }

            SizeT id() const override
            {
                return iteratorId(it_);
            }
        };

        class IteratorVariant
        {
        protected:
            using IteratorM = detail::Variant<LinearIterator, IteratorInterface>;

            IteratorM iterator_;

            // MA_CONSTEXPR IteratorVariant(IteratorInterface * ptr) noexcept :
            //     iterator_(detail::initBig, ptr)
            // {}
            
        public:
            MA_CONSTEXPR IteratorVariant(SizeT start = 0, DiffT step = 1) noexcept :
                iterator_(detail::initSmall, start, step)
            {}

            MA_CONSTEXPR IteratorVariant(const LinearIterator & li) noexcept :
                iterator_(detail::initSmall, li)
            {}

            template<
                typename T, typename BaseT = decay_t<T>,
                typename = IsNotSame<IteratorVariant, BaseT>,
                typename = IsNotSame<LinearIterator, BaseT>
            >
            IteratorVariant(T && it) noexcept :
                iterator_(new IteratorImpl<BaseT>(std::forward<T>(it)))
            {}

            IteratorVariant(const IteratorVariant & it):
                iterator_(it.clone())
            {}

            IteratorVariant(IteratorVariant &&) noexcept = default;

            IteratorVariant& operator=(const IteratorVariant & it)
            {
                iterator_ = it.clone();
                return *this;
            }

            IteratorVariant& operator=(IteratorVariant &&) noexcept = default;

            SizeT value(DiffT pos = 0) const
            {
                return iterator_.isSmall()
                    ? iterator_.small().value(pos)
                    : iterator_.big().value(pos);
            }

            void increase(DiffT pos)
            {
                if(iterator_.isSmall()){
                    iterator_.small().increase(pos);

                    // Just check if increase is not modifying variant bit ctrl
                    massert(iterator_.isSmall());
                }
                else
                    iterator_.big().increase(pos);
            }

            IteratorM clone() const
            {
                return iterator_.isSmall()
                    ? IteratorM(detail::initSmall, iterator_.small())
                    : IteratorM(iterator_.big().clone().release());
            }

            SizeT id() const
            {
                return iterator_.isSmall()
                    ? iteratorId(iterator_.small())
                    : iterator_.big().id();
            }
        };

        class IteratorLegacy
        {
        protected:
            using IteratorM = uIterator;

            IteratorM iterator_;

            // IteratorLegacy(IteratorM && it) noexcept:
            //     iterator_(std::move(it))
            // {}

        public:
            IteratorLegacy(SizeT start = 0, DiffT step = 1):
                iterator_(makeIteratorImpl(LinearIterator(start, step)))
            {}

            template<
                typename T, typename BaseT = decay_t<T>,
                typename = IsNotSame<IteratorLegacy, BaseT>,
                typename = IsNotSame<IteratorM, BaseT>
            >
            IteratorLegacy(T && it) noexcept :
                iterator_(makeIteratorImpl(std::forward<T>(it)))
            {}

            IteratorLegacy(const IteratorLegacy & it):
                iterator_(it.clone())
            {}

            IteratorLegacy(IteratorLegacy && it) noexcept = default;

            IteratorLegacy& operator=(const IteratorLegacy & it)
            {
                iterator_ = it.clone();
                return *this;
            }

            IteratorLegacy& operator=(IteratorLegacy && it) noexcept = default;

            SizeT value(DiffT pos = 0) const
            {
                return iterator_->value(pos);
            }

            void increase(DiffT pos)
            {
                iterator_->increase(pos);
            }

            uIterator clone() const
            {
                return iterator_->clone();
            }

            SizeT id() const
            {
                return iterator_->id();
            }
        };

        template<typename Impl>
        class IteratorT
        {
        protected:
            Impl it_;

        public:
            explicit IteratorT(SizeT start = 0, DiffT step = 1)
                :it_(start, step)
            {}

            template<
                typename T, typename BaseT = decay_t<T>,
                typename = IsAll<not is_same_v<IteratorT, BaseT>, is_random_it_v<BaseT> >
                // typename = IsNotSame<IteratorT, BaseT>,
                // typename = IsRandomIt<BaseT>
            >
            IteratorT(T && it)
                :it_(forward<T>(it))
            {}

            IteratorT(const IteratorT & ii) = default;
            IteratorT(IteratorT && ii) = default;

            IteratorT& operator=(const IteratorT & ii) = default;
            IteratorT& operator=(IteratorT && ii) = default;

            IteratorT add(DiffT pos) const
            {
                IteratorT tmp(*this);
                return tmp.increase(pos);
            }

            IteratorT& increase(DiffT pos)
            {
                it_.increase(pos);
                return *this;
            }

            IteratorT postIncrease(DiffT pos)
            {
                IteratorT tmp(*this);
                increase(pos);
                return tmp;
            }

            bool equal(const IteratorT & it) const
            {
                return it_.id() == it.it_.id();
            }

            SizeT operator[](DiffT pos) const {return it_.value(pos);}
            
            SizeT operator*() const {return it_.value();}

            IteratorT& operator++() {return increase(1); }
            IteratorT operator++(int) { return postIncrease(1); }

            IteratorT& operator--() { return increase(-1); }
            IteratorT operator--(int) { return postIncrease(-1); }

            IteratorT& operator+=(DiffT pos) {return increase(pos);}
            IteratorT& operator-=(DiffT pos) {return increase(-pos);}


            IteratorT operator+(DiffT pos) const
            {
                return add(pos);
            }

            IteratorT operator-(DiffT pos) const
            {
                return add(-pos);
            }
            bool operator==(const IteratorT & li) const
            {
                return equal(li);
            }
            bool operator!=(const IteratorT & li) const
            {
                return !equal(li);
            }
        };

        #ifdef LIGHTVARIANT
        using Iterator = IteratorT<IteratorVariant>;
        #else
        using Iterator = IteratorT<IteratorLegacy>;
        #endif
    }
}

namespace std
{

    template<typename T> struct iterator_traits<ma::iterator::IteratorT<T>>
    {
            using value_type = ma::SizeT;
            using difference_type = ma::DiffT;
            using pointer = void;
            using reference = void;
            using iterator_category = std::random_access_iterator_tag;
    };
}

#endif //MA_ITERATOR_ITERATOR_H
