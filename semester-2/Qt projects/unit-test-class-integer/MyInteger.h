#ifndef MYINTEGER_H
#define MYINTEGER_H

#include <QVector>

class MyInteger {
private:
    int a;
    int b;

public:
    MyInteger() : a(0), b(0) {}

    MyInteger(int first, int second) : a(first), b(second) {}

    int getA() const {
        return a;
    }

    int getB() const {
        return b;
    }

    void setA(int value) {
        a = value;
    }

    void setB(int value) {
        b = value;
    }

    void setValues(int first, int second) {
        a = first;
        b = second;
    }

    bool isPrimeA() const {
        return isPrime(a);
    }

    bool isPrimeB() const {
        return isPrime(b);
    }

    QVector<int> divisorsA() const {
        return primeFactors(a);
    }

    QVector<int> divisorsB() const {
        return primeFactors(b);
    }

    long long nod() const {
        long long first = absValue(a);
        long long second = absValue(b);

        while (second != 0) {
            long long temp = second;
            second = first % second;
            first = temp;
        }

        return first;
    }

    long long nok() const {
        if (a == 0 || b == 0) {
            return 0;
        }

        return absValue(static_cast<long long>(a) / nod() * b);
    }

private:
    long long absValue(long long value) const {
        if (value < 0) {
            return -value;
        }

        return value;
    }

    bool isPrime(int value) const {
        value = absValue(value);

        if (value < 2) {
            return false;
        }

        for (long long divisor = 2; divisor * divisor <= value; ++divisor) {
            if (value % divisor == 0) {
                return false;
            }
        }

        return true;
    }

    QVector<int> primeFactors(int value) const {
        QVector<int> factors;
        long long number = value;

        number = absValue(number);

        if (number < 2) {
            return factors;
        }

        for (long long divisor = 2; divisor * divisor <= number; ++divisor) {
            while (number % divisor == 0) {
                factors.push_back(static_cast<int>(divisor));
                number /= divisor;
            }
        }

        if (number > 1) {
            factors.push_back(static_cast<int>(number));
        }

        return factors;
    }
};

#endif // MYINTEGER_H
