#include <iostream>
#include <cstring>
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

public:
    LongNumber()
    {
        number = nullptr;
        size = 0;
    }

    LongNumber(const char* str)
    {
        number = nullptr;
        size = 0;
        fromStringToNumber(str);
    }

    LongNumber(int n)
    {
        number = nullptr;
        size = 0;
        fromNumberToString(n);
    }

    LongNumber(const LongNumber& other)
    {
        size = other.size;
        number = new char[size];
        for (size_t i = 0; i < size; i++) {
            number[i] = other.number[i];
        }
    }

    ~LongNumber()
    {
        delete[] number;
        number = nullptr;
    }
    
    //// Не мои операторы
    ////Оператор строго меньше<
    //bool operator < (const LongNumber& other) const {
    //    if (size < other.size) return true;
    //    if (size > other.size) return false;
    //    for (int i = size; i > 0; i--) {
    //        if (number[i - 1] < other.number[i - 1])
    //            return true;
    //        if (number[i - 1] > other.number[i - 1])
    //            return false;
    //    }

    //    return false;
    //}

    ////Оператор строго больше > 
    //bool operator > (const LongNumber& other) const {
    //    return other < *this;
    //}

    ////Оператор меньше либо равно <= 
    //bool operator <= (const LongNumber& other) const {
    //    return !(*this > other);
    //}

    ////Оператор больше либо равно >= 
    //bool operator >= (const LongNumber& other) const {
    //    return !(*this < other);
    //}

    ////Оператор равно == 
    //bool operator == (const LongNumber& other) const {
    //    if (size != other.size) return false;
    //    for (int i = 0; i < size; i++)
    //    {
    //        if (number[i] != other.number[i]) return false;
    //    }

    //    return true;
    //}

    ////Оператор не равно != 
    //bool operator != (const LongNumber& other) const {
    //    return !(*this == other);
    //}

    LongNumber& operator = (const LongNumber& other)
    {
        if (this != &other)
        {
            delete[] number;
            number = nullptr;
            size = other.size;
            number = new char[other.size];
            for (size_t i = 0; i < size; i++) {
                number[i] = other.number[i];
            }
        }
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

    LongNumber operator + (const LongNumber& other) const
    {
        LongNumber result;
        size_t shift = 0;

        result.size = max(size, other.size) + 1;
        result.number = new char[result.size];

        for (int i = 0; i < max(size, other.size); i++)
        {
            size_t sum = shift;

            if (i < size)
            {
                sum += number[i];
            }

            if (i < other.size)
            {
                sum += other.number[i];
            }

            result.number[i] = sum % 10;
            shift = sum / 10;
        }

        if (shift != 0)
        {
            result.number[max(size, other.size)] = shift;
        }
        else
        {
            result.size--;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
        {
            result.size--;
        }

        return result;
    }

    LongNumber operator-(const LongNumber& other) const
    {
        LongNumber result;
        result.size = max(size, other.size);
        result.number = new char[result.size]();
        int shift = 0;
        for (int i = 0; i < result.size; i++)
        {
            int diff = shift;
            if (i < size)
            {
                diff += number[i];
            }

            if (i < other.size)
            {
                diff -= other.number[i];
            }

            if (diff < 0)
            {
                diff += 10;
                shift = -1;
            }
            else
            {
                shift = 0;
            }

            result.number[i] = diff;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
        {
            result.size--;
        }

        return result;
    }

    LongNumber operator * (const LongNumber& other) const
    {
        LongNumber result;
        result.size = size + other.size;
        result.number = new char[size + other.size];

        for (int i = 0; i < (size + other.size); i++)
        {
            result.number[i] = 0;
        }

        for (int i = 0; i < size; i++)
        {
            size_t shift = 0;
            for (int j = 0; j < other.size; j++)
            {
                size_t product = number[i] * other.number[j] + result.number[i + j] + shift;
                result.number[i + j] = product % 10;
                shift = product / 10;
            }

            if (shift > 0)
            {
                result.number[i + other.size] += shift;
            }
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
        {
            result.size--;
        }

        return result;
    }

    LongNumber operator / (const LongNumber& other)
    {
        if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
        {
            throw runtime_error("Division by zero");
        }

        if (*this < other)
        {
            return LongNumber("0");
        }

        if (*this == other)
        {
            return LongNumber("1");
        }

        LongNumber result;
        LongNumber current;
        current.size = 0;
        result.size = size;
        result.number = new char[result.size]();

        for (int i = size - 1; i >= 0; i--)
        {
            char* newNum = new char[current.size + 1];
            for (int j = current.size - 1; j >= 0; j--)
            {
                newNum[j + 1] = current.number[j];
            }
            newNum[0] = number[i];

            delete[] current.number;
            current.number = newNum;
            current.size++;

            while (current.size > 1 && current.number[current.size - 1] == 0)
            {
                current.size--;
            }

            int quotient = 0;
            while (!(current < other))
            {
                current = current - other;
                quotient++;
            }

            result.number[i] = quotient;
        }

        while (result.size > 1 && result.number[result.size - 1] == 0)
        {
            result.size--;
        }

        return result;
    }


    LongNumber operator % (const LongNumber& other)
    {
        if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
        {
            throw runtime_error("Division by zero");
        }

        if (*this < other)
        {
            return *this;
        }

        if (*this == other)
        {
            return LongNumber("0");
        }

        LongNumber current;
        current.size = 0;

        for (int i = size - 1; i >= 0; i--)
        {
            char* newNum = new char[current.size + 1];
            for (int j = current.size - 1; j >= 0; j--)
            {
                newNum[j + 1] = current.number[j];
            }
            newNum[0] = number[i];

            delete[] current.number;
            current.number = newNum;
            current.size++;

            while (current.size > 1 && current.number[current.size - 1] == 0)
            {
                current.size--;
            }

            while (!(current < other))
            {
                current = current - other;
            }
        }

        return current;
    }


    void fromStringToNumber(const char* str)
    {
        size = strlen(str);
        delete[] number;         // Освобождаем память, если number уже указывает на выделенный массив
        number = new char[size];  // Выделяем память для числа

        // Заполняем массив в обратном порядке и преобразуем символы в числа
        for (int i = 0; i < size; i++) {
            number[i] = str[size - i - 1] - '0';  // Преобразуем символ в число
        }
    }

    void fromNumberToString(int n) {
        // Определяем размер числа
        int temp = n;
        size = 0;
        do {
            temp /= 10;
            size++;
        } while (temp != 0);

        // Освобождаем память, если number уже указывает на выделенный массив
        delete[] number;
        number = new char[size];

        // Записываем цифры числа n в массив number в обратном порядке
        for (int i = 0; i < size; i++) {
            number[i] = (n % 10);
            n /= 10;
        }
    }


    //void fromNumberToString(int n) {
    //    char str[12]; // Массив, достаточный для хранения int с возможным минусом и '\0'
    //    int index = 0;

    //    // Получение цифр в обратном порядке
    //    int start = index;
    //    do {
    //        str[index++] = (n % 10) + '0';
    //        n /= 10;
    //    } while (n != 0);

    //    // Переворот строки для правильного порядка
    //    for (int i = start, j = index - 1; i < j; i++, j--) {
    //        char temp = str[i];
    //        str[i] = str[j];
    //        str[j] = temp;
    //    }

    //    str[index] = '\0'; // Завершение строки символом конца строки

    //    // Вызов функции fromStringToNumber
    //    fromStringToNumber(str);
    //}

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

    //// На 1 поднимает вверх
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
    //}

    // Оставить это, работает отлично, но теперь алгоритм не работает (или работает?)
    LongNumber sqrt1(LongNumber n)
    {
        if (n == LongNumber("0"))
            return LongNumber("0");

        LongNumber low("1");
        LongNumber high = n;
        LongNumber mid;
        LongNumber midSquared;

        while (low <= high)
        {
            // Вычисляем середину
            mid = (low + high) / LongNumber("2");

            // mid^2
            midSquared = mid * mid;

            if (midSquared == n)
                return mid;  // нашли точный корень
            else if (midSquared < n)
                low = mid + LongNumber("1");  // ищем в правой половине
            else
                high = mid - LongNumber("1");  // ищем в левой половине
        }

        return high;  // В конце high будет максимальным числом, чьи квадрат меньше или равен n
    }


    void generateRandomNumber(int length)
    {
        delete[] number;
        number = nullptr;
        size = length;
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


    void fermatFactorization(LongNumber& a) {

        if (a.endelim() == 0) {
            cout << "Число должно быть нечётным для факторизации методом Ферма." << endl;
            return;
        }

        LongNumber x = a.sqrt1(a);
        cout << "Корень числа: " << x.toString() << endl;
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
        for (size_t i = size; i > 0; --i) cout << number[i - 1];
        cout << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    LongNumber result;
    LongNumber a = "111 111 111 111 111 111 113";
    auto start = high_resolution_clock::now();
    result.fermatFactorization(a);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Время выполнения: " << duration.count() << " миллисекунд" << endl;

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
