package main

import (
 "errors"
 "fmt"
)

//gcd — вычисление НОД
func gcd(a, b uint64) uint64 {
 for b != 0 {
  a, b = b, a%b
 }
 return a
}

//проверка взаимной простоты
func RelativelyPrime(a, b uint64) bool {
 return gcd(a, b) == 1
}

//проверка на простоту
func IsPrime(n uint64) bool {
 if n < 2 {
  return false
 }
 if n%2 == 0 && n != 2 {
  return false
 }
 for i := uint64(3); i*i <= n; i += 2 {
  if n%i == 0 {
   return false
  }
 }
 return true
}

//проверка условий теоремы Ферма
func FermatControl(a, x, p uint64) error {
 if !IsPrime(p) {
  return errors.New("модуль не простой")
 } else if a < 1 {
  return errors.New("число должно быть больше либо равно 1")
 } else if a%p == 0 {
  return errors.New("число не должно быть кратно модулю")
 }
 return nil
}

//проверка условий алгоритма Евклида
func EuclidControl(a, p uint64) error {
 if a == 0 || p == 0 {
  return errors.New("оба числа должны быть больше 0")
 }
 return nil
}

//упрощение степени по модулю
func DegreeSimplify(x, p uint64) uint64 {
 return x % (p - 1)
}

//расчёт степени с учётом теоремы Ферма
func DegreeCalc(a, x, p uint64) (uint64, error) {
 err := FermatControl(a, x, p)
 if err != nil {
  return 0, err
 }

 if x == p-1 {
  return 1, nil
 }
 reduced := DegreeSimplify(x, p)
 if reduced >= 1000 {
  fmt.Println("Вычисление по Ферма будет затруднительным, практичнее через логарифм!!!!!")
 }
 return reduced, nil
}

//возведение в степень по модулю через логарифм
func BinaryExpansDegree(a, x, p uint64) uint64 {
 result := uint64(1)
 a %= p
 for x > 0 {
  if x%2 == 1 {
   result = (result * a) % p
  }
  a = (a * a) % p
  x /= 2
 }
 return result
}

//обратный элемент по модулю
func inversedC(c, m int64) int64 {
 a, b := m, c
 z0, z1 := int64(0), int64(1)

 for b != 0 {
  q := a / b
  r := a % b
  z := z0 - q*z1
  a, b = b, r
  z0, z1 = z1, z
 }

 if a != 1 {
  return -1
 }
 if z0 < 0 {
  z0 += m
 }
 return z0
}

//решение линейного уравнения
func SolveDiophantine(a, b, d int64) []int64 {
 x0, y0 := int64(1), int64(0)
 x1, y1 := int64(0), int64(1)
 aa, bb := a, b

 for bb != 0 {
  q := aa / bb
  r := aa % bb
  x2 := x0 - q*x1
  y2 := y0 - q*y1
  aa, bb = bb, r
  x0, x1 = x1, x2
  y0, y1 = y1, y2
 }

 if d%aa != 0 {
  return nil
 }
 k := d / aa
 return []int64{x0 * k, y0 * k}
}

func powmod() {
    var a, x, p uint64
    fmt.Print("Введите a, x и p: ")
    fmt.Scan(&a, &x, &p)

    reduced, err := DegreeCalc(a, x, p)
    if err != nil {
        fmt.Println("Ошибка:", err)
        return
    }

    result := BinaryExpansDegree(a, reduced, p)
    fmt.Printf("%d^%d mod %d = %d\n", a, x, p, result)
}

func euclid() {
    var a, b int64
    fmt.Print("Введите два числа a и b: ")
    fmt.Scan(&a, &b)

    r0, r1 := a, b
    u0, u1 := int64(1), int64(0)
    v0, v1 := int64(0), int64(1)

    for r1 != 0 {
        q := r0 / r1
        r2 := r0 - q*r1
        u2 := u0 - q*u1
        v2 := v0 - q*v1
        r0, r1 = r1, r2
        u0, u1 = u1, u2
        v0, v1 = v1, v2
    }

    fmt.Printf("НОД = %d, u = %d, v = %d\n", r0, u0, v0)
}

func inverse() {
    var c, m int64
    fmt.Print("Введите число (c) и модуль (m): ")
    fmt.Scan(&c, &m)

    inv := inversedC(c, m)
    if inv == -1 {
        fmt.Println("Обратного элемента не существует.")
    } else {
        fmt.Printf("Обратный элемент: %d^(-1) mod %d = %d\n", c, m, inv)
    }
}

func solveEquation() {
    var a, b, d int64
    fmt.Print("Введите a, b и d: ")
    fmt.Scan(&a, &b, &d)

    result := SolveDiophantine(a, b, d)
    if result == nil {
        fmt.Println("Решений нет (НОД не делит d).")
    } else {
        fmt.Printf("Решение уравнения: x = %d, y = %d\n", result[0], result[1])
    }
}