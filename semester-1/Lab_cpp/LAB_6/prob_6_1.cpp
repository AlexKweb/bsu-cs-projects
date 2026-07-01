#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// Получить палиндром из числа путем перестановки цифр (если возможно)
// Использовать статический массив размерности 10

bool canFormPalindrome(int digits[10], int count) {
    int oddCount = 0;
    for (int i = 0; i < 10; i++) {
        if (digits[i] % 2 != 0) {
            oddCount++;
        }
    }
    return oddCount <= 1;
}

long long formPalindrome(int digits[10], int count) {
    int result[count];
    int left = 0, right = count - 1;
    int middle = -1;
    
    for (int i = 0; i < 10; i++) {
        if (digits[i] % 2 == 1) {
            middle = i;
            digits[i]--;
            break;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        while (digits[i] >= 2) {
            result[left++] = i;
            result[right--] = i;
            digits[i] -= 2;
        }
    }
    
    if (middle != -1) {
        result[left] = middle;
    }
    
    long long palindrome = 0;
    for (int i = 0; i < count; i++) {
        palindrome = palindrome * 10 + result[i];
    }
    
    return palindrome;
}

int main() {
    long long number;
    cout << "Введите натуральное число: ";
    cin >> number;
    
    if (number <= 0) {
        cout << "Ошибка: введите натуральное число!" << endl;
        return 1;
    }
    
    // Статический массив размерности 10 для подсчета цифр
    int digits[10] = {0};
    int count = 0;
    long long temp = number;
    
    while (temp > 0) {
        digits[temp % 10]++;
        temp /= 10;
        count++;
    }
    
    if (canFormPalindrome(digits, count)) {
        long long palindrome = formPalindrome(digits, count);
        cout << "Палиндром: " << palindrome << endl;
    } else {
        cout << "Невозможно получить палиндром из данного числа" << endl;
    }
    
    return 0;
}
