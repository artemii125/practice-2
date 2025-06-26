#include "frac.h"
#include <iostream>
#include <vector>

using namespace std;

//решение линейного уравнения
vector<int64_t> solveEquation(int64_t a, int64_t b, int64_t d) {
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    int64_t aa = a, bb = b;

    //алгоритм расширенного Евклида
    while (bb != 0) {
        int64_t q = aa / bb;
        int64_t r = aa % bb;
        int64_t x2 = x0 - q * x1;
        int64_t y2 = y0 - q * y1;

        aa = bb;
        bb = r;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }

    if (d % aa != 0) {
        return {}; 
    }

    int64_t k = d / aa;
    return { x0 * k, y0 * k };
}

void solveEquation() {
    int64_t a = 275, b = 1456, d = 10;
    vector<int64_t> result = solveEquation(a, b, d);

    if (result.empty()) {
        cout << "Решений нет (НОД не делит d).\n";
    } else {
        cout << "Решение уравнения: a = " << result[0] << ", b = " << result[1] << endl;
    }
}
