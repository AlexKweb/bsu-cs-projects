#include <iostream>
#include "stack.h"

int main() {
    Stack<int> s(3);

    try {
        s.push(1);
        s.print();

        s.push(2);
        s.print();

        s.push(3);
        s.print();

        std::cout << "Popped: " << s.pop() << std::endl;
        s.print();

        s.push(4);
        s.print();

        std::cout << "Popped: " << s.pop() << std::endl;
        s.print();

        std::cout << "Popped: " << s.pop() << std::endl;
        s.print();

        std::cout << "Popped: " << s.pop() << std::endl;
        s.print();

        std::cout << "Popped: " << s.pop() << std::endl; 
        s.print();
    } catch (const Exception& ex) {
        std::cout << "Caught Exception: " << ex.message() << std::endl;
    }

    return 0;
}
