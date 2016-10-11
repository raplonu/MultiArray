/*
* Copyright (c) 2016 Julien Bernard
* 
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
* 
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#ifndef MA_CUARRAY_H
#define MA_CUARRAY_H

#include <iostream>
#include <cuda_runtime_api.h>

#include "../int/Array.h"

namespace ma
{
	static void HandleError(
		cudaError_t err,
		const char *file,
		i32 line
	){
		if (err != cudaSuccess) {
			cerr
				<< cudaGetErrorString( err )
				<< " in " << file
				<< " at line " << line << endl;
			exit( EXIT_FAILURE );
		}
	}
	
	#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

	int const INVALIDDEVICE(-1);

	struct DeviceId
	{
		int id;
		
		DeviceId() :
			id(INVALIDDEVICE) {}

		DeviceId(int id)
			:id(id)
		{}

		DeviceId(DeviceId const & deviceId)
			:id(deviceId.id)
		{}

		bool isValid() const
		{
			return id != INVALIDDEVICE;
		}

		void set() const
		{
			if (isValid())
				HANDLE_ERROR(cudaSetDevice(id));
		}

		void operator=(int newId)
		{
			id = newId;
		}

		operator int() const
		{
			return id;
		}
	};

	template<typename T>
	struct Device
	{
		T* _ptr;
		size_t _size;
		DeviceId _id;
		
		Device():_ptr(nullptr), _size(), _id(){}

		Device(size_t nbElement):_ptr(nullptr), _size(nbElement * sizeof(T)), _id()
		{
			allocate();
		}

		Device(size_t nbElement, DeviceId id):_ptr(nullptr), _size(nbElement * sizeof(T)), _id(id)
		{
			allocate();
		}

		~Device()
		{
			deallocate();
		}

		void allocate()
		{
			if(_ptr)
				deallocate();

			_id.set();
			HANDLE_ERROR(cudaMalloc((void**)_ptr, _size * sizeof(T)));
		}

		void deallocate()
		{
			if(_ptr)
			{
		    	cudaFree(_ptr);
		    	_ptr = nullptr;
			}
		}

		void setDevice(DeviceId newId)
		{
			if(_id != newId)
			{
				_id = newId;
				allocate();
			}
		}

		T* ptr() const
		{
        		return _ptr;
		}
		
		operator T*() const
		{
			return _ptr;
		}

		T* operator+(size_t offset) const
		{
			return _ptr + offset;
		}
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

}

#endif //MA_CUARRAY_H
