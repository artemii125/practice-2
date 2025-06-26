#include <iostream>
#include "math.h"
#include "shamir.h"
#include "frac.h"
#include "utils.h"

using namespace std;

int main() {
    int choice;

    cout << "Меню:\n";
    cout << "1 — Возведение в степень по модулю (Ферма + логарифм)\n";
    cout << "2 — Расширенный алгоритм Евклида (u и v)\n";
    cout << "3 — Обратный элемент по модулю\n";
    cout << "4 — Шифрование по алгоритму Шамира\n";
    cout << "5 — Решение уравнения 275a + 1456b = 10\n";
    cout << "6 — Эмуляция атаки на алгоритм Шамира\n";
    cout << "0 — Выход из программы\n";

    while (true) {
        cout << "\nВаш выбор: ";
        cin >> choice;

        switch (choice) {
            case 1: powmod(); break;
            case 2: euclid(); break;
            case 3: inverse(); break;
            case 4: shamir(); break;
            case 5: solveEquation(); break;
            case 6: shamirAttack(); break;
            case 0:
                cout << "Завершение работы.\n";
                return false;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}
