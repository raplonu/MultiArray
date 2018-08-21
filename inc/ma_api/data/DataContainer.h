#ifndef MA_DATA_DATA_CONTAINER_H
#define MA_DATA_DATA_CONTAINER_H

#include <memory>

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
                virtual ~DataContainerInterface() =default;

                virtual Ptr  ptr() = 0;
                virtual CPtr ptr() const = 0;
            };

            template<typename T, typename Ptr, typename CPtr, typename Data>
            class DataContainerImpl : public DataContainerInterface<T, Ptr, CPtr>
            {
            protected:
                Data data_;

            public:
                DataContainerImpl(Data && data) noexcept :
                    data_(forward<Data>(data))
                {}

                Ptr ptr() override
                {
                    return ptrOf<T>(data_);
                }

                CPtr ptr() const override
                {
                    return ptrOf<const T>(data_);
                }
            };

            template
            <
                typename T, typename Ptr, typename CPtr, typename Data,
                typename Res = SharedDataContainer<T, Ptr, CPtr>,
                typename Impl =  DataContainerImpl<T, Ptr, CPtr, Data>
            >
            Res make(Data && data)
            {
                return Res(new Impl(forward<Data>(data)));
            }
        }

        


        template<typename T, typename Ptr = T *, typename CPtr = const T *>
        class DataContainer
        {
            using Impl = impl::SharedDataContainer<T, Ptr, CPtr>;

        protected:
            Impl data_;

        public:
            constexpr DataContainer() noexcept = default;

            template<typename Data>
            DataContainer(Data && data):
                data_(impl::make<T, Ptr, CPtr>(forward<Data>(data)))
            {}

            DataContainer(DataContainer const &) = default;
            DataContainer(DataContainer &&) = default;

            ~DataContainer() = default;

            template<typename Data>
            void reset(Data && data)
            {
                data_ = impl::make<T, Ptr, CPtr>(forward<Data>(data));
            }

            Ptr ptr()
            {
                return data_->ptr();
            }

            CPtr ptr() const
            {
                return data_->ptr();
            }
        };
    }
}

#endif //MA_DATA_DATA_CONTAINER_H
