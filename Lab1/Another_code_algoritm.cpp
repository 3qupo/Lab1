#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class LongNumber {
private:
    int* number;
    int size;
    bool isNegative; 

public:
    LongNumber() {
        number = nullptr;
        size = 0;
        isNegative = false;
    }

    LongNumber(const char* str) {
        number = nullptr;
        size = 0;
        isNegative = false;
        fromStringToNumber(str);
    }

    LongNumber(int n) {
        number = nullptr;
        size = 0;
        isNegative = false;
        fromNumberToString(n);
    }

    LongNumber(const LongNumber& other) {
        size = other.size;
        isNegative = other.isNegative;
        number = new int[size];
        for (size_t i = 0; i < size; i++) {
            number[i] = other.number[i];
        }
    }

    ~LongNumber() {
        delete[] number;
        number = nullptr;
    }

    LongNumber& operator = (const LongNumber& other) {
        if (this != &other) {
            delete[] number;
            number = nullptr;
            size = other.size;
            isNegative = other.isNegative;
            number = new int[other.size];
            for (size_t i = 0; i < size; i++) {
                number[i] = other.number[i];
            }
        }
        return *this;
    }

    bool absGreaterThan(const LongNumber& other) const {
        if (size != other.size)
            return size > other.size;

        for (int i = size - 1; i >= 0; i--) {
            if (number[i] != other.number[i])
                return number[i] > other.number[i];
        }
        return false;
    }

    bool absEquals(const LongNumber& other) const {
        if (size != other.size)
            return false;

        for (int i = 0; i < size; i++) {
            if (number[i] != other.number[i])
                return false;
        }
        return true;
    }

    bool operator == (const LongNumber& other) {
        return isNegative == other.isNegative && absEquals(other);
    }

    bool operator != (const LongNumber& other) {
        return !(*this == other);
    }

    bool operator < (const LongNumber& other) {
        if (isNegative != other.isNegative)
            return isNegative;

        if (isNegative)
            return absGreaterThan(other);
        return !absGreaterThan(other) && !absEquals(other);
    }

    bool operator <= (const LongNumber& other) {
        return (*this < other) || (*this == other);
    }

    bool operator > (const LongNumber& other) {
        return !(*this <= other);
    }

    bool operator >= (const LongNumber& other) {
        return !(*this < other);
    }

    LongNumber operator + (const LongNumber& other) const {
        if (isNegative != other.isNegative) {
            LongNumber temp = other;
            temp.isNegative = !temp.isNegative;
            return *this - temp;
        }

        LongNumber result;
        result.isNegative = isNegative; 
        size_t shift = 0;

        result.size = max(size, other.size) + 1;
        result.number = new int[result.size];

        for (int i = 0; i < max(size, other.size); i++) {
            size_t sum = shift;
            if (i < size) sum += number[i];
            if (i < other.size) sum += other.number[i];
            result.number[i] = sum % 10;
            shift = sum / 10;
        }

        if (shift != 0)
            result.number[max(size, other.size)] = shift;
        else
            result.size--;

        while (result.size > 1 && result.number[result.size - 1] == 0)
            result.size--;

        return result;
    }

    LongNumber operator - (const LongNumber& other) const {
        if (isNegative != other.isNegative) {
            LongNumber temp = other;
            temp.isNegative = !temp.isNegative;
            return *this + temp;
        }

        LongNumber result;
        bool needSwap = false;

        if (!absGreaterThan(other) && !absEquals(other)) {
            needSwap = true;
        }

        const LongNumber& larger = needSwap ? other : *this;
        const LongNumber& smaller = needSwap ? *this : other;

        result.isNegative = isNegative ? !needSwap : needSwap;

        result.size = larger.size;
        result.number = new int[result.size]();
        int shift = 0;

        for (int i = 0; i < result.size; i++) {
            int diff = shift;
            if (i < larger.size) diff += larger.number[i];
            if (i < smaller.size) diff -= smaller.number[i];

            if (diff < 0) {
                diff += 10;
                shift = -1;
            }
            else {
                shift = 0;
            }

            result.number[i] = diff;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
            result.size--;

        return result;
    }

    LongNumber operator * (const LongNumber& other) const {
        LongNumber result;
        result.isNegative = isNegative != other.isNegative;
        result.size = size + other.size;
        result.number = new int[size + other.size]();

        for (int i = 0; i < size; i++) {
            size_t shift = 0;
            for (int j = 0; j < other.size; j++) {
                size_t product = number[i] * other.number[j] + result.number[i + j] + shift;
                result.number[i + j] = product % 10;
                shift = product / 10;
            }
            if (shift > 0) result.number[i + other.size] += shift;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
            result.size--;

        if (result.size == 1 && result.number[0] == 0)
            result.isNegative = false;

        return result;
    }

    LongNumber operator / (const LongNumber& other) {
        if (other.size == 0 || (other.size == 1 && other.number[0] == 0)) {
            throw runtime_error("Division by zero");
        }

        bool resultNegative = isNegative != other.isNegative;

        LongNumber positiveThis = *this;
        LongNumber positiveOther = other;
        positiveThis.isNegative = false;
        positiveOther.isNegative = false;

        if (positiveThis < positiveOther) {
            return LongNumber("0");
        }

        if (positiveThis == positiveOther) {
            LongNumber result("1");
            result.isNegative = resultNegative;
            return result;
        }

        LongNumber result;
        LongNumber current;
        current.size = 0;
        result.size = positiveThis.size;
        result.number = new int[result.size]();
        result.isNegative = resultNegative;

        for (int i = positiveThis.size - 1; i >= 0; i--) {
            int* newNum = new int[current.size + 1];
            for (int j = current.size - 1; j >= 0; j--) {
                newNum[j + 1] = current.number[j];
            }
            newNum[0] = positiveThis.number[i];

            delete[] current.number;
            current.number = newNum;
            current.size++;

            while (current.size > 1 && current.number[current.size - 1] == 0) {
                current.size--;
            }

            int quotient = 0;
            while (!(current < positiveOther)) {
                current = current - positiveOther;
                quotient++;
            }

            result.number[i] = quotient;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0) {
            result.size--;
        }

        if (result.size == 1 && result.number[0] == 0)
            result.isNegative = false;

        return result;
    }

    LongNumber operator % (const LongNumber& other) {
        if (other.size == 0 || (other.size == 1 && other.number[0] == 0)) {
            throw runtime_error("Division by zero");
        }

        LongNumber positiveThis = *this;
        LongNumber positiveOther = other;
        positiveThis.isNegative = false;
        positiveOther.isNegative = false;

        if (positiveThis < positiveOther) {
            return *this;
        }

        if (positiveThis == positiveOther) {
            return LongNumber("0");
        }

        LongNumber current;
        current.size = 0;

        for (int i = positiveThis.size - 1; i >= 0; i--) {
            int* newNum = new int[current.size + 1];
            for (int j = current.size - 1; j >= 0; j--) {
                newNum[j + 1] = current.number[j];
            }
            newNum[0] = positiveThis.number[i];

            delete[] current.number;
            current.number = newNum;
            current.size++;

            while (current.size > 1 && current.number[current.size - 1] == 0) {
                current.size--;
            }

            while (!(current < positiveOther)) {
                current = current - positiveOther;
            }
        }

        current.isNegative = isNegative;
        if (current.size == 1 && current.number[0] == 0)
            current.isNegative = false;

        return current;
    }

    void fromStringToNumber(const char* str) {
        const char* start = str;
        isNegative = false;

        if (*start == '-') {
            isNegative = true;
            start++;
        }

        // поменяем потом strlen
        size = strlen(start);
        number = new int[size];
        for (size_t i = 0; i < size; i++) {
            number[i] = start[i] - '0';
        }

        int s = 0;
        int e = size - 1;
        while (s < e) {
            int temp = number[s];
            number[s] = number[e];
            number[e] = temp;
            s++;
            e--;
        }
    }

    void fromNumberToString(int n) {
        isNegative = (n < 0);
        if (isNegative) n = -n;

        char str[12];
        int index = 0;

        do {
            str[index++] = (n % 10) + '0';
            n /= 10;
        } while (n != 0);

        str[index] = '\0';

        for (int i = 0, j = index - 1; i < j; i++, j--) {
            char temp = str[i];
            str[i] = str[j];
            str[j] = temp;
        }

        fromStringToNumber(str);
    }

    char* toString() const {
        int strSize = size + (isNegative ? 2 : 1);  
        char* str = new char[strSize];
        int index = 0;

        if (isNegative)
            str[index++] = '-';

        for (int i = size - 1; i >= 0; i--)
            str[index++] = number[i] + '0';

        str[index] = '\0';
        return str;
    }

    int endelim() {
        int b = number[0] % 2;
        return b;
    }

    LongNumber sqrt1(LongNumber n) {
        LongNumber i("1");
        while (i <= n) {
            if ((i * i) >= n) {
                return i;
            }
            else {
                i = i + LongNumber("1");
            }
        }
        return i; 
    }

    void generateRandomNumber(int length) {
        delete[] number;
        number = nullptr;
        size = length;
        isNegative = rand() % 2; 
        number = new int[size];
        srand(time(0));
        for (int i = 0; i < size; i++) {
            number[i] = rand() % 10;
        }
        while (number[size - 1] == 0) {
            number[size - 1] = rand() % 9 + 1;
        }
    }

    void fermatFactorization(LongNumber& a) {
        if (a.isNegative) {
            cout << "Факторизация работает только для положительных чисел." << endl;
            return;
        }

        if (a.endelim() == 0) {
            cout << "Число должно быть нечётным для факторизации методом Ферма." << endl;
            return;
        }

        LongNumber x = a.sqrt1(a);
        if (x * x == a) {
            cout << "Факторы: " << x.toString() << " * " << x.toString() << endl;
            return;
        }

        LongNumber y("0");
        LongNumber R = x * x - y * y;
        LongNumber Rx = x * LongNumber("2") + LongNumber("1");
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
        if (isNegative)  // Если число отрицательное, выводим минус
        {
            cout << "-";
        }
        for (size_t i = size; i > 0; --i)
        {
            cout << number[i - 1];  // Выводим цифры числа
        }
        cout << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    LongNumber num1("-12");
    LongNumber num2("11");

    cout << "Числа: " << num1.toString() << " и " << num2.toString() << endl;

    cout << "Сложение: ";
    (num1 + num2).print();

    cout << "Вычитание: ";
    (num1 - num2).print();

    cout << "Умножение: ";
    (num1 * num2).print();

    cout << "Деление: ";
    (num1 / num2).print();

    //LongNumber result;
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
