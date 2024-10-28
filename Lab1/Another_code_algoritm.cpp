
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
    int* number;
    int size;

public:
    LongNumber()
    {
        number = nullptr;
        size = 0;
    }

    LongNumber(string str)
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
        number = new int[size];
        for (size_t i = 0; i < size; i++) {
            number[i] = other.number[i];
        }
    }

    ~LongNumber()
    {
        delete[] number;
    }

    LongNumber& operator = (const LongNumber& other)
    {
        if (this != &other)
        {
            delete[] number;
            size = other.size;
            number = new int[other.size];
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
        result.number = new int[result.size];

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
        result.number = new int[result.size]();
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
        result.number = new int[size + other.size];

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
        result.number = new int[result.size]();

        for (int i = size - 1; i >= 0; i--)
        {
            int* newNum = new int[current.size + 1];
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
            int* newNum = new int[current.size + 1];
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


    void fromStringToNumber(const string& str)
    {
        size = str.size();
        number = new int[size];
        for (size_t i = 0; i < str.size(); i++)
        {
            number[i] = str[i] - '0';
        }

        int start = 0;
        int end = size - 1;
        while (start < end)
        {
            int temp = number[start];
            number[start] = number[end];
            number[end] = temp;
            start++;
            end--;
        }
    }

    void fromNumberToString(int n)
    {
        string str = to_string(n);
        fromStringToNumber(str);
    }

    string toString() const
    {
        string str;
        for (int i = size - 1; i >= 0; i--)
        {
            str += to_string(number[i]);
        }

        return str;
    }

    int endelim()
    {
        int b = number[0] % 2;
        return b;
    }

    LongNumber sqrt1(LongNumber n)
    {
        LongNumber i("1");
        while (i <= n)
        {
            if ((i * i) >= n)
            {
                return i;
            }
            else
            {
                i = i + LongNumber("1");
            }
        }
    }

    void generateRandomNumber(int length)
    {
        delete[] number;
        number = new int[size];
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


    void fermatFactorization(LongNumber& n)
    {
        if (n.endelim() == 0)
        {
            cout << "Должно быть нечетное число" << endl;
            return;
        }


        LongNumber x = n.sqrt1(n);
        if (x * x == n)
        {
            cout << "Factors: " << x.toString() << " * " << x.toString() << endl;
            return;
        }

        LongNumber y("0");
        LongNumber x_squared = x * x;
        LongNumber y_squared = y * y;
        LongNumber diff = x_squared - y_squared;

        while (diff != n)
        {
            if (diff > n)
            {
                y = y + LongNumber("1");
                y_squared = y * y;
            }
            else
            {
                x = x + LongNumber("1");
                x_squared = x * x;
            }
            diff = x_squared - y_squared;
        }

        LongNumber factor1 = x - y;
        LongNumber factor2 = x + y;
        cout << "Factors: " << factor1.toString() << " * " << factor2.toString() << endl;
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

    LongNumber a = 1;
    LongNumber b = 2;
    if (a < b) cout << "Yes" << endl;
    else
    {
        cout << "No" << endl;
    }

    LongNumber result;
    ifstream file("Simple_numbers.txt");
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл." << endl;
        return 1;
    }

    LongNumber numbers[5];
    string line;

    for (size_t i = 0; i < 5 && getline(file, line); ++i) {
        numbers[i] = LongNumber(line);
    }
    file.close();

    cout << "Считанные числа:" << endl;
    for (size_t i = 0; i < 5; ++i) {
        numbers[i].print();
    }

    cout << endl;

    for (size_t i = 0; i < 5; ++i) {
        cout << "Факторизация числа " << i + 1 << ": ";
        auto start = high_resolution_clock::now();

        result.fermatFactorization(numbers[i]);

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Время выполнения: " << duration.count() << " миллисекунд" << endl;
    }

    return 0;
}
