#pragma once
#ifndef __I_MANAGER_H__
#define __I_MANAGER_H__

template<typename T>
class IManager {
protected:
	static T* instance;
	IManager() { instance = static_cast<T*> (this); };

public:
	static T* getInstance();
	static void freeInstance();

private:
	IManager(IManager const&) {};
	IManager& operator=(IManager const&) {};
};

template<typename T>
typename T* IManager<T>::instance = nullptr;

template<typename T>
T* IManager<T>::getInstance() {
	if (instance == nullptr) {
		IManager<T>::instance = new T();
	}
	return instance;
}

template<typename T>
void IManager<T>::freeInstance() {
	delete IManager<T>::instance;
	IManager<T>::instance = nullptr;
}

#endif