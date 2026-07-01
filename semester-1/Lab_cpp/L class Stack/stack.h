#ifndef STACK_H
#define STACK_H

#include <exception>
#include <iostream>
#include <string>

class Exception : public std::exception {
    std::string msg;
public:
    explicit Exception(const std::string& text) : msg(text) {}
    const std::string& message() const { return msg; }
};

template<class T>
class Stack {
    int size;
    T* p;
    int top;

public:
    Stack(int size);
    Stack(const Stack& s);
    ~Stack();

    void push(const T& value);
    T pop();
    bool isEmpty() const;
    bool isFull() const;
    void print() const;
};

#include "stack.cpp"

#endif
