#include <iostream> 
#include <cstdint>
#include <stdexcept>
#include <vector>
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
    if (n % 2 == 0 && n != 2) return false;
    for (uint64_t i = 3; i * i <= n; i += 2){
        if (n % i == 0) return false;
    }

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

//решение линейного уравнения
vector<int64_t> solveDiophantine(int64_t a, int64_t b, int64_t d) {
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    int64_t aa = a, bb = b;

    //алгоритм расширенного Евклида
    while (bb != 0) {
        int64_t q = aa / bb;
        int64_t r = aa % bb;
        int64_t x2 = x0 - q * x1;
        int64_t y2 = y0 - q * y1;

        aa = bb;
        bb = r;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }

    if (d % aa != 0) {
        return {}; 
    }

    int64_t k = d / aa;
    return { x0 * k, y0 * k };
}

//возведение в степень по модулю 
void powmod() {
    uint64_t a, x, p;
    cout << "a^x mod p\n";
    cout << "Введите a, x и p: ";
    cin >> a >> x >> p;

    uint64_t reducedX = degreeCalc(a, x, p);
    if (reducedX == -1) return;

    cout << "Вычисление через теорему Ферма: " << a << "^" << x << " mod " << p << " = " << binaryExpansDegree(a, reducedX, p) << endl;
    cout << "Вычисление через логарифм: " << a << "^" << x << " mod " << p << " = " << binaryExpansDegree(a, reducedX, p) << endl;
}

//алгоритм Евклида
void euclid() {
    int64_t a, b;
    cout << "Введите два числа a и b: ";
    cin >> a >> b;
    
    int64_t r0 = a, r1 = b;
    int64_t u0 = 1, u1 = 0;
    int64_t v0 = 0, v1 = 1;

    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;
        int64_t u2 = u0 - q * u1;
        int64_t v2 = v0 - q * v1;
        r0 = r1; r1 = r2;
        u0 = u1; u1 = u2;
        v0 = v1; v1 = v2;
    }

    cout << "НОД = " << r0 << ", u = " << u0 << ", v = " << v0 << endl;
}

//вычисление взаимнообратного числа
void inverse() {
    uint64_t c, m;
    cout << "Введите число (c) и модуль (m): ";
    cin >> c >> m;

    if (inversedC(c, m) == -1)
        cout << "Обратного элемента не существует.\n";
    else
        cout << "Обратный элемент: " << c << "^-1" << " mod " << m << " = " << inversedC(c, m) << endl;
}

//вычисление цепной дроби
void solveEquation() {
    int64_t a = 275, b = 145, d = 10;
    vector<int64_t> result = solveDiophantine(a, b, d);

    if (result.empty()) {
        cout << "Решений нет (НОД не делит d).\n";
    } else {
        cout << "Решение уравнения: a = " << result[0] << ", b = " << result[1] << endl;
    }
}