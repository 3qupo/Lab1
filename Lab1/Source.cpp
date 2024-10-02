#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include "BigNumber.h"

std::string generate_long_random_number(int large) {
    // Инициализация генератора случайных чисел
    std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());

    // Диапазон для цифр от 0 до 9
    std::uniform_int_distribution<int> dist(0, 9);

    std::string random_number;

    // Первая цифра не может быть 0
    random_number += std::to_string(dist(rng) % 9 + 1);

    // Генерация остальных цифр
    for (int i = 1; i < large; ++i) {
        random_number += std::to_string(dist(rng));
    }

    return random_number;
}