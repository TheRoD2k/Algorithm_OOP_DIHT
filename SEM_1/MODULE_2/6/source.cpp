/*Быстрейшая сортировка.
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

ТОЛЬКО QS, см. задачу 7!
*/


#include <iostream>
#include <fstream>
#include <random>

void swp(int &l, int &r)
{
	int tmp = l;
	l = r;
	r = tmp;
}

void Partition(int *arr, int left, int right)
{
		int med = (left + right) / 2;


		int it1 = left, it2 = right;
		int pivot = arr[med];

		while (it1 <= it2)
		{
			while (arr[it1] < pivot)
				++it1;
			while (arr[it2] > pivot)
				--it2;

			if (it1 <= it2) 
				swp(arr[it1++], arr[it2--]);
		}
		
		if (left < it2 && right > it1)
		{
			return Partition(arr, left, it2), Partition(arr, it1, right);
		}
		else
			if (left < it2)
				return Partition(arr, left, it2);
			else
				if (right > it1)
					return Partition(arr, it1, right);
}

void QuickSort(int *arr, int N)
{
	Partition(arr, 0, N - 1);
}

int main()
{
	std::ifstream T("input.txt");
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	int *arr = new int[25000000];

	int N = 0;
	while (T >> arr[N])
	{
		++N;
	}
	QuickSort(arr, N);

	std::ofstream To("output.txt");

	for (int i = 9; i < N; i += 10)
	{
		To << arr[i] << " ";
	}

	delete[] arr;
	system("pause");
}