#include <iostream>
#include <string>
#include <fstream>	// for files
#include <chrono>	// for time
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

	LongNumber& operator=(const LongNumber& other)
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

	bool operator==(const LongNumber& other)
	{
		// Если оба числа пустые, они равны
		if (this->size == 0 && other.size == 0)
			return true;

		// Если одно из чисел пустое, а другое нет, они не равны
		if (this->size == 0 || other.size == 0)
			return false;

		// Если размеры чисел различаются, они не равны
		if (this->size != other.size)
			return false;

		// Сравниваем каждый элемент
		for (int i = 0; i < this->size; i++)
		{
			if (this->number[i] != other.number[i])
				return false;
		}

		return true; // Все элементы совпадают
	}

	bool operator!=(const LongNumber& other)
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

		for (auto i = static_cast<long long int> (other.size - 1); i >= 0; i--)
		{
			if (other.number[i] != this->number[i])
			{
				return this->number[i] < other.number[i];
			}
		}

		return false;
	}

	bool operator<=(const LongNumber& other)
	{
		bool result = (this->operator==(other) || this->operator < (other));

		return result;
	}

	bool operator>(const LongNumber& other)
	{
		bool result = !(this->operator<=(other));

		return result;
	}

	bool operator>=(const LongNumber& other)
	{
		bool result = !(this->operator < (other));

		return result;
	}

	LongNumber operator+(const LongNumber& other) const
	{
		LongNumber result;
		size_t carry = 0;

		result.size = max(size , other.size) + 1;
		result.number = new int[result.size];

		for (int i = 0; i < max(size, other.size); i++)
		{
			size_t sum = carry;

			if (i < size)
			{
				sum += number[i];
			}

			if (i < other.size)
			{
				sum += other.number[i];
			}

			result.number[i] = sum % 10;
			carry = sum / 10;
		}

		if (carry != 0)
		{
			result.number[max(size, other.size)] = carry;
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
		int borrow = 0;
		for (int i = 0; i < result.size; i++)
		{
			int diff = borrow;
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
				borrow = -1;
			}
			else
			{
				borrow = 0;
			}

			result.number[i] = diff;
		}

		// убрали ведущие нули
		while (result.size > 1 && result.number[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator*(const LongNumber& other) const
	{
		LongNumber result;
		result.size = size + other.size;
		result.number = new int[size + other.size];

		// очищаем место для накопления сумм произведения
		for (int i = 0; i < (size + other.size); i++)
		{
			result.number[i] = 0;
		}

		for (int i = 0; i < size; i++)
		{
			size_t carry = 0;
			for (int j = 0; j < other.size; j++)
			{
				size_t product = number[i] * other.number[j] + result.number[i + j] + carry;
				result.number[i + j] = product % 10;
				carry = product / 10;
			}

			// перенос на следующий элемент после умножения всех разрядов
			if (carry > 0)
			{
				result.number[i + other.size] += carry;
			}
		}

		while (result.size > 1 && result.number[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator/(const LongNumber& other)
	{
		if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		// Если делимое меньше делителя, результат 0
		if (*this < other)
		{
			return LongNumber("0");
		}

		// Если числа равны, результат 1
		if (*this == other)
		{
			return LongNumber("1");
		}

		LongNumber result;
		LongNumber current;
		current.size = 0;
		result.size = size;
		result.number = new int[result.size]();  // Инициализируем нулями

		// Идем по цифрам делимого слева направо
		for (int i = size - 1; i >= 0; i--)
		{
			// Сдвигаем текущее число влево и добавляем новую цифру
			int* newNum = new int[current.size + 1];
			for (int j = current.size - 1; j >= 0; j--)
			{
				newNum[j + 1] = current.number[j];
			}
			newNum[0] = number[i];

			delete[] current.number;
			current.number = newNum;
			current.size++;

			// Убираем ведущие нули
			while (current.size > 1 && current.number[current.size - 1] == 0)
			{
				current.size--;
			}

			// Подбираем максимальное число раз, которое делитель входит в текущее число
			int quotient = 0;
			while (!(current < other))
			{
				current = current - other;
				quotient++;
			}

			result.number[i] = quotient;
		}

		// Убираем ведущие нули в результате
		while (result.size > 1 && result.number[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}


	LongNumber operator%(const LongNumber& other)
	{
		if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		// Если делимое меньше делителя, остаток равен делимому
		if (*this < other)
		{
			return *this;
		}

		// Если числа равны, остаток 0
		if (*this == other)
		{
			return LongNumber("0");
		}

		LongNumber current;
		current.size = 0;

		// Идем по цифрам делимого слева направо
		for (int i = size - 1; i >= 0; i--)
		{
			// Сдвигаем текущее число влево и добавляем новую цифру
			int* newNum = new int[current.size + 1];
			for (int j = current.size - 1; j >= 0; j--)
			{
				newNum[j + 1] = current.number[j];
			}
			newNum[0] = number[i];

			delete[] current.number;
			current.number = newNum;
			current.size++;

			// Убираем ведущие нули
			while (current.size > 1 && current.number[current.size - 1] == 0)
			{
				current.size--;
			}

			// Вычитаем делитель максимальное возможное число раз
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
			number[size - 1] = rand() % 9 + 1; // Генерация цифры от 1 до 9
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
		for (size_t i = size; i > 0; --i) cout << number[i-1];
		cout << endl;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");

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
	}*/

	// Будем использовать для демонстрации кода
	/*a.generateRandomNumber(8);
	a.print();

	return 0;
}
