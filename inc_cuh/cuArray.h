#pragma once
#include <iostream>
#include <cuda_runtime_api.h>

#include "../int/Array.h"

int const INVALIDDEVICE(-1);

struct DeviceId
{
	DeviceId() :
			id(INVALIDDEVICE) {}

	DeviceId(int id) :
			id(id) {}

	DeviceId(DeviceId const & deviceId) :
			id(deviceId.id) {}

	bool isValid() const
	{
		return id != INVALIDDEVICE;
	}

	void set() const
	{
		if (isValid())
			cudaSetDevice(id);
	}

	void operator=(int newId)
	{
		id = newId;
	}

	operator int() const
	{
		return id;
	}

	int id;
};

template<typename T>
struct Device
{
	Device():ptr_(nullptr), size_(), id_(){}

	Device(size_t nbElement):ptr_(nullptr), size_(nbElement * sizeof(T)), id_()
    {
        allocate();
    }

	Device(size_t nbElement, DeviceId id):ptr_(nullptr), size_(nbElement * sizeof(T)), id_(id)
    {
        allocate();
    }

    ~Device()
    {
    	deallocate();
    }

    void allocate()
    {
        if(ptr_)
        	deallocate();

        cudaMalloc<T>(&ptr_, size_, id_);
    }

    void deallocate()
    {
        if(ptr_)
        {
            cudaFree(ptr_);
            ptr_ = nullptr;
        }
    }

    void setDevice(DeviceId newId)
    {
    	if(id_ != newId)
    	{
    		id_ = newId;
    		allocate();
    	}
    }

    operator T*() const
    {
        return ptr_;
    }

    T* operator+(size_t offset) const
    {
    	return ptr_ + offset;
    }

    T* ptr_;
    size_t size_;
    DeviceId id_;
};

template<typename T, typename T2>
void memCopy(
		Device<T> & dst, T2 const & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc
){
	HANDLE_ERROR(
		cudaMemcpy(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyHostToDevice
	));
}

template<typename T, typename T2>
void memCopy(
		Device<T> & dst, T2 const & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc,
		cudaStream_t stream
){
	HANDLE_ERROR(
		cudaMemcpyAsync(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyHostToDevice, stream
	));
}

template<typename T1, typename T>
void memCopy(
		T1 & dst, Device<T> & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc
){
	HANDLE_ERROR(
		cudaMemcpy(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyDeviceToHost
	));
}

template<typename T1, typename T>
void memCopy(
		T1 & dst, Device<T> const & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc,
		cudaStream_t stream
){
	HANDLE_ERROR(
		cudaMemcpyAsync(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyDeviceToHost, stream
	));
}

template<typename T>
void memCopy(
		Device<T> & dst, Device<T> const & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc
){
	HANDLE_ERROR(
		cudaMemcpy(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyDeviceToDevice
	));
}

template<typename T>
void memCopy(
		Device<T> & dst, Device<T> const & src, size_t nbElement,
		size_t offsetDst, size_t offsetSrc,
		cudaStream_t stream
){
	HANDLE_ERROR(
		cudaMemcpyAsync(
			dst + offsetDst, src + offsetSrc, nbElement * sizeof(T),
			cudaMemcpyDeviceToDevice, stream
	));
}

template<typename T>
void memFill(
	Device<T> & dst, T const value, size_t nbElement,
	size_t offsetDst
){
	T *tmpPtr = new T[nbElement];
    std::fill<T*, T>(tmpPtr, tmpPtr + nbElement, value);
    memCopy(dst, tmpPtr, nbElement, offsetDst, 0);
    delete[] tmpPtr;
}

template<typename T>
void memFill(
	Device<T> & dst, T const value, size_t nbElement,
	size_t offsetDst, cudaStream_t stream
){
	T *tmpPtr = new T[nbElement];
    std::fill(tmpPtr, tmpPtr + nbElement, value);
    memCopy<Device<T>, T*>(dst, tmpPtr, nbElement, offsetDst, 0, stream);
}


template<typename T>
class DeviceArray : public BaseArray<T, Device<T>>
{
public:
	using BaseArray<T, Device<T>>::BaseArray;

	void setDevice(DeviceId const newId)
	{
		BaseArray<T, Device<T>>::container_->board_.setDevice(newId);
	}

	DeviceId deviceId() const
	{
		return BaseArray<T, Device<T>>::container_->board_.id_;
	}
};

template<typename T>
using HostArray = Array<T>;
