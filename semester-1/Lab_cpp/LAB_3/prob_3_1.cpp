#include <iostream>
#include <cmath>
using namespace std;

// лонг лонг 8 байт 2^1023-1

bool isPrime(long long x) {
    if (x < 2) return false;
    for (long long i = 2; i * i <= x; i++)
        if (x % i == 0)
            return false;
    return true;
}

int main() {
    int a, b;
    cout << "Enter limite of interval [1, 1023] : "<<endl;
    cin >> a >> b;
    for (; a < b; a++)
    {
        if (isPrime(a))
        {
            if(isPrime(pow(2, a) - 1))
            cout << " " << pow(2, a) - 1;
        }
    }
    return 0;
}



