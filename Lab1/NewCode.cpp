// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono> // for time

using namespace std;
using namespace std::chrono;

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
			remainder = remainder - divisor;
			quotient = quotient + LongNumber(1);
		}

		return quotient;
	}
	LongNumber squareRoot(LongNumber inputNumber)
	{
		LongNumber left("1");
		LongNumber right = inputNumber;
		LongNumber mid;
		while (left <= right) {
			mid = (left + right) / LongNumber(2);
			LongNumber midSquared = mid * mid;
			if (midSquared == inputNumber) return mid;
			if (midSquared < inputNumber) left = mid + LongNumber(1);
			else right = mid - LongNumber(1);
		}
		return right;
	}

	bool isNegative() const {
		return number[0] < 0;  // Проверяем знак числа по последнему элементу
	}

	LongNumber operator % (const LongNumber& right) const
	{
		LongNumber quotient = *this / right;  // Делим текущий объект на правый операнд
		LongNumber result = *this - (quotient * right);  // Остаток - разница между числом и произведением
		if (result.isNegative() == true) {
			result = result + right;  // Если остаток отрицательный, добавляем делитель, чтобы результат был положительным
		}
		return result;
	}

	void FromStringToNumber(string str)
	{
		// Задаем размер, равный длине строки
		size = str.size();
		number = new int[size];

		// Заполняем массив числами, преобразуя символы
		for (int i = 0; i < size; ++i) {
			number[i] = str[size - 1 - i] - '0';  // сохраняем цифры в обратном порядке
		}
	}


	// Преобразование числа в строку (для примера используется только положительное число)
	void FromNumberToString(int Number)
	{
		string str = to_string(Number);  // Преобразуем число в строку
		size = str.size();
		number = new int[size];

		// Заполняем массив числами
		for (int i = 0; i < size; ++i) {
			number[i] = str[size - 1 - i] - '0';  // сохраняем цифры в обратном порядке
		}
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
			while (i < this->size - 1 && this->number[i] >= 10)
			{
				this->number[i] = 0;
				i++;
				this->number[i] += 1;
			}

			// Если на последнем разряде произошел перенос
			if (i == this->size - 1 && this->number[i] >= 10)
			{
				this->number[i] = 0;

				// Увеличиваем размер массива для новой цифры
				int* newNumber = new int[this->size + 1];
				for (int j = 0; j < this->size; j++)
				{
					newNumber[j] = this->number[j];
				}

				newNumber[this->size] = 1;
				delete[] this->number;
				this->number = newNumber;
				this->size++;
			}
		}
		return *this;
	}


	// ААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААаа

	// Метод для извлечения целой части квадратного корня
	LongNumber sqrtF(LongNumber& x)
	{
		if (x < LongNumber(0))
		{
			throw std::invalid_argument("Negative argument.");
		}

		if (x == LongNumber(0) || x == LongNumber(1))
		{
			return x;
		}

		LongNumber y = x / LongNumber(2);

		while (y > x / y)
		{
			y = (x / y + y) / LongNumber(2);
		}

		return y;
	}

	// Метод для нахождения верхней границы квадратного корня
	LongNumber sqrtC(LongNumber& x)
	{
		LongNumber y = sqrtF(x);

		if (x == y * y)
		{
			return y;
		}
		else
		{
			return y + LongNumber(1);
		}
	}

	/*void FermatFactors(LongNumber& N)
	{
		if (N % 2 == 0) return;
		LongNumber a = sqrtC(N);
		LongNumber b;
		LongNumber b1;
		if (a * a == N)
		{
			a.print();
			a.print();
		}
		while (true)
		{
			b1 = a * a - N;
			b = sqrtF(b1);
			if (b * b == b1) break;
			else a = a + LongNumber(1);
		}

		(a - b).print();
		(a + b).print();
	}*/

	// АААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААа

	void fermatFactorization(LongNumber& N) {
		LongNumber x = squareRoot(N) + LongNumber(1);  // Начинаем с квадратного корня
		LongNumber y;

		while (true) {
			LongNumber xSquared = x * x;
			LongNumber diff = xSquared - N;
			y = squareRoot(diff);  // Пытаемся найти квадратный корень из разности

			if (y * y == diff) {  // Если разность — это точный квадрат
				LongNumber factor1 = x + y;
				LongNumber factor2 = x - y;
				cout << "Factors found: " << factor1.ConvertToString() << " and " << factor2.ConvertToString() << endl;
				break;  // Выход, как только нашли факторизацию
			}

			x = x + LongNumber(1);  // Увеличиваем x
		}
	}

	void print() const
	{
		for (size_t i = 0; i < size; ++i) cout << number[i];
		cout << endl;
	}
};

int main()
{
	// Засекаем время начала
	auto start = high_resolution_clock::now();

	LongNumber ln("51"); // Пример длинного числа
	ln.fermatFactorization(ln); // Вызов метода факторизации

	//ln.FermatFactors(ln);

	// Засекаем время окончания
	auto end = high_resolution_clock::now();

	// Вычисляем продолжительность в секундах
	auto duration = duration_cast<seconds>(end - start);

	// Выводим время выполнения в секундах
	cout << "Execution time: " << duration.count() << " seconds" << endl;

	return 0;
}
