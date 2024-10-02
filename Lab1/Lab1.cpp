#include <iostream>
#include <cmath>
#include <sstream>
#include "funk.h"
#include "BigNumber.h"  // Подключаем BigNumber

int main() 
{
    std::string random_number_str = generate_long_random_number(1000);
    BigNumber random_number(random_number_str);  // Создаем большое число

    // Создаем блоки
    long long blocks[MAX_DIGITS / BLOCK_SIZE] = { 0 };
    int blockCount = 0;

    // Преобразуем массив символов в блоки (используем геттеры для доступа к number и length)
    random_number.convertCharArrayToBlocks(random_number.getNumber(), random_number.getLength(), blocks, blockCount);

    // Выводим блоки
    random_number.printBlocks(blocks, blockCount);

    return 0;
}