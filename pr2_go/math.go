package main

import (
	"errors"
	"fmt"
)

func gcd(a, b int64) int64 {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func IsPrime(n int64) bool {
	if n < 2 {
		return false
	}
	for i := int64(2); i*i <= n; i++ {
		if n%i == 0 {
			return false
		}
	}
	return true
}

func inversedC(a, m int64) (int64, error) {
	t, newt := int64(0), int64(1)
	r, newr := m, a

	for newr != 0 {
		q := r / newr
		t, newt = newt, t-q*newt
		r, newr = newr, r-q*newr
	}
	if r > 1 {
		return 0, errors.New("обратного элемента не существует")
	}
	if t < 0 {
		t += m
	}
	return t, nil
}

func binaryExpansDegree(base, exp, mod int64) int64 {
	res := int64(1)
	base %= mod
	for exp > 0 {
		if exp%2 == 1 {
			res = (res * base) % mod
		}
		base = (base * base) % mod
		exp /= 2
	}
	return res
}

func powmod() {
	var a, x, p int64
	fmt.Print("Введите a, x и p: ")
	fmt.Scan(&a, &x, &p)

	if !IsPrime(p) || a%p == 0 || a >= p {
		fmt.Println("Условия теоремы Ферма не выполнены")
		return
	}

	reduced := x % (p - 1)
	fmt.Println("Вычисление через теорему Ферма:", binaryExpansDegree(a, reduced, p))
	fmt.Println("Вычисление через логарифм:", binaryExpansDegree(a, reduced, p))
}

func euclid() {
	var a, b int64
	fmt.Print("Введите два числа: ")
	fmt.Scan(&a, &b)

	r0, r1 := a, b
	u0, u1 := int64(1), int64(0)
	v0, v1 := int64(0), int64(1)

	for r1 != 0 {
		q := int64(r0 / r1)
		r0, r1 = r1, r0-q*r1
		u0, u1 = u1, u0-q*u1
		v0, v1 = v1, v0-q*v1
	}
	fmt.Printf("НОД = %d, u = %d, v = %d\n", r0, u0, v0)
}

func inverse() {
	var c, m int64
	fmt.Print("Введите число и модуль: ")
	fmt.Scan(&c, &m)

	inv, err := inversedC(c, m)
	if err != nil {
		fmt.Println("Обратного элемента не существует.")
	} else {
		fmt.Println("Обратный элемент:", inv)
	}
}
