#include "BigNumber.h"

// Конструктор для преобразования строки в число
BigNumber::BigNumber(const std::string& str)
{
    is_negative = false;
    int start = 0;

    if (str[0] == '-')
    {
        is_negative = true;
        start = 1;  // Пропускаем знак
    }

    length = str.length() - start;
    for (int i = 0; i < length; i++)
    {
        number[i] = str[str.length() - i - 1] - '0';  // Записываем число в обратном порядке
    }
}

// Функция сравнения двух чисел
int BigNumber::Compare(const char* num1, int len1, const char* num2, int len2)
{
    if (len1 != len2) return len1 > len2 ? 1 : -1;
    for (int i = len1 - 1; i >= 0; --i)
    {
        if (num1[i] != num2[i]) return num1[i] > num2[i] ? 1 : -1;
    }
    return 0;
}

// Преобразование массива символов в блоки long long
void BigNumber::convertCharArrayToBlocks(const char* number, int length, long long* blocks, int& blockCount)
{
    blockCount = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (int i = 0; i < blockCount; ++i)
    {
        blocks[i] = 0;
        int start = length - (i + 1) * BLOCK_SIZE;
        int end = length - i * BLOCK_SIZE;

        if (start < 0) start = 0;
        for (int j = start; j < end; ++j)
        {
            blocks[i] = blocks[i] * 10 + (number[j] - '0');
        }
    }
}

// Вывод блоков числа
void BigNumber::printBlocks(const long long* blocks, int blockCount)
{
    std::cout << blocks[blockCount - 1];
    for (int i = blockCount - 2; i >= 0; --i)
    {
        std::cout << std::setfill('0') << std::setw(BLOCK_SIZE) << blocks[i];
    }
    std::cout << std::endl;
}

// Сложение
void BigNumber::Addition(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len)
{
    int carry = 0;
    result_len = std::max(len1, len2);

    for (int i = 0; i < result_len; ++i)
    {
        int digit1 = i < len1 ? num1[i] : 0;
        int digit2 = i < len2 ? num2[i] : 0;
        int sum = digit1 + digit2 + carry;
        result[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry) result[result_len++] = carry;
}

// Вычитание
void BigNumber::Subtraction(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len)
{
    int borrow = 0;
    result_len = len1;

    for (int i = 0; i < result_len; ++i)
    {
        int digit1 = num1[i];
        int digit2 = i < len2 ? num2[i] : 0;
        int diff = digit1 - digit2 - borrow;

        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else borrow = 0;

        result[i] = diff;
    }

    while (result_len > 1 && result[result_len - 1] == 0) result_len--;
}

// Умножение
void BigNumber::Multiplication(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len)
{
    result_len = len1 + len2;
    memset(result, 0, result_len);

    for (int i = 0; i < len1; ++i)
    {
        int carry = 0;
        for (int j = 0; j < len2; ++j)
        {
            int product = num1[i] * num2[j] + result[i + j] + carry;
            result[i + j] = product % 10;
            carry = product / 10;
        }
        result[i + len2] += carry;
    }

    while (result_len > 1 && result[result_len - 1] == 0) result_len--;
}

// Деление
void BigNumber::Division(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len)
{
    char temp[MAX_DIGITS] = { 0 };
    int temp_len = 0;
    result_len = 0;

    for (int i = len1 - 1; i >= 0; --i)
    {
        for (int j = temp_len; j > 0; --j)
        {
            temp[j] = temp[j - 1];
        }
        temp[0] = num1[i];
        temp_len++;

        while (temp_len > 1 && temp[temp_len - 1] == 0) temp_len--;

        int quotient = 0;
        char prod[MAX_DIGITS] = { 0 };
        int prod_len = 0;

        while (Compare(temp, temp_len, num2, len2) >= 0)
        {
            Subtraction(temp, temp_len, num2, len2, prod, prod_len);
            memcpy(temp, prod, prod_len);
            temp_len = prod_len;
            quotient++;
        }
        result[result_len++] = quotient;
    }
    while (result_len > 1 && result[result_len - 1] == 0) result_len--;

    for (int i = 0; i < result_len / 2; ++i)
    {
        std::swap(result[i], result[result_len - i - 1]);
    }
}