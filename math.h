#ifndef MATH_H
#define MATH_H
#include <cstdint>

uint64_t gcd(uint64_t a, uint64_t b);
bool relativelyPrime(uint64_t a, uint64_t b);
bool isPrime(uint64_t n);
bool fermatControl(uint64_t a, uint64_t x, uint64_t p);
uint64_t degreeSimplify(uint64_t x, uint64_t p);
uint64_t degreeCalc(uint64_t a, uint64_t x, uint64_t p);
uint64_t fermatDegree(uint64_t a, uint64_t reducedX, uint64_t p);
uint64_t binaryExpansDegree(uint64_t a, uint64_t x, uint64_t p);
uint64_t inversedC(int64_t c, int64_t m);
void powmod();
void euclid();
void inverse();

#endif
