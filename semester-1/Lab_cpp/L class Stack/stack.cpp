#include "stack.h"

template<class T>
Stack<T>::Stack(int size)
    : size(size), p(new T[size]), top(0) {
    for (int i = 0; i < size; ++i) {
        p[i] = T();
    }
}

template<class T>
Stack<T>::Stack(const Stack& s)
    : size(s.size), p(new T[s.size]), top(s.top) {
    for (int i = 0; i < s.size; ++i) {
        p[i] = s.p[i];
    }
}

template<class T>
Stack<T>::~Stack() {
    delete[] p;
}

template<class T>
void Stack<T>::push(const T& value) {
    if (isFull()) {
        throw Exception("Stack is full.");
    }
    p[top] = value;
    ++top;
}

template<class T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw Exception("Stack is empty.");
    }
    --top;
    T value = p[top];
    p[top] = T();
    return value;
}

template<class T>
bool Stack<T>::isEmpty() const {
    return top == 0;
}

template<class T>
bool Stack<T>::isFull() const {
    return top == size;
}

template<class T>
void Stack<T>::print() const {
    if (isEmpty()) {
        std::cout << "Stack is empty." << std::endl;
        return;
    }
    std::cout << "Stack (top -> bottom): ";
    for (int i = top - 1; i >= 0; --i) {
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
}
