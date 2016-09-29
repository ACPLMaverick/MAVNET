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
	T GetPixelScaled(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void Fill(T val);

	uint16_t GetWidth() { return m_width; }
	uint16_t GetHeight() { return m_height; }

#pragma endregion
};

#pragma region Function Definitions


template <class T> Buffer<T>::Buffer(uint16_t width, uint16_t height) :
	m_width(width),
	m_height(height)
{
	m_data = new T[width * height];
	ZeroMemory(m_data, width * height * sizeof(T));
}

template <class T> Buffer<T>::~Buffer()
{
	delete[] m_data;
}


#pragma endregion

template<class T>
inline void Buffer<T>::SetPixel(uint16_t x, uint16_t y, T val)
{
	if (x >= 0 && x <= m_width && y >= 0 && y <= m_height)
	{
		m_data[y * (m_width - 1) + x] = val;
	}
}

template<class T>
inline T Buffer<T>::GetPixel(uint16_t x, uint16_t y)
{
	return m_data[y * (m_width - 1) + x];
}

template<class T>
inline T Buffer<T>::GetPixelScaled(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	uint16_t coordX = (uint16_t)((float)x * (float)(m_width) / (float)(w));
	uint16_t coordY = (uint16_t)((float)y * (float)(m_height) / (float)(y));
	return GetPixel(x, y);
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
