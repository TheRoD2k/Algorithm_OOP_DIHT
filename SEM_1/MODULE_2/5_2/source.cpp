/*5_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. 
Количество чисел не больше 106. 
Отсортировать массив методом поразрядной сортировки LSD по байтам.


INPUT:
3
4 1000000 7

OUTPUT:
4 7 1000000

*/

#include <iostream>



void Sort(long long *(*arr), int N, int *sortarr)
{
	long long deg = 255;
	long long *sorted = new long long[N];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j <= 256; j++)
		{
			sortarr[j] = 0;
		}
		
		for (int j = 0; j < N; j++)
		{
			++(sortarr[(((*arr)[j] & deg) >> i*8) + 1]);
		}

		for (int j = 1; j <= 256; j++)
		{
			sortarr[j] += sortarr[j - 1];
		}
		for (int j = 0; j < N; j++)
		{
			sorted[sortarr[((*arr)[j] & deg) >> i*8]] = (*arr)[j];
			++sortarr[((*arr)[j] & deg) >> i*8];
		}
		deg *= 256;
		long long *temp = *arr;
		*arr = sorted;
		sorted = temp;
		//delete[] temp;
	}
	delete[] sorted;
}

int main()
{
	int N;
	std::cin >> N;
	long long *arr = new long long[N];
	int *sortarr = new int[257];
	for (int i = 0; i < N; i++)
	{
		std::cin >> arr[i];
	}

	Sort(&arr, N, sortarr);

	for (int i = 0; i < N; i++)
	{
		std::cout << arr[i] << std::endl;
	}

	delete[] arr;
	delete[] sortarr;
	system("pause");
}