#include <iostream>

using namespace std;

int mul(int& a, int& b) {
    int a1 = a;
    int b1 = b;
    int sum = 0;

    _asm {
        mov eax, a1
        mov ebx, b1
        xor ecx, ecx

        while_loop :
            cmp ebx, 0
            jz done
            mov edx, ebx
            and edx, 1
            cmp edx, 0
            je skip
            add ecx, eax
        skip :
            shr ebx, 1
            shl eax, 1
            jmp while_loop

        done :
            mov sum, ecx
    }

    return sum;
}

int main() {
    int a{ 2 }, b{ 5 };
    cout << "Hello World!" << endl;

    cout << mul(a, b) << std::endl;
    return 0;
}