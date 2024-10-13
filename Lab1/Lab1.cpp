// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class LongNumber {

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
		FromStringToNumber(str);
	}

	LongNumber(int Number)
	{
		number = nullptr;
		size = 0;
		FromNumberToString(Number);
	}

	// Конструктор копирования
	LongNumber(const LongNumber& other)
	{
		number = new int[other.size];
		copy(other.number, other.number + other.size, number);		// Копируем other.number в number, начиная с адреса other.number, заканчивая other.number + other.size
		size = other.size;
	}

	~LongNumber()
	{
		delete[] number;
	}

	// Оператор присваивания
	LongNumber& operator = (const LongNumber& other)
	{
		// Проверяем на самоприсваивание 
		if (this != &other)
		{
			delete[] number;
			number = new int[other.size];
			copy(other.number, other.number + other.size, number);
			size = other.size;
		}

		return *this;
	}

	bool operator == (const LongNumber& other)
	{
		// Является ли другой объект пустым?
		if (other.size == 0)
		{
			// Проверим на равенство с пустым объектом
			// Если текущий объект тоже пуст (размер равен 0) или содержит один элемент, равный нулю, то оба объекта считаются равными, и функция возвращает true. 
			if (this->size == 0 || (this->size == 1 && this->number[0] == 0))
			{
				return true;
			}

			return false;
		}

		// Проверка, пуст ли текущий объект
		else if (this->size == 0)
		{
			if (other.size == 1 && other.number[0] == 0)
			{
				return true;
			}

			return false;
		}

		else if (other.size != this->size)
		{
			return false;
		}

		// Побайтово сравним содержимое массива
		for (size_t i = 0; i < other.size; i++)
		{
			if (other.number[i] != this->number[i]) return false;
		}

		return true;
	}

	bool operator != (const LongNumber& other)
	{
		// Вызовем оператор равенства
		// Если оператор равенства вернет true, значит оператор != вернет false 
		bool result = !(this -> operator == (other));

		return result;
	}

	bool operator < (const LongNumber& other)
	{
		// Вызовем оператор равенства
		if (this -> operator == (other))
		{
			return false;
		}


		if (other.size != this->size)
		{
			// Если размеры массивов не равны, оператор возвращает результат сравнения размеров: this -> size < other.size. 
			// То есть, если массив текущего объекта (this) меньше по размеру, чем массив объекта other, то вернем true; иначе — false.
			return this->size < other.size;
		}

		for (auto i = static_cast<long long int>(other.size - 1); i >= 0; i--)
		{
			if (other.number[i] != this->number[i])
			{
				return this->number[i] < other.number[i];
			}
		}

		return false;
	}

	bool operator <= (const LongNumber& other)
	{
		bool result = (this -> operator == (other) || this -> operator < (other));

		return result;
	}

	bool operator > (const LongNumber& other)
	{
		bool result = !(this -> operator <= (other));

		return result;
	}

	bool operator >= (const LongNumber& other)
	{
		bool result = !(this -> operator < (other));

		return result;
	}

	LongNumber operator + (const LongNumber& other) const
	{
		LongNumber result;
		int carry = 0;
		
		result.size = max(size, other.size) + 1;
		result.number = new int[result.size];

		for (int i = 0; i < max(size, other.size); i++)
		{
			int sum = carry;

			if (i < size) sum += number[i];
			if (i < other.size) sum += other.number[i];

			result.number[i] = sum % 10;
			carry = sum / 10;
		}

		if (carry != 0) result.number[max(size, other.size)] = carry;
		else result.size--;

		return result;
	}

	LongNumber operator - (const LongNumber& other) const
	{
		int borrow = 0;
		LongNumber result;
		result.size = max(size, other.size);
		result.number = new int[result.size]();

		for (int i = 0; i < result.size; i++)
		{
			int diff = borrow;

			if (i < size) diff += number[i];
			if (i < other.size) diff -= other.number[i];
			if (diff < 0)
			{
				diff += 10;
				borrow = -1;
			}
			else borrow = 0;

			result.number[i] = diff;
		}

		while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;

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
			int carry = 0;
			for (int j = 0; j < other.size; j++)
			{
				int product = number[i] * other.number[j] + result.number[i + j] + carry;
				result.number[i + j] = product % 10;
				carry = product / 10;
			}
			if (carry > 0) result.number[i + other.size] += carry;
		}

		while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;

		return result;
	}

	// Метод для деления
	LongNumber operator / (const LongNumber& right) const {
		// Предположим, что у нас есть вспомогательная функция для выполнения деления
		LongNumber dividend = *this;  // Текущее число (левая часть операнда)
		LongNumber divisor = right;   // Делитель (правая часть операнда)

		if (divisor == LongNumber(0)) {  // Проверка на деление на ноль
			throw std::invalid_argument("Division by zero");
		}

		LongNumber quotient = LongNumber(0);  // Результат деления (целая часть)
		LongNumber remainder = dividend;      // Остаток

		// Алгоритм деления (например, с использованием вычитания)
		while (remainder >= divisor) {
			remainder = remainder + divisor;
			quotient = quotient + LongNumber(1);
		}

		// Если результат отрицателен, применяем знак
		if ((dividend.isNegative() && !divisor.isNegative()) ||
			(!dividend.isNegative() && divisor.isNegative())) {
			quotient = quotient;
		}

		return quotient;
	}

	// нужна для ферма
	LongNumber squareRoot(LongNumber number)
	{
		LongNumber i("1");

		while (i <= number)
		{
			if ((i * i) >= number)	return i;
			else i = i + LongNumber("1");
		}
	}

	bool isNegative() const {
		return number[size - 1] < 0;  // Проверяем знак числа по последнему элементу
	}

	// Метод для взятия остатка
	LongNumber operator % (const LongNumber& right) const
	{
		LongNumber quotient = *this / right;  // Делим текущий объект на правый операнд
		LongNumber result = *this - (quotient * right);  // Остаток - разница между числом и произведением
		if (result.isNegative() == true) {
			result = false;  // Если остаток отрицательный, добавляем делитель, чтобы результат был положительным
		}
		return result;
	}

	void FromStringToNumber(const string& str)
	{
		int start = 0;
		size = str.size();
		number = new int[size];

		for (int i = 0; i < str.size(); i++) number[i] = str[i] - '0';

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

	void FromNumberToString(int number)
	{
		string str = to_string(number);
		FromStringToNumber(str);
	}

	string ConvertToString()
	{
		string str;
		for (int i = size - 1; i >= 0; i--)
		{
			str += to_string(number[i]);
		}

		return str;
	}

	int getLeastSignificantDigit()
	{
		int leastSignificantDigit = number[0];

		return leastSignificantDigit;
	}

	LongNumber roundingup(const LongNumber& number)
	{
		int i = 0;
		if (this->size > 1 || this->number[0] > 0)
		{
			// Добавляем 1 к числу, начиная с младшего разряда
			this->number[i] += 1;

			// Обработка переноса
			while (this->number[i] >= 10 && i < this->size - 1)
			{
				this->number[i] = 0;
				i++;
				this->number[i] += 1;
			}
		}

		// Если на последнем разряде произошел перенос
		if (i == this->size - 1 && this->number[i] >= 10)
		{
			this->number[i] = 0;

			// Увеличиваем размер массива для новой цифры
			int* newNumber = new int[this->size + 1];
			for (int j = 0; j < this->size; j++)
			{
				newNumber[j] = this->number[i];
			}

			newNumber[this->size] = 1;
			delete[] this->number;
			this->number = newNumber;
			this->size++;
		}
		return *this;
	}

	void fermatFactorization(LongNumber count) {
		// Проверим, что n нечетное, так как метод Ферма работает только с нечетными числами
		/*if (count % 2 == 0) {
			std::cout << "There should only be an odd number" << std::endl;
			return;
		}*/
		
		// Поменять на функции, реализованные в этом классе
		LongNumber x = roundingup(squareRoot(count));
		LongNumber y2 = x * x - count;
		LongNumber y = roundingup(squareRoot(y2));

		// Пока y^2 не является точным квадратом
		while (y * y != y2) {
			x = x + 1;
			y2 = x * x - count;
			y = roundingup(squareRoot(y2));
		}

		LongNumber factor1 = x - y;
		LongNumber factor2 = x + y;

		// std::cout << "Multipliers of a number: " << count << ": " << factor1 << " and " << factor2 << std::endl;
		count.print();
		factor1.print();
		factor2.print();
	}

	// надо оставить 
	void print() const {
		for (size_t i = 0; i < size; ++i) {
			cout << number[i] << " ";
		}
		cout << endl;
	}
};

int main()
{
	LongNumber number;

	LongNumber ln("101"); // Пример длинного числа
	ln.fermatFactorization(number); // Вызов метода факторизации

	return 0;
}
