#ifndef MA_SHAPE_SIMPLE_SHAPE_H
#define MA_SHAPE_SIMPLE_SHAPE_H

#include <stdexcept>

#include <ma/detail/base.h>

namespace ma
{
    namespace shape
    {
        class SimpleShape
        {
        public:
            using SizeVect = std::vector<SizeT>;

        protected:
            SizeT start_;
            SizeT stop_;
            SizeT size_;

        public:
            SimpleShape():start_(0), stop_(0), size_(0){}

            SimpleShape(SizeT size):start_(0), stop_(size), size_(size){}

            SimpleShape(SizeT start, SizeT size):
                start_(start), stop_(start + size), size_(size)
            {}

            SimpleShape(SimpleShape const & ss) = default;
            SimpleShape& operator=(SimpleShape const & ss) = default;

        protected:
            SimpleShape(SizeT start, SizeT stop, SizeT size):
                start_(start), stop_(stop), size_(size)
            {}

        public:
            SizeT at(SizeT pos) const
            {
                if(start_ + pos > stop_)
                    throw std::out_of_range("Out of range's shape");

                return start_ + pos;
            }

            SimpleShape subShape(SizeT pos) const
            {
                if(start_ + pos >= stop_ || start_ + pos > stop_)
                    throw std::out_of_range("Out of range's shape");

                return SimpleShape(start_ + pos, start_ + pos, size_);
            }

            SimpleShape subShape(SizeT start, SizeT stop) const
            {
                if(start_ + start >= stop_ || start_ + stop > stop_)
                    throw std::out_of_range("Out of range's shape");

                return SimpleShape(start_ + start, start_ + stop, size_);
            }

            SimpleShape subShape(SizeT start, detail::Delay_) const
            {
                if(start_ + start >= stop_)
                    throw std::out_of_range("Out of range's shape");

                return SimpleShape(start_ + start, stop_, size_);
            }

            SizeT size() const
            {
                return stop_ - start_;
            }

            SizeT baseSize() const
            {
                return size_;
            }

            SizeT step() const
            {
                return size();
            }

            bool isContiguous() const
            {
                return true;
            }

            void reset()
            {
                start_ = 0;
                stop_ = size_;
            }

            SizeT start() const
            {
                return start_;
            }

            SizeT stop() const
            {
                return stop_;
            }

            SizeVect shape() const
            {
                return SizeVect(1, size());
            }

        };
    }
}
#endif //MA_SHAPE_SIMPLE_SHAPE_H
