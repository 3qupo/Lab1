#include <iostream>
#include <cmath>
#include <ctime> // clock
#include "funk.h"
#include "BigNumber.h"

void fermatFactorization(int count) {
    // Проверим, что n нечетное, так как метод Ферма работает только с нечетными числами
    if (count % 2 == 0) {
        std::cout << "There should only be an odd number" << std::endl;
        return;
    }

    long long x = std::ceil(std::sqrt(count));
    // Google: floor вычисляет ближайшее целое число, большее или равное n, и возвращает его в виде числа с плавающей точкой
    long long y2 = x * x - count;
    long long y = std::round(std::sqrt(y2));

    // Пока y^2 не является точным квадратом
    while (y * y != y2) {
        x++;
        y2 = x * x - count;
        y = std::round(std::sqrt(y2));
    }

    long long factor1 = x - y;
    long long factor2 = x + y;

    std::cout << "Multipliers of a number: " << count << ": " << factor1 << " and " << factor2 << std::endl;

}
