/*Наиыстрейшая сортировка.
Дан массив целых чисел в диапазоне [0..10^9]. 
Размер массива кратен 10 и ограничен сверху значением 2.5 * 106 элементов. 
Все значения массива являются элементами псевдо-рандомной последовательности. 
Необходимо отсортировать элементы массива за минимальное время и вывести каждый десятый элемент отсортированной последовательности.
Реализуйте сортировку, основанную на QuickSort.
Минимальный набор оптимизаций, который необходимо реализовать:
1. Оптимизация ввода/вывода
2. Оптимизация выбора опорного элемента
3. Оптимизация Partition
4. Написать без рекурсии
5. Оптимизация концевой рекурсии

НЕТ ОГРАНИЧЕНИЙ НА АЛГОРИТМЫ СОРТИРОВКИ
*/


#include <cstdio>


void swp(int &l, int &r)
{
	int tmp = l;
	l = r;
	r = tmp;
}


void Sort(int *(*arr), int N, int *sortarr)
{
	int deg = 255;
	int *sorted = new int[N];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j <= 256; j++)
		{
			sortarr[j] = 0;
		}

		for (int j = 0; j < N; j++)
		{
			++(sortarr[(((*arr)[j] & deg) >> i * 8) + 1]);
		}

		for (int j = 1; j <= 256; j++)
		{
			sortarr[j] += sortarr[j - 1];
		}
		for (int j = 0; j < N; j++)
		{
			sorted[sortarr[((*arr)[j] & deg) >> i * 8]] = (*arr)[j];
			++sortarr[((*arr)[j] & deg) >> i * 8];
		}
		deg *= 256;
		int *temp = *arr;
		*arr = sorted;
		sorted = temp;
	}
	delete[] sorted;
}


int main()
{
	int *arr = new int[25000000];

	int N = 0;
	int c, temp = 0;
	while ((c = getchar_unlocked()) != '\n')
	{
		if (!(c >= '0' && c <= '9'))
		{
			arr[N] = temp;
			N++;
			temp = 0;
		}
		else
		{
			temp*=10;
			temp += c - '0';
			arr[N] = temp;
		}
	}
	++N;
	int *sortarr = new int[256];
	Sort(&arr, N,sortarr);

	for (int i = 9; i < N; i += 10)
	{
        int temp = arr[i], deg = 1;
        while (temp / deg > 0)
          {deg *= 10;}
        deg/=10;
        while (deg > 0)
        {
          putchar_unlocked(temp/deg + '0');
          temp %= deg;
          deg /=10;
        }
		putchar_unlocked(' ');
	}

	delete[] arr;
}