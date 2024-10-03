#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>


// �� ��������� (������������� ������)
// ��������� �����
// ����������� �� �����
// �������� ������������

using namespace std;

struct  BigNumber
{
    static const long long BASE = 1e9;             //������ �������
    static const long long SIZE = 100000000;       //���������� ��������� ��������

    unsigned long long digits[SIZE];

    BigNumber()                                    //����������� ����������� ����� ��������
    {                                  
        for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }
    }

    BigNumber(long long BigNumber)                        //����������� ��� �������������� �������� ����� � ������� ����� ��������
    {
        /*for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }*/

        int next = 0;
        while (BigNumber)
        {
            digits[next++] = BigNumber % BASE;
            BigNumber /= BASE;
        }
    }

    BigNumber(BigNumber& other)
    {               //����������� ����������� ����� ��������
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }
    }

    BigNumber& operator = (BigNumber& other) {    //�������� ������������
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }
        return *this;
    }

    void operator+=(BigNumber& other) {
        for (int i = 0; i < SIZE; i++) {        //������� ������ ����� ����������,
            digits[i] += other.digits[i];       //��������� ������������
        }

        for (int i = 0; i < SIZE - 1; i++) {    //� ����� ���������� �������� ��������
            if (digits[i] >= BASE) {            //��� ������� �������
                digits[i] -= BASE;
                digits[i + 1]++;
            }
        }
    }

};

// ���������� ����� ������� �����
unsigned long long Addition()
{
    int size_a;
    int size_b;
    unsigned long long length;
    int b[1000000];
    int a[1000000];

    if (size_a > size_b) length = size_a + 1;
    else length = size_b + 1;

    for (int ix = 0; ix < length; ix++)
    {
        b[ix] += a[ix]; // ��������� ��������� ������� �����
        b[ix + 1] += (b[ix] / 10); // ���� ���� ������ ��� ��������, ��������� ��� � ��������� ������
        b[ix] %= 10; // ���� ���� ������ ��� �������� �� ����������
    }

    if (b[length - 1] == 0)
    length--;

    return length;
}

unsigned long long Subtraction()
{
    int k = 3; // ���� � == 3, ������ ����� ���������� ������
    int length;
    int size_a;
    int size_b;
    int a[1000000];
    int b[1000000];

    length = size_a;
    if (size_a > size_b)
    {
        length = size_a;
        k = 1; // ���� � == 1, ������ ������ ����� ������� �������
    }
    else
    {
        if (size_b > size_a)
        {
            length = size_b;
            k = 2; // ���� � == 2, ������ ������ ����� ������� �������
        }
        else // ���� ����� ���������� ������, �� ���������� �������� �� ����
        {
            for (int ix = 0; ix < length;) // ����������� ��������� ����� �����
            {
                if (a[ix] > b[ix]) // ���� ������ ������� ����� ������
                {
                    k = 1; // ������ ������ ����� ������� �������
                    break; // ����� �� ����� for
                }

                if (b[ix] > a[ix]) // ���� ������ ������� ����� ������
                {
                    k = 2; // ������ ������ ����� ������� �������
                    break; // ����� �� ����� for
                }
            }
        }
    }
}

int Difference(int* x, int* y, int* z, int length)
{
    for (int ix = 0; ix < (length - 1); ix++) // ������ �� ���� �������� �����, ������� � ����������, �� ������ �� �������
    {
        if (ix < (length - 1)) // ���� ������� ������ ����� �� ������
        {
            x[ix + 1]--; // � ���������� ������� �������� ����� �������� 1.
            z[ix] += 10 + x[ix]; // � ����� ���������� ����� �������� �������� ������� �������� ����� � 10-��

        }
        else  // ���� ������� ������ ����� - ������
        {
            z[ix] += x[ix]; // � ����� ��������� �������� �������� ������� �������� �����
        }

        z[ix] -= y[ix]; // �������� �������� �������� ������� �������� �����

        if (z[ix] / 10 > 0) // ���� �������� � ������� ������� �������������
        {
            z[ix + 1]++; // ��������� ������� � ������� ������
            z[ix] %= 10; // � ������� ������� �������� ��
        }
    }

    return x - y;
}

int Multiplication()
{
    int length;
    int size_a = 0;
    int size_b = 0;
    int a[1000000];
    int b[1000000];
    int c[1000000];

    length = size_a + size_b + 1;

    for (int ix = 0; ix < size_a; ix++)
    {
        for (int jx = 0; jx < size_b; jx++)
        {
            c[ix + jx - 1] += a[ix] * b[jx];
        }
    }

    for (int ix = 0; ix < length; ix++)
    {
        c[ix + 1] += c[ix] / 10;
        c[ix] %= 10;
    }

    while (c[length] == 0) length--;

}