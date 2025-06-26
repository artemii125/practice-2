package main

import "fmt"

func solveEquation() {
	a, b, d := int64(275), int64(1456), int64(10)
	g := gcd(a, b)
	if d%g != 0 {
		fmt.Println("Решений нет (НОД не делит свободный член)")
		return
	}

	x0, y0 := extendedEuclid(a, b)
	var k int64 = d / g
	x := x0 * k
	y := y0 * k

	fmt.Printf("Решение уравнения: a = %d, b = %d\n", x, y)
}

func extendedEuclid(a, b int64) (int64, int64) {
	x0, x1 := int64(1), int64(0)
	y0, y1 := int64(0), int64(1)
	for b != 0 {
		q := a / b
		a, b = b, a%b
		x0, x1 = x1, x0-q*x1
		y0, y1 = y1, y0-q*y1
	}
	return x0, y0
}