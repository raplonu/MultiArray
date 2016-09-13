#pragma once
#include <iostream>
#include <Array.h>
#include <cuda_runtime_api.h>

#include "DeviceAllocator.h"
#include "type.h"

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

        razMalloc<T>(&ptr_, size_, id_);
    }

    void deallocate()
    {
        if(ptr_)
        {
            razFree(ptr_);
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


//#pragma once
//
//#include <cuda_runtime_api.h>
//#include <iostream>
//#include <memory>
//#include <cstring>
//
//struct Host;
//struct Device;
//
//i32 const INVALIDDEVICE(-1);
//
//struct DeviceId
//{
//	DeviceId() :
//			id(INVALIDDEVICE) {}
//
//	DeviceId(int id) :
//			id(id) {}
//
//	DeviceId(DeviceId const & deviceId) :
//			id(deviceId.id) {}
//
//	void set() const
//	{
//		if (id != INVALIDDEVICE)
//			cudaSetDevice(id);
//	}
//
//	void operator=(i32 newId)
//	{
//		id = newId;
//	}
//
//	operator i32() const
//	{
//		return id;
//	}
//
//	i32 id;
//};
//
//template<typename T, typename Board> struct SetMem;
//
//template<typename T>
//struct SetMem<T, Host>
//{
//	static void setMem(T * dst, T const value, size_t const nbElement,
//			size_t offset = 0)
//	{
//		for (size_t i(offset); i < nbElement; ++i)
//			dst[i] = value;
//	}
//
//	static void setMem(T * dst, T const * array, size_t const nbElement,
//			size_t offset = 0)
//	{
//		for (size_t i(offset); i < nbElement; ++i)
//			dst[i] = array[i];
//	}
//};
//
//template<typename T>
//struct SetMem<T, Device>
//{
//	static void setMem(T * dst, T const value, size_t const nbElement,
//			size_t offset = 0)
//	{
//		T* hPtr = new T[nbElement - offset];
//		for (size_t i(offset); i < nbElement; ++i)
//			hPtr[i] = value;
//		HANDLE_ERROR(
//				cudaMemcpy(dst + offset, hPtr, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice));
//		delete[] hPtr;
//	}
//
//	static void setMemAsync(T * dst, T const value, size_t const nbElement,
//			size_t offset = 0, cudaStream_t stream = 0)
//	{
//		T* hPtr = new T[nbElement - offset];
//		for (size_t i(offset); i < nbElement; ++i)
//			hPtr[i] = value;
//		HANDLE_ERROR(
//				cudaMemcpy(dst + offset, hPtr, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice, stream));
//		delete[] hPtr;
//	}
//
//	static void setMem(T * dst, T const * array, size_t const nbElement,
//			size_t offset = 0)
//	{
//		HANDLE_ERROR(
//				cudaMemcpy(dst + offset, array, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice));
//	}
//
//	static void setMemAsync(T * dst, T const * array, size_t const nbElement,
//			size_t offset = 0, cudaStream_t stream = 0)
//	{
//		HANDLE_ERROR(
//				cudaMemcpy(dst + offset, array, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice, stream));
//	}
//};
//
//template<typename T, typename Board> struct Allocator;
//
//template<typename T>
//struct Allocator<T, Host>
//{
//	static T* allocate(size_t nbElement)
//	{
//		return new T[nbElement];
//	}
//
//	static void desallocate(T* & ptr)
//	{
//		if (ptr != nullptr)
//		{
//			delete[] ptr;
//			ptr = nullptr;
//		}
//	}
//};
//
//template<typename T>
//struct Allocator<T, Device>
//{
//	static T* allocate(size_t nbElement)
//	{
//		T* ptr;
//		HANDLE_ERROR(cudaMalloc((void**) &ptr, nbElement * sizeof(T)));
//		return ptr;
//	}
//
//	static void desallocate(T* & ptr)
//	{
//		if (ptr != nullptr)
//		{
//			cudaFree(ptr);
//			ptr = nullptr;
//		}
//	}
//};
//
//template<typename T, typename BDst, typename BSrc> struct DataMovement;
//
//template<typename T>
//struct DataMovement<T, Host, Host>
//{
//	static void copyData(T * dst, T const * src, size_t nbElement)
//	{
//		std::memcpy(dst, src, nbElement * sizeof(T));
//	}
//
//	static void moveData(T * & dst, T * & src, size_t nbElement)
//	{
//		if (dst != nullptr)
//			delete[] dst;
//		dst = src;
//		src = nullptr;
//	}
//};
//
//template<typename T>
//struct DataMovement<T, Device, Device>
//{
//	static void copyData(T * dst, T const * src, size_t nbElement)
//	{
//		HANDLE_ERROR(
//				cudaMemcpy(dst, src, nbElement * sizeof(T),
//						cudaMemcpyDeviceToDevice));
//	}
//
//	static void copyDataAsync(T * dst, T const * src, size_t nbElement,
//			cudaStream_t stream = 0)
//	{
//		HANDLE_ERROR(
//				cudaMemcpyAsync(dst, src, nbElement * sizeof(T),
//						cudaMemcpyDeviceToDevice, stream));
//	}
//
//	static void moveData(T * & dst, T * & src, size_t nbElement)
//	{
//		if (dst != nullptr)
//			cudaFree(dst);
//		dst = src;
//		src = nullptr;
//	}
//
//	static void moveDataAsync(T * & dst, T * & src, size_t nbElement,
//			cudaStream_t stream = 0)
//	{
//		if (dst != nullptr)
//			cudaFree(dst);
//		dst = src;
//		src = nullptr;
//	}
//};
//
//template<typename T>
//struct DataMovement<T, Host, Device>
//{
//	static void copyData(T * dst, T const * src, size_t nbElement)
//	{
//		HANDLE_ERROR(
//				cudaMemcpy(dst, src, nbElement * sizeof(T),
//						cudaMemcpyDeviceToHost));
//	}
//
//	static void copyDataAsync(T * dst, T const * src, size_t nbElement,
//			cudaStream_t stream = 0)
//	{
//		HANDLE_ERROR(
//				cudaMemcpyAsync(dst, src, nbElement * sizeof(T),
//						cudaMemcpyDeviceToHost, stream));
//	}
//};
//
//template<typename T>
//struct DataMovement<T, Device, Host>
//{
//	static void copyData(T * dst, T const * src, size_t nbElement)
//	{
//		HANDLE_ERROR(
//				cudaMemcpy(dst, src, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice));
//	}
//
//	static void copyDataAsync(T * dst, T const * src, size_t nbElement,
//			cudaStream_t stream = 0)
//	{
//		HANDLE_ERROR(
//				cudaMemcpyAsync(dst, src, nbElement * sizeof(T),
//						cudaMemcpyHostToDevice, stream));
//	}
//};
//
//
//
//
///*
// *	Reference
// */
//template<typename T, typename Board> class Reference;
//
//template<typename T>
//class Reference<T, Host>
//{
//public:
//	Reference(T *ptr) :
//			ptr_(ptr){}
//
//	virtual ~Reference() {}
//
//	operator T() const
//	{
//		return *ptr_;
//	}
//
//	Reference<T, Host>& operator=(T value)
//	{
//		*ptr_ = value;
//		return *this;
//	}
//
//	template<typename Board>
//	Reference<T, Host>& operator=(Reference<T,Board> const & ref)
//	{
//		*ptr_ = ref;
//		return *this;
//	}
//
//protected:
//	T *ptr_;
//};
//
//template<typename T>
//class Reference<T, Device>
//{
//public:
//	Reference(T *ptr) :
//			ptr_(ptr) {}
//
//	virtual ~Reference() {}
//
//	operator T() const
//	{
//		T value;
//		HANDLE_ERROR(
//			cudaMemcpy(&value, ptr_, sizeof(T), cudaMemcpyDeviceToHost));
//		return value;
//	}
//
//	Reference<T, Device>& operator=(T value)
//	{
//		HANDLE_ERROR(
//			cudaMemcpy(ptr_, &value, sizeof(T), cudaMemcpyHostToDevice));
//		return *this;
//	}
//
//	template<typename Board>
//	Reference<T, Host>& operator=(Reference<T,Board> ref)
//	{
//		HANDLE_ERROR(
//			cudaMemcpy(ptr_, ref, sizeof(T), cudaMemcpyHostToDevice));
//		return *this;
//	}
//
//protected:
//	T *ptr_;
//};
//
//
///*
// *	Accessor
// */
//template<typename T, typename Board>
//struct Accessor;
//
//template<typename T>
//struct Accessor<T, Host>
//{
//	static T getData(T * ptr)
//	{
//		return *ptr;
//	}
//
//	static Reference<T, Host> getReference(T *ptr)
//	{
//		return Reference<T, Host>(ptr);
//	}
//};
//
//template<typename T>
//struct Accessor<T, Device>
//{
//	static T getData(T * ptr)
//	{
//		T value;
//		HANDLE_ERROR(
//				cudaMemcpy(&value, ptr, sizeof(T), cudaMemcpyDeviceToHost));
//		return value;
//	}
//
//	static Reference<T, Device> getReference(T *ptr)
//	{
//		return Reference<T, Device>(ptr);
//	}
//};
//
//
///*
// * BaseMemory
// */
//
//template<typename T, typename Board>
//class BaseMemory
//{
//public:
//	BaseMemory() :
//			ptr_(nullptr), nbElement_(0){}
//
//	BaseMemory(size_t const nbElement) :
//			ptr_(Allocator<T, Board>::allocate(nbElement)), nbElement_(nbElement) {}
//
//	BaseMemory(size_t nbElement, T const initValue) :
//			ptr_(Allocator<T, Board>::allocate(nbElement)), nbElement_(nbElement)
//	{
//		SetMem<T, Board>::setMem(ptr_, initValue, nbElement_);
//	}
//
//	BaseMemory(size_t nbElement, T const * initArray) :
//			ptr_(Allocator<T, Board>::allocate(nbElement)), nbElement_(nbElement)
//	{
//		SetMem<T, Board>::setMem(ptr_, initArray, nbElement_);
//	}
//
//	virtual ~BaseMemory()
//	{
//		Allocator<T, Board>::desallocate(ptr_);
//	}
//
//	void setNbElement(size_t nbElement)
//	{
//		nbElement_ = nbElement;
//
//		Allocator<T, Board>::desallocate(ptr_);
//		ptr_ = Allocator<T, Board>::allocate(nbElement_);
//	}
//
//	size_t nbElement() const
//	{
//		return nbElement_;
//	}
//
//	size_t size() const
//	{
//		return nbElement_ * sizeof(T);
//	}
//
//	operator T*() const
//	{
//		return ptr_;
//	}
//
//	T* ptr() const
//	{
//		return ptr_;
//	}
//
//protected:
//
//	T*& getPtrRef()
//	{
//		return ptr_;
//	}
//
//	T* ptr_;
//	size_t nbElement_;
//};
//
///*
// * Memory
// */
//
//template<typename T, typename Board> class Memory;
//
//template<typename T>
//class Memory<T, Host> : public BaseMemory<T, Host>
//{
//public:
//	using BaseMemory<T, Host>::BaseMemory;
//};
//
//template<typename T>
//class Memory<T, Device> : public BaseMemory<T, Device>
//{
//public:
//	using BaseMemory<T, Device>::BaseMemory;
//
//	Memory() :
//		BaseMemory<T, Device>(), deviceId_(){}
//
//	Memory(DeviceId const deviceId) :
//		BaseMemory<T, Device>(), deviceId_(deviceId){}
//
//	Memory(size_t const nbElement, DeviceId const deviceId) :
//		BaseMemory<T, Device>(), deviceId_(deviceId)
//	{
//		this->nbElement_ = nbElement;
//
//		deviceId.set();
//		this->ptr_ = Allocator<T, Device>::allocate(this->nbElement_);
//	}
//
//	Memory(size_t nbElement, T const initValue, DeviceId const deviceId) :
//		BaseMemory<T, Device>(), deviceId_(deviceId)
//	{
//		this->nbElement_ = nbElement;
//
//		deviceId.set();
//		this->ptr_ = Allocator<T, Device>::allocate(this->nbElement_);
//
//		SetMem<T, Device>::setMem(this->ptr_, initValue, this->nbElement_);
//	}
//
//	Memory(size_t nbElement, T const * initArray, DeviceId const deviceId) :
//		BaseMemory<T, Device>(), deviceId_(deviceId)
//	{
//		this->nbElement_ = nbElement;
//
//		deviceId.set();
//		this->ptr_ = Allocator<T, Device>::allocate(this->nbElement_);
//
//		SetMem<T, Device>::setMem(this->ptr_, initArray, this->nbElement_);
//	}
//
//
//	DeviceId deviceId() const
//	{
//		return deviceId_;
//	}
//
//	void setDeviceId(DeviceId deviceId)
//	{
//		deviceId_ = deviceId;
//		T* tmpPtr = Allocator<T, Device>::allocate(this->nbElement_);
//		DataMovement<T, Device, Device>::copyData(tmpPtr, this->ptr_, this->nbElement_);
//		Allocator<T, Device>::desallocate(this->ptr_);
//		this->ptr_ = tmpPtr;
//	}
//
//protected:
//	DeviceId deviceId_;
//};
//
//
///*
// * BaseArray
// */
//
//template<typename T, typename Board> class BaseArray;
//template<typename T, typename Board> class Array;
//
//template<typename T, typename Board>
//class BaseArray : public Memory<T, Board>
//{
//public:
//	using Memory<T, Board>::Memory;
//
//	void setMem(T const value, size_t nbElement = 0, size_t const offset = 0)
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		SetMem<T, Board>::setMem(this->ptr_, value, nbElement, offset);
//	}
//
//	void setMemAsync(T const value, size_t nbElement = 0, size_t const offset = 0, cudaStream_t stream = 0)
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		SetMem<T, Board>::setMemAsync(this->ptr_, value, nbElement, offset, stream);
//	}
//
//	void setMem(T const * array, size_t nbElement = 0, size_t const offset = 0)
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		SetMem<T, Board>::setMem(this->ptr_, array, nbElement, offset);
//	}
//
//	void setMemAsync(T const * array, size_t nbElement = 0, size_t const offset = 0, cudaStream_t stream = 0)
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		SetMem<T, Board>::setMemAsync(this->ptr_, array, nbElement, offset, stream);
//	}
//
//	template<typename OtherBoard>
//	void setMem(Array<T, OtherBoard> const & oArray, size_t nbElement = 0, size_t const offset = 0)
//	{
//		if(nbElement == 0)
//			nbElement = oArray.nbElement();
//
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		DataMovement<T, Board, OtherBoard>::copyData(this->ptr_ + offset, oArray, nbElement);
//	}
//
//	template<typename OtherBoard>
//	void setMemAsync(Array<T, OtherBoard> const & oArray, size_t nbElement = 0, size_t const offset = 0, cudaStream_t stream = 0)
//	{
//		if(nbElement == 0)
//			nbElement = oArray.nbElement();
//
//		DataMovement<T, Board, OtherBoard>::copyDataAsync(this->ptr_ + offset, oArray, nbElement, stream);
//	}
//
//	void copyTo(T * array, size_t nbElement = 0, size_t const offset = 0) const
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		DataMovement<T, Host, Board>::copyData(array, this->ptr_ + offset, nbElement);
//	}
//
//	void copyToAsync(T * array, size_t nbElement = 0, size_t const offset = 0, cudaStream_t stream = 0) const
//	{
//		if(nbElement == 0)
//			nbElement = this->nbElement_;
//
//		if(nbElement + offset > this->nbElement_)
//			return;
//
//		DataMovement<T, Host, Board>::copyDataAsync(array, this->ptr_ + offset, nbElement, stream);
//	}
//
//	operator T*() const {
//		return this->ptr_;
//	}
//
//	T operator[](size_t i) const {
//		return Accessor<T, Board>::getData(this->ptr_ + i);
//	}
//
//	Reference<T, Board> operator[](size_t i) {
//		return Accessor<T, Board>::getReference(this->ptr_ + i);
//	}
//};
//
//
//
///*
// * Array
// */
//
//template<typename T>
//class Array<T, Host> : public BaseArray<T, Host>
//{
//public:
//	using BaseArray<T, Host>::BaseArray;
//
//	Array<T, Host>& operator=(Array<T, Host> & array)
//	{
//		if(this->nbElement_ != array.nbElement())
//			Allocator<T, Host>::desallocate(this->ptr_);
//
//		if(this->ptr_ == nullptr)
//		{
//			this->nbElement_ = array.nbElement();
//			this->ptr = Allocator<T, Host>::allocate(this->nbElement_);
//		}
//
//		DataMovement<T, Host, Host>::copyData(this->ptr_, array, this->nbElement_);
//
//		return *this;
//	}
//
//	Array<T, Host>& operator=(Array<T, Host> && array)
//	{
//		if(this->nbElement_ != array.nbElement())
//			Allocator<T, Host>::desallocate(this->ptr_);
//
//		if(this->ptr_ == nullptr)
//		{
//			this->nbElement_ = array.nbElement();
//			this->ptr_ = Allocator<T, Host>::allocate(this->nbElement_);
//		}
//
//		DataMovement<T, Host, Host>::moveData(this->ptr_, array.getPtrRef(), this->nbElement_);
//
//		return *this;
//	}
//};
//
//template<typename T>
//class Array<T, Device> : public BaseArray<T, Device>
//{
//public:
//	using BaseArray<T, Device>::BaseArray;
//
//	Array<T, Device>& operator=(Array<T, Device> & array)
//	{
//		if(this->nbElement_ != array.nbElement() || this->deviceId_.id != array.deviceId().id)
//			Allocator<T, Device>::desallocate(this->ptr_);
//
//		if(this->ptr_ == nullptr)
//		{
//			this->nbElement_ = array.nbElement();
//			this->deviceId_.id = array.deviceId().id;
//			this->deviceId_.set();
//			this->ptr = Allocator<T, Device>::allocate(this->nbElement_);
//		}
//
//		DataMovement<T, Device, Device>::copyData(this->ptr_, array, this->nbElement_);
//
//		return *this;
//	}
//
//	Array<T, Device>& operator=(Array<T, Device> && array)
//	{
//		if(this->nbElement_ != array.nbElement() || this->deviceId_.id != array.deviceId().id)
//			Allocator<T, Device>::desallocate(this->ptr_);
//
//		if(this->ptr_ == nullptr)
//		{
//			this->nbElement_ = array.nbElement();
//			this->deviceId_.id = array.deviceId().id;
//			this->deviceId_.set();
//			this->ptr_ = Allocator<T, Device>::allocate(this->nbElement_);
//		}
//
//		DataMovement<T, Device, Device>::moveData(this->ptr_, array.getPtrRef(), this->nbElement_);
//
//		return *this;
//	}
//};
//
//template<typename T>
//using HostArray = Array<T, Host>;
//
//template<typename T>
//using DeviceArray = Array<T, Device>;
