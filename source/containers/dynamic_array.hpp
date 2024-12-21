#pragma once
#include <algorithm>

template <class T> class DynamicArray {
private:
	T* data;
	int capacity;
	int max_capacity;
	int len;
	int initialized;
	T default_value;
public:
	DynamicArray(int initial_capacity, int _max_capacity = 100000) {
		initialized = 0;
		capacity = initial_capacity;
		max_capacity = _max_capacity;
		len = 0;
		if (initial_capacity > 0) {
			data = new T[initial_capacity];
		}
		else {
			data = nullptr;
		}
	}
	
	DynamicArray(const DynamicArray&) = delete;
	
	DynamicArray& operator=(const DynamicArray&) = delete;

	~DynamicArray() {
		if (data != nullptr) {
			delete[] data;
		}
	}

	T& operator[](int index) {
		if (index >= capacity) {
			Resize(2 * (index + 1));
		}

		if (index + 1 > len) {
			len = index + 1;
		}

		return data[index];
	}

	T& GetAt(int index) {
		return data[index];
	}

	int GetSize() {
		return capacity;
	}
	
	int GetMaxIndex() {
		return max_capacity;
	}
	
	void SetMaxIndex(int max_index) {
		len = max_index;
	}
	
	int Resize(int new_size) {
		if (new_size <= 0) {
			return 0;
		}

		if (new_size < capacity) {
			len = new_size - 1;
		}

		T* temp_elements = data;
		data = new T[new_size];
		if (data == nullptr) {
			return 0;
		}

		int size = std::min(capacity, new_size);
		for (int i = 0; i < size; i++)
		{
			data[i] = temp_elements[i];
		}

		// if the array grew, initialize the new elements to default_value
		if ((capacity < size) && initialized) {
			for (int i = capacity; i < size; i++) {
				data[i] = default_value;
			}
		}

		delete[] temp_elements;
		capacity = new_size;
		return 1;
	}

	int Compact() {
		return Resize(len + 1);
	}

	void Initialize(T _default_value) {
		for (int i = 0; i < capacity; i++) {
			data[i] = _default_value;
		}
		default_value = _default_value;
		initialized = 1;
	}
};