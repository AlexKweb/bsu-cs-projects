#include <iostream>
#include "queue.h"

int main() {
    ArrayQueue q(3);

    try {
        q.push_back(1);
        q.print();

        q.push_back(2);
        q.print();

        std::cout << "Popped: " << q.pop_front() << std::endl;
        q.print();

        q.push_back(3);
        q.print();

        q.push_back(4);
        q.print();
        q.push_back(6);
        q.print();

        std::cout << "Popped: " << q.pop_front() << std::endl;
        q.print();

        q.push_back(5);
        q.print();

        std::cout << "Popped: " << q.pop_front() << std::endl;
        q.print();

        std::cout << "Popped: " << q.pop_front() << std::endl;
        q.print();

        std::cout << "Popped: " << q.pop_front() << std::endl;
        q.print();  
    } catch (const Full_ex& ex) {
        std::cout << "Caught Full_ex: " << ex.what() << std::endl;
    } catch (const Empty_ex& ex) {
        std::cout << "Caught Empty_ex: " << ex.what() << std::endl;
    }

    return 0;
}
