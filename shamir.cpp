#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include "shamir.h"
#include "math.h"
#include "utils.h"

using namespace std;

void shamir() {
    uint64_t p, Ca, Cb, Da, Db;
    string inputFile = "input.txt";
    string encryptedFile = "encrypted.txt";
    string decryptedFile = "decrypted.txt";

    string message;
    try {
        message = readFile(inputFile);
    } catch (const exception& e) {
        cerr << "Ошибка чтения файла: " << e.what() << endl;
        return;
    }

    int choice;
    cout << "Выберите режим:\n";
    cout << "1 — Ввести параметры вручную\n";
    cout << "2 — Сгенерировать всё автоматически\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    switch(choice) {
        case 1:{
            cout << "Введите простое p (больше 255): ";
            cin >> p;

            if (!isPrime(p) || p <= 255) {
                cerr << "Ошибка: p должно быть простым числом больше 255.\n";
                return;
            }   

            uint64_t phi = p - 1;
            cout << "Введите Ca: ";
            cin >> Ca;

            if (gcd(Ca, phi) != 1) {
                cerr << "Ca не взаимно просто с p-1\n";
                return;
            }
            Da = inversedC(Ca, phi);

            if (Da == -1) {
                cerr << "Обратного элемента для Ca не существует\n";
                return;
            }
            cout << "Введите Cb: ";
            cin >> Cb;

            if (gcd(Cb, phi) != 1) {
                cerr << "Cb не взаимно просто с p-1\n";
                return;
            }
            Db = inversedC(Cb, phi);
            if (Db == -1) {
                cerr << "Обратного элемента для Cb не существует\n";
                return;
            }
            break;
        }
        case 2:{
            //генерация p
            do {p = getRandom(256, 1000);} while (!isPrime(p));
            cout << "Случайное простое p = " << p << endl;

            uint64_t phi = p - 1;

            //генерация ключа Ca
            do {Ca = getRandom(2, phi - 1);} while (gcd(Ca, phi) != 1);

            Da = inversedC(Ca, phi);

            if (Da == -1) {
                cerr << "Ошибка: обратного элемента для Ca не существует\n";
            return;
            }

            //генерация ключа Cb
            do {Cb = getRandom(2, phi - 1);} while (gcd(Cb, phi) != 1);

            Db = inversedC(Cb, phi);

            if (Db == -1) {
                cerr << "Ошибка: обратного элемента для Cb не существует\n";
            return;
            }

            cout << "Секретные ключи сгенерированы:\n";
            cout << "Ca = " << Ca << ", Da = " << Da << endl;
            cout << "Cb = " << Cb << ", Db = " << Db << endl;
            break;
        }
    }
    //шифрование
    stringstream encryptedStream;
    for (char c : message) {
        uint64_t m = static_cast<unsigned char>(c);
        uint64_t x1 = binaryExpansDegree(m, Ca, p);
        uint64_t x2 = binaryExpansDegree(x1, Cb, p);
        encryptedStream << x2 << " ";
    }

    writeFile(encryptedFile, encryptedStream.str());

    //дешифрование
    string encryptedText = readFile(encryptedFile);
    stringstream in(encryptedText);
    stringstream decryptedStream;
    uint64_t x2;

    while (in >> x2) {
        uint64_t x3 = binaryExpansDegree(x2, Da, p);
        uint64_t dec = binaryExpansDegree(x3, Db, p);
        decryptedStream << static_cast<char>(dec);
    }

    writeFile(decryptedFile, decryptedStream.str());

    cout << "Шифрование завершено.\n";
}

//атака на алгоритм Шамира
void shamirAttack() {
    uint64_t p, Ca, Cb, Da, Db;
    cout << "Введите простое p (больше 255): ";
    cin >> p;

    if (!isPrime(p) || p <= 255) {
        cerr << "Ошибка: p должно быть простым числом больше 255.\n";
        return;
    }

    string message;
    try {
        message = readFile("input_1.txt");
    } catch (const exception& e) {
        cerr << "Ошибка чтения input.txt: " << e.what() << endl;
        return;
    }

    uint64_t phi = p - 1;
    cout << "Введите Ca: ";
    cin >> Ca;
    cout << "Введите Cb: ";
    cin >> Cb;

    if (gcd(Ca, phi) != 1 || gcd(Cb, phi) != 1) {
        cerr << "Ошибка: Ca и Cb должны быть взаимно простыми с p - 1.\n";
        return;
    }

    Da = inversedC(Ca, phi);
    Db = inversedC(Cb, phi);

    if (Da == -1 || Db == -1) {
        cerr << "Ошибка: не удалось найти обратный элемент.\n";
        return;
    }

    //предположительно, если ключи совпаают возрастает возможность атаки
    if (Ca == Cb) {
        cout << "\nВНИМАНИЕ: Обнаружено совпадение ключей \n";
        cout << "Это может привести к атаке Евы и искажению сообщения.\n";
    }

    cout << "Ключи: Ca = " << Ca << ", Da = " << Da << " | Cb = " << Cb << ", Db = " << Db << endl;

    cout << "\n[Произошел перехват...]\n";

    stringstream attackedMessage;
    for (char ch : message) {
        uint64_t m = static_cast<unsigned char>(ch);
        uint64_t x1 = binaryExpansDegree(m, Ca, p);
        uint64_t fake = getRandom(1, p - 1); //Ева заменяет сообщение
        uint64_t x2 = binaryExpansDegree(fake, Cb, p); //операция с подмененным сообщением
        //расшифровывает подменённое сообщение
        uint64_t x3 = binaryExpansDegree(x2, Da, p);
        uint64_t m2 = binaryExpansDegree(x3, Db, p);

        attackedMessage << static_cast<char>(m2);
    }

    cout << "Полученное Бобом сообщение после атаки: " << attackedMessage.str() << endl;
}
