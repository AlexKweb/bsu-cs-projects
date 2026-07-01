#include <iostream>
#include <cstddef>
#include <utility>
#include <stdexcept>

using namespace std;

template <typename T>
class MyList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    MyList() : head(nullptr), tail(nullptr), count(0) {}

    MyList(const MyList& other) : head(nullptr), tail(nullptr), count(0) {
        const Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    MyList& operator=(const MyList& other) {
        if (this == &other) {
            return *this;
        }

        while (head != nullptr) {
            pop_front();
        }

        const Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }

        return *this;
    }

    MyList(MyList&& other) noexcept : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    MyList& operator=(MyList&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        while (head != nullptr) {
            pop_front();
        }

        head = other.head;
        tail = other.tail;
        count = other.count;

        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
        return *this;
    }

    ~MyList() {
        while (head != nullptr) {
            pop_front();
        }
    }

    void push_front(const T& value) {
        Node* node = new Node{value, nullptr, head};
        if (head == nullptr) {
            head = tail = node;
        } else {
            head->prev = node;
            head = node;
        }
        ++count;
    }

    void push_back(const T& value) {
        Node* node = new Node{value, tail, nullptr};
        if (tail == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++count;
    }

    bool insert_after(const T& after, const T& value) {
        Node* temp = head;
        while (temp != nullptr && temp->data != after) {
            temp = temp->next;
        }

        if (temp == nullptr) {
            return false;
        }

        Node* next_node = temp->next;
        Node* node = new Node{value, temp, next_node};
        temp->next = node;

        if (next_node != nullptr) {
            next_node->prev = node;
        } else {
            tail = node;
        }

        ++count;
        return true;
    }

    void pop_front() {
        if (head == nullptr) {
            return;
        }

        Node* node = head;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }

        delete node;
        --count;
    }

    void pop_back() {
        if (tail == nullptr) {
            return;
        }

        Node* node = tail;
        tail = tail->prev;

        if (tail == nullptr) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }

        delete node;
        --count;
    }

    std::size_t size() const {
        return count;
    }

    void print() const {
        const Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << '\n';
    }
};

template <typename T>
class MyForwardList {
private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;
    std::size_t count;

public:
    MyForwardList() : head(nullptr), tail(nullptr), count(0) {}

    ~MyForwardList() {
        while (head != nullptr) {
            pop_front();
        }
    }

    void push_back(const T& value) {
        Node* node = new Node{value, nullptr};
        if (tail == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++count;
    }

    void pop_front() {
        if (head == nullptr) {
            return;
        }

        Node* node = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }

        delete node;
        --count;
    }

    void pop_back() {
        if (head == nullptr) {
            return;
        }

        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            --count;
            return;
        }

        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
        --count;
    }

    std::size_t size() const {
        return count;
    }

    void print() const {
        const Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << '\n';
    }
};

template <typename T>
class MyArray {
private:
    std::size_t capacity_;
    std::size_t size_;
    T* arr_;

    void resize(std::size_t needed) {
        if (needed <= capacity_) {
            return;
        }
        std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_;
        while (new_capacity < needed) {
            new_capacity *= 2;
        }
        T* data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            data[i] = arr_[i];
        }
        delete[] arr_;
        arr_ = data;
        capacity_ = new_capacity;
    }

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    explicit MyArray(std::size_t cap = 4) : capacity_(cap), size_(0), arr_(new T[cap]) {}

    MyArray(const MyArray& other) : capacity_(other.capacity_), size_(other.size_), arr_(new T[other.capacity_]) {
        for (std::size_t i = 0; i < size_; ++i) {
            arr_[i] = other.arr_[i];
        }
    }

    ~MyArray() {
        delete[] arr_;
    }

    MyArray& operator=(const MyArray& other) {
        if (this == &other) {
            return *this;
        }
        resize(other.capacity_);
        size_ = other.size_;
        for (std::size_t i = 0; i < size_; ++i) {
            arr_[i] = other.arr_[i];
        }
        return *this;
    }

    void add(const T& value) {
        resize(size_ + 1);
        arr_[size_++] = value;
    }

    void push_back(const T& value) {
        add(value);
    }

    void pop_front() {
        if (size_ == 0) {
            return;
        }
        for (std::size_t i = 1; i < size_; ++i) {
            arr_[i - 1] = arr_[i];
        }
        --size_;
    }

    void pop_back() {
        if (size_ == 0) {
            return;
        }
        --size_;
    }

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return arr_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return arr_[index];
    }

    MyArray operator+(const MyArray& other) const {
        MyArray result(size_ + other.size_);
        for (std::size_t i = 0; i < size_; ++i) {
            result.add(arr_[i]);
        }
        for (std::size_t i = 0; i < other.size_; ++i) {
            result.add(other.arr_[i]);
        }
        return result;
    }

    void insertionSort() {
        for (std::size_t i = 1; i < size_; ++i) {
            T key = arr_[i];
            long j = static_cast<long>(i) - 1;
            while (j >= 0 && arr_[j] > key) {
                arr_[j + 1] = arr_[j];
                --j;
            }
            arr_[j + 1] = key;
        }
    }

    void selectionSort() {
        for (std::size_t i = 0; i + 1 < size_; ++i) {
            std::size_t min_index = i;
            for (std::size_t j = i + 1; j < size_; ++j) {
                if (arr_[j] < arr_[min_index]) {
                    min_index = j;
                }
            }
            if (min_index != i) {
                std::swap(arr_[i], arr_[min_index]);
            }
        }
    }

    void bubbleSort() {
        if (size_ < 2) {
            return;
        }
        for (std::size_t i = 0; i + 1 < size_; ++i) {
            for (std::size_t j = 0; j + 1 < size_ - i; ++j) {
                if (arr_[j] > arr_[j + 1]) {
                    std::swap(arr_[j], arr_[j + 1]);
                }
            }
        }
    }

    std::size_t binarySearch(const T& value) const {
        if (size_ == 0) {
            return npos;
        }
        std::size_t left = 0;
        std::size_t right = size_ - 1;
        while (left <= right) {
            std::size_t mid = left + (right - left) / 2;
            if (arr_[mid] == value) {
                return mid;
            }
            if (arr_[mid] < value) {
                left = mid + 1;
            } else {
                if (mid == 0) {
                    break;
                }
                right = mid - 1;
            }
        }
        return npos;
    }

    std::size_t size() const {
        return size_;
    }

    void print() const {
        for (std::size_t i = 0; i < size_; ++i) {
            if (i > 0) {
                std::cout << ' ';
            }
            std::cout << arr_[i];
        }
        std::cout << '\n';
    }
};

template <typename T,  typename Container = MyList<T>>
class MyQueue
{
private:
    Container data;

public:
    MyQueue() = default;
    void push(const T& value) {
        data.push_back(value);
    }
    void pop() {
        if (data.size() > 0) {
            data.pop_front();
        }
    }
    void pop_back() {
        if (data.size() > 0) {
            data.pop_back();
        }
    }
    void print() const {
        data.print();
    }
};

int main(){
    MyQueue<int, MyList<int>> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.print(); 

    q.pop();
    q.print();

    q.pop_back();
    q.print(); 

    

    MyQueue<std::string, MyList<std::string>> q_str;    
    q_str.push("Hello");
    q_str.push("World");
    q_str.push("!");
    q_str.print(); 

    q_str.pop();
    q_str.print();

    q_str.pop_back();
    q_str.print(); 

    MyQueue<int, MyForwardList<int>> q_forward;
    q_forward.push(10);
    q_forward.push(20);
    q_forward.push(30);
    q_forward.print();

    q_forward.pop();
    q_forward.print();

    q_forward.pop_back();
    q_forward.print();

    MyQueue<int, MyArray<int>> q_array;

    q_array.push(100);
    q_array.push(200);
    q_array.push(300);
    q_array.print();
    q_array.pop();
    q_array.print();
    q_array.pop_back();
    q_array.print();
    q_array.pop_back();
    q_array.print();

    
    return 0;
}
