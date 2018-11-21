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
#ifndef MA_DETAIL_VARIANT_H
#define MA_DETAIL_VARIANT_H

#include <bitset>

#include <ma_api/type.h>

#include <ma_api/function.h>

namespace ma
{
    namespace detail
    {
        const std::size_t ByteSize(8);

        template<std::size_t BitNb>
        struct Mask : std::bitset<BitNb>
        {
            using Base = std::bitset<BitNb>;
            
            constexpr Mask() noexcept = default;
            
            void enable() noexcept
            {
                Base::operator[](BitNb - 1) = 1;
            }
            
            void disable() noexcept
            {
                Base::operator[](BitNb - 1) = 0;
            }
            
            constexpr bool isEnable() const noexcept
            {
                return Base::operator[](BitNb - 1) == 1;
            }
        };

        enum InitSmall {initSmall};
        enum InitBig {initBig};

        template<typename Small, typename Big, int BitNb = max(sizeof(Small), sizeof(Big*)) * ByteSize>
        union VariantImpl
        {
            Small small;
            Big * big;
            
            Mask<BitNb> mode;

            constexpr VariantImpl() noexcept : mode(){}
            constexpr VariantImpl(const VariantImpl & vi) noexcept : mode(vi.mode){}

            constexpr bool isBig() const noexcept
            {
                return mode.isEnable();
            }
            
            constexpr bool isSmall() const noexcept
            {
                return ! mode.isEnable();
            }

            void enableBig() noexcept
            {
                mode.enable();
            }
            
            void enableSmall() noexcept
            {
                mode.disable();
            } 
            
            void reset() noexcept
            {
                mode.disable();
            }
        };

        namespace impl
        {
            template<typename T>
            auto big(T vi) noexcept -> decltype(*vi.big)
            {
                vi.reset();
                return *vi.big;
            }

            template<typename T>
            constexpr auto small(T & vi) noexcept -> decltype(vi.small) &
            {
                return vi.small;
            }

            template<typename T>
            constexpr auto small(const T & vi) noexcept -> const decltype(vi.small) &
            {
                return vi.small;
            }
        }





        template<typename Small, typename Big>
        class Variant
        {
            using VariantI = VariantImpl<Small, Big>;

            VariantI data_;

        public:
            
            constexpr Variant() noexcept : data_(){}

            // Emplace constructor for small data
            template<typename... Args>
            MA_CONSTEXPR Variant(InitSmall, Args && ... args) noexcept : data_()
            {
                data_.small = Small(std::forward<Args>(args)...);

                testSmall();
            }

            MA_CONSTEXPR Variant(Big * big) noexcept : data_()
            {
                data_.big = big;

                testBig();
            }

            // Copy mode because it is sure than sizeof mode >= of others sizes
            Variant(const Variant & v):
                data_(v.data_)
            {
                // Can only copy if is isSmall, cannot know type of big (can be devired)
                massert(isSmall());
            }

            Variant(Variant && v) noexcept:
                data_(v.release())
            {}

            Variant& operator=(Variant && v) noexcept
            {
                data_ = v.release();
                return *this;
            }

            ~Variant()
            {
                reset();
            }

            VariantI release() noexcept
            {
                auto data = data_;

                // Release big ownership
                data_.enableSmall();
                
                return data;
            }

            void reset() noexcept
            {
                //Need to test if is not small
                if(isBig())
                    delete &impl::big(data_);
            }
            
            constexpr bool maskUnset() const noexcept
            {
                return data_.isSmall();
            }

            constexpr bool isBig() const noexcept
            {
                return data_.isBig();
            }
            
            constexpr bool isSmall() const noexcept
            {
                return data_.isSmall();
            }

            void testSmall() const noexcept
            {
                //Test if mask bit is not used
                massert(maskUnset());
                
                //No need to set the flag
            }

            void testBig() noexcept
            {
                //Test if mask bit is not used
                massert(maskUnset());
                
                //set mask to enable big mode
                data_.enableBig();
            }
            
            template<typename... Args>
            void emplaceSmall(Args && ... args) noexcept
            {
                //Clean ptr
                reset();
                
                data_.small = Small(std::forward<Args>(args)...);
                
                testSmall();
            }
            

            void setBig(Big * big)
            {
                //Clean ptr
                reset();
                
                data_.big = big;
                
                testBig();
            }
            
            Small & small() noexcept
            {
                massert(isSmall());
                
                return impl::small(data_);
            }

            const Small & small() const noexcept
            {
                massert(isSmall());

                return impl::small(data_);
            }

            Big & big() const
            {
                massert(isBig());
                
                return impl::big(data_);
            }

            // Big big() const
            // {
            //     massert(isBig());
                
            //     return impl::big(data_);
            // }
        };
    }
}

#endif //MA_DETAIL_VARIANT_H