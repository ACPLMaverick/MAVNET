#pragma once

#include "stdafx.h"

template <class T>
class Buffer
{
protected:

	T* m_data;

	uint16_t m_width;
	uint16_t m_height;

public:

#pragma region Functions Public

	Buffer<T>(uint16_t width, uint16_t height);
	~Buffer<T>();

	void SetPixel(uint16_t x, uint16_t y, T val);
	T GetPixel(uint16_t x, uint16_t y);

	void Fill(T val);

	uint16_t GetWidth() { return m_width; }
	uint16_t GetHeight() { return m_height; }

#pragma endregion
};

#pragma region Function Definitions


template <class T> Buffer<T>::Buffer(uint16_t width, uint16_t height)
{
	m_data = new T[width * height];
	ZeroMemory(m_data, width * height);
}

template <class T> Buffer<T>::~Buffer()
{
	delete[] m_data;
}


#pragma endregion

template<class T>
inline void Buffer<T>::SetPixel(uint16_t x, uint16_t y, T val)
{
	m_data[x * m_width + y] = val;
}

template<class T>
inline T Buffer<T>::GetPixel(uint16_t x, uint16_t y)
{
	return m_data[x * m_width + y];
}

template<class T>
inline void Buffer<T>::Fill(T val)
{
	size_t t = m_width * m_height;
	for (size_t i = 0; i < t; ++i)
	{
		m_data[i] = val;
	}
}
