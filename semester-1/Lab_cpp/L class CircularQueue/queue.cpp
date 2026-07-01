#include "queue.h"
#include <algorithm>
#include <iostream>

ArrayQueue::ArrayQueue(int size)
    : size(size), p(new int[size]), head(0), tail(0), n(0) {
    std::fill(p, p + size, 0);
}

ArrayQueue::ArrayQueue(const ArrayQueue& q)
    : size(q.size), p(new int[q.size]), head(q.head), tail(q.tail), n(q.n) {
    std::copy(q.p, q.p + q.size, p);
}

ArrayQueue::~ArrayQueue() {
    delete[] p;
}

void ArrayQueue::push_back(int value) {
    if (isFull()) {
        throw Full_ex(value);
    }
    p[tail] = value;
    tail = (tail + 1) % size;
    ++n;
}

int ArrayQueue::pop_front() {
    if (isEmpty()) {
        throw Empty_ex();
    }
    int value = p[head];
    p[head] = 0;  
    head = (head + 1) % size;
    --n;
    return value;
}

bool ArrayQueue::isEmpty() const {
    return n == 0;
}

bool ArrayQueue::isFull() const {
    return n == size;
}

void ArrayQueue::print() const {
    if (isEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return;
    }
    std::cout << "Queue (front -> back): ";
    for (int i = 0; i < n; ++i) {
        int idx = (head + i) % size;
        std::cout << p[idx] << " ";
    }
    std::cout << std::endl;
}
