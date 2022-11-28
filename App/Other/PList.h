#pragma once
#include <vector>

template <typename T>
class PList {

private:

	std::vector<T*> lst;

public:

	PList() {
		lst = std::vector<T*>();
	};
	~PList() {
		for (int i = 0; i < lst.size(); i++) {
			deallocate(lst[i]);
		}
	}

	void push_back(T item) {
		T* it = new T();
		*it = item;
		lst.push_back(it)
	}

	void insert(T item, int index) {
		T* it = new T();
		*it = item;
		lst.insert(it, index);
	}
	T index(int index) {
		return *lst[index];
	}
	int size() {
		return lst.size()
	}
	void remove(int index) {
		T* ip = lst[index];
		lst.erase(index);
		deallocate(ip);
	}
	T pop(int index) {
		T* ip = lst[index];
		T item = *ip;
		lst.erase(index);
		deallocate(ip);
		return item;
	}
	

private:

	T* allocate(T item) {
		T* ip = new T();
		*ip = item;
		return ip;
	}
	void deallocate(T* item) {
		free(item);
	}

};