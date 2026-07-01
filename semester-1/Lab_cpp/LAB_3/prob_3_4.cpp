#include <iostream>
#include <cstdarg>
using namespace std;

//  0  сумма,  1 произведение
int calc(int type, int n, ...) {
    va_list args;
    va_start(args, n);

    int result;
    if (type == 0) result = 0; 
    else result = 1;          

    for (int i = 0; i < n; i++) {
        int val = va_arg(args, int);
        if (type == 0) result += val;
        else result *= val;
    }

    va_end(args);
    return result;
}

int main() {
    cout << "\t==> "; 
    cout << "sum 3+5+7 = " << calc(0, 3, 3, 5, 7) << endl;
    cout << "Произведение 2*4*6 = " << calc(1, 3, 2, 4, 6) << endl;
    return 0;
}


