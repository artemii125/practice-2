package main

import "fmt"

func SolveFraction(a, b int64) {
	fmt.Printf("Цепная дробь для %d/%d: ", a, b)
	for b != 0 {
		fmt.Printf("%d ", a/b)
		a, b = b, a%b
	}
	fmt.Println()
}
