package main

import (
    "fmt"
	"os"
    "bufio"
    "strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Println("\nВыберите задание:")
		fmt.Println("1 — Возведение в степень по модулю")
		fmt.Println("2 — Расширенный алгоритм Евклида")
		fmt.Println("3 — Обратный элемент по модулю")
		fmt.Println("4 — Шифрование и дешифровка (Шамир)")
		fmt.Println("5 — Решение уравнения 275a + 1456b = 10")
		fmt.Println("6 — Эмуляция атаки на алгоритм Шамира")
		fmt.Println("0 — Выход")
		fmt.Print("\nВаш выбор: ")

		choice, _ := reader.ReadString('\n')
		choice = strings.TrimSpace(choice)

		switch choice {
		case "1": powmod()
		case "2": euclid()
		case "3": inverse()
		case "4": shamir()
		case "5": solveEquation()
		case "6": shamirAttack()
		case "0":
			fmt.Println("Завершение работы.")
			return
		default:
			fmt.Println("Неверный выбор. Попробуйте снова.")
		}
	}
}
