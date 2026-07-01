#include <iostream>

int main() {
    int n;
    std::cout << "Enter integer n: ";
    std::cin >> n;

    int isPrime = 1; 

    _asm {
            mov eax, n

            cmp eax, 1
            jg check_two
            mov isPrime, 0
            jmp done

        check_two :
            cmp eax, 2
            jne check_even
            mov isPrime, 1
            jmp done

        check_even :
            mov edx, 0
            mov ebx, 2
            cdq
            idiv ebx        
            cmp edx, 0
            jne start_loop
            mov isPrime, 0
            jmp done

        start_loop :
            mov ecx, 3      
        loop_begin :
            mov eax, ecx
            imul eax, ecx   
            cmp eax, n
            jg done       

            
            mov eax, n
            cdq
            idiv ecx        
            cmp edx, 0
            jne next_d
            mov isPrime, 0
            jmp done

        next_d :
            add ecx, 2     
            jmp loop_begin

        done :
    }

    if (isPrime)
        std::cout << n << " is prime\n";
    else
        std::cout << n << " is not prime\n";

    return 0;
}
