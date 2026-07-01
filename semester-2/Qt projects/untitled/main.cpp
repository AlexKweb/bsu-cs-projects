#include <iostream>

using namespace std;

int mul(int& a, int& b){

    int a1 = a;
    int b1 = b;

    int sum = 0;

    while(b1){
        if((b1 & 1) == 1){
            sum+=a1;
        }
        a1 = a1 << 1;
        b1 = b1 >> 1;

    }

    return sum;
}

int main()
{
    int a{2}, b{5};
    cout << "Hello World!" << endl;

    cout << mul(a, b) << std::endl;
    return 0;
}
