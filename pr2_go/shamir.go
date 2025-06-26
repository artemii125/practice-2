package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"
)

func getRandom(min, max int64) int64 {
	return rand.Int63n(max-min+1) + min
}

func getInt64(prompt string) int64 {
	fmt.Print(prompt)
	reader := bufio.NewReader(os.Stdin)
	text, _ := reader.ReadString('\n')
	text = strings.TrimSpace(text)
	n, _ := strconv.ParseInt(text, 10, 64)
	return n
}

func readFile(filename string) (string, error) {
	data, err := os.ReadFile(filename)
	if err != nil {
		return "", err
	}
	return string(data), nil
}

func shamir() {
	var p, Ca, Cb, Da, Db int64
	inputFile := "input.txt"
	encryptedFile := "encrypted.txt"
	decryptedFile := "decrypted.txt"

	data, err := os.ReadFile(inputFile)
	if err != nil {
		fmt.Println("Ошибка чтения файла:", err)
		return
	}
	message := string(data)

	var choice int
	fmt.Println("Выберите режим:")
	fmt.Println("1 — Ввести параметры вручную")
	fmt.Println("2 — Сгенерировать всё автоматически")
	fmt.Print("Ваш выбор: ")
	fmt.Scan(&choice)

	switch choice {
	case 1:
		p = getInt64("Введите простое число p (модуль), больше 255: ")
		if !IsPrime(p) || p <= 255 {
			fmt.Println("Ошибка: p должно быть простым числом больше 255.")
			return
		}
		phi := p - 1

		Ca = getInt64("Введите Ca: ")
		if gcd(Ca, phi) != 1 {
			fmt.Println("Ca не взаимно просто с p-1")
			return
		}
		Da, err = inversedC(Ca, phi)
		if err != nil {
			fmt.Println(err)
			return
		}

		Cb = getInt64("Введите Cb: ")
		if gcd(Cb, phi) != 1 {
			fmt.Println("Cb не взаимно просто с p-1")
			return
		}
		Db, err = inversedC(Cb, phi)
		if err != nil {
			fmt.Println(err)
			return
		}

	case 2:
		for {
			p = getRandom(256, 1000)
			if IsPrime(p) {
				break
			}
		}
		fmt.Println("Случайное простое p =", p)
		phi := p - 1

		for {
			Ca = getRandom(2, phi-1)
			if gcd(Ca, phi) == 1 {
				break
			}
		}
		Da, _ = inversedC(Ca, phi)

		for {
			Cb = getRandom(2, phi-1)
			if gcd(Cb, phi) == 1 {
				break
			}
		}
		Db, _ = inversedC(Cb, phi)

		fmt.Printf("Секретные ключи сгенерированы:\nCa = %d, Da = %d\nCb = %d, Db = %d\n", Ca, Da, Cb, Db)

	default:
		fmt.Println("Неверный выбор")
		return
	}

	// Шифрование
	var encryptedParts []string
	for _, c := range message {
		m := int64(c)
		x1 := binaryExpansDegree(m, Ca, p)
		x2 := binaryExpansDegree(x1, Cb, p)
		encryptedParts = append(encryptedParts, strconv.FormatInt(x2, 10))
	}
	os.WriteFile(encryptedFile, []byte(strings.Join(encryptedParts, " ")), 0644)

	// Дешифрование
	raw, err := os.ReadFile(encryptedFile)
	if err != nil {
		fmt.Println("Ошибка чтения зашифрованного файла:", err)
		return
	}
	tokens := strings.Fields(string(raw))
	var decrypted strings.Builder
	for _, token := range tokens {
		x2, _ := strconv.ParseInt(token, 10, 64)
		x3 := binaryExpansDegree(x2, Da, p)
		m2 := binaryExpansDegree(x3, Db, p)
		decrypted.WriteByte(byte(m2))
	}
	os.WriteFile(decryptedFile, []byte(decrypted.String()), 0644)

	fmt.Println("Шифрование завершено.")
	fmt.Println("Зашифрованный текст:", encryptedFile)
	fmt.Println("Расшифрованный текст:", decryptedFile)
}

func shamirAttack() {
	rand.Seed(time.Now().UnixNano())

	p := getInt64("Введите простое число p (модуль), больше 255: ")
	if !IsPrime(p) || p <= 255 {
		fmt.Println("Ошибка: p должно быть простым числом больше 255.")
		return
	}

	message, err := readFile("input1.txt")
	if err != nil {
		fmt.Println("Ошибка чтения input1.txt:", err)
		return
	}

	phi := p - 1

	Ca := getInt64("Введите Ca: ")
	Cb := getInt64("Введите Cb: ")

	if gcd(Ca, phi) != 1 || gcd(Cb, phi) != 1 {
		fmt.Println("Ошибка: Ca и Cb должны быть взаимно простыми с p - 1.")
		return
	}

	Da, err1 := inversedC(Ca, phi)
	Db, err2 := inversedC(Cb, phi)
	if err1 != nil || err2 != nil {
		fmt.Println("Ошибка: не удалось найти обратный элемент.")
		return
	}

	if Ca == Cb {
		fmt.Println("ВНИМАНИЕ: Обнаружено совпадение ключей")
		fmt.Println("Это может привести к атаке Евы и искажению сообщения.")
	}

	fmt.Printf("Ключи: Ca = %d, Da = %d | Cb = %d, Db = %d\n", Ca, Da, Cb, Db)
	fmt.Println("\n[Произошел перехват...]\n")

	var attackedMessage strings.Builder
	for _, ch := range message {
		m := int64(ch)
		x1 := binaryExpansDegree(m, Ca, p)
		_ = x1
		fake := getRandom(1, 254)
		x2 := binaryExpansDegree(fake, Cb, p)
		x3 := binaryExpansDegree(x2, Da, p)
		m2 := binaryExpansDegree(x3, Db, p)

		attackedMessage.WriteByte(byte(m2))
	}

	fmt.Println("Полученное Бобом сообщение после атаки:", attackedMessage.String())
}
