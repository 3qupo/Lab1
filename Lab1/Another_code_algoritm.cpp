#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>	// for time
#include <cstdlib> // Для rand и srand

using namespace std;
using namespace std::chrono;

class LongNumber
{
private:
	int* num;
	int size;

public:
	LongNumber()
	{
		num = nullptr;
		size = 0;
	}

	LongNumber(string str)
	{
		num = nullptr;
		size = 0;
		fromString(str);
	}

	LongNumber(int n)
	{
		num = nullptr;
		size = 0;
		fromInt(n);
	}

	LongNumber(const LongNumber& other)
	{
		num = new int[other.size];

		copy(other.num, other.num + other.size, num);
		size = other.size;
	}

	~LongNumber()
	{
		delete[] num;
	}

	LongNumber& operator=(const LongNumber& other)
	{
		if (this != &other)
		{
			delete[] num;

			num = new int[other.size];

			copy(other.num, other.num + other.size, num);
			size = other.size;
		}

		return *this;
	}

	bool operator==(const LongNumber& other)
	{
		if (other.size == 0)
		{
			if (this->size == 0 || (this->size == 1 && this->num[0] == 0))
			{
				return true;
			}

			return false;
		}
		else if (this->size == 0)
		{
			if (other.size == 1 && other.num[0] == 0)
			{
				return true;
			}

			return false;
		}
		else if (other.size != this->size)
		{
			return false;
		}

		for (size_t i = 0; i < other.size; i++)
		{
			if (other.num[i] != this->num[i]) return false;
		}

		return true;
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
			if (other.num[i] != this->num[i])
			{
				return this->num[i] < other.num[i];
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
		int carry = 0;

		result.size = max(size, other.size) + 1;
		result.num = new int[result.size];

		for (int i = 0; i < max(size, other.size); i++)
		{
			int sum = carry;

			if (i < size)
			{
				sum += num[i];
			}

			if (i < other.size)
			{
				sum += other.num[i];
			}

			result.num[i] = sum % 10;
			carry = sum / 10;
		}

		if (carry != 0)
		{
			result.num[max(size, other.size)] = carry;
		}
		else
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator-(const LongNumber& other) const
	{
		LongNumber result;
		result.size = max(size, other.size);
		result.num = new int[result.size]();
		int borrow = 0;
		for (int i = 0; i < result.size; i++)
		{
			int diff = borrow;
			if (i < size)
			{
				diff += num[i];
			}

			if (i < other.size)
			{
				diff -= other.num[i];
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

			result.num[i] = diff;
		}

		while (result.size > 1 && result.num[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator*(const LongNumber& other) const
	{
		LongNumber result;
		result.size = size + other.size;
		result.num = new int[size + other.size];
		for (int i = 0; i < (size + other.size); i++)
		{
			result.num[i] = 0;
		}

		for (int i = 0; i < size; i++)
		{
			int carry = 0;
			for (int j = 0; j < other.size; j++)
			{
				int product = num[i] * other.num[j] +
					result.num[i + j] + carry;
				result.num[i + j] = product % 10;
				carry = product / 10;
			}

			if (carry > 0)
			{
				result.num[i + other.size] += carry;
			}
		}

		while (result.size > 1 && result.num[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator/(const LongNumber& other) const
	{
		if (other.size == 0 || (other.size == 1 && other.num[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		LongNumber result;
		LongNumber current;
		current.size = 0;
		result.size = this->size;
		result.num = new int[result.size]();

		for (int i = this->size - 1; i >= 0; i--)
		{
			// Добавляем текущий разряд к делимому
			LongNumber temp = current;
			temp.size++;
			int* newNum = new int[temp.size];
			for (int j = 0; j < temp.size - 1; j++)
			{
				newNum[j] = current.num[j];
			}

			newNum[temp.size - 1] = this->num[i];
			delete[] current.num;
			current.num = newNum;

			// Убираем ведущие нули
			while (current.size > 1 && current.num[current.size - 1] == 0)
			{
				current.size--;
			}

			// Находим сколько раз other может быть вычтено из current
			int count = 0;
			while (current >= other)
			{
				current = current - other;
				count++;
			}

			result.num[i] = count;
		}

		// Убираем ведущие нули в результате
		while (result.size > 1 && result.num[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator%(const LongNumber& other) const
	{
		if (other.size == 0 || (other.size == 1 && other.num[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		LongNumber current;
		current.size = 0;

		for (int i = this->size - 1; i >= 0; i--)
		{
			// Добавляем текущий разряд к делимому
			LongNumber temp = current;
			temp.size++;
			int* newNum = new int[temp.size];
			for (int j = 0; j < temp.size - 1; j++)
			{
				newNum[j] = current.num[j];
			}
			newNum[temp.size - 1] = this->num[i];
			delete[] current.num;
			current.num = newNum;

			// Убираем ведущие нули
			while (current.size > 1 && current.num[current.size - 1] == 0)
			{
				current.size--;
			}

			// Вычитаем other из current, пока current >= other
			while (current >= other)
			{
				current = current - other;
			}
		}

		// Убираем ведущие нули
		while (current.size > 1 && current.num[current.size - 1] == 0)
		{
			current.size--;
		}

		return current; // Остаток от деления
	}


	void fromString(const string& str)
	{
		size = str.size();
		num = new int[size];
		for (int i = 0; i < str.size(); i++)
		{
			num[i] = str[i] - '0';
		}

		int start = 0;
		int end = size - 1;
		while (start < end)
		{
			int temp = num[start];
			num[start] = num[end];
			num[end] = temp;
			start++;
			end--;
		}
	}

	void fromInt(int n)
	{
		string str = to_string(n);
		fromString(str);
	}

	string toString() const
	{
		string str;
		for (int i = size - 1; i >= 0; i--)
		{
			str += to_string(num[i]);
		}

		return str;
	}

	int endelim()
	{
		int b = num[0];
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
		delete[] num; // Очищаем память для нового числа
		size = length;
		num = new int[size];

		// Инициализируем генератор случайных чисел
		srand(time(0));

		// Генерация случайного длинного числа
		for (int i = 0; i < size; i++)
		{
			num[i] = rand() % 10; // Генерация цифры от 0 до 9
		}

		// Проверяем, чтобы старшая цифра (последняя в массиве) не была нулем
		while (num[size - 1] == 0)
		{
			num[size - 1] = rand() % 9 + 1; // Генерация цифры от 1 до 9
		}
	}


	void fermatFactorization(LongNumber& n)
	{
		/*if (n % 2 == 0)
		{
			cout << "Должно быть нечетное число" << endl;
			return;
		}*/
		// Инициализация переменных
		LongNumber x = n.sqrt1(n);	// Приближенное значение для корня из n
		if (x * x == n)
		{
			// Если n — это точный квадрат
			cout << "Factors: " << x.toString() << " *" << x.toString() << endl;
			return;
		}

		LongNumber y("0");	// Начальное значение для y
		LongNumber x_squared = x * x;
		LongNumber y_squared = y * y;
		LongNumber diff = x_squared - y_squared;	// Разность квадратов

		// Пока разность квадратов не станет равна n
		while (diff != n)
		{
			if (diff > n)
			{
				// Если разность больше n, увеличиваем y
				y = y + LongNumber("1");
				y_squared = y * y;	// Обновляем y^2
			}
			else
			{
				// Иначе увеличиваем x
				x = x + LongNumber("1");
				x_squared = x * x;	// Обновляем x^2
			}

			// Пересчитываем разность квадратов
			diff = x_squared - y_squared;
		}

		// Если мы вышли из цикла, значит нашли разложение
		LongNumber factor1 = x - y;
		LongNumber factor2 = x + y;
		cout << "Factors: " << factor1.toString() << " * " << factor2.toString() << endl;
	}

	void print() const
	{
		for (size_t i = 0; i < size; ++i) cout << num[i];
		cout << endl;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");

	auto start = high_resolution_clock::now();

	LongNumber a = 1940031;
	LongNumber result;

	/*a.generateRandomNumber(5);
	a.print();*/

	result.fermatFactorization(a);
	// Засекаем время окончания
	auto end = high_resolution_clock::now();

	// Вычисляем продолжительность в секундах
	auto duration = duration_cast<seconds> (end - start);

	// Выводим время выполнения в секундах
	cout << "Execution time: " << duration.count() << " seconds" << endl;

	return 0;
}
