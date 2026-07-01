#include <iostream>

int main() {
    int n;
    std::cout << "Enter integer n: ";
    std::cin >> n;

    char* arr = new char[32];
    for (size_t i = 0; i < 32; i++) { arr[i] = 0; }

    _asm {
            mov eax, n
            mov esi, arr
            mov ecx, 32

        for1_:
            shr eax, 1
            setc dl

            mov[esi], dl
            add esi, 1

            loop for1_
    }

    for (size_t i = 0; i < 32; i++)
    {
        std::cout << (int)arr[31 - i];
    }
    std::cout << std::endl;

    delete[] arr;

    return 0;
}