//// This is a personal academic project. Dear PVS-Studio, please check it.
//// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <chrono> // for time
//
//using namespace std;
//using namespace std::chrono;
//
//class LongNumber {
//private:
//    int* number;  // массив для хранения цифр числа
//    int size;     // количество цифр
//
//public:
//    LongNumber() {
//        number = nullptr;
//        size = 0;
//    }
//
//    LongNumber(string str) {
//        number = nullptr;
//        size = 0;
//        FromStringToNumber(str);
//    }
//
//    LongNumber(int Number) {
//        number = nullptr;
//        size = 0;
//        FromNumberToString(Number);
//    }
//
//    // Конструктор копирования
//    LongNumber(const LongNumber& other) {
//        number = new int[other.size];
//        copy(other.number, other.number + other.size, number);
//        size = other.size;
//    }
//
//    ~LongNumber() {
//        delete[] number;
//    }
//
//    // Оператор присваивания
//    LongNumber& operator=(const LongNumber& other) {
//        if (this != &other) {
//            delete[] number;
//            number = new int[other.size];
//            copy(other.number, other.number + other.size, number);
//            size = other.size;
//        }
//        return *this;
//    }
//
//    bool operator==(const LongNumber& other) const {
//        if (other.size == 0) {
//            if (this->size == 0 || (this->size == 1 && this->number[0] == 0)) {
//                return true;
//            }
//            return false;
//        }
//
//        if (this->size == 0) {
//            if (other.size == 1 && other.number[0] == 0) {
//                return true;
//            }
//            return false;
//        }
//
//        if (other.size != this->size) {
//            return false;
//        }
//
//        for (size_t i = 0; i < other.size; i++) {
//            if (other.number[i] != this->number[i]) return false;
//        }
//
//        return true;
//    }
//
//    bool operator!=(const LongNumber& other) const {
//        return !(this->operator==(other));
//    }
//
//    bool operator<(const LongNumber& other) const {
//        if (this->operator==(other)) {
//            return false;
//        }
//
//        if (other.size != this->size) {
//            return this->size < other.size;
//        }
//
//        for (auto i = static_cast<long long int>(other.size - 1); i >= 0; i--) {
//            if (other.number[i] != this->number[i]) {
//                return this->number[i] < other.number[i];
//            }
//        }
//
//        return false;
//    }
//
//    bool operator<=(const LongNumber& other) const {
//        return this->operator==(other) || this->operator<(other);
//    }
//
//    bool operator>(const LongNumber& other) const {
//        return !(this->operator<=(other));
//    }
//
//    bool operator>=(const LongNumber& other) const {
//        return !(this->operator<(other));
//    }
//
//    LongNumber operator+(const LongNumber& other) const {
//        LongNumber result;
//        int carry = 0;
//        result.size = max(size, other.size) + 1;
//        result.number = new int[result.size];
//
//        for (int i = 0; i < max(size, other.size); i++) {
//            int sum = carry;
//
//            if (i < size) sum += number[i];
//            if (i < other.size) sum += other.number[i];
//
//            result.number[i] = sum % 10;
//            carry = sum / 10;
//        }
//
//        if (carry != 0) result.number[max(size, other.size)] = carry;
//        else result.size--;
//
//        return result;
//    }
//
//    LongNumber operator-(const LongNumber& other) const {
//        int borrow = 0;
//        LongNumber result;
//        result.size = max(size, other.size);
//        result.number = new int[result.size]();
//
//        for (int i = 0; i < result.size; i++) {
//            int diff = borrow;
//
//            if (i < size) diff += number[i];
//            if (i < other.size) diff -= other.number[i];
//            if (diff < 0) {
//                diff += 10;
//                borrow = -1;
//            }
//            else borrow = 0;
//
//            result.number[i] = diff;
//        }
//
//        while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;
//
//        return result;
//    }
//
//    LongNumber operator*(const LongNumber& other) const {
//        LongNumber result;
//        result.size = size + other.size;
//        result.number = new int[size + other.size]();
//
//        for (int i = 0; i < size; i++) {
//            int carry = 0;
//            for (int j = 0; j < other.size; j++) {
//                int product = number[i] * other.number[j] + result.number[i + j] + carry;
//                result.number[i + j] = product % 10;
//                carry = product / 10;
//            }
//            if (carry > 0) result.number[i + other.size] += carry;
//        }
//
//        while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;
//
//        return result;
//    }
//
//    LongNumber operator/(const LongNumber& right) const {
//        LongNumber dividend = *this;
//        LongNumber divisor = right;
//
//        if (divisor == LongNumber(0)) {
//            throw invalid_argument("Division by zero");
//        }
//
//        LongNumber quotient = LongNumber(0);
//        LongNumber remainder = dividend;
//
//        while (remainder >= divisor) {
//            remainder = remainder - divisor;
//            quotient = quotient + LongNumber(1);
//        }
//
//        return quotient;
//    }
//
//    LongNumber squareRoot(LongNumber number) const {
//        LongNumber left("1");
//        LongNumber right = number;
//        LongNumber mid;
//        while (left <= right) {
//            mid = (left + right) / LongNumber(2);
//            LongNumber midSquared = mid * mid;
//            if (midSquared == number) return mid;
//            if (midSquared < number) left = mid + LongNumber(1);
//            else right = mid - LongNumber(1);
//        }
//        return right;
//    }
//
//    LongNumber operator%(const LongNumber& right) const {
//        LongNumber quotient = *this / right;
//        LongNumber result = *this - (quotient * right);
//        return result;
//    }
//
//    bool isNegative() const {
//        return number[0] < 0;
//    }
//
//    void FromStringToNumber(string str) {
//        if (number != nullptr) {
//            delete[] number;
//        }
//
//        size = str.size();
//        number = new int[size];
//
//        for (int i = 0; i < size; ++i) {
//            number[i] = str[size - 1 - i] - '0';
//        }
//    }
//
//    void FromNumberToString(int Number) {
//        if (number != nullptr) {
//            delete[] number;
//        }
//
//        string str = to_string(Number);
//        size = str.size();
//        number = new int[size];
//
//        for (int i = 0; i < size; ++i) {
//            number[i] = str[size - 1 - i] - '0';
//        }
//    }
//
//    string ConvertToString() const {
//        string str;
//        for (int i = size - 1; i >= 0; i--) {
//            str += to_string(number[i]);
//        }
//        return str;
//    }
//};
//
//int main() {
//    LongNumber num1("12345678901234567890");
//    LongNumber num2("98765432109876543210");
//
//    LongNumber sum = num1 + num2;
//    LongNumber diff = num1 - num2;
//    LongNumber prod = num1 * num2;
//    LongNumber quotient = num1 / LongNumber(2);
//    LongNumber remainder = num1 % LongNumber(2);
//
//    cout << "Sum: " << sum.ConvertToString() << endl;
//    cout << "Difference: " << diff.ConvertToString() << endl;
//    cout << "Product: " << prod.ConvertToString() << endl;
//    cout << "Quotient: " << quotient.ConvertToString() << endl;
//    cout << "Remainder: " << remainder.ConvertToString() << endl;
//
//    return 0;
//}
