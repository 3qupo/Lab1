// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
#include <string>
#include <fstream>   // for files
#include <chrono>  // for time
#include <cstdlib> // Для rand и srand

using namespace std;
using namespace std::chrono;

class LongNumber
{
private:
    char* number;
    int size;
    int length;

public:
    LongNumber()
    {
        number = nullptr;
        size = 0;
        length = 0;
    }

    LongNumber(const char* str)
    {
        if (str == nullptr) {
            throw invalid_argument("String pointer is null!");
        }

        length = 0;
        while (str[length] != '\0') length++;
        size = length + 1;
        number = new char[size];
        int result = length;

        for (int i = 0; i < length; i++) {
            number[i] = str[result - 1];
            result--;
        }

        number[size - 1] = '\0';
    }

    LongNumber(int n)
    {
        number = nullptr;
        size = 0;
        length = 0;
        fromNumberToString(n);
    }

    LongNumber(const LongNumber& other)
    {
        length = other.length;
        size = other.size;
        number = new char[size];
        for (size_t i = 0; i < length; i++) {
            number[i] = other.number[i];
        }
        number[length] = '\0';
    }

    ~LongNumber()
    {
        delete[] number;
        number = nullptr;
        length = 0;
        size = 0;
    }

    int GetLength() const {
        return length;
    }

    /*void enscure_size(int new_length)
    {
        size = new_length + 1;
        char* new_number = new char[size];
        for (int i = 0; i < length; i++)
        {
            new_number[i] = number[i];
        }
        for (int i = length; i < new_length; i++)
        {
            new_number[i] = '0';
        }
        delete[] number;
        number = new_number;
        number[size - 1] = '\0';
    }*/


    // Перевыделение памяти
    void enscure_size(int new_length)
    {
        size = new_length + 1;
        char* new_number = new char[size];
        for (int i = 0; i < new_length; i++)
        {
            if (i < length) new_number[i] = number[i];
            else new_number[i] = '0';
        }

        new_number[length] = '\0';
        delete[] number;
        number = new char[size];
        for (int i = 0; i < length; i = i + 1)
        {
            number[i] = new_number[i];
        }

        delete[] new_number;
    }

    int my_strlen(const char* str)
    {
        length = 0;
        while (str[length] != '\0') length++;
        return length;
    }

    LongNumber& operator = (const LongNumber& other)
    {
        if (this == &other) return *this;

        length = other.length;
        size = other.size;

        delete[] number;

        number = new char[size]; // size or length?

        for (int i = 0; i < length; i++) number[i] = other.number[i];
        number[length] = '\0';

        return *this;
    }

    bool operator == (const LongNumber& other)
    {
        if (this->size != other.size)
            return false;

        for (int i = 0; i < this->size; i++)
        {
            if (this->number[i] != other.number[i])
                return false;
        }

        return true;
    }

    bool operator != (const LongNumber& other)
    {
        bool result = !(this->operator==(other));

        return result;
    }

    bool operator < (const LongNumber& other)
    {
        if (this->operator==(other))
        {
            return false;
        }

        if (other.size != this->size)
        {
            return this->size < other.size;
        }

        for (size_t i = this->size; i > 0; i--)
        {
            if (this->number[i - 1] != other.number[i - 1])
            {
                return this->number[i - 1] < other.number[i - 1];
            }
        }

        return false;
    }

    bool operator <= (const LongNumber& other)
    {
        bool result = (this->operator==(other) || this->operator < (other));

        return result;
    }

    bool operator > (const LongNumber& other)
    {
        bool result = !(this->operator<=(other));

        return result;
    }

    bool operator >= (const LongNumber& other)
    {
        bool result = !(this->operator < (other));

        return result;
    }

    LongNumber operator + (const LongNumber& others)
    {
        LongNumber other = others;
        if (*this < others)
        {
            other = *this;
            *this = others;
        }

        int max_size_sum = max(length, other.length) + 1;
        enscure_size(max_size_sum);

        int carry = 0;
        for (int i = 0; i < length + 1; i++)
        {
            int storage1 = (i < length) ? (number[i] - '0') : 0;
            int storage2 = (i < other.length) ? (other.number[i] - '0') : 0;
            int sum = storage1 + storage2 + carry;

            carry = sum / 10;
            number[i] = (sum % 10) + '0';
        }

        if (number[length] != '0') length++;
        else enscure_size(length);
        
        number[length] = '\0';

        return *this;
    }

    LongNumber operator-(const LongNumber& other)
    {
        if (*this < other) throw invalid_argument("Error!");
        if (*this == other)
        {
            LongNumber temp("0");
            *this = temp;
        }

        else
        {
            int borrow = 0;
            for (int i = 0; i < length; i++)
            {
                int digit1 = (i < length) ? (number[i] - '0') : 0;
                int digit2 = (i < other.length) ? (other.number[i] - '0') : 0;
                int diff = digit1 - digit2 - borrow;

                borrow = (diff < 0) ? 1 : 0;
                if (borrow) diff += 10;
                number[i] = diff + '0';
            }

            int leading_zero = -1;
            for (int i = length; i > 0; i--)
            {
                if (number[i - 1] == '0') leading_zero += 1;
                else break;
            }

            if (leading_zero != -1)
            {
                length = length - leading_zero - 1;
                enscure_size(length);
            }
            else number[length] = '\0';
        }

        return *this;
    }

    LongNumber operator * (LongNumber& other)
    {
        if ((*this == LongNumber("0")) || (other == LongNumber("0"))) *this = LongNumber("0");
        else
        {
            LongNumber result(length + other.length);

            for (int i = 0; i < length; i++)
            {
                int carry = 0;
                for (int j = 0; j < other.length; j++)
                {
                    int digit1 = number[i] - '0';
                    int digit2 = other.number[j] - '0';
                    int current_result = result.number[i + j] - '0';
                    int mul = digit1 * digit2 + carry + current_result;

                    result.number[i + j] = (mul % 10) + '0';
                    carry = mul / 10;
                }

                result.number[i + other.length] += carry;
            }

            int leading_zero = -1;
            for (int i = result.length; i > 0; i--)
            {
                if (result.number[i - 1] == '0') leading_zero++;
                else break;
            }

            if (leading_zero != -1)
            {
                result.length = result.length - leading_zero - 1;
                result.enscure_size(result.length);
            }
            else result.number[result.length] = '\0';

            *this = result;
        }

        return *this;
    }

    LongNumber operator / (LongNumber& divisor)
    {
        // Проверка деления на ноль
        if (divisor == LongNumber("0")) {
            throw std::invalid_argument("Division by zero");
        }

        // Если делимое меньше делителя, результат деления будет 0
        if (*this < divisor) {
            return LongNumber("0");
        }

        // Если делимое и делитель равны, результат деления — 1
        if (*this == divisor) {
            return LongNumber("1");
        }

        // Инициализация текущего значения делимого и результата
        LongNumber remainder = *this;
        LongNumber quotient("0");

        // Повторяем вычитание делителя, пока остаток больше или равен делителю
        while (!(remainder < divisor)) {
            remainder = remainder - divisor;
            quotient = quotient + LongNumber("1");  // Увеличиваем результат на 1
        }

        // Возвращаем результат целочисленного деления
        return quotient;
    }

    LongNumber operator % (const LongNumber& divisor) {

        // If the dividend is smaller than the divisor, the remainder is the dividend itself
        if (*this < divisor) {
            return *this;
        }

        // If the numbers are equal, the remainder is zero
        if (*this == divisor) {
            return LongNumber("0");
        }

        // Initialize the remainder with a copy of the current object (dividend)
        LongNumber remainder = *this;

        // Repeatedly subtract the divisor from the remainder until it's less than the divisor
        while (!(remainder < divisor)) {
            remainder = remainder - divisor;
        }

        // The remainder now holds the modulus result
        return remainder;
    }

    void fromStringToNumber(const char* str)
    {
        length = strlen(str);
        size = length + 1;
        number = new char[size];
        for (int i = 0; i < length; i++) {
            number[i] = str[length - i - 1];
        }
        number[length] = '\0';
    }

    void fromNumberToString(int n) 
    {
        char str[12]; // Массив, достаточный для хранения int с возможным минусом и '\0'
        int index = 0;

        // Получение цифр в обратном порядке
        int start = index;
        do {
            str[index++] = (n % 10) + '0';
            n /= 10;
        } while (n != 0);

        // Переворот строки для правильного порядка
        for (int i = start, j = index - 1; i < j; i++, j--) {
            char temp = str[i];
            str[i] = str[j];
            str[j] = temp;
        }

        str[index] = '\0'; // Завершение строки символом конца строки

        // Вызов функции fromStringToNumber
        fromStringToNumber(str);
    }

    char* toString() const
    {
        char* str = new char[size + 1];
        for (int i = size - 1, j = 0; i >= 0; i--, j++)
        {
            str[j] = number[i] + '0';
        }
        str[size] = '\0';
        return str;
    }

    int endelim()
    {
        int b = number[0] % 2;
        return b;
    }

    LongNumber sqrt1(LongNumber n) 
    {
        // Если n = 0 или n = 1, возвращаем само число
        if (n == LongNumber("0") || n == LongNumber("1")) {
            return n;
        }

        LongNumber left("1");
        LongNumber right = n;
        LongNumber result("0");
        LongNumber two("2");

        while (left <= right) 
        {
            LongNumber mid = (left + right) / two;  // Находим середину

            // Проверяем, если mid^2 == n
            LongNumber square = mid * mid;
            if (square == n) {
                return mid;
            }

            // Если mid^2 < n, сохраняем результат и передвигаем левую границу
            if (square < n) {
                result = mid;
                left = mid + LongNumber("1");  // Увеличиваем left
            }
            // Если mid^2 > n, передвигаем правую границу
            else {
                right = mid - LongNumber("1");
            }
        }

        // Возвращаем результат, который является целой частью квадратного корня
        return result;
    }



    //// Надо переделать (сделать бинарно)
    //LongNumber sqrt1(LongNumber n)
    //{
    //    LongNumber i("1");
    //    while (i <= n)
    //    {
    //        if ((i * i) >= n)
    //        {
    //            return i;
    //        }
    //        else
    //        {
    //            i = i + LongNumber("1");
    //        }
    //    }

    //    return *this;
    //}

    void generateRandomNumber(int length1)
    {
        delete[] number;
        number = nullptr;
        size = length1;
        number = new char[size];
        srand(time(0));
        for (int i = 0; i < size; i++)
        {
            number[i] = rand() % 10;
        }
        while (number[size - 1] == 0)
        {
            number[size - 1] = rand() % 9 + 1;
        }
    }

    void fermatFactorization(LongNumber& a) 
    {
        LongNumber result;
        if (a.endelim() == 0) {
            cout << "Число должно быть нечётным для факторизации методом Ферма." << endl;
            return;
        }

        LongNumber x = result.sqrt1(a);
        x.print();
        if (x * x == a) {
            cout << "Факторы: " << x.toString() << " * " << x.toString() << endl;
            return;
        }

        LongNumber y("0");
        LongNumber R = x * x - y * y;
        LongNumber res1("2");
        LongNumber res2("1");
        LongNumber Rx = x * res1 + res2;
        LongNumber Ry = LongNumber("1");

        while (R != a) {
            if (R > a) {
                // Если R(x, y) > a, увеличиваем y на 1 и пересчитываем R и R_y
                y = y + LongNumber("1");
                R = R - Ry;
                Ry = Ry + LongNumber("2");  // Обновляем R_y
            }
            else {
                // Если R(x, y) < a, увеличиваем x на 1 и пересчитываем R и R_x
                x = x + LongNumber("1");
                R = R + Rx;
                Rx = Rx + LongNumber("2");  // Обновляем R_x
            }
        }

        // Когда R(x, y) == a, находим делители (x - y) и (x + y)
        LongNumber factor1 = x - y;
        LongNumber factor2 = x + y;
        cout << "Факторы: " << factor1.toString() << " * " << factor2.toString() << endl;
    }

    void print() const
    {
        for (size_t i = size; i > 0; --i) cout << number[i - 1];
        cout << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    LongNumber result;
    LongNumber resultnumber;
    LongNumber a = 1098751;
    LongNumber b = 20;

    resultnumber = result.sqrt1(b);
    resultnumber.print();

    //ifstream file("Simple_numbers.txt");
    //if (!file.is_open()) {
    //    cerr << "Ошибка: не удалось открыть файл." << endl;
    //    return 1;
    //}

    //LongNumber numbers[5];
    //char line[1024];  // Буфер для хранения строки из файла

    //for (size_t i = 0; i < 5 && file.getline(line, sizeof(line)); ++i) {
    //    numbers[i] = LongNumber(line);
    //}
    //file.close();

    //cout << "Считанные числа:" << endl;
    //for (size_t i = 0; i < 5; ++i) {
    //    numbers[i].print();
    //}

    //cout << endl;

    //for (size_t i = 0; i < 5; ++i) {
    //    cout << "Факторизация числа " << i + 1 << ": ";
    //    auto start = high_resolution_clock::now();

    //    result.fermatFactorization(numbers[i]);

    //    auto end = high_resolution_clock::now();
    //    auto duration = duration_cast<milliseconds>(end - start);
    //    cout << "Время выполнения: " << duration.count() << " миллисекунд" << endl;
    //}

    //LongNumber a;
    //a.generateRandomNumber(5);
    //cout << "Сгенерированное число: ";
    //a.print();

    //auto start = high_resolution_clock::now();
    //cout << "Факторизация числа: ";
    //a.fermatFactorization(a);

    //auto end = high_resolution_clock::now();

    //auto duration = duration_cast<milliseconds>(end - start);
    //cout << "Время выполнения: " << duration.count() << " миллисекунд" << endl;

    return 0;
}
