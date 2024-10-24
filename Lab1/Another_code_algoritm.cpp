#include <iostream>
#include <string>
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
		if (other.size == 0)
		{
			if (this->size == 0 || (this->size == 1 && this->number[0] == 0))
			{
				return true;
			}

			return false;
		}
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

		for (size_t i = 0; i < other.size; i++)
		{
			if (other.number[i] != this->number[i]) return false;
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
		for (int i = 0; i < (size + other.size); i++)
		{
			result.number[i] = 0;
		}

		for (int i = 0; i < size; i++)
		{
			int carry = 0;
			for (int j = 0; j < other.size; j++)
			{
				int product = number[i] * other.number[j] +
					result.number[i + j] + carry;
				result.number[i + j] = product % 10;
				carry = product / 10;
			}

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

	LongNumber operator/(const LongNumber& other) const
	{
		if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		LongNumber result;
		LongNumber current;
		current.size = 0;
		result.size = this->size;
		result.number = new int[result.size]();

		for (int i = this->size - 1; i >= 0; i--)
		{
			LongNumber temp = current;
			temp.size++;
			int* newNum = new int[temp.size];
			for (int j = 0; j < temp.size - 1; j++)
			{
				newNum[j] = current.number[j];
			}

			newNum[temp.size - 1] = this->number[i];
			delete[] current.number;
			current.number = newNum;

			while (current.size > 1 && current.number[current.size - 1] == 0)
			{
				current.size--;
			}

			int count = 0;
			while (current >= other)
			{
				current = current - other;
				count++;
			}

			result.number[i] = count;
		}

		while (result.size > 1 && result.number[result.size - 1] == 0)
		{
			result.size--;
		}

		return result;
	}

	LongNumber operator%(const LongNumber& other) const
	{
		if (other.size == 0 || (other.size == 1 && other.number[0] == 0))
		{
			throw runtime_error("Division by zero");
		}

		LongNumber current;
		current.size = 0;

		for (int i = this->size - 1; i >= 0; i--)
		{
			LongNumber temp = current;
			temp.size++;
			int* newNum = new int[temp.size];
			for (int j = 0; j < temp.size - 1; j++)
			{
				newNum[j] = current.number[j];
			}
			newNum[temp.size - 1] = this->number[i];
			delete[] current.number;
			current.number = newNum;

			while (current.size > 1 && current.number[current.size - 1] == 0)
			{
				current.size--;
			}

			while (current >= other)
			{
				current = current - other;
			}
		}

		while (current.size > 1 && current.number[current.size - 1] == 0)
		{
			current.size--;
		}

		return current; 
	}


	void fromStringToNumber(const string& str)
	{
		size = str.size();
		number = new int[size];
		for (int i = 0; i < str.size(); i++)
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

	auto start = high_resolution_clock::now();

	LongNumber result;
	/*LongNumber num1 = 11;
	LongNumber num2 = 22;
	result = num1 + num2;
	result.print();*/
	LongNumber a;

	a.generateRandomNumber(8);
	a.print();

	result.fermatFactorization(a);

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds> (end - start);
	cout << "Execution time: " << duration.count() << " nanoseconds" << endl;

	return 0;
}
