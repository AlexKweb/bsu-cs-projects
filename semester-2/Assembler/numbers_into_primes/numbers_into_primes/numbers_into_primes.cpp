#include <iostream>

void print_factor();


int main() {
    int n;
    std::cout << "Enter positive integer n: ";
    std::cin >> n;

    std::cout << n << " = ";

    _asm {
            mov eax, n
            
            mov ebx, 2
            cmp eax, 1
            jle done

        for1_:
            push eax
            cdq
            div ebx
        if1_:
            cmp edx, 0 
            je else1_
            pop eax
            inc ebx
            jmp for1_
        else1_:
            pop ecx       
            push eax      
            call print_factor
            pop eax   
            cmp eax, 1  
            jne for1_     

            
        done :
    }

    std::cout << std::endl;
    return 0;
}

void print_factor() {
    int f;
    {
        _asm mov f, ebx
    }
    std::cout << f << "*";
}
