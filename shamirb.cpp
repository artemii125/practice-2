#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "shamirb.h"
#include "math.h"
#include "utils.h"

using namespace std;

void shamirb() {
    uint64_t p, Ca, Cb, Da, Db;;
    string inputFile = "input.txt";
    string encryptedFile = "encrypted_b.bin";
    string decryptedFile = "decrypted_b.txt";

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
            cout << "Введите простое число p (модуль), больше 255: ";
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

            long long phi = p - 1;

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

    ofstream fout(encryptedFile, ios::binary);
    if (!fout) {
        cerr << "Ошибка открытия файла для записи.\n";
        return;
    }

    for (char ch : message) {
        uint64_t m = static_cast<unsigned char>(ch);
        uint64_t x1 = binaryExpansDegree(m, Ca, p);
        uint64_t x2 = binaryExpansDegree(x1, Cb, p);
        uint64_t byte = static_cast<uint64_t>(x2);
        fout.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    fout.close();

    ifstream fin(encryptedFile, ios::binary);
    if (!fin) {
        cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    stringstream decrypted;
    uint64_t byte;
    while (fin.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        uint64_t x2 = byte;
        uint64_t x3 = binaryExpansDegree(x2, Da, p);
        uint64_t m2 = binaryExpansDegree(x3, Db, p);
        decrypted << static_cast<char>(m2);
    }
    fin.close();

    writeFile(decryptedFile, decrypted.str());
    cout << "Шифрование завершено. Файл: " << encryptedFile << "\n";
    cout << "Расшифровка завершена. Файл: " << decryptedFile << "\n";
}
void shamirAttack() {
    uint64_t p;
    cout << "Введите простое число p (модуль), больше 255: ";
    cin >> p;

    if (!isPrime(p) || p <= 255) {
        cerr << "Ошибка: p должно быть простым числом больше 255.\n";
        return;
    }

    string message = "ATTACK";
    cout << "\nСообщение: " << message << "\n";

    uint64_t phi = p - 1;
    uint64_t Ca, Cb, Da, Db;

    do {Ca = getRandom(2, phi - 1);} while (gcd(Ca, phi) != 1);
    Da = inversedC(Ca, phi);

    do {Cb = getRandom(2, phi - 1);} while (gcd(Cb, phi) != 1);
    Db = inversedC(Cb, phi);

    cout << "Злоумышленник подделывает передачу!\n";

    vector<uint64_t> x1, x2, x3, m_final;

    for (char ch : message) {
        uint64_t m = static_cast<unsigned char>(ch);
        uint64_t enc1 = binaryExpansDegree(m, Ca, p);
        uint64_t fake = getRandom(1, p - 1);
        uint64_t enc2 = binaryExpansDegree(fake, Cb, p);
        uint64_t dec1 = binaryExpansDegree(enc2, Da, p);
        uint64_t m2 = binaryExpansDegree(dec1, Db, p);

        x1.push_back(enc1);
        x2.push_back(enc2);
        x3.push_back(dec1);
        m_final.push_back(m2);
    }

    cout << "\n[Поток атаки искажен злоумышленником]\n";
    cout << "Итоговое сообщение после атаки: ";
    for (auto c : m_final) cout << static_cast<char>(c);
    cout << "\n";
}