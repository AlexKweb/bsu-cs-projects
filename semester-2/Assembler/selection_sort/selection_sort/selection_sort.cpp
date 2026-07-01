#include <iostream>

int main() {
    int* arr = new int[6] { 12, 43, 32, 12, 21, 1 };

    _asm {
            mov ecx, 5
            mov edi, arr

        for1_ :
            mov eax, [edi]
            mov edx, edi

            mov esi, edi
            add esi, 4

            mov ebx, ecx

        for2_ :
            cmp[esi], eax
            jge skip

            mov eax, [esi]
            mov edx, esi

        skip :
            add esi, 4
            dec ebx
            cmp ebx, 0
            jne for2_

            mov ebx, [edi]
            mov[edi], eax
            mov[edx], ebx

            add edi, 4
            loop for1_
    }

    for (size_t i = 0; i < 6; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;

    return 0;
}