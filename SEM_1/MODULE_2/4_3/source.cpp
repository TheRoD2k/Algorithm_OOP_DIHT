/*4_3 Дана последовательность целых чисел из диапазона (-1000000000 .. 1000000000). 
Длина последовательности не больше 1000000.
Числа записаны по одному в строке. Количество чисел не указано.
Пусть количество элементов n, и числа записаны в массиве a = a[i]: i из [0..n-1].
Требуется напечатать количество таких пар индексов (i,j) из [0..n-1], что (i < j и a[i] > a[j]).
Указание: количество инверсий может быть больше 4*1000000000 - используйте int64_t.


INPUT:
1
2
3
4

OUTPUT:
0

*/


#include <iostream>
#include <fstream>

long long Merge(long long *arr, int N, int left, int mid, int right, long long &count)
{
	long long *newarr1 = new long long[mid - left + 1];
	long long *newarr2 = new long long[right - mid];
	for (int i = 0; i < mid-left + 1; i++)
	{
		newarr1[i] = arr[left + i];
	}
	for (int i = 0; i < right - mid; i++)
	{
		newarr2[i] = arr[mid + 1 + i];
	}


	int ind1 = 0, ind2 = 0;
	for (int i = left; i <= right; i++)
	{
		if (left + ind1 <= mid && mid + 1 + ind2 <= right)
		{
			if (newarr1[ind1] > newarr2[ind2])
			{
				count += mid - left + 1 - ind1;
				arr[i] = newarr2[ind2];
				++ind2;
			}
			else
			{
				arr[i] = newarr1[ind1];
				++ind1;
			}
		}
		else
		{
			if (left + ind1 <= mid)
			{
				arr[i] = newarr1[ind1];
				++ind1;
			}
			else
			{
				count += mid - left + 1 - ind1;
				arr[i] = newarr2[ind2];
				++ind2;
			}

		}
	}
	delete[] newarr1;
	delete[] newarr2;
	return count;
}

long long MergeSort(long long *arr, int N, int left, int right, long long &count)
{
	if (left < right)
	{
		int mid = (left + right) / 2;
		MergeSort(arr, N, left, mid, count);
		MergeSort(arr, N, mid + 1, right, count);
		Merge(arr, N, left, mid, right, count);
	}
	else
		return arr[left];
}

long long CalculateInversions(long long *arr, int N)
{
	long long count = 0;
	MergeSort(arr, N, 0, N - 1,count);
	return count;
}

int main()
{
	std::ifstream T("input.txt");
	long long *arr = new long long[1000000];
	int N = 0;
	while (T >> arr[N++]);

	std::ofstream To("output.txt");
	long long answer = CalculateInversions(arr, N-1);
	To << answer << std::endl;
	delete[] arr;
	system("pause");
}