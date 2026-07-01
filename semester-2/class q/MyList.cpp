#include <cstddef>
#include <iostream>
#include <string>

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
    std::size_t count;

public:
    MyList() : head(nullptr), tail(nullptr), count(0) {}

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

        Node* next_node = temp->next; // [temp] []
        Node* node = new Node{value, temp, next_node};

        
        temp->next = node; // [t] [+] []

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

int main() {
    MyList<int> L;

    L.push_back(10);
    L.push_back(20);
    L.push_front(5);
    L.push_front(1);
    L.insert_after(10, 15);

    L.print();

    L.pop_front();
    L.pop_back();

    L.print();

    std::cout << "size: " << L.size() << '\n';
  
  
    MyList<std::string> L_str;

    L_str.push_back("hello");
    L_str.push_back("world");
    L_str.push_front("first");
    L_str.push_front("second");
    L_str.insert_after("world", "middle");

    L_str.print();

    L_str.pop_front();
    L_str.pop_back();

    L_str.print();

    std::cout << "size: " << L_str.size() << '\n';


  
    return 0;
}
