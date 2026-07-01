#ifndef QUEUE_H
#define QUEUE_H

#include <exception>
#include <string>

class Full_ex : public std::exception {
    int value;
    std::string message;

public:
    explicit Full_ex(int v) : value(v), message("Queue is full. Cannot add: " + std::to_string(v)) {}
    const char* what() const noexcept { return message.c_str(); }
    int getValue() const noexcept { return value;}
};

class Empty_ex : public std::exception {
    std::string message = "Queue is empty.";

public:
    const char* what() const noexcept { return message.c_str();}
};

class ArrayQueue {    /* кольцевая очередь на массиве */
    int size;         /* размерность массива */
    int* p;           /* указатель на массив */
    int head;         /* индекс первого занятого элемента */
    int tail;         /* индекс первого свободного элемента для вставки */
    int n;            /* количество элементов в очереди */

public:
    explicit ArrayQueue(int size);       /* инициализация очереди */
    ArrayQueue(const ArrayQueue& q);     /* конструктор копирования */
    ~ArrayQueue();                       /* разрушить очередь */

    void push_back(int value);           /* втолкнуть элемент в очередь */
    int pop_front();                     /* удалить элемент из очереди */
    bool isEmpty() const;                /* пустая очередь? */
    bool isFull() const;                 /* полная очередь? */
    void print() const;                  /* вывести элементы очереди */
};

#include "queue.cpp"

#endif
