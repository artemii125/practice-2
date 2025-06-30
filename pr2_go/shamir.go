package main

import (
 "bufio"
 "fmt"
 "os"
 "strconv"
 "strings"
 "time"
 "math/rand"
)

func readFile(filename string) (string, error) {
	data, err := os.ReadFile(filename)
 	if err != nil {
  		return "", err
 	}
 	return string(data), nil
}

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
  		if !IsPrime(uint64(p)) || p <= 255 {
   			fmt.Println("Ошибка: p должно быть простым числом больше 255.")
   			return
  		}
  		phi := p - 1

  		Ca = getInt64("Введите Ca: ")
  		if gcd(uint64(Ca), uint64(phi)) != 1 {
   			fmt.Println("Ca не взаимно просто с p-1")
   			return
  		}
  		Da = inversedC(Ca, phi)
  		if Da == -1 {
   			fmt.Println("Не удалось найти обратный элемент для Ca")
   			return
  		}

  		Cb = getInt64("Введите Cb: ")
  		if gcd(uint64(Cb), uint64(phi)) != 1 {
   			fmt.Println("Cb не взаимно просто с p-1")
   			return
  		}
  		Db = inversedC(Cb, phi)
  		if Db == -1 {
   			fmt.Println("Не удалось найти обратный элемент для Cb")
   			return
  		}

 	case 2:
  		rand.Seed(time.Now().UnixNano())
  		for {
   			p = getRandom(256, 1000)
   			if IsPrime(uint64(p)) {
    			break
   			}
  		}
  		fmt.Println("Случайное простое p =", p)
  		phi := p - 1

  		for {
   			Ca = getRandom(2, phi-1)
   			if gcd(uint64(Ca), uint64(phi)) == 1 {
    			break
   			}
  		}
  		Da = inversedC(Ca, phi)

  		for {
   			Cb = getRandom(2, phi-1)
   			if gcd(uint64(Cb), uint64(phi)) == 1 {
    			break
   			}
  		}
  		Db = inversedC(Cb, phi)

  		fmt.Printf("Ключи:\nCa = %d, Da = %d\nCb = %d, Db = %d\n", Ca, Da, Cb, Db)

 	default:
  		fmt.Println("Неверный выбор")
  		return
 	}

 	//шифрование
 	var encryptedParts []string
 	for _, c := range message {
  		m := int64(c)
  		x1 := BinaryExpansDegree(uint64(m), uint64(Ca), uint64(p))
  		x2 := BinaryExpansDegree(x1, uint64(Cb), uint64(p))
  		encryptedParts = append(encryptedParts, strconv.FormatUint(x2, 10))
 	}
 	os.WriteFile(encryptedFile, []byte(strings.Join(encryptedParts, " ")), 0644)

 //дешифрование
 	raw, err := os.ReadFile(encryptedFile)
 	if err != nil {
  		fmt.Println("Ошибка чтения зашифрованного файла:", err)
  		return
 	}
 	tokens := strings.Fields(string(raw))
 	var decrypted strings.Builder
 	for _, token := range tokens {
  		x2, _ := strconv.ParseUint(token, 10, 64)
  		x3 := BinaryExpansDegree(x2, uint64(Da), uint64(p))
  		m2 := BinaryExpansDegree(x3, uint64(Db), uint64(p))
  		decrypted.WriteByte(byte(m2))
 	}
 	os.WriteFile(decryptedFile, []byte(decrypted.String()), 0644)

 	fmt.Println("Шифрование завершено.")
 	fmt.Println("Зашифрованный текст:", encryptedFile)
 	fmt.Println("Расшифрованный текст:", decryptedFile)
}

func shamirAttack(){
	rand.Seed(time.Now().UnixNano())

	p := getInt64("Введите простое число p (модуль), больше 255: ")
 	if !IsPrime(uint64(p)) || p <= 255 {
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

 	if gcd(uint64(Ca), uint64(phi)) != 1 || gcd(uint64(Cb), uint64(phi)) != 1 {
  		fmt.Println("Ошибка: Ca и Cb должны быть взаимно простыми с p - 1.")
  		return
 	}

 	Da := inversedC(Ca, phi)
 	Db := inversedC(Cb, phi)
 	if Da == -1 || Db == -1 {
  		fmt.Println("Ошибка: не удалось найти обратный элемент.")
  		return
 }

 	if Ca == Cb {
  		fmt.Println("ВНИМАНИЕ: Обнаружено совпадение ключей Ca = Cb")
  		fmt.Println("Это может позволить атакующему изменить сообщение.")
 	}

 	fmt.Printf("Ключи: Ca = %d, Da = %d | Cb = %d, Db = %d\n", Ca, Da, Cb, Db)
 	fmt.Println("\n[Атака MITM началась: злоумышленник перехватывает сообщение...]\n")

 	var attackedMessage strings.Builder
 	for _, ch := range message {
  		m := int64(ch)
  		x1 := BinaryExpansDegree(uint64(m), uint64(Ca), uint64(p))
  		_ = x1 //теоретический перехват

  		fake := getRandom(1, 254)
  		x2 := BinaryExpansDegree(uint64(fake), uint64(Cb), uint64(p))
  		x3 := BinaryExpansDegree(x2, uint64(Da), uint64(p))
  		m2 := BinaryExpansDegree(x3, uint64(Db), uint64(p))

  		attackedMessage.WriteByte(byte(m2))
 	}

 	fmt.Println("Полученное Бобом сообщение после атаки:")
 	fmt.Println(attackedMessage.String())
}