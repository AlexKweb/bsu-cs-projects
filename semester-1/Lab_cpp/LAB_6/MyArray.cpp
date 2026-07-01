#include "MyArray.h"

template <typename T>
MyArray<T>::MyArray() : data(nullptr), size(0) {}

template <typename T>
MyArray<T>::MyArray(size_t initialSize) : size(initialSize) {
    if (initialSize > 0) {
        data = new T[size]();
    } else {
        data = nullptr;
    }
}

template <typename T>
MyArray<T>::MyArray(const MyArray& other) : size(other.size) {
    if (size > 0) {
        data = new T[size];
        std::copy(other.data, other.data + size, data);
    } else {
        data = nullptr;
    }
}

template <typename T>
MyArray<T>::MyArray(MyArray&& other) noexcept 
    : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

template <typename T>
MyArray<T>::~MyArray() {
    delete[] data;
}

template <typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray& other) {
    if (this != &other) {
        delete[] data;

        size = other.size;
        if (size > 0) {
            data = new T[size];
            std::copy(other.data, other.data + size, data);
        } else {
            data = nullptr;
        }
    }
    return *this;
}

template <typename T>
MyArray<T>& MyArray<T>::operator=(MyArray&& other) noexcept {
    if (this != &other) {
        delete[] data; 

        data = other.data;
        size = other.size;

        other.data = nullptr; 
        other.size = 0;
    }
    return *this;
}

template <typename T>
T& MyArray<T>::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds.");
    }
    return data[index];
}

template <typename T>
const T& MyArray<T>::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds.");
    }
    return data[index];
}

template <typename T>
size_t MyArray<T>::getSize() const {
    return size;
}

template <typename T>
void MyArray<T>::resize(size_t newSize, const T& defaultValue) {
    if (newSize == size) {
        return;
    }

    T* new_data = nullptr;
    if (newSize > 0) {
        new_data = new T[newSize];
    }
    
    size_t copy_size = std::min(size, newSize);
    for (size_t i = 0; i < copy_size; ++i) {
        new_data[i] = data[i];
    }

    if (newSize > size) {
        for (size_t i = size; i < newSize; ++i) {
            new_data[i] = defaultValue;
        }
    }

    delete[] data;
    data = new_data;
    size = newSize;
}

template <typename T>
void MyArray<T>::print() const {
    for (size_t i = 0; i < size; ++i) {
        std::cout << data[i];
        if (i < size - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}
