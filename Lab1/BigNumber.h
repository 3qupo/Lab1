#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>

const int MAX_DIGITS = 1000;
const int BLOCK_SIZE = 18;
const long long BASE = 1000000000000000000LL;

class BigNumber
{
public:
    char number[MAX_DIGITS];
    bool is_negative;
    int length;

    int Compare(const char* num1, int len1, const char* num2, int len2);

    BigNumber(const std::string& str);

    const char* getNumber() const { return number; }
    int getLength() const { return length; }

    void convertCharArrayToBlocks(const char* number, int length, long long* blocks, int& blockCount);

    void printBlocks(const long long* blocks, int blockCount);
    void Addition(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len);
    void Subtraction(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len);
    void Multiplication(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len);
    void Division(const char* num1, int len1, const char* num2, int len2, char* result, int& result_len);
};