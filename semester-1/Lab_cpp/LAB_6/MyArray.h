#ifndef MYARRAY_H
#define MYARRAY_H

#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class MyArray {
private:
    T* data;
    size_t size;

public:
    MyArray();
    MyArray(size_t initialSize);
    MyArray(const MyArray& other);
    MyArray(MyArray&& other) noexcept;
    ~MyArray();

    MyArray& operator=(const MyArray& other);
    MyArray& operator=(MyArray&& other) noexcept;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t getSize() const;
    void resize(size_t newSize, const T& defaultValue = T());
    void print() const;
};

#include "MyArray.cpp"


#endif
