#pragma once

#include "stdafx.h"

template <class T>
class Buffer
{
protected:

#pragma region Protected

	int64_t _size;
	T* _data = nullptr;

#pragma endregion

public:

#pragma region Functions Public

	Buffer();
	~Buffer();

	void Allocate(int64_t size);
	void Destroy();

	T operator[](int64_t index) const;
	T& operator[](int64_t index);

	inline T* GetDataPtr() { return _data; }
	inline int64_t GetSize() const { return _size; }

#pragma endregion
};

#pragma region Definitions

template<class T>
inline Buffer<T>::Buffer()
{
}

template<class T>
inline Buffer<T>::~Buffer()
{
	if (_data != nullptr)
	{
		Destroy();
	}
}

template<class T>
inline void Buffer<T>::Allocate(int64_t size)
{
	ASSERT(size != 0);
	_size = size;
	_data = new T[size];
	ZeroMemory(_data, size * sizeof(T));
}

template<class T>
inline void Buffer<T>::Destroy()
{
	delete[] _data;
	_data = nullptr;
	_size = 0;
}

template<class T>
inline T Buffer<T>::operator[](int64_t index) const
{
	ASSERT(_data != nullptr && index >= 0 && index < _size);
	return _data[index];
}

template<class T>
inline T& Buffer<T>::operator[](int64_t index)
{
	ASSERT(_data != nullptr && index >= 0 && index < _size);
	return _data[index];
}


#pragma endregion