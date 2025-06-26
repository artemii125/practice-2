#include <iostream> 
#include <cstdint>
#include <stdexcept>
#include "math.h"

using namespace std;

//вычисление НОД
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

//проверка взаимной простоты
bool relativelyPrime(uint64_t a, uint64_t b) {
    return gcd(a, b) == 1;
}

//проверка числа на простоту
bool isPrime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;

    return true;
}

//проверка условий теоремы Ферма
bool fermatControl(uint64_t a, uint64_t x, uint64_t p) {
    if (!isPrime(p)) throw invalid_argument("Модуль не проcтой");
    else if (a < 1) throw invalid_argument("Число должно быть больше либо равно 1");
    else if (a % p == 0) throw invalid_argument("Число не должно быть кратно модулю");

    return true;
}

//проверка условий алгоритма Евклида
bool euclidControl(uint64_t a, uint64_t p) {
    if (a <= 0) throw invalid_argument("Число должно быть больше 0");
    else if (p <= 0) throw invalid_argument("Модуль должен быть больше 0");

    return true;
}

//упрощение степени по модулю
uint64_t degreeSimplify(uint64_t x, uint64_t p) {
    return x % (p - 1);
}

//расчёт степени с учётом теоремы Ферма
uint64_t degreeCalc(uint64_t a, uint64_t x, uint64_t p) {
    uint64_t reducedX = x;

    if (fermatControl(a, x, p)) {
        if (x == p - 1) return 1;
        reducedX = degreeSimplify(x, p);
    } else {
        throw invalid_argument("Условия теоремы Ферма не выполнены");
    }
    if (reducedX >= 1000) { 
        cout << "Вычисление по Ферма будет затуднительным, практичнее через логарифм!!!!!";
    }
    return reducedX;
}

//возведение в степень по модулю через Ферма
uint64_t fermatDegree(uint64_t a, uint64_t reducedX, uint64_t p) {
    uint64_t res = 1;
    for (uint64_t i = 0; i < reducedX; ++i)
        res = (res * a) % p;

    return res;
}

//возведение в степень по модулю через логарифм
uint64_t binaryExpansDegree(uint64_t a, uint64_t x, uint64_t p) {
    uint64_t res = 1;
    a %= p;
    while (x > 0) {
        if (x % 2 == 1) res = (res * a) % p;
        a = (a * a) % p;       
        x /= 2;
    }
    return res;
}

//нахождение обратного элемента по модулю 
uint64_t inversedC(int64_t c, int64_t m) {
    int64_t a = m, b = c;
    int64_t z0 = 0, z1 = 1;


    while (b != 0) {
        int64_t q = a / b;
        int64_t r = a % b;
        int64_t z = z0 - q * z1;
        a = b; b = r;
        z0 = z1; z1 = z;
    }

    if (a != 1) return -1;
    if (z0 < 0) z0 += m;

    return z0;
    
}

//возведение в степень по модулю 
void powmod() {
    uint64_t a, x, p;
    cout << "a^x mod p\n";
    cout << "Введите a, x и p: ";
    cin >> a >> x >> p;

    uint64_t reducedX = degreeCalc(a, x, p);
    if (reducedX == -1) return;

    cout << "Вычисление через теорему Ферма: " << binaryExpansDegree(a, reducedX, p) << endl;
    cout << "Вычисление через логарифм: " << binaryExpansDegree(a, reducedX, p) << endl;
}

//алгоритм Евклида
void euclid() {
    uint64_t a, b;
    cout << "Введите два числа a и b: ";
    cin >> a >> b;
    
    uint64_t r0 = a, r1 = b;
    uint64_t u0 = 1, u1 = 0;
    uint64_t v0 = 0, v1 = 1;

    while (r1 != 0) {
        uint64_t q = r0 / r1;
        uint64_t r2 = r0 - q * r1;
        uint64_t u2 = u0 - q * u1;
        uint64_t v2 = v0 - q * v1;
        r0 = r1; r1 = r2;
        u0 = u1; u1 = u2;
        v0 = v1; v1 = v2;
    }

    cout << "НОД = " << r0 << ", u = " << u0 << ", v = " << v0 << endl;
}

//вычисление взаимнообратного числа
void inverse() {
    uint64_t c, m;
    cout << "Введите число и модуль: ";
    cin >> c >> m;

    uint64_t inv = inversedC(c, m);
    if (inv == -1)
        cout << "Обратного элемента не существует.\n";
    else
        cout << "Обратный элемент: " << inv << endl;
}