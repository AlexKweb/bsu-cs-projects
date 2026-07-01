#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    int n;
    // cin >> n;
    
    vector<int> a = 
     {2,33,4,6,22,1,1,33,4,6,4,72,43,6,99};
    // {2, 5, 8, 11, 14};
    // {3, 6, 12, 24, 48};
    // {7, 7, 7, 7, 7};


    // for (int i = 0; i < n; i++) {
    //     cin >> a[i];
    // }
    
    bool isArithmetic = true;
    bool isGeometric = true;
    int difference = 0;
    double ratio = 0;
    
    if (n > 1) {
        difference = a[1] - a[0];
        for (int i = 2; i < n; i++) {
            if (a[i] - a[i-1] != difference) {
                isArithmetic = false;
                break;
            }
        }
        
        if (a[0] != 0) {
            ratio = static_cast<double>(a[1]) / a[0];
            for (int i = 2; i < n; i++) {
                if (a[i-1] == 0 || fabs(static_cast<double>(a[i]) / a[i-1] - ratio) > 1e-10) {
                    isGeometric = false;
                    break;
                }
            }
        } else {
            isGeometric = false;
            for (int i = 1; i < n; i++) {
                if (a[i] != 0) {
                    isGeometric = false;
                    break;
                }
            }
            if (isGeometric) ratio = 0;
        }
    } else {
        isArithmetic = false;
        isGeometric = false;
    }
    
    if (isArithmetic && isGeometric) {
        cout << "Arifmetic (d=" << difference << "), Geometric (q=" << ratio << ")" << endl;
    } else if (isArithmetic) {
        cout << "Arifmetic, d= " << difference << endl;
    } else if (isGeometric) {
        cout << "Geometric, q= " << ratio << endl;
    } else {
        cout << "Not" << endl;
    }
    
    return 0;
}
