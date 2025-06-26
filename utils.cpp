#include <iostream>
#include <fstream>
#include <random>
#include "utils.h"

using namespace std;

string readFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        throw runtime_error("Файл не найден: " + filename);
    }
    string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    return content;
}

void writeFile(const string& filename, const string& content) {
    ofstream fout(filename);
    if (!fout) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }
    fout << content;
    cout << "Данные успешно записаны в " << filename << "\n";
}

int getRandom(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}