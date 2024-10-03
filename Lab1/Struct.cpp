#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>


// по умолчанию (инициализация нулями)
// занесение числа
// копирование из числа
// оператор присваивания

using namespace std;

struct  BigNumber
{
    static const long long BASE = 1e9;             //размер разряда
    static const long long SIZE = 100000000;       //количество вмещаемых разрядов

    unsigned long long digits[SIZE];

    BigNumber()                                    //стандартный конструктор НУЖНО ОСТАВИТЬ
    {                                  
        for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }
    }

    BigNumber(long long BigNumber)                        //конструктор для преобразования обычного числа в длинное НУЖНО ОСТАВИТЬ
    {
        /*for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }*/

        int next = 0;
        while (BigNumber)
        {
            digits[next++] = BigNumber % BASE;
            BigNumber /= BASE;
        }
    }

    BigNumber(BigNumber& other)
    {               //конструктор копирования НУЖНО ОСТАВИТЬ
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }
    }

    BigNumber& operator = (BigNumber& other) {    //оператор присваивания
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }
        return *this;
    }

    void operator+=(BigNumber& other) {
        for (int i = 0; i < SIZE; i++) {        //сначала сложим числа поразрядно,
            digits[i] += other.digits[i];       //игнорируя переполнения
        }

        for (int i = 0; i < SIZE - 1; i++) {    //а затем поочередно выполним переносы
            if (digits[i] >= BASE) {            //для каждого разряда
                digits[i] -= BASE;
                digits[i + 1]++;
            }
        }
    }

};

// определяем длину массива суммы
unsigned long long Addition()
{
    int size_a;
    int size_b;
    unsigned long long length;
    int b[1000000];
    int a[1000000];

    if (size_a > size_b) length = size_a + 1;
    else length = size_b + 1;

    for (int ix = 0; ix < length; ix++)
    {
        b[ix] += a[ix]; // суммируем последние разряды чисел
        b[ix + 1] += (b[ix] / 10); // если есть разряд для переноса, переносим его в следующий разряд
        b[ix] %= 10; // если есть разряд для переноса он отсекается
    }

    if (b[length - 1] == 0)
    length--;

    return length;
}

unsigned long long Subtraction()
{
    int k = 3; // если к == 3, значит числа одинаковой длинны
    int length;
    int size_a;
    int size_b;
    int a[1000000];
    int b[1000000];

    length = size_a;
    if (size_a > size_b)
    {
        length = size_a;
        k = 1; // если к == 1, значит первое число длиннее второго
    }
    else
    {
        if (size_b > size_a)
        {
            length = size_b;
            k = 2; // если к == 2, значит второе число длиннее первого
        }
        else // если числа одинаковой длинны, то необходимо сравнить их веса
        {
            for (int ix = 0; ix < length;) // поразрядное сравнение весов чисел
            {
                if (a[ix] > b[ix]) // если разряд первого числа больше
                {
                    k = 1; // значит первое число длиннее второго
                    break; // выход из цикла for
                }

                if (b[ix] > a[ix]) // если разряд второго числа больше
                {
                    k = 2; // значит второе число длиннее первого
                    break; // выход из цикла for
                }
            }
        }
    }
}

int Difference(int* x, int* y, int* z, int length)
{
    for (int ix = 0; ix < (length - 1); ix++) // проход по всем разрядам числа, начиная с последнего, не доходя до первого
    {
        if (ix < (length - 1)) // если текущий разряд чисел не первый
        {
            x[ix + 1]--; // в следующуем разряде большего числа занимаем 1.
            z[ix] += 10 + x[ix]; // в ответ записываем сумму значения текущего разряда большего числа и 10-ти

        }
        else  // если текущий разряд чисел - первый
        {
            z[ix] += x[ix]; // в ответ суммируем значение текущего разряда большего числа
        }

        z[ix] -= y[ix]; // вычитаем значение текущего разряда меньшего числа

        if (z[ix] / 10 > 0) // если значение в текущем разряде двухразрядное
        {
            z[ix + 1]++; // переносим единицу в старший разряд
            z[ix] %= 10; // в текущем разряде отсекаем ее
        }
    }

    return x - y;
}

int Multiplication()
{
    int length;
    int size_a = 0;
    int size_b = 0;
    int a[1000000];
    int b[1000000];
    int c[1000000];

    length = size_a + size_b + 1;

    for (int ix = 0; ix < size_a; ix++)
    {
        for (int jx = 0; jx < size_b; jx++)
        {
            c[ix + jx - 1] += a[ix] * b[jx];
        }
    }

    for (int ix = 0; ix < length; ix++)
    {
        c[ix + 1] += c[ix] / 10;
        c[ix] %= 10;
    }

    while (c[length] == 0) length--;

}