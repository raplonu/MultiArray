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

        // template<std::size_t N>
        // struct MaskLock
        // {
        //     Mask<N> & mask_;
            
        //     MaskLock(Mask<N> & mask) noexcept : mask_(mask) { mask_.disable(); }

        //     ~MaskLock() { mask_.enable(); }
        // };

        // template<std::size_t N>
        // MaskLock<N> mask_lock(Mask<N> & mask)
        // {
        //     return MaskLock<N>(mask);
        // }

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
            template<typename Small, typename Big>
            Big & big(VariantImpl<Small, Big> vi) noexcept
            {
                vi.reset();
                return *vi.big;
            }

            template<typename Small, typename Big>
            constexpr Small & small(VariantImpl<Small, Big> & vi) noexcept
            {
                return vi.small;
            }

            template<typename Small, typename Big>
            constexpr const Small & small(const VariantImpl<Small, Big> & vi) noexcept
            {
                return vi.small;
            }
        }





        template<typename Small, typename Big>
        class Variant
        {
            using VariantImpl = VariantImpl<Small, Big>;

            VariantImpl data_;

            // constexpr Variant(VariantImpl data) noexcept : data_(data){}
        public:
            
            constexpr Variant() noexcept : data_(){}

            // Emplace constructor for small data
            template<typename... Args>
            CONSTEXPR14 Variant(InitSmall, Args && ... args) noexcept : data_()
            {
                data_.small = Small(std::forward<Args>(args)...);

                testSmall();
            }

            CONSTEXPR14 Variant(Big * big) noexcept : data_()
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

            VariantImpl release() noexcept
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

            Big & big()
            {
                massert(isBig());
                
                return impl::big(data_);
            }

            const Big & big() const
            {
                massert(isBig());
                
                return impl::big(data_);
            }
        };
    }
}

#endif //MA_DETAIL_VARIANT_H