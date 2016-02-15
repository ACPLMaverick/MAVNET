#pragma once
template <class T> class Singleton
{
protected:
	static T* instance;

public:
	static T* Singleton<T>::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

	static void Singleton<T>::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
		}
	}

};

template <class T>
T* Singleton<T>::instance = nullptr;

