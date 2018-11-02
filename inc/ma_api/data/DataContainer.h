#ifndef MA_DATA_DATA_CONTAINER_H
#define MA_DATA_DATA_CONTAINER_H

#include <memory>
#include <stdexcept>

#include <ma_api/function.h>

namespace ma
{
    namespace data
    {
        namespace impl
        {
            template<typename T, typename Ptr, typename CPtr> class DataContainerInterface;

            template<typename T, typename Ptr, typename CPtr>
            using SharedDataContainer = std::shared_ptr<DataContainerInterface<T, Ptr, CPtr>>;


            template<typename T, typename Ptr, typename CPtr>
            class DataContainerInterface
            {
            public:
                virtual ~DataContainerInterface() = default;

                virtual Ptr  ptr() = 0;
                virtual CPtr ptr() const = 0;

                virtual int size() const = 0;
            };

            template<typename T, typename Ptr, typename CPtr, typename Data>
            class DataContainerImpl : public DataContainerInterface<T, Ptr, CPtr>
            {
            protected:
                int size_;
                Data data_;

            public:
                DataContainerImpl(int size, Data && data) noexcept :
                    size_(size), data_(forward<Data>(data))
                {}

                Ptr ptr() override
                {
                    return ptrOf(data_);
                }

                CPtr ptr() const override
                {
                    return ptrOf(data_);
                }

                int size() const override
                {
                    return size_;
                }

            };

            template
            <
                typename T, typename Ptr, typename CPtr, typename Data,
                typename Res = SharedDataContainer<T, Ptr, CPtr>,
                typename Impl = DataContainerImpl<T, Ptr, CPtr, Data>
            >
            Res make(int size, Data && data)
            {
                return Res(new Impl(size, forward<Data>(data)));
            }
        }

        template<typename T, typename Ptr = T *, typename CPtr = const T *>
        class DataContainer
        {
            using Impl = impl::SharedDataContainer<T, Ptr, CPtr>;

        protected:
            int offset_;
            int size_;
            Impl data_;

            DataContainer(Impl data, int offset, int size):
                offset_(offset), size_(size), data_(std::move(data))
            {
                checkLimit();                
            }

            constexpr void checkLimit() const
            {
                // if(offset_ >= baseSize() || size_ > baseSize())
                    // throw std::range_error("Base resize of dataContainer");
            }

        public:
            constexpr DataContainer() noexcept = default;

            template<typename Data, typename = HasSize<Data>>
            DataContainer(Data && data):
                offset_(0), size_(ma::size(data)),
                data_(impl::make<T, Ptr, CPtr>(ma::size(data), forward<Data>(data)))
            {
                checkLimit();                
            }

            template<typename Data>
            DataContainer(Data && data, int size):
                offset_(0), size_(size),
                data_(impl::make<T, Ptr, CPtr>(size, forward<Data>(data)))
            {
                // Cannot check limit : don't have data size
            }

            DataContainer(DataContainer const &) = default;
            DataContainer(DataContainer &&) = default;

            ~DataContainer() = default;

            template<typename Data, typename = HasSize<Data>>
            void reset(Data && data)
            {
                setSize(0, ma::size(data));

                data_ = impl::make<T, Ptr, CPtr>(ma::size(data), forward<Data>(data));

                checkLimit();
            }

            template<typename Data>
            void reset(Data && data, int size)
            {
                setSize(0, size);

                data_ = impl::make<T, Ptr, CPtr>(size, forward<Data>(data));

                // Cannot check limit : don't have data size
            }

            constexpr void setSize(int offset, int size) noexcept
            {
                offset_ = offset;
                size_ = size;
            }

            DataContainer resize(int offset, int size)
            {
                return DataContainer(data_, offset, size);
            }

            DataContainer addOffset(int offset)
            {
                return DataContainer(data_, offset, size());
            }

            Ptr basePtr()
            {
                return data_->ptr();
            }

            CPtr basePtr() const
            {
                return data_->ptr();
            }

            Ptr ptr()
            {
                return basePtr() + offset_;
            }

            CPtr ptr() const
            {
                return basePtr() + offset_;
            }

            Ptr begin()
            {
                return ptr();
            }

            CPtr begin() const
            {
                return ptr();
            }

            Ptr end()
            {
                return begin() + size();
            }

            CPtr end() const
            {
                return begin() + size();
            }

            constexpr int size() const noexcept
            {
                return size_;
            }

            int baseSize() const noexcept
            {
                return data_->size();
            }

            constexpr int offset() const noexcept
            {
                return offset_;
            }

        };
    }
}

#endif //MA_DATA_DATA_CONTAINER_H
