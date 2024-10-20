//// This is a personal academic project. Dear PVS-Studio, please check it.
//// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//#include <iostream>
//#include <algorithm> // copy + max and min, but i using another max and min
//
//#include <random>
//#include <chrono>
//#include <string>
//#include <sstream>
//
//using namespace std;
//
//class LongNumber
//{
//private:
//	int* number;		// указатель на длинное число
//	int size;			// размер длинного числа
//
//public:
//	// Конструктор по умолчанию
//	LongNumber() : number(nullptr), size(0) {}
//
//	// Конструктор из строки
//	LongNumber(const string& str)
//	{
//		FromStringToNumber(str);
//	}
//
//	// Конструктор из целого числа
//	LongNumber(int Number)
//	{
//		FromNumberToString(Number);
//	}
//
//	// Конструктор копирования
//	LongNumber(const LongNumber& other)
//	{
//		size = other.size;
//		number = new int[size];
//		for (int i = 0; i < size; i++)
//		{
//			number[i] = other.number[i];
//		}
//	}
//
//	// Деструктор
//	~LongNumber()
//	{
//		delete[] number;
//	}
//
//	void FromStringToNumber(const string& str)
//	{
//		size = str.size();
//		number = new int[size];
//
//		for (int i = 0; i < size; i++)
//		{
//			number[i] = str[size - i - 1] - '0';
//		}
//	}
//
//	void FromNumberToString(int Number)
//	{
//		string str = to_string(Number);
//		size = str.size();
//		number = new int[size];
//
//		for (int i = 0; i < size; i++)
//		{
//			number[i] = str[size - i - 1] - '0';
//		}
//	}
//
//	LongNumber& operator = (const LongNumber& other)
//	{
//		if (this == &other) return *this;			// Проверили на самоприсваивание
//		delete[] number;
//		size = other.size;
//		number = new int[size];
//		copy(other.number, other.number + other.size, number);
//		return *this;
//	}
//	
//	int my_max(int a, int b) { return (a > b) ? a : b; }
//	int my_min(int a, int b) { return (a < b) ? a : b; }
//
//	LongNumber operator + (const LongNumber& other)
//	{
//		int maxSize = my_max(size, other.size) + 1;			// + 1, т.к. возможно переполнение
//		LongNumber result;
//		result.size = maxSize;
//		result.number = new int[result.size]();				// Выделили память для нового числа
//
//		int carry = 0;
//		int i = 0;
//
//		while (i < size || i < other.size || carry)
//		{
//			int sum = carry;										// Начинаем с остаточного переноса
//			if (i < size) sum = sum + number[i];					// Добавляем цифру из текущего разряда
//			if (i < other.size) sum = sum + other.number[i];		// Добавляем цифру из другого числа
//			carry = sum / 10;										// Для расчета нового переноса
//			result.number[i] = sum % 10;							// Запись остатка в новый объект
//			i = i + 1;
//		}
//
//		// Что-то говорилось про ведущие нули, но нужно ли их убирать?
//		while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;		// Убираем ведущие нули
//		return result;
//	}
//
//	LongNumber operator - (const LongNumber& other)
//	{
//		int maxSize = my_max(size, other.size);
//		int minSize = my_min(size, other.size);
//		bool isNegative = false;
//		int borrow = 0;
//		int digit1 = 0;
//		int digit2 = 0;
//		int diff = 0;
//
//		LongNumber result;
//		result.size = maxSize;
//		result.number = new int[result.size];
//
//		for (int i = 0; i < maxSize; i++)
//		{
//			digit1 = (i < size) ? number[i] : 0;			// Разряд первого числа
//			digit2 = (i < other.size) ? other.number[i] : 0;		// Разряд второго числа
//			diff = digit1 - digit2 - borrow;
//			if (diff < 0)
//			{
//				diff += 10;
//				borrow = 1;
//			}
//			else { borrow = 0; }
//			result.number[i] = diff;
//		}
//
//		if (borrow == 1) isNegative = true;					// Проверка на отрицательность результата (разобраться в проблеме isNegative)
//		return result;
//	}
//
//	LongNumber operator * (const LongNumber& other)
//	{
//		LongNumber result;
//		result.size = this->size + other.size;				// Результат будет иметь максимальный результат
//		result.number = new int[result.size]();				// Инициализируем массив нулями
//		
//		for (int i = 0; i < this->size; i++)
//		{
//			for (int j = 0; j < other.size; j++)
//			{
//				result.number[i + j] = result.number[i + j] + this->number[i] * other.number[j];
//				if (result.number[i + j] >= 10)
//				{
//					result.number[i + j + 1] = result.number[i + j + 1] + result.number[i + j] / 10;			// Перенос
//					result.number[i + j] = result.number[i + j] % 10;										// Оставляем остаток
//				}
//			}
//		}
//
//		while (result.size > 1 && result.number[result.size - 1] == 0) result.size--;
//		return result; 
//	}
//
//	LongNumber operator / (const LongNumber& other)
//	{
//		if (other.size == 0 || other.number == nullptr) throw invalid_argument("Error!");
//
//		LongNumber result;			
//		LongNumber currentDividend;			// Текущая часть делимого
//		LongNumber zero("0");
//		int quotientDigit = 0;
//
//		result.size = size;
//		result.number = new int[result.size](); 	// Инициализация нулями
//		for (int i = size - 1; i >= 0; i--)
//		{
//			currentDividend = currentDividend * 10 + number[i];				// Добавляем текущий разряд к делимому 
//			quotientDigit = 0;
//			// Поиск максимального числа, на которое можно поделить
//			while (currentDividend >= other)
//			{
//				currentDividend = currentDividend - other;
//				quotientDigit++;
//			}
//			result.number[i] = quotientDigit;
//		}
//
//		result.RemoveLeadingZeros();
//		return result;
//	}
//
//	LongNumber operator ++ ()
//	{
//		LongNumber one("1");
//		*this = *this + one;   // Увеличиваем текущее число на 1
//		return *this;          // Возвращаем обновленное число
//	}
//
//	LongNumber operator -- ()
//	{
//		LongNumber one("1");
//		*this = *this - one;   // Уменьшаем текущее число на 1
//		return *this;          // Возвращаем обновленное число
//	}
//
//	bool operator >= (const LongNumber& other) const
//	{
//		LongNumber a = *this;
//		LongNumber b = other;
//
//		a.RemoveLeadingZeros();
//		b.RemoveLeadingZeros();
//
//		if (a.size > b.size) return true;
//		if (a.size < b.size) return false;
//		for (int i = a.size - 1; i >= 0; i--)
//		{
//			if (a.number[i] > b.number[i]) return true;
//			if (a.number[i] < b.number[i]) return false;
//		}
//		return true;
//	}
//
//	bool operator < (const LongNumber& other) const
//	{
//		LongNumber a = *this;
//		LongNumber b = other;
//
//		a.RemoveLeadingZeros();
//		b.RemoveLeadingZeros();
//
//		if (a.size < b.size) return true;
//		if (a.size > b.size) return false;
//		for (int i = a.size - 1; i >= 0; i--)
//		{
//			if (a.number[i] < b.number[i]) return true;
//			if (a.number[i] > b.number[i]) return false;
//		}
//		return false;
//	}
//
//	bool operator <= (const LongNumber& other) const
//	{
//		LongNumber a = *this;
//		LongNumber b = other;
//
//		a.RemoveLeadingZeros();
//		b.RemoveLeadingZeros();
//
//		if (size < other.size) return true;
//		if (size > other.size) return false;
//		for (int i = size - 1; i >= 0; i--)
//		{
//			if (number[i] > other.number[i]) return true;
//			if (number[i] < other.number[i]) return false;
//		}
//		return true;
//	}
//
//	bool operator > (const LongNumber& other) const
//	{
//		if (size > other.size) return true;
//		if (size < other.size) return false;
//		for (int i = size - 1; i >= 0; i--)
//		{
//			if (number[i] > other.number[i]) return true;
//			if (number[i] < other.number[i]) return false;
//		}
//		return false;
//	}
//
//	bool operator != (const LongNumber& other) const
//	{
//		LongNumber a = *this;
//		LongNumber b = other;
//
//		a.RemoveLeadingZeros();
//		b.RemoveLeadingZeros();
//
//		return !(*this == other);														// Костыль?
//	}
//
//	bool operator == (const LongNumber& other) const
//	{
//		LongNumber a = *this;
//		LongNumber b = other;
//
//		a.RemoveLeadingZeros();
//		b.RemoveLeadingZeros();
//
//		if (a.size != b.size) return false;        // Разные размеры => не равны
//
//		for (int i = 0; i < a.size; i++)
//		{
//			if (a.number[i] != b.number[i]) return false;
//		}
//		return true;
//	}
//
//	LongNumber operator % (const LongNumber& divisor) const
//	{
//		if (divisor == LongNumber("0")) throw invalid_argument("Division by zero");
//		LongNumber dividend = *this;														// Копия текущего числа
//		LongNumber remainder("0");															// Остаток
//		LongNumber current;																	// Текущий делитель
//		LongNumber div = divisor;
//		dividend.RemoveLeadingZeros();
//		div.RemoveLeadingZeros();
//
//		if (dividend < divisor) return dividend;											// Если делимое меньше делителя, остаток равен делимому
//		for (int i = dividend.size - 1; i >= 0; i--)
//		{
//			current = remainder * 10 + LongNumber(to_string(dividend.number[i]));			// Добавляем очередную цифру к остатку
//			remainder = current;
//
//			while (remainder >= div) remainder = remainder - div;							// Вычитаем делитель, пока остаток больше делителя
//		}
//		remainder.RemoveLeadingZeros();                    
//		return remainder;
//	}
//
//	void RemoveLeadingZeros()
//	{
//		while (size > 1 && number[size - 1] == 0) size--;
//	}
//
//	LongNumber Sqrt(const LongNumber& num) const
//	{
//		LongNumber left("0");
//		LongNumber right = num;
//		LongNumber one("1");
//		LongNumber mid;
//
//		while (right - left > one)
//		{
//			mid = (left + right) / 2;
//			if (mid * mid <= num)
//			{
//				left = mid;
//			}
//			else
//			{
//				right = mid;
//			}
//		}
//		return left;
//	}
//
//	LongNumber roundingup(const LongNumber& num)
//	{
//		int i = 0;
//		// Если число уже состоит более чем из одного разряда или хотя бы один разряд больше 0
//		if (this->size > 0 && this->number[0] > 0)
//		{
//			// Увеличиваем первый разряд
//			this->number[i]++;
//
//			// Проходим по всем разрядам для обработки переноса
//			while (this->number[i] >= 10)
//			{
//				this->number[i] = 0;
//				i++;
//				if (i < this->size) this->number[i]++;
//				else
//				{
//					int* newNumber = new int[this->size + 1];
//					for (int j = 0; j < this->size; j++) newNumber[j] = this->number[j];
//					newNumber[this->size] = 1;
//					delete[] this->number;
//					this->number = newNumber;
//					this->size++;
//					break;
//				}
//			}
//
//		}
//		return *this;
//	}
//
//	LongNumber roundingdown(const LongNumber& num)
//	{
//		LongNumber result = num;
//		result.RemoveLeadingZeros();
//		if (result.size > 1)
//		{
//			for (int i = 1; i < result.size; i++) result.number[i] = 0;
//		}
//
//		return result;
//	}
//
//	string GetNumberAsString() const
//	{
//		string result = "";
//		for (int i = size - 1; i >= 0; i--)
//		{
//			result += (number[i] + '0');  // Преобразуем цифру в символ
//		}
//		return result;
//	}
//
//	void GenerateRandomLongNumber(int length)
//	{
//		if (length <= 0)
//		{
//			throw invalid_argument("Length must be greater than 0");
//		}
//
//		size = length;
//		delete[] number; // Очищаем предыдущее число, если оно было
//		number = new int[size];
//
//		// Используем генератор случайных чисел
//		random_device rd;
//		mt19937 gen(rd());
//		uniform_int_distribution<> dist(0, 9);
//
//		// Первая цифра не может быть нулём, чтобы число не начиналось с 0
//		number[size - 1] = dist(gen) % 9 + 1;
//
//		// Генерация остальных цифр
//		for (int i = size - 2; i >= 0; i--)
//		{
//			number[i] = dist(gen);
//		}
//
//		// Если последнее число чётное, делаем его нечётным
//		if (number[size - 1] % 2 == 0)
//		{
//			number[size - 1] = dist(gen) % 2 == 0 ? dist(gen) % 9 + 1 : dist(gen) % 9 + 2;
//		}
//	}
//
//	LongNumber fermatFactorization(const LongNumber& result)
//	{
//		if (result % 2 == 0) {
//			cout << "Number must be odd" << endl;
//			return *this;
//		}
//
//		LongNumber x = roundingup(Sqrt(result));
//		LongNumber y2 = x * x - result;
//
//		cout << "Initial x: "; x.print(); cout << endl;
//		cout << "Initial y2: "; y2.print(); cout << endl;
//
//		while (Sqrt(y2) != roundingdown(Sqrt(y2))) {
//			x = x + 1;
//			y2 = x * x - result;
//
//			// Отладочный вывод значений на каждом шаге
//			cout << "x: "; x.print(); cout << endl;
//			cout << "y2: "; y2.print(); cout << endl;
//		}
//
//		LongNumber y = Sqrt(y2);
//		LongNumber factor1 = x - y;
//		LongNumber factor2 = x + y;
//
//		cout << "Factor 1: "; factor1.print(); cout << endl;
//		cout << "Factor 2: "; factor2.print(); cout << endl;
//
//		return *this;
//	}
//
//	void print()
//	{	
//		if (size == 0 || number == nullptr)
//		{
//			cout << "0";
//			return;
//		}
//		for (int i = size - 1; i >= 0; i--) cout << number[i];
//	}
//	
//};
//    
//int main()
//{
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_int_distribution<> dis(1000, 100000); // Random number range
//
//	int randomNum = dis(gen);  // Generate a random number
//	LongNumber N(randomNum);  // Create LongNumber object
//
//	cout << "Result";
//	N.print(); 
//
//	fermatFactorization(N);
//	return 0;
//}
