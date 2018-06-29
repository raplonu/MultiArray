#ifndef MA_DETAIL_FORCE_COPY_H
#define MA_DETAIL_FORCE_COPY_H

namespace ma
{
    namespace detail
    {
        template<typename T>
        class BaseForceCopy{};

        template<typename T, typename Data>
        struct ForceCopy : public BaseForceCopy<T>
        {
            using DataT = Data;

            Data const & data_;

            ForceCopy(Data const & data):data_(data){}

            Data const & data() const
            {
                return data_;
            }

            operator Data const &() const
            {
                return data();
            }
        };
    }
}

#endif //MA_DETAIL_FORCE_COPY_H
